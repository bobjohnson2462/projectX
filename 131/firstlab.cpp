#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

const double EPS = 1e-6;
const int MAX_ITER = 1000;

//  УРАВНЕНИЕ 1: 3*sin(x) - x + 3 = 0 
double f1(double x) {
    return 3 * sin(x) - x + 3;
}

double f1_prime(double x) {
    return 3 * cos(x) - 1;
}

double phi1(double x) {
    return 3 * sin(x) + 3;
}

//  УРАВНЕНИЕ 2: x^3 + 6*ln(x) - 5 = 0 
double f2(double x) {
    return pow(x, 3) + 6 * log(x) - 5;
}

double f2_prime(double x) {
    return 3 * x * x + 6 / x;
}

double phi2(double x) {
    return pow(5 - 6 * log(x), 1.0/3.0);
}

//  МЕТОД ДИХОТОМИИ 
double bisection(double (*f)(double), double a, double b, double eps, int& iter_count) {
    if (f(a) * f(b) >= 0) {
        cout << "Ошибка: f(a) и f(b) имеют одинаковые знаки" << endl;
        return NAN;
    }
    
    iter_count = 0;
    double c;
    
    cout << fixed << setprecision(8);
    cout << "Метод дихотомии:" << endl;
    cout << "№ итерации |       a       |       b       |       c       |      f(c)" << endl;
    cout << "-----------|---------------|---------------|---------------|---------------" << endl;
    
    while ((b - a) / 2 > eps && iter_count < MAX_ITER) {
        c = (a + b) / 2;
        cout << setw(10) << iter_count << " | " << setw(13) << a << " | " << setw(13) << b 
             << " | " << setw(13) << c << " | " << setw(13) << f(c) << endl;
        
        if (f(c) == 0) {
            break;
        } else if (f(a) * f(c) < 0) {
            b = c;
        } else {
            a = c;
        }
        iter_count++;
    }
    
    c = (a + b) / 2;
    cout << setw(10) << iter_count << " | " << setw(13) << a << " | " << setw(13) << b 
         << " | " << setw(13) << c << " | " << setw(13) << f(c) << endl;
    
    return c;
}

// МЕТОД ПРОСТЫХ ИТЕРАЦИЙ
double fixed_point(double (*phi)(double), double x0, double eps, int& iter_count) {
    iter_count = 0;
    double x_prev = x0;
    double x_curr;
    
    cout << fixed << setprecision(8);
    cout << "Метод простых итераций:" << endl;
    cout << "№ итерации |      x_n      |     x_{n+1}    |    |x_{n+1}-x_n|" << endl;
    cout << "-----------|---------------|----------------|-------------------" << endl;
    
    do {
        x_curr = phi(x_prev);
        cout << setw(10) << iter_count << " | " << setw(13) << x_prev << " | " 
             << setw(14) << x_curr << " | " << setw(15) << fabs(x_curr - x_prev) << endl;
        
        if (fabs(x_curr - x_prev) < eps) break;
        
        x_prev = x_curr;
        iter_count++;
        
        if (iter_count > MAX_ITER) {
            cout << "Превышено максимальное число итераций" << endl;
            break;
        }
    } while (true);
    
    return x_curr;
}

//  МЕТОД НЬЮТОНА 
double newton(double (*f)(double), double (*f_prime)(double), double x0, double eps, int& iter_count) {
    iter_count = 0;
    double x_prev = x0;
    double x_curr;
    
    cout << fixed << setprecision(8);
    cout << "Метод Ньютона:" << endl;
    cout << "№ итерации |      x_n      |     x_{n+1}    |    |x_{n+1}-x_n|" << endl;
    cout << "-----------|---------------|----------------|-------------------" << endl;
    
    do {
        double f_val = f(x_prev);
        double f_prime_val = f_prime(x_prev);
        
        if (fabs(f_prime_val) < 1e-12) {
            cout << "Производная близка к нулю" << endl;
            break;
        }
        
        x_curr = x_prev - f_val / f_prime_val;
        
        cout << setw(10) << iter_count << " | " << setw(13) << x_prev << " | " 
             << setw(14) << x_curr << " | " << setw(15) << fabs(x_curr - x_prev) << endl;
        
        if (fabs(x_curr - x_prev) < eps) break;
        
        x_prev = x_curr;
        iter_count++;
        
        if (iter_count > MAX_ITER) {
            cout << "Превышено максимальное число итераций" << endl;
            break;
        }
    } while (true);
    
    return x_curr;
}

//  МЕТОД СЕКУЩИХ 
double secant(double (*f)(double), double x0, double x1, double eps, int& iter_count) {
    iter_count = 0;
    double x_prev = x0;
    double x_curr = x1;
    double x_next;
    
    cout << fixed << setprecision(8);
    cout << "Метод секущих:" << endl;
    cout << "№ итерации |     x_{n-1}    |      x_n      |     x_{n+1}    |    |x_{n+1}-x_n|" << endl;
    cout << "-----------|----------------|---------------|----------------|-------------------" << endl;
    
    do {
        double f_prev = f(x_prev);
        double f_curr = f(x_curr);
        
        if (fabs(f_curr - f_prev) < 1e-12) {
            cout << "Знаменатель близок к нулю" << endl;
            break;
        }
        
        x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev);
        
        cout << setw(10) << iter_count << " | " << setw(14) << x_prev << " | " 
             << setw(13) << x_curr << " | " << setw(14) << x_next << " | " 
             << setw(15) << fabs(x_next - x_curr) << endl;
        
        if (fabs(x_next - x_curr) < eps) break;
        
        x_prev = x_curr;
        x_curr = x_next;
        iter_count++;
        
        if (iter_count > MAX_ITER) {
            cout << "Превышено максимальное число итераций" << endl;
            break;
        }
    } while (true);
    
    return x_next;
}

