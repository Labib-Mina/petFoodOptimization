#include "pet.h"
#include "Product.h"
#include <iostream>
#include <vector>
#include <iomanip> // for fixed and setprecision
using namespace std;

int main()
{
    cout << "==== Pet & Product Interactive Test ====\n";

    int weightClass, age, meals;
    string breed;

    cout << "Enter pet weight class (1 = small, 2 = medium, 3 = large): ";
    cin >> weightClass;
    cout << "Enter pet age: ";
    cin >> age;
    cout << "Enter breed: ";
    cin >> breed;
    cout << "Enter meals per day: ";
    cin >> meals;

    // create the Pet object
    Pet pet(weightClass, age, breed);
    pet.setMealsPerDay(meals);

    cout << "\n=== Pet Info ===\n";
    cout << "Breed: " << pet.getBreed() << endl;
    cout << "Age: " << pet.getAge() << " years" << endl;
    cout << "Weight Class: " << pet.getWeightClass() << endl;
    cout << "Meals per day: " << pet.getMealsPerDay() << endl;

    // get the pet’s ideal daily nutrition
    vector<double> ideal = pet.idealDiet();
    cout << fixed << setprecision(2);
    cout << "\nIdeal daily nutrients:\n";
    cout << "  Protein: " << ideal.at(0) << " g/day\n";
    cout << "  Fat:     " << ideal.at(1) << " g/day\n";

    // now enter the product’s nutrient content
    double fat, protein;
    cout << "\nEnter product fat per meal (g): ";
    cin >> fat;
    cout << "Enter product protein per meal (g): ";
    cin >> protein;

    // create the Product object
    Product product(fat, protein);

    // compute score
    double score = product.computeScore(pet);

    // calculate actual daily intake for display
    double actualProtein = product.getProtein() * pet.getMealsPerDay();
    double actualFat = product.getFat() * pet.getMealsPerDay();

    cout << "\n=== Nutrition Comparison ===\n";
    cout << "Protein: " << actualProtein << " g/day (ideal " << ideal.at(0)
         << ", diff " << (actualProtein - ideal.at(0)) << ")\n";
    cout << "Fat:     " << actualFat << " g/day (ideal " << ideal.at(1)
         << ", diff " << (actualFat - ideal.at(1)) << ")\n";

    cout << "\nComputed Score: " << score << " / 10\n";

    // allow user to modify values
    cout << "\n--- Try modifying the product ---\n";
    double newFat, newProtein;
    cout << "Enter new fat per meal (g): ";
    cin >> newFat;
    product.setFat(newFat);
    cout << "Enter new protein per meal (g): ";
    cin >> newProtein;
    product.setProtein(newProtein);

    // recompute
    score = product.computeScore(pet);
    actualProtein = product.getProtein() * pet.getMealsPerDay();
    actualFat = product.getFat() * pet.getMealsPerDay();

    cout << "\n=== Updated Nutrition ===\n";
    cout << "Protein: " << actualProtein << " g/day (ideal " << ideal.at(0)
         << ", diff " << (actualProtein - ideal.at(0)) << ")\n";
    cout << "Fat:     " << actualFat << " g/day (ideal " << ideal.at(1)
         << ", diff " << (actualFat - ideal.at(1)) << ")\n";
    cout << "\nNew Computed Score: " << score << " / 10\n";

    cout << "\n==== End of Program ====\n";
    return 0;
}

