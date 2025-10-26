#include "pet.h"
#include <vector>
#include <string>

using namespace std;

class Cat : public Pet
{
public:
    Cat(int weightClass, int age, string breed);
    ~Cat();
    vector<double> idealDiet();
    
};

vector<double> idealDiet();