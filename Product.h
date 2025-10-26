#ifndef PRODUCT_H
#define PRODUCT_H

class Pet;

class Product
{
private:
    double fat;
    double protein;
    double score = 10.0;

    void balanceScore(double percentNutrient);
public:
    Product(double fat, double protein)
    {
        this->fat = fat;
        this->protein = protein;
    }

    ~Product()
    {      
    }

    double getFat() const;
    void setFat(double fat);

    double getProtein() const;
    void setProtein(double protein);


    double getScore() const;
    void setScore(double score);


    double computeScore(Pet& p);
};

#endif