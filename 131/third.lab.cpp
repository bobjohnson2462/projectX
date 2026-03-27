#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

const double EPS = 1e-10;

struct Point {
    double x;
    double y;
};

vector<Point> read_csv(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return points;
    }
    
    while (getline(file, line)) {
        stringstream ss(line);
        string x_str, y_str;
        
        if (getline(ss, x_str, ',') && getline(ss, y_str)) {
            Point p;
            p.x = stod(x_str);
            p.y = stod(y_str);
            points.push_back(p);
        }
    }
    
    file.close();
    return points;
}

double lagrange(const vector<Point>& points, double x) {
    double result = 0.0;
    int n = points.size();
    
    for (int i = 0; i < n; i++) {
        double term = points[i].y;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                term *= (x - points[j].x) / (points[i].x - points[j].x);
            }
        }
        result += term;
    }
    
    return result;
}

vector<Point> find_nodes(const vector<Point>& points, double x, int degree) {
    vector<Point> nodes;
    int n = points.size();
    
    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (fabs(points[indices[i]].x - x) > fabs(points[indices[j]].x - x)) {
                swap(indices[i], indices[j]);
            }
        }
    }
    
    for (int i = 0; i < degree + 1 && i < n; i++) {
        nodes.push_back(points[indices[i]]);
    }
    
    for (int i = 0; i < nodes.size() - 1; i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            if (nodes[i].x > nodes[j].x) {
                swap(nodes[i], nodes[j]);
            }
        }
    }
    
    return nodes;
}

double newton_forward(const vector<Point>& points, double x) {
    int n = points.size();
    vector<vector<double>> diff(n, vector<double>(n, 0.0));
    
    for (int i = 0; i < n; i++) {
        diff[i][0] = points[i].y;
    }
    
    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            diff[i][j] = diff[i+1][j-1] - diff[i][j-1];
        }
    }
    
    double h = points[1].x - points[0].x;
    double t = (x - points[0].x) / h;
    double result = diff[0][0];
    double term = 1.0;
    
    for (int i = 1; i < n; i++) {
        term *= (t - (i - 1)) / i;
        result += term * diff[0][i];
    }
    
    return result;
}

double newton_backward(const vector<Point>& points, double x) {
    int n = points.size();
    vector<vector<double>> diff(n, vector<double>(n, 0.0));
    
    for (int i = 0; i < n; i++) {
        diff[i][0] = points[i].y;
    }
    
    for (int j = 1; j < n; j++) {
        for (int i = n - 1; i >= j; i--) {
            diff[i][j] = diff[i][j-1] - diff[i-1][j-1];
        }
    }
    
    double h = points[1].x - points[0].x;
    double t = (x - points[n-1].x) / h;
    double result = diff[n-1][0];
    double term = 1.0;
    
    for (int i = 1; i < n; i++) {
        term *= (t + (i - 1)) / i;
        result += term * diff[n-1][i];
    }
    
    return result;
}

double stirling(const vector<Point>& points, double x) {
    int n = points.size();
    int center_idx = 0;
    
    for (int i = 0; i < n; i++) {
        if (fabs(points[i].x - x) < fabs(points[center_idx].x - x)) {
            center_idx = i;
        }
    }
    
    int left = center_idx - 1;
    int right = center_idx + 1;
    
    vector<Point> central_points;
    central_points.push_back(points[center_idx]);
    
    while (central_points.size() < 5 && (left >= 0 || right < n)) {
        if (left >= 0 && (right >= n || fabs(points[left].x - x) <= fabs(points[right].x - x))) {
            central_points.insert(central_points.begin(), points[left]);
            left--;
        } else if (right < n) {
            central_points.push_back(points[right]);
            right++;
        }
    }
    
    int m = central_points.size();
    vector<vector<double>> diff(m, vector<double>(m, 0.0));
    
    for (int i = 0; i < m; i++) {
        diff[i][0] = central_points[i].y;
    }
    
    for (int j = 1; j < m; j++) {
        for (int i = 0; i < m - j; i++) {
            diff[i][j] = diff[i+1][j-1] - diff[i][j-1];
        }
    }
    
    double h = central_points[1].x - central_points[0].x;
    double t = (x - central_points[0].x) / h;
    double result = diff[0][0];
    double term = 1.0;
    
    for (int i = 1; i < m; i++) {
        if (i % 2 == 1) {
            term *= (t - (i - 1) / 2.0);
        } else {
            term *= (t + (i - 2) / 2.0);
        }
        term /= i;
        
        int idx = i / 2;
        if (i % 2 == 0) {
            result += term * (diff[idx][i] + diff[idx-1][i]) / 2.0;
        } else {
            result += term * diff[idx][i];
        }
    }
    
    return result;
}

