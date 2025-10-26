#ifndef PRODUCT_H
#define PRODUCT_H

class Pet;

class Product
{
private:
    double fat;
    double sodium;
    double protein;
public:
    Product(double fat, double sodium, double protein)
    {
        this->fat = fat;
        this->sodium = sodium;
        this->protein = protein;
    }

    ~Product()
    {      
    }

    double computeScore(Pet& p);
};

#endif