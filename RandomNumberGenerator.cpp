//
// Created by Sageblatt on 28.07.2021.
//

#include "RandomNumberGenerator.h"

int RandomNumberGenerator::getRandomInt(int min, int max) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static default_random_engine e(seed);
    std::uniform_int_distribution<int> d(min, max);
    return d(e);
}

float RandomNumberGenerator::getRandomFloat(float min, float max) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static std::default_random_engine e(seed);
    uniform_real_distribution<float> d(min, max);
    return d(e);
}

double RandomNumberGenerator::getRandomDouble(double min, double max) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static std::default_random_engine e(seed);
    uniform_real_distribution<double> d(min, max);
    return d(e);
}
