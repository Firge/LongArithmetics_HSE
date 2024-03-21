#include <BigNum/BigNum.hpp>
#include <gtest/gtest.h>

using namespace bignum::literals;

TEST(Operations, Division)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{ "5" } / bignum::BigNum{ "2" },
        bignum::BigNum{ "2.5" });
}

TEST(Operations, DivisionZero)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_THROW("2"_BN / "0"_BN, bignum::ZeroDivisionException);
}

TEST(Operations, Division_Whole)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ("2"_BN / "0.5"_BN, "4"_BN);
}


TEST(Operations, Multiplication_integer)
{
    EXPECT_EQ(
        bignum::BigNum{"12"} * bignum::BigNum{"5"},
        bignum::BigNum{"60"});
}

TEST(Operations, Multiplication_decimal)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.532892359238723458247834783587357845782478249824893578947845782478239823953013412387" } *
            bignum::BigNum{ "0.013847573271544723872848562129120101029293845474732892398349355353398891823824823" },
        bignum::BigNum{
            "0.007379265990404556058329326132289801815521995154283447034859252825741903236071734233191206166718958663754110494034393713380302836648339733173060750333897593346282501" });
}

TEST(Operations, Sum_Positive)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{"0.532892359238723458247834783587357845782478249824893578947845782478239823953013412387" } +
            bignum::BigNum{ "0.013847573271544723872848562129120101029293845474732892398349355353398891823824823" },
        bignum::BigNum{ "0.546739932510268182120683345716477946811772095299626471346195137831638715776838235387" });
}

TEST(Operations, Sum_Negative) { EXPECT_EQ("-11.6"_BN + -"23.5"_BN, "-35.1"_BN); }

TEST(Operations, Sum_PositiveNegative) { EXPECT_EQ("11.6"_BN + "-23.5"_BN, "-11.9"_BN); }

TEST(Operations, Sum_DigitAdd) { EXPECT_EQ("0.999999999"_BN + "0.000000001"_BN, 1_BN); }

TEST(Operations, Difference_Positive)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{"0.532892359238723458247834783587357845782478249824893578947845782478239823953013412387" } -
            bignum::BigNum{"0.013847573271544723872848562129120101029293845474732892398349355353398891823824823" },
        bignum::BigNum{ "0.519044785967178734374986221458237744753184404350160686549496427124840932129188589387" });
}

TEST(Operations, Difference_PositiveNegative) { EXPECT_EQ("950.0001"_BN - "-14.7"_BN, "964.7001"_BN); }

TEST(Operations, Pow)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(bignum::BigNum::pow("3.49"_BN, 4_BN), "148.35483601"_BN);
}

TEST(Operations, Factorial) { EXPECT_EQ("11"_BN.factorial(), "39916800"_BN); }

// comparators
TEST(Comparison, LT) { EXPECT_LT(645786709.0678_BN, 9999999999_BN); }

TEST(Comparison, LE_Equal) { EXPECT_LE(1485.5_BN, 1485.5_BN); }

TEST(Comparison, LE_Less) { EXPECT_LE(1_BN, "1.000000000001"_BN); }

TEST(Comparison, GT) { EXPECT_GT(100_BN, "0.000098000001"_BN); }

TEST(Comparison, EQ){EXPECT_EQ(bignum::BigNum{ "459.90000123" }, "459.90000123"_BN);}

TEST(Comparison, EQ_Substract) { EXPECT_EQ("6.061"_BN - "0"_BN, "6.061"_BN); }

TEST(Comparison, NE) { EXPECT_NE(bignum::BigNum{ "-1.1" }, "1.1"_BN); }

TEST(Comparison, EQ_Zero) { EXPECT_EQ(bignum::BigNum{ "-0" }, "0"_BN); }

// conv
TEST(General, ToString) { EXPECT_EQ((std::string)bignum::BigNum{ -345 }, "-345"); }

TEST(General, ToString_SmallExp) { EXPECT_EQ((std::string)bignum::BigNum{ "0.048701" }, "0.048701"); }

TEST(General, ToString_Zero) { EXPECT_EQ((std::string)bignum::BigNum{ -0 }, "0"); }

// services

TEST(General, GetPrecision)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(bignum::BigNum::getMinimalPrecision(), 100);
}

TEST(General, Cout) { EXPECT_NO_THROW(std::cout << bignum::BigNum{ -987456321 }); }

TEST(General, DemoCase)
{
    bignum::BigNum bignumFromString{ "-234.567" };
    bignum::BigNum numFromBaseType1{ 13948 };
    bignum::BigNum numFromBaseType2{ 13948.333 };
    12_BN - 3.85_BN;
    "94815.7902"_BN;

    1 / bignumFromString;
    bignumFromString.inverse();

    bignum::BigNum::setMinimalPrecision(100);
    auto a{ -1234.123_BN };
    bignum::BigNum b{ 1234.01 };

    (a * b);
    (a / b);
    (a + b);
    (a - b);

    (a > b);
    (a < b);
    (a <= b);
    (a >= b);
    (a == b);
    (a <=> b);
}