// МЕТОД ХОРД 
double chord(double (*f)(double), double a, double b, double eps, int& iter_count) {
    if (f(a) * f(b) >= 0) {
        cout << "Ошибка: f(a) и f(b) имеют одинаковые знаки" << endl;
        return NAN;
    }
    
    iter_count = 0;
    double a_fixed = a;
    double b_curr = b;
    double x_new;
    
    cout << fixed << setprecision(8);
    cout << "Метод хорд:" << endl;
    cout << "№ итерации |       a       |       b       |     x_new     |    |x_new-b|" << endl;
    cout << "-----------|---------------|---------------|---------------|---------------" << endl;
    
    do {
        x_new = a_fixed - f(a_fixed) * (b_curr - a_fixed) / (f(b_curr) - f(a_fixed));
        
        cout << setw(10) << iter_count << " | " << setw(13) << a_fixed << " | " 
             << setw(13) << b_curr << " | " << setw(13) << x_new << " | " 
             << setw(13) << fabs(x_new - b_curr) << endl;
        
        if (fabs(x_new - b_curr) < eps) break;
        
        if (f(a_fixed) * f(x_new) < 0) {
            b_curr = x_new;
        } else {
            a_fixed = x_new;
        }
        
        iter_count++;
        
        if (iter_count > MAX_ITER) {
            cout << "Превышено максимальное число итераций" << endl;
            break;
        }
    } while (true);
    
    return x_new;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
   
    cout << "Лабораторная работа №1" << endl;
    cout << "Вариант 10" << endl;
    cout << "Уравнение 1: 3*sin(x) - x + 3 = 0" << endl;
    cout << "Уравнение 2: x^3 + 6*ln(x) - 5 = 0" << endl;
    cout << "Точность: 10^-6" << endl;
  
    
    int iter;
    double root;
    
    // УРАВНЕНИЕ 1
    cout << "\n\n" << string(80, '=') << endl;
    cout << "УРАВНЕНИЕ 1: 3*sin(x) - x + 3 = 0" << endl;
    cout << string(80, '=') << endl;
    
    // Отрезок изоляции [3.1, 3.2]
    cout << "\n--- Отрезок изоляции: [3.1, 3.2] ---" << endl;
    cout << "f(3.1) = " << f1(3.1) << "   f(3.2) = " << f1(3.2) << endl << endl;
    
    // Метод дихотомии
    iter = 0;
    root = bisection(f1, 3.1, 3.2, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f1(root) << endl;
    
    // Метод простых итераций
    cout << "\n--- Проверка условия сходимости для метода простых итераций ---" << endl;
    cout << "phi'(x) = 3*cos(x)" << endl;
    cout << "|phi'(3.15)| = " << fabs(3 * cos(3.15)) << " < 1? ";
    if (fabs(3 * cos(3.15)) < 1) cout << "Да" << endl;
    else cout << "Нет (метод может расходиться)" << endl;
    
    cout << "\n";
    iter = 0;
    root = fixed_point(phi1, 3.15, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f1(root) << endl;
    
    // Метод Ньютона
    cout << "\n";
    iter = 0;
    root = newton(f1, f1_prime, 3.15, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f1(root) << endl;
    
    // Метод секущих
    cout << "\n";
    iter = 0;
    root = secant(f1, 3.1, 3.2, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f1(root) << endl;
    
    // Метод хорд
    cout << "\n";
    iter = 0;
    root = chord(f1, 3.1, 3.2, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f1(root) << endl;
    
    // ==================== УРАВНЕНИЕ 2 ====================
    cout << "\n\n" << string(80, '=') << endl;
    cout << "УРАВНЕНИЕ 2: x^3 + 6*ln(x) - 5 = 0" << endl;
    cout << string(80, '=') << endl;
    
    // Отрезок изоляции [1.5, 2.0]
    cout << "\n--- Отрезок изоляции: [1.5, 2.0] ---" << endl;
    cout << "f(1.5) = " << f2(1.5) << "   f(2.0) = " << f2(2.0) << endl << endl;
    
    // Метод дихотомии
    iter = 0;
    root = bisection(f2, 1.5, 2.0, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f2(root) << endl;
    
    // Метод простых итераций
    cout << "\n--- Проверка условия сходимости для метода простых итераций ---" << endl;
    cout << "phi'(x) = -2/(x*(5-6*ln(x))^(2/3))" << endl;
    cout << "|phi'(1.75)| = " << fabs(phi2_prime(1.75)) << " < 1? ";
    if (fabs(phi2_prime(1.75)) < 1) cout << "Да" << endl;
    else cout << "Нет" << endl;
    
    cout << "\n";
    iter = 0;
    root = fixed_point(phi2, 1.75, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f2(root) << endl;
    
    // Метод Ньютона
    cout << "\n";
    iter = 0;
    root = newton(f2, f2_prime, 1.75, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f2(root) << endl;
    
    // Метод секущих
    cout << "\n";
    iter = 0;
    root = secant(f2, 1.5, 2.0, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f2(root) << endl;
    
    // Метод хорд
    cout << "\n";
    iter = 0;
    root = chord(f2, 1.5, 2.0, EPS, iter);
    cout << "\nКорень: " << fixed << setprecision(8) << root << ", итераций: " << iter << endl;
    cout << "Значение функции: " << f2(root) << endl;
    
    return 0;
}