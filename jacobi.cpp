#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

const double delta_L = 0.01; // グリッド幅
const double LL = 1.0;       // 正方形の幅
const int L = static_cast<int>(LL / delta_L);
const double V = 5.0; // 境界y=0の電位 (境界条件)
const double convergence_criterion = 1e-5;
const double eps0 = 1.0;
const double charge_density = 1000.0;

void initialize_phi(std::vector<std::vector<double>> &phi)
{
    for (int i = 0; i <= L; i++)
    {
        phi[0][i] = V; // 境界条件
    }
}

void initialize_charge(std::vector<std::vector<double>> &charge)
{
    int CC = L / 4;
    int CC2 = L / 2;
    for (int i = CC; i <= CC2; i++)
    {
        for (int j = CC; j <= CC2; j++)
        {
            charge[i][j] = charge_density; // roh
        }
    }
}

int main()
{
    std::vector<std::vector<double>> phi(L + 1, std::vector<double>(L + 1, 0.0));
    std::vector<std::vector<double>> phi2(L + 1, std::vector<double>(L + 1, 0.0));
    std::vector<std::vector<double>> charge(L + 1, std::vector<double>(L + 1, 0.0));

    initialize_phi(phi);
    initialize_charge(charge);

    double delta = 1.0;
    int n_iter = 0;

    // Jacobi法による解法
    while (delta > convergence_criterion)
    {
        delta = 0.0; // 収束判定
        for (int i = 0; i <= L; i++)
        {
            for (int j = 0; j <= L; j++)
            {
                if (i == 0 || i == L || j == 0 || j == L)
                {
                    phi2[i][j] = phi[i][j];
                }
                else
                {
                    phi2[i][j] = (phi[i + 1][j] + phi[i - 1][j] + phi[i][j + 1] + phi[i][j - 1]) / 4.0; // + (delta_L * delta_L / 4.0) * charge[i][j];
                }
            }
        }

        // 収束条件のチェック
        delta = 0.0;
        for (int i = 0; i <= L; i++)
        {
            for (int j = 0; j <= L; j++)
            {
                delta = std::max(delta, std::abs(phi[i][j] - phi2[i][j]));
            }
        }

        phi.swap(phi2);
        n_iter++;
    }

    // 電場の計算
    std::vector<std::vector<double>> Ex(L, std::vector<double>(L, 0.0));
    std::vector<std::vector<double>> Ey(L, std::vector<double>(L, 0.0));

    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < L; j++)
        {
            Ex[i][j] = -(phi[i + 1][j] - phi[i][j]) / delta_L;
            Ey[i][j] = -(phi[i][j + 1] - phi[i][j]) / delta_L;
        }
    }

    // 結果の出力（gnuplotでの描画用）
    std::ofstream phi_file("phi_data.dat");
    for (int i = 0; i <= L; i++)
    {
        for (int j = 0; j <= L; j++)
        {
            phi_file << i * delta_L << " " << j * delta_L << " " << phi[i][j] << "\n";
        }
        phi_file << "\n";
    }
    phi_file.close();

    std::ofstream electric_field_file("electric_field.dat");
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < L; j++)
        {
            electric_field_file << i * delta_L << " " << j * delta_L << " " << Ex[i][j] << " " << Ey[i][j] << "\n";
        }
        electric_field_file << "\n";
    }
    electric_field_file.close();

    std::cout << "Simulation completed. Data saved to 'phi_data.dat' and 'electric_field.dat'.\n";
    std::cout << "Use gnuplot to visualize: \n";
    std::cout << "splot 'phi_data.dat' using 1:2:3 with pm3d\n";
    std::cout << "plot 'electric_field.dat' using 1:2:3:4 with vectors head filled lt 2\n";

    return 0;
}
