#include <BigNum/BigNum.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace bignum
{

uint64_t BigNum::s_Precision{ 100 };

void BigNum::setMinimalPrecision(uint64_t value) { s_Precision = value; }

uint64_t BigNum::getMinimalPrecision() { return BigNum::s_Precision; }

BigNum::BigNum() : m_Digits(1, 0) {}

BigNum BigNum::pow(const BigNum& base, const BigNum& power)
{
    BigNum result = 1_BN;
    for (BigNum i = 0_BN; i < power; i = i + 1_BN) result = result * base;
    return result;
}

BigNum::BigNum(const std::string& nums)
{
    std::string digits{ nums };

    if (nums[0] == '-') {
        this->m_Negative = true;
        digits = nums.substr(1);
    }

    m_Exponent = static_cast<int32_t >(nums.size()) - static_cast<int32_t >(this->m_Negative);

    for (int32_t i{ 0 }; i < digits.size(); ++i) {
        if (digits[i] != '.') this->m_Digits.push_back(digits[i] - '0');
        else this->m_Exponent = i;
    }

    this->_removeInsignificantZeroes();
}



void BigNum::_normalize()
{
    DigitType trans;
    for (int32_t i{ static_cast<int32_t>(m_Digits.size()) - 1 }; i > 0; --i) {
        if (m_Digits[i] >= BigNum::s_Base) {
            trans = m_Digits[i] / BigNum::s_Base;
            m_Digits[i - 1] += trans;
            m_Digits[i] -= trans * BigNum::s_Base;
        }
        else if (m_Digits[i] < 0) {
            trans = (m_Digits[i] + 1) / BigNum::s_Base - 1;
            m_Digits[i - 1] += trans;
            m_Digits[i] -= trans * BigNum::s_Base;
        }
    }
}

void BigNum::_removeInsignificantZeroes()
{
    while (this->m_Digits.size() > (m_Exponent > 1L? m_Exponent: 1L) &&
           !this->m_Digits.back())
    {
        this->m_Digits.pop_back();
    }

    while (!this->m_Digits.empty() && !this->m_Digits.front())
    {
        this->m_Digits.erase(this->m_Digits.begin());
        this->m_Exponent--;
    }
}

BigNum BigNum::factorial() const
{
    BigNum result = 1_BN ;
    for (BigNum i = 2_BN; i <= *this; i = i + 1_BN){
        result = result * i;
    }
    return result;
}

BigNum BigNum::inverse() const
{
    if (*this == 0_BN) throw ZeroDivisionException{};

    BigNum copy{ *this };
    copy.m_Negative = false;
    BigNum result;
    result.m_Exponent = 1;
    result.m_Negative = this->m_Negative;
    result.m_Digits = {};
    BigNum dividend = 1_BN;

    while (copy < 1_BN) {
        copy.m_Exponent++;
        result.m_Exponent++;
    }

    while (dividend < copy) dividend.m_Exponent++;

    result.m_Exponent -= dividend.m_Exponent - 1;

    uint64_t currentDigitsEvald{ 0 };
    uint64_t totalDigits{ result.m_Exponent>0L?result.m_Exponent:0L +
                          BigNum::s_Precision + 2 }; // for rounding
    BigNum::DigitType digit;
    do {
        digit = 0;
        while (dividend >= copy) {
            digit++;
            dividend -= copy;
        }

        dividend.m_Exponent++;
        dividend._removeInsignificantZeroes();
        result.m_Digits.push_back(digit);
        currentDigitsEvald++;
    } while (dividend != 0_BN && currentDigitsEvald < totalDigits);

    return result;
}

void BigNum::_commonExponent(BigNum& num1, BigNum& num2)
{
    while (num1.m_Exponent < num2.m_Exponent) {
        num1.m_Digits.insert(num1.m_Digits.begin(), 0);
        num1.m_Exponent++;
    }
    while (num2.m_Exponent < num1.m_Exponent) {
        num2.m_Digits.insert(num2.m_Digits.begin(), 0);
        num2.m_Exponent++;
    }
}

void BigNum::_commonLength(BigNum& num1, BigNum& num2) {
    while (num1.m_Digits.size() < num2.m_Digits.size()) num1.m_Digits.push_back(0);
    while (num2.m_Digits.size() < num1.m_Digits.size()) num2.m_Digits.push_back(0);
}

BigNum BigNum::operator-() const {
    BigNum copy{ *this };
    copy.m_Negative = !copy.m_Negative;
    return copy;
}

BigNum& operator-=(BigNum& a, const BigNum& b) { return (a = a - b); }

BigNum operator+(const BigNum& a, const BigNum& b) {
    if (a.m_Negative) {
        if (b.m_Negative) return -(-a + (-b));
        return b - (-a);
    }
    if (b.m_Negative) return a - (-b);

    BigNum num1{ a };
    BigNum num2{ b };
    BigNum::_commonExponent(num1, num2);

    BigNum result;
    result.m_Digits.resize(std::max(num1.m_Digits.size(), num2.m_Digits.size()) + 1);

    BigNum::_commonLength(num1, num2);

    for (int32_t i{ 0 }; i < result.m_Digits.size() - 1; ++i) {
        result[i + 1] = num1[i] + num2[i];
    }
    result.m_Exponent = std::max(a.m_Exponent, b.m_Exponent) + 1;
    result._normalize();
    result._removeInsignificantZeroes();
    return result;
}

BigNum operator-(const BigNum& a, const BigNum& b) {
    if (b.m_Negative) return a + (-b);
    else if (a.m_Negative) return -(-a + b);
    else if (a < b) return -(b - a);

    BigNum num1{ a };
    BigNum num2{ b };
    BigNum::_commonExponent(num1, num2);

    BigNum result;
    result.m_Digits.resize(std::max(num1.m_Digits.size(), num2.m_Digits.size()) + 1);

    BigNum::_commonLength(num1, num2);

    for (int32_t i{ 0 }; i < result.m_Digits.size() - 1; ++i) {
        result[i + 1] = num1[i] - num2[i];
    }
    result.m_Exponent = std::max(a.m_Exponent, b.m_Exponent) + 1;
    result._normalize();
    result._removeInsignificantZeroes();
    return result;
}

BigNum operator*(const BigNum& a, const BigNum& b) {
    BigNum result;
    result.m_Digits.resize(a.m_Digits.size() + b.m_Digits.size());

    for (int32_t i{ 0 }; i < a.m_Digits.size(); ++i) {
        for (int32_t j{ 0 }; j < b.m_Digits.size(); ++j) {
            result[i + j + 1] += a[i] * b[j];
        }
    }
    result.m_Exponent = a.m_Exponent + b.m_Exponent;
    result.m_Negative = a.m_Negative != b.m_Negative;
    result._normalize();
    result._removeInsignificantZeroes();
    return result;
}

BigNum operator/(const BigNum& a, const BigNum& b) {
    auto inv{ b.inverse() };
    auto result{ a * inv };
    auto leaveDigits{ (result.m_Exponent > 0 ? result.m_Exponent : 0) + BigNum::s_Precision };
    result.m_Digits.resize(std::min(result.m_Digits.size(), leaveDigits));
    return result;
}

BigNum::operator std::string() const {
    if (this->m_Digits.empty()) return "0";

    std::string result;
    int32_t i{ 0 };
    int64_t exp{ this->m_Exponent };
    if (exp <= 0) {
        result += "0.";
        result += std::string(std::abs(exp), '0');
        exp = 0;
    }

    for (i = 0; i < std::min(static_cast<int64_t>(this->m_Digits.size()), this->m_Exponent); ++i) {
        result += std::to_string(this->m_Digits[i]);
        exp--;
    }

    while (i < exp) {
        result += "0";
        --exp;
    }

    if (this->m_Exponent < this->m_Digits.size() && this->m_Exponent > 0) result += ".";

    for (; i < this->m_Digits.size(); ++i) {
        result += std::to_string(this->m_Digits[i]);
    }

    return (m_Negative ? "-" : "") + result;
}

std::strong_ordering operator<=>(const BigNum& a, const BigNum& b) {
    using namespace bignum;

    if (a.m_Digits.empty() && b.m_Digits.empty()) return std::strong_ordering::equal;

    if (a.m_Negative != b.m_Negative) {
        return a.m_Negative < b.m_Negative? std::strong_ordering::greater : std::strong_ordering::less;
    }

    if (a.m_Exponent != b.m_Exponent) {
        return ((a.m_Exponent > b.m_Exponent) ^ (a.m_Negative)? std::strong_ordering::greater : std::strong_ordering::less);
    }

    BigNum aCp{ a };
    BigNum bCp{ b };
    while (aCp.m_Digits.size() > bCp.m_Digits.size()) bCp.m_Digits.push_back(0);

    while (aCp.m_Digits.size() < bCp.m_Digits.size()) aCp.m_Digits.push_back(0);

    for (int32_t i{ 0 }; i < aCp.m_Digits.size(); ++i) {
        if (aCp[i] != bCp[i]) {
            return ((aCp[i] > bCp[i]) ^ (aCp.m_Negative)? std::strong_ordering::greater: std::strong_ordering::less);
        }
    }
    return std::strong_ordering::equal;
}

constexpr const BigNum::DigitType& BigNum::operator[](int32_t i) const {
    return this->m_Digits[i];
}

constexpr BigNum::DigitType& BigNum::operator[](int32_t i) {
    return this->m_Digits[i];
}

std::ostream& operator<<(std::ostream& os, const BigNum& n) {
    return os << (std::string)n;
}

bool operator==(const BigNum& a, const BigNum& b) {
    return (a <=> b) == 0;
}

bool operator!=(const BigNum& a, const BigNum& b) {
    return (a <=> b) != 0;
}
}
