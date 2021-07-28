//
// Created by Sageblatt on 28.07.2021.
//

#ifndef THE_ARK_RANDOMNUMBERGENERATOR_H
#define THE_ARK_RANDOMNUMBERGENERATOR_H

#include <chrono>
#include <random>

using std::default_random_engine;
using std::uniform_real_distribution;

class RandomNumberGenerator
{
public:
    int getRandomInt(int min, int max);
    float getRandomFloat(float min, float max);
    double getRandomDouble(float min, float max);
};

#endif //THE_ARK_RANDOMNUMBERGENERATOR_H
