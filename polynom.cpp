#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <regex>
#include <sstream>

#include "matrix.cpp"

template <typename T>
class Polynomial {
private:
    std::vector<T> coefficients;

public:
    void cleanUp() {
        while (coefficients.size() > 1 && coefficients.back() == T()) {
            coefficients.pop_back();
        }
    }

    Polynomial() : coefficients(1) {}

    explicit Polynomial(T constant) : coefficients(1, constant) {}

    explicit Polynomial(const std::vector<T>& coeffs) : coefficients(coeffs) {
        cleanUp();
    }

    explicit Polynomial(const std::string& str) : coefficients(1, 0) {
        std::string cleanedStr;
        for (char c : str) {
            if (!isspace(c)) {
                cleanedStr += c;
            }
        }

        std::vector<std::vector<std::string>> monoms;
        size_t i = 0;
        while (i < cleanedStr.size()) {
            monoms.push_back(std::vector<std::string>(3));

            size_t start = i;
            while (i < cleanedStr.size() && (cleanedStr.at(i) == '+' || cleanedStr.at(i) == '-')) {
                ++i;
            }
            monoms.back().at(0) = cleanedStr.substr(start, i - start);

            start = i;
            while (i < cleanedStr.size() && std::isdigit(cleanedStr.at(i))) {
                ++i;
            }
            monoms.back().at(1) = cleanedStr.substr(start, i - start);

            start = i;
            if (i < cleanedStr.size() && cleanedStr.at(i) == 'x') {
                ++i;
                if (i < cleanedStr.size() && cleanedStr.at(i) == '^') {
                    ++i;
                    while (i < cleanedStr.size() && std::isdigit(cleanedStr.at(i))) {
                        ++i;
                    }
                }
            }
            monoms.back().at(2) = cleanedStr.substr(start, i - start);
        }

        size_t maxDegree = 0;
        for (const std::vector<std::string>& monom : monoms) {
            ssize_t coefficient = 1;
            size_t signs = std::count(monom.at(0).begin(), monom.at(0).end(), '-');
            if (signs % 2 == 1) {
                coefficient = -1;
            }

            if (!monom.at(1).empty()) {
                coefficient *= std::stoll(monom.at(1));
            }

            size_t degree;
            if (monom.at(2).empty()) {
                degree = 0;
            } else if (monom.at(2) == "x") {
                degree = 1;
            } else if (monom.at(2).size() > 2 && monom.at(2).at(0) == 'x' && monom.at(2).at(1) == '^') {
                degree = std::stoll(monom.at(2).substr(2, monom.at(2).size() - 2));
            } else {
                throw std::invalid_argument("can not parse string polynomial");
            }

            if (degree > maxDegree) {
                maxDegree = degree;
                coefficients.resize(maxDegree + 1, 0);
            }

            coefficients.at(degree) += coefficient;
        }

        cleanUp();
    }


    size_t degree() const {
        return coefficients.size() - 1;
    }

    T getCoefficient(size_t degree) const {
        if (degree >= coefficients.size()) {
            return 0;
        }
        return coefficients.at(degree);
    }

    Polynomial& operator+=(T constant) {
        coefficients.at(0) += constant;
        return *this;
    }

    Polynomial& operator-=(T constant) {
        coefficients.at(0) -= constant;
        return *this;
    }

    Polynomial& operator+=(const Polynomial& other) {
        coefficients.resize(std::max(coefficients.size(), other.coefficients.size()), T());
        for (size_t i = 0; i < other.coefficients.size(); ++i) {
            coefficients.at(i) += other.coefficients.at(i);
        }
        cleanUp();
        return *this;
    }

    Polynomial& operator-=(const Polynomial& other) {
        coefficients.resize(std::max(coefficients.size(), other.coefficients.size()), T());
        for (size_t i = 0; i < other.coefficients.size(); ++i) {
            coefficients.at(i) -= other.coefficients.at(i);
        }
        cleanUp();
        return *this;
    }

    Polynomial& operator*=(const Polynomial& other) {
        std::vector<T> newCoefficients(coefficients.size() + other.coefficients.size() - 1, T());
        for (size_t i = 0; i < coefficients.size(); ++i) {
            for (size_t j = 0; j < other.coefficients.size(); ++j) {
                newCoefficients.at(i+j) += coefficients.at(i) * other.coefficients.at(j);
            }
        }
        coefficients = newCoefficients;
        cleanUp();
        return *this;
    }

    Polynomial& operator*=(T constant) {
        for (T& coeff : coefficients) {
            coeff *= constant;
        }
        cleanUp();
        return *this;
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        result += other;
        return result;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial result = *this;
        result -= other;
        return result;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result = *this;
        result *= other;
        return result;
    }

    Polynomial operator*(T constant) const {
        Polynomial result = *this;
        result *= constant;
        return result;
    }

    Polynomial operator-() const {
        std::vector<T> newCoefficients(coefficients.size(), T());
        for (size_t i = 0; i < coefficients.size(); ++i) {
            newCoefficients.at(i) = -coefficients.at(i);
        }
        return Polynomial(newCoefficients);
    }

    Polynomial pow(size_t n) const {
        Polynomial result(T(1));
        Polynomial base = *this;
        while (n > 0) {
            if (n % 2 == 1) {
                result *= base;
            }
            base *= base;
            n /= 2;
        }
        return result;
    }
    
    Polynomial operator()(const Polynomial& other) const {
        Polynomial result;
        for (ssize_t i = degree(); i >= 0; --i) {
            result *= other;
            result += coefficients.at(i);
        }
        return result;
    }

    T operator()(T x) const {
        auto result = T();
        for (ssize_t i = degree(); i >= 0; --i) {
            result = result * x + coefficients.at(i);
        }
        return result;
    }

    template <typename M>
    Matrix<M> operator()(const Matrix<M>& matrix) const {
        Matrix<M> result(matrix.getSize());
        for (int i = degree(); i >= 0; --i) {
            result = result * matrix + Matrix<M>(matrix.getSize(), coefficients.at(i));
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
        for (ssize_t i = poly.degree(); i >= 0; --i) {
            if (poly.coefficients.at(i) == 0) {
                continue;
            }

            if (i < poly.degree()) {
                os << " + ";
            }
            os << poly.coefficients.at(i);
            if (i > 0) {
                os << "x";
                if (i > 1) {
                    os << "^" << i;
                }
            }
        }
        return os;
    }
};
