#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>

void double_to_short_word(double x, std::string &h, std::string &l)
{
    union binary64
    {
        double v;
        uint64_t i;
    };

    union binary64 tmp;
    tmp.v = x;

    std::stringstream buf;
    buf << std::hex << std::setw(16) << std::setfill('0') << tmp.i;

    std::string s = buf.str();
    h = s.substr(0, 8);
    l = s.substr(8);
}

std::string init_const(double x, int adr)
{
    std::string buf;
    std::string h, l;

    double_to_short_word(x, h, l);

    buf += "# $lr" + std::to_string(adr) + " : " + std::to_string(x) + " : " + h + l + "\n";
    buf += "iimm ui\"0x" + h + "\" $r" + std::to_string(adr) + "\n";
    buf += "iimm ui\"0x" + l + "\" $r" + std::to_string(adr + 1) + "\n";

    return buf;
}

int main(int narg, char **argv)
{
    std::cout << init_const(1.0, 0) << "\n";
    std::cout << init_const(1.0 / 3.0, 2) << "\n";
}