#include <BigNum/BigNum.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <ctime>

using namespace bignum::literals;

std::string readNum(const std::string& filename)
{
    std::ifstream file{ filename };
    std::string res;
    if (file.is_open())
    {
        file >> res;
        file.close();
    }
    return res;
}


bignum::BigNum calculate_pi() {
    auto C = bignum::BigNum("42698670.66633339581771288916065960827332088400250908280083800717885260515745759421630179991145566860134573716749408041139229273618126672819313688217058256346006679876648346079573598355233398548485458327624737749125075458503257821974567599121240039201532332127683544629648");
    auto S = bignum::BigNum("0");
    auto Mq = bignum::BigNum("1");
    auto Lq = bignum::BigNum("13591409");
    auto Xq = bignum::BigNum("1");

    for (size_t q = 0; q < 100 / 14 + 1; ++q) {
        S = S + Mq * Lq / Xq;
        Mq = Mq * bignum::BigNum(8 * (6 * q + 1) * (6 * q + 3) * (6 * q + 5));
        Mq = Mq / bignum::BigNum((q + 1) * (q + 1) * (q + 1));
        Lq = Lq + bignum::BigNum("545140134");
        Xq = Xq * bignum::BigNum("-262537412640768000");
    }

    return C / S;
}

int main()
{
    long precision;
    std::cout << "Enter precision of calculation" << std::endl;
    std::cin >> precision;
    bignum::BigNum::setMinimalPrecision(precision);

    bignum::BigNum сoeff{ readNum("coeff_string") };

    long start_time = clock();
    auto pi {calculate_pi()};
    long finish_time = clock();
    double duration = static_cast<double>(finish_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "Calculated pi: \n" << pi << std::endl;
    std::cout << "It takes " << duration << " s" << std::endl;
    std::string piStr{ (std::string)pi };
    std::string actualPiStr{ readNum("pi_string") };
    int32_t matchCount{ 0 };
    for (int32_t i{ 2 }; i < piStr.length(); ++i)
    {
        if (piStr[i] == actualPiStr[i])
        {
            matchCount++;
        }
        else
        {
            break;
        }
    }
    std::cout << "Matched " << matchCount << " digits" << std::endl;
    return 0;
}
