#ifndef PET_H
#define PET_H

#include <string>
#include <vector>
using namespace std;

class Pet
{
private:
    int weightClass;
    int age;
    string breed;
    int mealsPerDay;

public:
    Pet(int weightClass, int age, string breed);
    ~Pet();

    vector<double> idealDiet();

    int getWeightClass() const;
    void setWeightClass(int weightClass);

    int getAge() const;
    void setAge(int age);

    string getBreed() const;

    int getMealsPerDay() const;
    void setMealsPerDay(int mealsPerDay);
};

#endif
