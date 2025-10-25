#include "cat.h"
#include <vector>
#include <string>

using namespace std;

Cat::Cat(int weightClass, int age, string breed) : Pet(weightClass, age, breed)
{

}

Cat::~Cat()
{
}

vector<double> Cat::idealDiet()
{
    vector<double> diet;
    // Placeholder logic for ideal diet calculation
    if (getWeightClass() == 1) // small
    {
        diet = {50.0, 30.0, 20.0}; // carbs, protein, fat percentages
    }
    else if (getWeightClass() == 2) // medium
    {
        diet = {40.0, 40.0, 20.0};
    }
    else // large
    {
        diet = {30.0, 50.0, 20.0};
    }
    return diet;
}