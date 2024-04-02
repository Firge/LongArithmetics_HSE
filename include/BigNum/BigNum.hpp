#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

namespace bignum
{

class ZeroDivisionException : public std::runtime_error
{
    public:
        ZeroDivisionException() : std::runtime_error("ZeroDivisionError") {}
};

class BigNum
{
    using DigitType = int64_t;

public:
    BigNum();

//    template <typename Number,
//              typename std::enable_if<
//                  std::is_integral<Number>::value ||
//                  std::is_floating_point<Number>::value>::type* = nullptr>
//    BigNum(Number number) : BigNum{ std::to_string(number) }
//    {
//    }

    static BigNum fromInteger(int64_t val) { return BigNum{ std::to_string(val) }; }
    static BigNum fromUnsignedInteger(uint64_t val) { return BigNum{ std::to_string(val) }; }
    static BigNum fromDouble(long double val) { return BigNum{ std::to_string(val) }; }

    BigNum(const std::string&);

    static void setMinimalPrecision(uint64_t);
    static uint64_t getMinimalPrecision();

    static BigNum pow(const BigNum&, const BigNum&);

    BigNum operator-() const;

    friend BigNum& operator-=(BigNum&, const BigNum&);
    friend BigNum operator+(const BigNum&, const BigNum&);
    friend BigNum operator-(const BigNum&, const BigNum&);
    friend BigNum operator*(const BigNum&, const BigNum&);
    friend BigNum operator/(const BigNum&, const BigNum&);

    friend std::strong_ordering operator<=>(const BigNum &, const BigNum &);
    friend bool operator==(const BigNum &, const BigNum &);

    explicit operator std::string() const;

    [[nodiscard]] BigNum inverse() const;
    [[nodiscard]] BigNum factorial() const;

private:
    template <typename Iterator>
    BigNum(const Iterator& begin, const Iterator& end)
        : m_Digits(begin, end)
    {
    }

    static void _commonExponent(BigNum&, BigNum&);
    static void _commonLength(BigNum&, BigNum&);

    void _normalize();

    constexpr const DigitType& operator[](int32_t i) const;
    constexpr DigitType& operator[](int32_t i);

    static uint64_t s_Precision;
    static constexpr int64_t s_Base{ 10 };

    std::vector<DigitType> m_Digits;
    bool m_Negative{ false };
    int64_t m_Exponent{ 1 };
};

std::ostream& operator<<(std::ostream&, const BigNum&);

std::strong_ordering operator<=>(const BigNum &, const BigNum &);

BigNum operator+(const BigNum&, const BigNum&);
BigNum operator-(const BigNum&, const BigNum&);
BigNum operator*(const BigNum&, const BigNum&);
BigNum operator/(const BigNum&, const BigNum&);
BigNum& operator-=(BigNum&, const BigNum&);

bool operator==(const BigNum &, const BigNum &);
bool operator!=(const BigNum &, const BigNum &);


inline BigNum operator""_BN(unsigned long long num) { return BigNum::fromUnsignedInteger(num); }
inline BigNum operator""_BN(long double num) { return BigNum::fromDouble(num); }
inline BigNum operator"" _BN(const char* str, size_t len) { return BigNum{ str }; }

}
