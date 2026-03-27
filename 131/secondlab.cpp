#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

const double EPS = 1e-4;
const int MAX_ITER = 1000;

double vector_norm(const vector<double>& x1, const vector<double>& x2) {
    double max_diff = 0.0;
    for (size_t i = 0; i < x1.size(); i++) {
        max_diff = max(max_diff, fabs(x1[i] - x2[i]));
    }
    return max_diff;
}

double check_convergence(const vector<vector<double>>& B) {
    double max_sum = 0.0;
    for (size_t i = 0; i < B.size(); i++) {
        double row_sum = 0.0;
        for (size_t j = 0; j < B[i].size(); j++) {
            row_sum += fabs(B[i][j]);
        }
        max_sum = max(max_sum, row_sum);
    }
    return max_sum;
}

void print_table_header() {
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Итер. |      x1      |      x2      |      x3      |      x4      |   погрешн." << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

void print_iteration(int iter, const vector<double>& x, double error) {
    cout << fixed << setprecision(6);
    cout << setw(5) << iter << "  |";
    for (size_t i = 0; i < x.size(); i++) {
        cout << setw(12) << x[i] << " |";
    }
    cout << setw(12) << error << endl;
}

void simple_iteration(const vector<vector<double>>& B, const vector<double>& c, const vector<double>& x0) {
    int n = B.size();
    vector<double> x_curr = x0;
    vector<double> x_prev;
    int iter = 0;
    double error = 0.0;
    
    cout << "\nМЕТОД ПРОСТОЙ ИТЕРАЦИИ" << endl;
    cout << "Норма матрицы B: " << check_convergence(B) << endl;
    
    print_table_header();
    print_iteration(iter, x_curr, 0.0);
    
    do {
        x_prev = x_curr;
        for (int i = 0; i < n; i++) {
            x_curr[i] = c[i];
            for (int j = 0; j < n; j++) {
                x_curr[i] += B[i][j] * x_prev[j];
            }
        }
        iter++;
        error = vector_norm(x_curr, x_prev);
        print_iteration(iter, x_curr, error);
        
        if (iter > MAX_ITER) {
            cout << "Превышено максимальное число итераций!" << endl;
            break;
        }
    } while (error > EPS);
    
    cout << "\nРешение: ";
    for (int i = 0; i < n; i++) {
        cout << "x" << i+1 << " = " << fixed << setprecision(6) << x_curr[i];
        if (i < n-1) cout << ", ";
    }
    cout << endl;
    cout << "Количество итераций: " << iter << endl;
    cout << "Невязка: " << fixed << setprecision(8) << vector_norm(x_curr, x_prev) << endl;
}

void seidel(const vector<vector<double>>& B, const vector<double>& c, const vector<double>& x0) {
    int n = B.size();
    vector<double> x_curr = x0;
    vector<double> x_prev;
    int iter = 0;
    double error = 0.0;
    
    cout << "\nМЕТОД ЗЕЙДЕЛЯ" << endl;
    cout << "Норма матрицы B: " << check_convergence(B) << endl;
    
    print_table_header();
    print_iteration(iter, x_curr, 0.0);
    
    do {
        x_prev = x_curr;
        for (int i = 0; i < n; i++) {
            x_curr[i] = c[i];
            for (int j = 0; j < n; j++) {
                x_curr[i] += B[i][j] * x_curr[j];
            }
        }
        iter++;
        error = vector_norm(x_curr, x_prev);
        print_iteration(iter, x_curr, error);
        
        if (iter > MAX_ITER) {
            cout << "Превышено максимальное число итераций!" << endl;
            break;
        }
    } while (error > EPS);
    
    cout << "\nРешение: ";
    for (int i = 0; i < n; i++) {
        cout << "x" << i+1 << " = " << fixed << setprecision(6) << x_curr[i];
        if (i < n-1) cout << ", ";
    }
    cout << endl;
    cout << "Количество итераций: " << iter << endl;
    cout << "Невязка: " << fixed << setprecision(8) << error << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
   
    cout << "Лабораторная работа №3. Решение СЛАУ итерационными методами" << endl;
    cout << "Вариант 10" << endl;
    cout << "Точность: 10^-4" << endl;
 
    
    // СЛАУ 1 (№5) 
    cout << "\n\n" << string(80, '=') << endl;
    cout << "СЛАУ 1 (система №5)" << endl;
    cout << "x1 = 0.32x1 - 0.13x2 - 0.08x3 + 0.16x4 + 2.42" << endl;
    cout << "x2 = 0.17x1 - 0.22x2 + 0.13x3 - 0.21x4 + 1.48" << endl;
    cout << "x3 = 0.05x1 - 0.08x2 + 0.34x4 - 0.16" << endl;
    cout << "x4 = 0.12x1 + 0.11x2 - 0.19x3 + 0.06x4 + 1.64" << endl;
    cout << string(80, '=') << endl;
    
    vector<vector<double>> B1 = {
        {0.32, -0.13, -0.08, 0.16},
        {0.17, -0.22, 0.13, -0.21},
        {0.05, -0.08, 0.0, 0.34},
        {0.12, 0.11, -0.19, 0.06}
    };
    
    vector<double> c1 = {2.42, 1.48, -0.16, 1.64};
    vector<double> x0_1 = {0.0, 0.0, 0.0, 0.0};
    
    simple_iteration(B1, c1, x0_1);
    seidel(B1, c1, x0_1);
    
    //  СЛАУ 2 (№18) 
    cout << "\n\n" << string(80, '=') << endl;
    cout << "СЛАУ 2 (система №18)" << endl;
    cout << "x1 = 0.08x1 - 0.03x2 - 0.06x4 - 1.7" << endl;
    cout << "x2 = 0.54x1 + 0.24x3 - 0.08x4 + 0.81" << endl;
    cout << "x3 = 0.33x1 - 0.37x2 + 0.25x4 - 0.98" << endl;
    cout << "x4 = 0.11x1 + 0.03x2 + 0.52x4 + 0.14" << endl;
    cout << string(80, '=') << endl;
    
    vector<vector<double>> B2 = {
        {0.08, -0.03, 0.0, -0.06},
        {0.54, 0.0, 0.24, -0.08},
        {0.33, -0.37, 0.0, 0.25},
        {0.11, 0.03, 0.0, 0.52}
    };
    
    vector<double> c2 = {-1.7, 0.81, -0.98, 0.14};
    vector<double> x0_2 = {0.0, 0.0, 0.0, 0.0};
    
    simple_iteration(B2, c2, x0_2);
    seidel(B2, c2, x0_2);
    
    // СЛАУ 3 (№52)
    cout << "\n\n" << string(80, '=') << endl;
    cout << "СЛАУ 3 (система №52) - приведение к итерационному виду" << endl;
    cout << "Исходная система:" << endl;
    cout << "7.6x1 + 5.8x2 + 4.7x3 = 10.1" << endl;
    cout << "3.8x1 + 4.2x2 + 2.5x3 = 9.7" << endl;
    cout << "2.9x1 + 2.2x2 + 3.8x3 = 7.4" << endl;
    cout << "Приведенный вид (x = Bx + c):" << endl;
    cout << "x1 = -0.7632x2 - 0.6184x3 + 1.3289" << endl;
    cout << "x2 = -0.9048x1 - 0.5952x3 + 2.3095" << endl;
    cout << "x3 = -0.7632x1 - 0.5789x2 + 1.9474" << endl;
    cout << string(80, '=') << endl;
    
    vector<vector<double>> B3 = {
        {0.0, -5.8/7.6, -4.7/7.6},
        {-3.8/4.2, 0.0, -2.5/4.2},
        {-2.9/3.8, -2.2/3.8, 0.0}
    };
    
    vector<double> c3 = {10.1/7.6, 9.7/4.2, 7.4/3.8};
    vector<double> x0_3 = {0.0, 0.0, 0.0};
    
    simple_iteration(B3, c3, x0_3);
    seidel(B3, c3, x0_3);
    
    // СЛАУ 4 (№56) 
    cout << "\n\n" << string(80, '=') << endl;
    cout << "СЛАУ 4 (система №56) - приведение к итерационному виду" << endl;
    cout << "Исходная система:" << endl;
    cout << "1.7x1 + 2.9x2 + 1.8x3 = 0.8" << endl;
    cout << "2.1x1 + 3.3x2 + 1.9x3 = 1.3" << endl;
    cout << "4.2x1 - 1.7x2 + 1.3x3 = 2.8" << endl;
    cout << "Приведенный вид (x = Bx + c):" << endl;
    cout << "x1 = -1.7059x2 - 1.0588x3 + 0.4706" << endl;
    cout << "x2 = -0.6364x1 - 0.5758x3 + 0.3939" << endl;
    cout << "x3 = -3.2308x1 + 1.3077x2 + 2.1538" << endl;
    cout << string(80, '=') << endl;
    
    vector<vector<double>> B4 = {
        {0.0, -2.9/1.7, -1.8/1.7},
        {-2.1/3.3, 0.0, -1.9/3.3},
        {-4.2/1.3, 1.7/1.3, 0.0}
    };
    
    vector<double> c4 = {0.8/1.7, 1.3/3.3, 2.8/1.3};
    vector<double> x0_4 = {0.0, 0.0, 0.0};
    
    simple_iteration(B4, c4, x0_4);
    seidel(B4, c4, x0_4);
    
    cout << "\n================================================================================" << endl;
    cout << "Работа завершена" << endl;
    
    return 0;
}