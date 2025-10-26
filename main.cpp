#include "pet.h"
#include "Product.h"
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[])
{
    int weightClass, age, meals;
    string breed;
    double fat, protein, newFat, newProtein;

    if (argc == 9) {
        weightClass = stoi(argv[1]);
        age = stoi(argv[2]);
        breed = argv[3];
        meals = stoi(argv[4]);
        fat = stod(argv[5]);
        protein = stod(argv[6]);
        newFat = stod(argv[7]);
        newProtein = stod(argv[8]);
    } else {
        cout << "Enter weight class (1=small, 2=medium, 3=large): ";
        cin >> weightClass;
        cout << "Enter age: ";
        cin >> age;
        cout << "Enter breed: ";
        cin.ignore();
        getline(cin, breed);
        cout << "Enter meals per day: ";
        cin >> meals;
        cout << "Enter fat per meal (g): ";
        cin >> fat;
        cout << "Enter protein per meal (g): ";
        cin >> protein;
        cout << "Enter new fat per meal (g): ";
        cin >> newFat;
        cout << "Enter new protein per meal (g): ";
        cin >> newProtein;
    }

    Pet pet(weightClass, age, breed);
    pet.setMealsPerDay(meals);

    cout << fixed << setprecision(2);
    cout << "Breed: " << pet.getBreed() << "\n";
    cout << "Age: " << pet.getAge() << " years\n";
    cout << "Weight Class: " << pet.getWeightClass() << "\n";
    cout << "Meals per day: " << pet.getMealsPerDay() << "\n";

    vector<double> ideal = pet.idealDiet();
    cout << "Ideal daily nutrients:\n";
    cout << "  Protein: " << ideal[0] << " g/day\n";
    cout << "  Fat:     " << ideal[1] << " g/day\n";

    // --- Initial product ---
    Product product(fat, protein);
    double initialScore = product.computeScore(pet);
    double actualProtein = product.getProtein() * pet.getMealsPerDay();
    double actualFat = product.getFat() * pet.getMealsPerDay();

    cout << "\n--- Initial Product ---\n";
    cout << "Protein: " << actualProtein << " g/day (ideal " << ideal[0]
         << ", diff " << (actualProtein - ideal[0]) << ")\n";
    cout << "Fat:     " << actualFat << " g/day (ideal " << ideal[1]
         << ", diff " << (actualFat - ideal[1]) << ")\n";
    cout << "Computed Score: " << initialScore << " / 10\n";

    // --- Adjusted product ---
    product.setFat(newFat);
    product.setProtein(newProtein);

    double newScore = product.computeScore(pet);
    actualProtein = product.getProtein() * pet.getMealsPerDay();
    actualFat = product.getFat() * pet.getMealsPerDay();

    cout << "\n--- Adjusted Product ---\n";
    cout << "Protein: " << actualProtein << " g/day (ideal " << ideal[0]
         << ", diff " << (actualProtein - ideal[0]) << ")\n";
    cout << "Fat:     " << actualFat << " g/day (ideal " << ideal[1]
         << ", diff " << (actualFat - ideal[1]) << ")\n";
    cout << "New Computed Score: " << newScore << " / 10\n";

    return 0;
}
