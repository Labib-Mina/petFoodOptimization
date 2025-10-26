#include "pet.h"
using namespace std;

Pet::Pet(int weightClass, int age, string breed)
{
    this->weightClass = weightClass;
    this->age = age;
    this->breed = breed;
    this->mealsPerDay = 0; // default value
}

Pet::~Pet() {}

int Pet::getWeightClass() const
{
    return weightClass;
}

int Pet::getAge() const
{
    return age;
}

string Pet::getBreed() const
{
    return breed;
}

int Pet::getMealsPerDay() const
{
    return mealsPerDay;
}

void Pet::setWeightClass(int weightClass)
{
    this->weightClass = weightClass;
}

void Pet::setAge(int age)
{
    this->age = age;
}

void Pet::setMealsPerDay(int mealsPerDay)
{
    this->mealsPerDay = mealsPerDay;
}

vector<double> Pet::idealDiet()
{
    vector<double> standard = {14.5, 5.5}; // base protein/fat

    if (age < 2)
    {
        standard.at(0) *= 0.8;
        standard.at(1) *= 0.8;
    }
    else if (age >= 9)
    {
        standard.at(0) *= 1.2;
        standard.at(1) *= 1.2;
    }

    switch (weightClass)
    {
        case 1: // small
            standard.at(0) *= 0.9;
            standard.at(1) *= 0.9;
            break;
        case 3: // large
            standard.at(0) *= 1.1;
            standard.at(1) *= 1.1;
            break;
        default:
            break;
    }

    return standard;
}
