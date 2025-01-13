#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

union binary64
{
    double v;
    unsigned long long i;
};

#define MASK(x) ((0x1ULL << (x)) - 0x1ULL)

static inline uint64_t accessn(uint64_t x, uint64_t hi, uint64_t lo)
{
    uint64_t size, res;
    size = hi - lo + 1;
    res = (x >> lo) & MASK(size);
    return res;
}

static inline uint64_t access1(uint64_t x, uint64_t o)
{
    uint64_t res;
    res = (x >> o) & 0x1ULL;
    return res;
}

void dump_set_dv(std::ostringstream &vsm, char mem, double *x, int n, int addr)
{
    for (int i = 0; i < n; i++)
    {
        union binary64 tmp;
        tmp.v = x[i];
        for (int k = 0; k < 4; k++)
        {
            vsm << "d set $l" << mem << (8 * i + 2 * k + addr) << "n0c0b0m0p0 1 "
                << std::hex << tmp.i << std::dec << "\n";
        }
    }
}

void dump_set_d(std::ostringstream &vsm, char mem, double *x, int n, int addr)
{
    for (int i = 0; i < n; i++)
    {
        union binary64 tmp;
        tmp.v = x[i]; // LM0[i];
        vsm << "d set $l" << mem << (2 * i + addr) << "n0c0b0m0p0 1 "
            << std::hex << tmp.i << std::dec << "\n";
    }
}

void dump_set_d_flexible_mab_pe(std::ostringstream &vsm, char mem, double *x, int n, int addr)
{
    for (int mab = 0; mab < 2; mab++)
    {
        for (int peid = 0; peid < 4; peid++) // peid
        {
            for (int i = 0; i <= 5; i++)
            {
                union binary64 tmp;
                tmp.v = 5.0; // LM0[i]; 一旦5.0で初期化
                vsm << "d set $l" << mem << (addr * i) << "n0c0b0m" << mab << "p" << peid << " 1 "
                    << std::hex << tmp.i << std::dec << "\n";
            }
        }
    }
}

void dump_get_d(std::ostringstream &vsm, char mem, int n, int addr_offset, int pe = 0, int m = 0)
{
    int addr = addr_offset;
    for (int i = 0; i < n; i++)
    {
        vsm << "d getd $l" << mem << addr << "n0c0b0m" << m << std::dec << "p" << pe << " " << 1 << " \n";
        addr += 2;
    }
}

void put_d(std::ostringstream &vsm, double *x, int n, int addr)
{
    vsm << std::hex << std::setfill('0') << std::setw(5) << addr << " ";
    vsm << std::hex << std::setfill('0') << std::setw(3) << n << " ";

    for (int i = 0; i < n; i++)
    {
        union binary64 tmp;
        tmp.v = x[i];
        vsm << std::hex << std::setw(16) << tmp.i << std::dec;
    }
    vsm << std::endl;
}

void put_l1bm_d(std::ostringstream &vsm, double *x, int n, int addr)
{
    vsm << "p ";
    put_d(vsm, x, n, addr);
}

void put_l2bm_d(std::ostringstream &vsm, double *x, int n, int addr)
{
    vsm << "l 0 ";
    put_d(vsm, x, n, addr);
}

void move_l2bm_l1bm(std::ostringstream &vsm, int l2adr, int l1adr, int n)
{
    const int nb = n / 64;
    for (int i = 0; i < nb; i++)
    {
        vsm << "l2bmb $lc" << l2adr + 64 * i << " $lb" << l1adr + 64 * i << std::endl;
    }
    vsm << std::endl;
}

void get_l1bm_d(std::ostringstream &vsm, int n, int addr)
{
    vsm << "g ";
    vsm << std::hex << std::setfill('0') << std::setw(5) << addr << " ";
    vsm << std::hex << std::setfill('0') << std::setw(3) << n << " ";
}

void get_l2bm_d(std::ostringstream &vsm, int n, int addr)
{
    vsm << "b 0 ";
    vsm << std::hex << std::setfill('0') << std::setw(5) << addr << " ";
    vsm << std::hex << std::setfill('0') << std::setw(3) << n << " ";
}

