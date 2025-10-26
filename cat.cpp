#include "pet.h"
#include <vector>
#include <string>

class Cat : public Pet
{
public:
    Cat(int weightClass, int age, std::string breed);
    ~Cat();
    std::vector<double> idealDiet() override;    
};

Cat::Cat(int weightClass, int age, std::string breed)
: Pet(weightClass, age, breed)
{}

Cat::~Cat() {}

std::vector<double> Cat::idealDiet()
{
    std::vector<double> standard = Pet::idealDiet();
    standard[0] *= 1.1; // Example: cats may need 10% more protein
    return standard;
}
