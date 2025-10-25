#include "pet.h"
using namespace std;

int Pet::getWeightClass() const //getter implementations
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
void Pet::setWeightClass(int weightClass) //setter implementations
{
    this->weightClass = weightClass;
}
void Pet::setAge(int age)
{
    this->age = age;
}

vector<double> Pet::idealDiet()
{
    vector<double> diet;
    //these are default values, edit in a proper function later
    if (weightClass == 1) // small
    {
        diet = {50.0, 20.0, 15.0, 10.0, 5.0}; // protein, fat, fiber, moisture, others
    }
    else if (weightClass == 2) // medium
    {
        diet = {60.0, 25.0, 10.0, 3.0, 2.0};
    }
    else if (weightClass == 3) // large
    {
        diet = {70.0, 20.0, 5.0, 3.0, 2.0};
    }
    else // extra large
    {
        diet = {80.0, 15.0, 3.0, 1.0, 1.0};
    }
    return diet;
}