void put_const_d(std::ostringstream &vsm, char mem, double x, int addr)
{
    union binary64 tmp;
    tmp.v = x; //  std::cout << std::hex << tmp.i << std::dec << std::endl;

    vsm << "## $l" << mem << addr << " : 0x" << std::hex << tmp.i << std::dec << " (binary64)" << std::endl;
    vsm << "iimm ui\"0x" << std::hex << std::setfill('0') << std::setw(8)
        << accessn(tmp.i, 64, 32) << std::dec << "\" $t ; nointr " << std::endl;
    vsm << "ipassa $ti $" << mem << addr << " ; nointr" << std::endl;

    vsm << "iimm ui\"0x" << std::hex << std::setfill('0') << std::setw(8)
        << ((tmp.i & MASK(32))) << std::dec << "\" $t ; nointr " << std::endl;
    vsm << "ipassa $ti $" << mem << addr + 1 << " ; nointr" << std::endl;
    vsm << "mvnop" << std::endl
        << std::endl;
}

struct pe_emulator
{
    static const int n = 2048;
    double LM0[8192];
    double LM1[8192];
    int debug_mab = 0;
    int debug_address_offset = 0;

    std::ostringstream vsm;

    void execute_kernel(std::string kernelfile, double *arg_LM0, double *arg_LM1, int nn = n)
    {
        const char vsmfile[] = "__tmp__.vsm";
        const char asmfile[] = "__tmp__.asm";
        const char resfile[] = "__tmp__.result";
        const char simfile[] = "__tmp__.output";

        memcpy(LM0, arg_LM0, sizeof(double) * nn);
        memcpy(LM1, arg_LM1, sizeof(double) * nn);

        // dump_set_d(vsm, 'm', LM0, nn, 0);
        // dump_set_d(vsm, 'n', LM1, nn, 0);
        dump_set_d_flexible_mab_pe(vsm, 'm', LM0, nn, 2);

        std::ifstream ifs_kernel(kernelfile);
        if (!ifs_kernel.is_open())
        {
            std::cerr << kernelfile << " is not existed\n";
            exit(-1);
        }
        std::string kernel_string((std::istreambuf_iterator<char>(ifs_kernel)), std::istreambuf_iterator<char>());
        vsm << kernel_string;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    for (int debug_pe = 0; debug_pe < 4; debug_pe++)
                    {
                        dump_get_d(vsm, 'm', 6, debug_address_offset, debug_pe, debug_mab);
                    }
                    debug_mab++;
                }
                debug_mab -= 2;
                debug_address_offset += 12;
            }
            debug_mab += 2;
            debug_address_offset = 0;
        }

        // dump_get_d(vsm, 'm', nn, 0);
        // dump_get_d(vsm, 'n', nn, 0);

        std::ofstream ofs(vsmfile);
        ofs << vsm.str() << std::endl;
        ofs.close();

        std::ostringstream assembler;
        std::ostringstream simulator;
        assembler << "../mncore2_emuenv_20240826/assemble3 " << vsmfile << " >| " << asmfile;
        simulator << "../mncore2_emuenv_20240826/gpfn3_package_main -i " << asmfile << " -d " << resfile << " >| " << simfile;

        int r;
        r = system(assembler.str().c_str());
        r = system(simulator.str().c_str());

        std::ifstream ifs_output(resfile);
        std::string res((std::istreambuf_iterator<char>(ifs_output)), std::istreambuf_iterator<char>());
        std::smatch m;
        std::regex re("\\(0x([0-9a-f]*)");
        std::vector<double> values;
        for (auto it = res.cbegin(); regex_search(it, res.cend(), m, re); it += m.position(0) + m.length(0))
        {
            union binary64 tmp;
            tmp.i = std::stoull(m[1].str().substr(0, 16), nullptr, 16);
            values.push_back(tmp.v);
        }

        memcpy(arg_LM0, values.data(), sizeof(double) * nn);
        memcpy(arg_LM1, values.data() + nn, sizeof(double) * nn);

        vsm.str("");
        vsm.clear();
    }
};