void print_table(const vector<Point>& points) {
    cout << "┌──────────┬──────────┐" << endl;
    cout << "│    x     │    y     │" << endl;
    cout << "├──────────┼──────────┤" << endl;
    for (const auto& p : points) {
        cout << "│ " << fixed << setprecision(6) << setw(8) << p.x 
             << " │ " << setw(8) << p.y << " │" << endl;
    }
    cout << "└──────────┴──────────┘" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    int variant = 10;
    string filename = "dataset_" + (variant < 10 ? "0" : "") + to_string(variant) + ".csv";
    vector<double> points_to_calc = {0.211, 0.95, 1.85};
    
    cout << "================================================================================" << endl;
    cout << "Лабораторная работа №3. Интерполирование функций" << endl;
    cout << "Вариант " << variant << endl;
    cout << "Файл данных: " << filename << endl;
    cout << "Точки интерполяции: " << points_to_calc[0] << ", " << points_to_calc[1] << ", " << points_to_calc[2] << endl;
    cout << "================================================================================" << endl;
    
    vector<Point> points = read_csv(filename);
    
    if (points.empty()) {
        cerr << "Не удалось загрузить данные из файла" << endl;
        return 1;
    }
    
    cout << "\nИсходные данные:" << endl;
    print_table(points);
    
    cout << "\n" << string(80, '=') << endl;
    cout << "ЗАДАНИЕ 1. Полином Лагранжа" << endl;
    cout << string(80, '=') << endl;
    
    for (double x : points_to_calc) {
        double result = lagrange(points, x);
        cout << "\nf(" << fixed << setprecision(6) << x << ") = " << setprecision(10) << result << endl;
    }
    
    cout << "\n" << string(80, '=') << endl;
    cout << "ЗАДАНИЕ 2. Полиномы Ньютона и Стирлинга" << endl;
    cout << string(80, '=') << endl;
    
    for (double x : points_to_calc) {
        vector<Point> nodes = find_nodes(points, x, 4);
        
        cout << "\nТочка x = " << fixed << setprecision(6) << x << endl;
        cout << "Используемые узлы:" << endl;
        for (const auto& p : nodes) {
            cout << "  x = " << p.x << ", y = " << p.y << endl;
        }
        
        +sort(nodes.begin(), nodes.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });
        
        double h = nodes[1].x - nodes[0].x;
        double t = (x - nodes[0].x) / h;
        int n_nodes = nodes.size();
        
        cout << "\nШаг h = " << h << endl;
        cout << "t = " << t << endl;
        
        if (t >= 0 && t <= n_nodes - 1 && fabs(t) <= n_nodes / 2.0) {
            cout << "\nИспользуется интерполяция вперед (первый полином Ньютона):" << endl;
            double result = newton_forward(nodes, x);
            cout << "Результат: " << setprecision(10) << result << endl;
        } 
        else if (t < 0 && t >= -(n_nodes - 1)) {
            cout << "\nИспользуется интерполяция назад (второй полином Ньютона):" << endl;
            double result = newton_backward(nodes, x);
            cout << "Результат: " << setprecision(10) << result << endl;
        }
        else {
            cout << "\nИспользуется интерполяция Стирлинга (центральная):" << endl;
            double result = stirling(nodes, x);
            cout << "Результат: " << setprecision(10) << result << endl;
        }
        
        double lagrange_result = lagrange(points, x);
        cout << "Сравнение с полиномом Лагранжа: " << setprecision(10) << lagrange_result << endl;
        cout << "Разница: " << fabs(lagrange_result - lagrange(points, x)) << endl;
    }
    
 
    cout << "Работа завершена" << endl;
    
    return 0;
}