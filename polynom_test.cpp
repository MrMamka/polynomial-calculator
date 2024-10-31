#include <iostream>
#include <vector>

#include "polynom.cpp"

template <typename T>
bool checkPolynomialsEqual(const Polynomial<T>& poly1, const Polynomial<T>& poly2) {
    if (poly1.degree() != poly2.degree()) {
        return false;
    }
    for (size_t i = 0; i <= poly1.degree(); ++i) {
        if (poly1.getCoefficient(i) != poly2.getCoefficient(i)) {
            return false;
        }
    }
    return true;
}

void testResult(const std::string& testName, bool result) {
    std::cout << testName << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

void testDefaultConstructor() {
    Polynomial<int> poly;
    testResult("DefaultConstructor", poly.degree() == 0 && poly.getCoefficient(0) == 0);
}

void testConstantConstructor() {
    Polynomial<int> poly(5);
    testResult("ConstantConstructor", poly.degree() == 0 && poly.getCoefficient(0) == 5);
}

void testVectorConstructor() {
    std::vector<int> coeffs = {1, 2, 3};
    Polynomial<int> poly(coeffs);
    testResult("VectorConstructor", poly.degree() == 2 &&
                                    poly.getCoefficient(0) == 1 &&
                                    poly.getCoefficient(1) == 2 &&
                                    poly.getCoefficient(2) == 3);
}

void testStringConstructor() {
    Polynomial<int> poly("3x^2 + 2x + 1 - x + 5");
    testResult("StringConstructor", poly.degree() == 2 &&
                                    poly.getCoefficient(0) == 6 &&
                                    poly.getCoefficient(1) == 1 &&
                                    poly.getCoefficient(2) == 3);
}

void testAdditionAssignment() {
    Polynomial<int> poly1(std::vector<int>{1, 2, 3});
    Polynomial<int> poly2(std::vector<int>{1, 2, 3});
    poly1 += poly2;
    testResult("AdditionAssignment", poly1.degree() == 2 &&
                                     poly1.getCoefficient(0) == 2 &&
                                     poly1.getCoefficient(1) == 4 &&
                                     poly1.getCoefficient(2) == 6);
}

void testSubtractionAssignment() {
    Polynomial<int> poly1(std::vector<int>{1, 2, 3});
    Polynomial<int> poly2(std::vector<int>{3, 3, 3});
    poly1 -= poly2;
    testResult("SubtractionAssignment", poly1.degree() == 1 &&
                                        poly1.getCoefficient(0) == -2 &&
                                        poly1.getCoefficient(1) == -1);
}

void testMultiplicationAssignment() {
    Polynomial<int> poly1(std::vector<int>{1, 2, 3});
    Polynomial<int> poly2(std::vector<int>{1, 2, 3});
    poly1 *= poly2;
    testResult("MultiplicationAssignment", poly1.degree() == 4 &&
                                           poly1.getCoefficient(0) == 1 &&
                                           poly1.getCoefficient(1) == 4 &&
                                           poly1.getCoefficient(2) == 10 &&
                                           poly1.getCoefficient(3) == 12 &&
                                           poly1.getCoefficient(4) == 9);
}

void testMultiplicationByConstant() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    poly *= 2;
    testResult("MultiplicationByConstant", poly.degree() == 2 &&
                                           poly.getCoefficient(0) == 2 &&
                                           poly.getCoefficient(1) == 4 &&
                                           poly.getCoefficient(2) == 6);
}

void testAddition() {
    Polynomial<int> poly1(std::vector<int>{1, 2, 3});
    Polynomial<int> poly2(std::vector<int>{1, 2, 3});
    Polynomial<int> result = poly1 + poly2;
    testResult("Addition", result.degree() == 2 &&
                          result.getCoefficient(0) == 2 &&
                          result.getCoefficient(1) == 4 &&
                          result.getCoefficient(2) == 6);
}

void testSubtraction() {
    Polynomial<int> poly1(std::vector<int>{1, 2, 3});
    Polynomial<int> poly2(std::vector<int>{3, 3, 3});
    Polynomial<int> result = poly1 - poly2;
    testResult("Subtraction", result.degree() == 1 &&
                              result.getCoefficient(0) == -2 &&
                              result.getCoefficient(1) == -1);
}

void testMultiplication() {
    Polynomial<int> poly1(std::vector<int>{1, 2, 3});
    Polynomial<int> poly2(std::vector<int>{1, 2, 3});
    Polynomial<int> result = poly1 * poly2;
    testResult("Multiplication", result.degree() == 4 &&
                                 result.getCoefficient(0) == 1 &&
                                 result.getCoefficient(1) == 4 &&
                                 result.getCoefficient(2) == 10 &&
                                 result.getCoefficient(3) == 12 &&
                                 result.getCoefficient(4) == 9);
}

void testNegation() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    Polynomial<int> result = -poly;
    testResult("Negation", result.degree() == 2 &&
                          result.getCoefficient(0) == -1 &&
                          result.getCoefficient(1) == -2 &&
                          result.getCoefficient(2) == -3);
}

void testPower() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    Polynomial<int> result = poly.pow(2);
    testResult("Power", result.degree() == 4 &&
                       result.getCoefficient(0) == 1 &&
                       result.getCoefficient(1) == 4 &&
                       result.getCoefficient(2) == 10 &&
                       result.getCoefficient(3) == 12 &&
                       result.getCoefficient(4) == 9);
}


void testSubstitution() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    Polynomial<int> result = poly(Polynomial<int>(std::vector<int>{1, 2}));
    testResult("Substitution", result.degree() == 2 &&
                               result.getCoefficient(0) == 6 &&
                               result.getCoefficient(1) == 16 &&
                               result.getCoefficient(2) == 12);
}

void testEvaluation() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    testResult("Evaluation", poly(2) == 17);
}

void testMatrixEvaluation() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    Matrix<int> matrix(2);
    Matrix<int> result = poly(matrix);
    testResult("MatrixEvaluation", result.getSize() == 2 &&
                                   result.getElement(0, 0) == 7 &&
                                   result.getElement(0, 1) == 6 &&
                                   result.getElement(1, 0) == 6 &&
                                   result.getElement(1, 1) == 7);
}

void testOutputStream() {
    Polynomial<int> poly(std::vector<int>{1, 2, 3});
    std::ostringstream oss;
    oss << poly;
    testResult("OutputStream", oss.str() == "3x^2 + 2x + 1");
}

int main() {
    testDefaultConstructor();
    testConstantConstructor();
    testVectorConstructor();
    testStringConstructor();
    testAdditionAssignment();
    testSubtractionAssignment();
    testMultiplicationAssignment();
    testMultiplicationByConstant();
    testAddition();
    testSubtraction();
    testMultiplication();
    testMultiplicationByConstant();
    testNegation();
    testPower();
    testSubstitution();
    testEvaluation();
    testMatrixEvaluation();
    testOutputStream();

    return 0;
}
