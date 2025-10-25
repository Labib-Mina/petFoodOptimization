#include "Product.h"
#include "Pet.h"
#include <cmath>

double Product::getFat() const
{
    return fat;
}

void Product::setFat(double fat)
{
    this->fat = fat;
}

double Product::getProtein() const
{
    return protein;
}

void Product::setProtein(double protein)
{
    this->protein = protein;
}

double Product::getScore() const
{
    return score;
}

void Product::setScore(double score)
{
    this->score = score;
}

void Product::balanceScore(double percentNutrient)
{
    if (percentNutrient >= 10)
    {
        score -= 3.0;
    }

    if (percentNutrient >= 25)
    {
        score -= (percentNutrient - 25) * 0.4;
    }

    if (percentNutrient <= -10)
    {
        score -= 1.5;
    }

    if (percentNutrient <= -25)
    {
        score += (percentNutrient + 25) * 0.1;
    }
}

double Product::computeScore(Pet& p)
{
    std::vector<double> ideal = p.idealDiet();
    double idealProtein = ideal.at(0);
    double idealFat = ideal.at(1);

    double score = 10.0;

    double actualProtein = protein * p.getMealsPerDay();
    double actualFat = fat * p.getMealsPerDay();

    //maybe change this to sqaured error later.... test it first tho
     //ex 550 - 230 = 320
    double diffProtein = actualProtein - idealProtein;
    double diffFat = actualFat - idealFat;

    double percentProtein = (diffProtein / idealProtein) * 100.0;
    double percentFat = (diffFat / idealFat) * 100.0;

    balanceScore(percentFat);
    balanceScore(percentProtein);

    return score;

}