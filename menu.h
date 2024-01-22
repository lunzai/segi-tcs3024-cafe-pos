/*
Assignment: TCS3024 - Introductory Programming
Author: Leong Hean Luen
Student ID: SUKD2301060
Email: sukd2301060@segi4u.my
Contact: +6013 344 6652
*/

#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "config.h"

using namespace std;

struct foodItem {
    string name;
    double price;
};

struct beverageItem {
    string name;
    double mediumPrice;
    double largePrice;
};

class Menu {
    private:

    const char* foodMenuFile = "menu/food.txt";
    const char* beverageMenuFile = "menu/beverage.txt";
    const char CHAR_COMMENT = '#';
    const char CHAR_DELIMITER = '|';
    const Config config;

    public :
    
    bool hasFoodItem = false;
    bool hasBeverageItem = false;
    int foodItemCount = 0;
    int beverageItemCount = 0;
    vector<foodItem> foodItems;
    vector<beverageItem> beverageItems;

    Menu(Config config) {
        ifstream foodFile(foodMenuFile);
        ifstream beverageFile(beverageMenuFile);
        if (!foodFile.is_open() || !beverageFile.is_open()) {
            cout << "[WARNING] Menu file not found or invalid. Please contact system administrator.";
            cout << endl;
            return;
        }
        loadFoodMenu(foodFile);
        loadBeverageMenu(beverageFile);
    }

    void loadFoodMenu(ifstream &file) {
        string line;
        while (getline(file, line)) {
            if (line[0] == CHAR_COMMENT || line.empty()) {
                continue;
            }
            size_t position = line.find(CHAR_DELIMITER);
            string name = line.substr(0, position);
            double price = stod(line.substr(position + 1));
            foodItems.push_back({ name, price });
            foodItemCount++;
            hasFoodItem = true;
        }
    }

    void loadBeverageMenu(ifstream &file) {
        string line;
        while (getline(file, line)) {
            if (line[0] == CHAR_COMMENT || line.empty()) {
                continue;
            }
            size_t mediumPosition = line.find(CHAR_DELIMITER);
            size_t largePosition = line.rfind(CHAR_DELIMITER);
            string name = line.substr(0, mediumPosition);
            double mediumPrice = stod(line.substr(mediumPosition + 1, largePosition - mediumPosition + 1));
            double largePrice = stod(line.substr(largePosition + 1));
            beverageItems.push_back({ name, mediumPrice, largePrice });
            beverageItemCount++;
            hasBeverageItem = true;
        }
    }

    void displayFoodItemSelection() {
        char const *format = "[%2i] %-35s %.2f\n";
        for (int i = 0; i < foodItemCount; i++) {
            printf(
                format, 
                i+1,
                foodItems[ i ].name.c_str(),
                foodItems[ i ].price
            );
        }
    }

    void displayBeverageItemSelection() {
        char const *format = "[%2i] %-35s (M) %-10.2f (L) %-10.2f\n";
        for (int i = 0; i < beverageItemCount; i++) {
            printf(
                format,
                i + 1,
                beverageItems[ i ].name.c_str(),
                beverageItems[ i ].mediumPrice,
                beverageItems[ i ].largePrice
            );
        }
    }

    void displayFoodMenu() {
        int tableWidth = 70;
        int priceWidth = 12;
        int descriptionWidth = tableWidth - priceWidth - 5;
        if (!hasFoodItem) {
            cout << "[INFO] Food menu - no record found." << endl;
            return;
        }
        // menu header
        cout << '+' << string(tableWidth, '-') << '+' << endl;
        cout << "| " << setw(descriptionWidth) << left << "Food menu" << " | " << setw(priceWidth) << "Price (" + config.currency + ")" << " |" << endl;
        cout << '+' << string(descriptionWidth + 2, '-') << '+' << string(priceWidth + 2, '-') << '+' << endl;
        // menu items
        for (int i = 0; i < foodItems.size(); i++) {
            cout << "| " << setw(descriptionWidth) << left << foodItems[ i ].name << " | ";
            cout << setw(priceWidth) << fixed << setprecision(2) << foodItems[ i ].price << " |" << endl;
            cout << '+' << string(descriptionWidth + 2, '-') << '+' << setprecision(2) << string(priceWidth + 2, '-') << '+' << endl;
        }
    }

    void displayBeverageMenu() {
        int tableWidth = 70;
        int priceWidth = 12;
        int descriptionWidth = tableWidth - (priceWidth * 2) - 8;
        if (!hasBeverageItem) {
            cout << "[INFO] Beverage menu - no record found." << endl;
            return;
        }
        // menu header
        cout << '+' << string(tableWidth, '-') << '+' << endl;
        cout << "| " << setw(descriptionWidth) << left << "Beverage menu" << " | ";
        cout << setw(priceWidth) << "Medium (" + config.currency + ")" << " | ";
        cout << setw(priceWidth) << "Large (" + config.currency + ")" << " |" << endl;
        cout << '+' << string(descriptionWidth + 2, '-') << '+';
        cout << string(priceWidth + 2, '-') << '+' << string(priceWidth + 2, '-') << '+' << endl;
        // menu items
        for (int i = 0; i < beverageItems.size(); i++) {
            cout << "| " << setw(descriptionWidth) << left << beverageItems[ i ].name << " | ";
            cout << setw(priceWidth) << fixed << setprecision(2) << beverageItems[ i ].mediumPrice << " | ";
            cout << setw(priceWidth) << fixed << setprecision(2) << beverageItems[ i ].largePrice << " |" << endl;
            cout << '+' << string(descriptionWidth + 2, '-') << '+';
            cout << string(priceWidth + 2, '-') << '+' << string(priceWidth + 2, '-') << '+' << endl;
        }
    }

};

#endif