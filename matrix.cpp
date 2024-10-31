#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t size;

public:
    Matrix(size_t size, const std::vector<std::vector<T>>& elements) : size(size), data(elements) {
        if (elements.size() != size || !std::all_of(elements.begin(), elements.end(), [size](const std::vector<T>& row) { return row.size() == size; })) {
            throw std::invalid_argument("Invalid matrix dimensions");
        }
    }

    Matrix(size_t size) : size(size), data(size, std::vector<T>(size, T(0))) {
        for (size_t i = 0; i < size; ++i) {
            data.at(i).at(i) = T(1);
        }
    }

    Matrix(size_t size, const T& elem) : size(size), data(size, std::vector<T>(size, elem)) {}

    size_t getSize() const {
        return size;
    }

    T getElement(size_t row, size_t col) const {
        return data.at(row).at(col);
    }

    Matrix& operator+=(const Matrix& other) {
        if (size != other.size) {
            throw std::invalid_argument("Matrix dimensions do not match for addition");
        }
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                data.at(i).at(j) += other.data.at(i).at(j);
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        if (size != other.size) {
            throw std::invalid_argument("Matrix dimensions do not match for addition");
        }
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                data.at(i).at(j) -= other.data.at(i).at(j);
            }
        }
        return *this;
    }

    Matrix& operator*=(const Matrix& other) {
        *this = *this * other;
        return *this;
    }

    Matrix operator+(const Matrix& other) const {
        Matrix result = *this;
        result += other;
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        Matrix result = *this;
        result -= other;
        return result;
    }

    Matrix operator-() const {
        Matrix result = *this;
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.data.at(i).at(j) = -result.data.at(i).at(j);
            }
        }
        return result;
    }

     Matrix operator*(const Matrix& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication");
        }
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.data.at(i).at(j) = T(0);
                for (size_t k = 0; k < size; ++k) {
                    result.data.at(i).at(j) += data.at(i).at(k) * other.data.at(k).at(j);
                }
            }
        }
        return result;
    }

    Matrix operator*(T scalar) const {
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.data.at(i).at(j) = data.at(i).at(j) * scalar;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << "(";
        for (const auto& row : matrix.data) {
            os << "[";
            for (const auto& elem: row) {
                os << elem << ",";
            }
            os << "]" << ",";
        }
        os << ")";
        return os;
    }
};
