#include "Product.h"
#include "Pet.h"
#include <cmath>

double Product::computeScore(Pet& p)
{
    std::vector<double> ideal = p.idealDiet();
    double idealSodium = ideal.at(0);
    double idealProtein = ideal.at(1);
    double idealFat = ideal.at(2);

    double score1, score2, score3, total = 0.0;
    double range = 0.2;

    //maybe change this to sqaured error later.... test it first tho
    int m1Diff1 = std::abs(idealSodium - sodium * p.getMealsPerDay()); //ex 550 - 230 = 320
    int m1Diff2 = std::abs(idealProtein - protein * p.getMealsPerDay());
    int m1Diff3 = std::abs(idealFat - fat * p.getMealsPerDay());

    return m1Diff1 + m1Diff2 + m1Diff3;

}