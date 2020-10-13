#include "matrix.h"

using namespace task;


namespace task {

    Row::Row(double *r) : row(r) {
    }

    double &Row::operator[](size_t index) {
        return row[index];
    }


    // Конструктор по-умолчанию, создающий единичную матрицу размера 1 x 1
    Matrix::Matrix() : matrix(new double[1]), rows(1), cols(1) {
        this->matrix[0] = {1.0};
    }

    // Конструктор единичной матрицы заданного размера
    Matrix::Matrix(size_t rows, size_t cols) : matrix(new double[rows * cols]), rows(rows), cols(cols) {
        size_t n = rows * cols;
        for (int i; i < n; i++) {
            uint c = i % rows;
            uint r = (int) (i / rows);
            if (c == r) {
                this->matrix[i] = 1.0;
            } else {
                this->matrix[i] = 0.0;
            }
        }
    }

    // Конструктор копирования
    Matrix::Matrix(const Matrix &copy) : rows(copy.rows), cols(copy.cols) {
        size_t n = this->rows * this->cols;
        this->matrix = new double[n];
        for (int i; i < n; i++) {
            this->matrix[i] = copy.matrix[i];
        }
    }

    // Оператор присваивания
    Matrix &Matrix::operator=(const Matrix &a) {
        if (this != &a) {
            delete[] this->matrix;
            this->cols = a.cols;
            this->rows = a.rows;
            size_t n = this->cols * this->rows;
            this->matrix = new double[rows * cols];
            for (int i = 0; i < n; ++i) {
                this->matrix[i] = a.matrix[i];
            }
        }
        return *this;
    }

    // Метод get
    double &Matrix::get(size_t row, size_t col) {
        if (row == 0 || col == 0 || row > this->rows || col > this->cols) {
            new OutOfBoundsException();
        }
        return this->matrix[row * this->rows + col];
    }

    const double &Matrix::get(size_t row, size_t col) const {
        if (row == 0 || col == 0 || row > this->rows || col > this->cols) {
            new OutOfBoundsException();
        }
        return this->matrix[row * this->rows + col];
    }

    void Matrix::set(size_t row, size_t col, const double &value) {
        if (row > this->rows || col > this->cols) {
            new OutOfBoundsException();
        }
        this->matrix[row * this->rows + col] = value;
    }

    // Метод resize, изменяющий размер матрицы
    void Matrix::resize(size_t new_rows, size_t new_cols){

    };

    // Оператор [], который должно быть можно применить дважды, в том числе и для константных матриц
    Row Matrix::operator[](size_t row) {
        if (row >= this->rows) {
            new OutOfBoundsException();
        }
        Row r = Row(this->matrix + (row * this->rows));
        return r;
    }

    Row Matrix::operator[](size_t row) const {
        if (row == 0 || row > this->rows) {
            new OutOfBoundsException();
        }
        Row r = Row(this->matrix + (row * this->rows));
        return r;
    }
    // Операторы сложения
    Matrix &Matrix::operator+=(const Matrix &a) {
        if (this->rows != a.rows || this->cols != a.cols) {
            new SizeMismatchException();
        }
        size_t n = this->rows * this->cols;
        for (size_t i = 0; i < n; i++) {
            this->matrix[i] += a.matrix[i];
        }
        return *this;
    }

    // Оператор умножения на скаляр
    Matrix &Matrix::operator*=(const double &number) {
        size_t n = this->rows * this->cols;
        for (size_t i = 0; i < n; i++) {
            this->matrix[i] *= number;
        }
        return *this;
    }

    // Операторы вычитания
    Matrix &Matrix::operator-=(const Matrix &a) {
        if (this->rows != a.rows || this->cols != a.cols) {
            new SizeMismatchException();
        }
        size_t n = this->rows * this->cols;
        for (size_t i = 0; i < n; i++) {
            this->matrix[i] -= a.matrix[i];
        }
        return *this;
    }

    // Оператор умножения на матрицу
    //    Matrix &Matrix::operator*=(const Matrix &a){};

    // Оператор составного присваивания
    Matrix Matrix::operator+(const Matrix &a) const {
        if (this->rows != a.rows || this->cols != a.cols) {
            new SizeMismatchException();
        }
        size_t n = this->rows * this->cols;
        auto result = new Matrix(this->rows, this->cols);
        for (size_t i = 0; i < n; i++) {
            result->matrix[i] = this->matrix[i] + a.matrix[i];
        }
        return *result;
    }

    // Оператор составного вычитания
    Matrix Matrix::operator-(const Matrix &a) const {
        if (this->rows != a.rows || this->cols != a.cols) {
            new SizeMismatchException();
        }
        size_t n = this->rows * this->cols;
        auto result = new Matrix(this->rows, this->cols);
        for (size_t i = 0; i < n; i++) {
            result->matrix[i] = this->matrix[i] - a.matrix[i];
        }
        return *result;
    }

    // Оператор составного умножения на скаляр
    Matrix Matrix::operator*(const double &a) const {
        size_t n = this->rows * this->cols;
        auto result = new Matrix(this->rows, this->cols);
        for (size_t i = 0; i < n; i++) {
            result->matrix[i] = this->matrix[i] * a;
        }
        return *result;
    }

//    Matrix operator*(const Matrix &a) const {};

    // Оператор унарного минуса
    Matrix Matrix::operator-() const {
        size_t n = this->rows * this->cols;
        auto result = new Matrix(this->rows, this->cols);
        for (size_t i = 0; i < n; i++) {
            result->matrix[i] = -this->matrix[i];
        }
        return *result;
    }

    // Оператор унарного плюса
    Matrix Matrix::operator+() const {
        return *this;
    }

    // Метод getRow, возвращающий заданную строчку в виде std::vector
    std::vector<double> Matrix::getRow(size_t row) {
        if (row >= this->rows) {
            new OutOfBoundsException();
        }
        auto result = std::vector<double>();
        for (int i = 0; i < this->cols; i++) {
            result.push_back(this->matrix[i + row * this->cols]);
        }
        return result;
    }

    //
    std::vector<double> Matrix::getColumn(size_t column) {
        if (column >= this->cols) {
            new OutOfBoundsException();
        }
        auto result = std::vector<double>();
        for (int i = 0; i < this->rows; i++) {
            result.push_back(this->matrix[i * this->rows + column]);
        }
        return result;
    }

    // Операторы сравнения матриц на равенство
    bool Matrix::operator==(const Matrix &a) const {
        if (this->rows != a.rows || this->cols != a.cols) {
            return false;
        }
        size_t n = this->rows * this->cols;
        for (size_t i = 0; i < n; i++) {
            double diff = this->matrix[i] - a.matrix[i];
            if (diff < 0) { diff = -diff; }
            if (diff > task::EPS) {
                return false;
            }
        }
        return true;
    }

    bool Matrix::operator!=(const Matrix &a) const {
        return !(*this == a);
    }

    //    double Matrix::det() const {}
    //    void Matrix::transpose() {}
    //    Matrix Matrix::transposed() const {}
    //    double Matrix::trace() const {}


    Matrix::~Matrix() {
        delete[] this->matrix;
    }

    std::ostream &operator<<(std::ostream &output, const Matrix &matrix) {
        size_t rows = matrix.rows;
        size_t cols = matrix.cols;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                output << matrix[i][j] << " ";
            }
            output << "\n";
        }
        return output;
    }
    //    std::istream &operator>>(std::istream &input, Matrix &matrix) {}

    size_t Matrix::getRows() {
        return this->rows;
    }

    size_t Matrix::getCols() {
        return this->cols;
    }
}// namespace task
