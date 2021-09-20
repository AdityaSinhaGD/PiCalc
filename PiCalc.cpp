// PiCalc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

std::mutex piMtx;
void calculatePi(int start, int end, double& result) {

    double sum = 0.0;

    for (int i = start; i < end; i++) {
        int sign = std::pow(-1, i);
        double term = 1.0 / (2.0 * i + 1);
        sum += sign * term;
    }

    std::unique_lock<std::mutex> lock(piMtx);
    result += sum * 4.0;
    lock.unlock();
}

std::vector<int> calcChunks(int numTerms, int numThreads) {
    std::vector<int> chunks;
    int delta = numTerms / numThreads;
    int remaining = numTerms % numThreads;

    int start = 0;
    int step = 0;

    chunks.push_back(start);

    for (int i = 0; i < numThreads; i++) {
        step += delta;
        if (i == numThreads - 1) {
            step += remaining;
        }

        chunks.push_back(step);
        start = step;
    }

    return chunks;
}

int main()
{
    /*To see maximum number of available threads use std::thread::hardware_concurrency()*/
    std::vector<std::thread> threads;
    int numThreads = std::thread::hardware_concurrency();
    int numTerms = 1E8;
    double result = 0.0;

    auto start = std::chrono::steady_clock::now();

    std::vector<int> chunks = calcChunks(numTerms, numThreads);

    for (int i = 0; i < numThreads; i++) {
        threads.push_back(std::thread(calculatePi, chunks[i], chunks[i + 1], std::ref(result)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << std::setprecision(15) << result << std::endl;
    std::cout << "Calculation Time:" << duration;
    
}
