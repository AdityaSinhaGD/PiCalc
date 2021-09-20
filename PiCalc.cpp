// PiCalc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>

void calculatePi(int start, int end, double& result) {

    double sum = 0.0;

    for (int i = start; i < end; i++) {
        int sign = std::pow(-1, i);
        double term = 1.0 / (2.0 * i + 1);
        sum += sign * term;
    }

    result += sum * 4.0;

}

int main()
{
    /*To see maximum number of available threads use std::thread::hardware_concurrency()*/

    int numTerms = 1E8;
    double result = 0.0;

    auto start = std::chrono::steady_clock::now();

    calculatePi(0, numTerms, result);

    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << std::setprecision(15) << result << std::endl;
    std::cout << "Calculation Time:" << duration;
    
}
