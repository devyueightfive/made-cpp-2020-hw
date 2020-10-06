#pragma once
#include <iostream>
#include <vector>

namespace task {

// unary operations

// unary plus: return self
std::vector<double> operator+(const std::vector<double> &a) {
    return std::vector<double>(a);// copy of a
}

// unary minus
std::vector<double> operator-(const std::vector<double> &a) {
    auto result = std::vector<double>(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = -a[i];
    }
    return result;
}

// binary operations

// addition
std::vector<double> operator+(const std::vector<double> &a,
                              const std::vector<double> &b) {
    // diff sizes -> no vector
    if (a.size() != b.size()) {
        return std::vector<double>();
    }
    auto result = std::vector<double>(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = a[i] + b[i];
    }
    return result;
}

// subtraction
std::vector<double> operator-(const std::vector<double> &a,
                              const std::vector<double> &b) {
    // diff sizes -> no vector
    if (a.size() != b.size()) {
        return std::vector<double>();
    }
    auto result = std::vector<double>(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = a[i] - b[i];
    }
    return result;
}

// scalar product
long double operator*(const std::vector<double> &a,
                      const std::vector<double> &b) {
    // diff sizes -> no scalar product
    if (a.size() != b.size()) {
        return 0;
    }
    long double result = 0;
    for (int i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    }
    return result;
}

// cross product
std::vector<double> operator%(const std::vector<double> &a,
                              const std::vector<double> &b) {
    // def https://en.wikipedia.org/wiki/Cross_product =)
    // result is {0, 0, 0}
    auto result = std::vector<double>(3, 0.0);

    // if vectors are not from R3
    if (a.size() != 3 && b.size() != 3) {
        // result is {0, 0, 0}
        return result;// return zero vector
    }

    // by def fill positions
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
    return result;
}

// output stream
// Оператор потокового вывода <<, выводящий все значения вектора через пробел,
// заканчивая символом переноса строки
std::ostream &operator<<(std::ostream &out, const std::vector<double> &a) {
    if (a.size() == 0)
        return out;
    // fill stream with data
    out << a[0];
    for (int i = 1; i < a.size(); i++) {
        out << " " << a[i];
    }
    out << '\n';
    return out;
}

// input stream
// Оператор потокового ввода >>, принимающий первым числом размер,
// далее значения
std::istream &operator>>(std::istream &in, std::vector<double> &a) {
    int n;
    in >> n;
    a.clear();// clear input container
    double value;
    for (int i = 0; i < n; i++) {
        in >> value;
        a.push_back(value);// fill container
    }
    return in;
}

// reverse
// Функция reverse, переставляющая элементы вектора в обратном порядке
void reverse(std::vector<double> &a) {
    double tmp;
    int j = a.size() - 1;
    int i = 0;
    while (i < j) {
        tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
        ++i;
        --j;
    }
}

bool is_zero(const std::vector<double> &a) {
    bool result = true;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != 0.0) {
            result = false;
            break;
        }
    }
    return result;
}

// Оператор ||, проверяющий коллинеарность
bool operator||(const std::vector<double> &a, const std::vector<double> &b) {
    const double EPS = 1e-7;
    int i;
    int N = a.size();
    for (i = 0; i < N; ++i)
        if (a[i] != 0 && b[i] != 0)
            break;
    if (i == N || a[i] == 0 || b[i] == 0)
        return false;
    double k = a[i] * 1.0 / b[i];
    while (++i != N)
        if (a[i] >= b[i] * k) {
            if (a[i] - b[i] * k > EPS) {
                return false;
            }
        } else {
            if (b[i] * k - a[i] > EPS) {
                return false;
            }
        }
    return true;
}

// Оператор `&&`, проверяющий сонаправленность
bool operator&&(const std::vector<double> &a, const std::vector<double> &b) {
    const double EPS = 1e-7;
    int i;
    int N = a.size();
    for (i = 0; i < N; ++i)
        if (a[i] != 0 && b[i] != 0)
            break;
    if (i == N || a[i] == 0 || b[i] == 0)
        return false;
    double k = a[i] * 1.0 / b[i];
    if (k <= 0) {
        return false;
    }
    while (++i != N)
        if (a[i] >= b[i] * k) {
            if (a[i] - b[i] * k > EPS) {
                return false;
            }
        } else {
            if (b[i] * k - a[i] > EPS) {
                return false;
            }
        }
    return true;
}

std::vector<int> operator|(const std::vector<int> &a,
                           const std::vector<int> &b) {
    // diff sizes -> no vector
    if (a.size() != b.size()) {
        return std::vector<int>();
    }
    auto result = std::vector<int>(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = a[i] | b[i];
    }
    return result;
}

std::vector<int> operator&(const std::vector<int> &a,
                           const std::vector<int> &b) {
    // diff sizes -> no vector
    if (a.size() != b.size()) {
        return std::vector<int>();
    }
    auto result = std::vector<int>(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = a[i] & b[i];
    }
    return result;
}

}// namespace task
