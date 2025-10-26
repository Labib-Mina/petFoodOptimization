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
    virtual ~Pet();  // make destructor virtual

    virtual vector<double> idealDiet(); // <--- make this virtual

    int getWeightClass() const;
    void setWeightClass(int weightClass);

    int getAge() const;
    void setAge(int age);

    string getBreed() const;

    int getMealsPerDay() const;
    void setMealsPerDay(int mealsPerDay);
};
