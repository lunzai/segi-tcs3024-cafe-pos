/*
Assignment: TCS3024 - Introductory Programming
Author: Leong Hean Luen
Student ID: SUKD2301060
Email: sukd2301060@segi4u.my
Contact: +6013 344 6652
*/

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <iostream>
#include <iomanip>
#include <typeinfo>
#include "order.h"

using namespace std;

class Navigation {
    public: 
    int menuHeaderWidth = 40;
    Order* order;

    Navigation() {}

    int main() {
        cout << endl;
        displayMenuHeader("Main Menu");
        cout << endl;
        cout << "[1] Show Menu" << endl;
        cout << "[2] Manage Order" << endl;
        cout << "[3] Show Config" << endl;
        cout << "[4] Order History" << endl;
        cout << "[5] Help" << endl;
        cout << "[0] Exit" << endl;
        return handleUserSelection(5);
    }

    int manageOrder() {
        cout << endl;
        displayMenuHeader("Manage Order");
        cout << endl;
        cout << "[1] Show Order" << endl;
        cout << "[2] Add Item To Order" << endl;
        cout << "[3] Update Item In Order" << endl;
        cout << "[4] Remove Item From Order" << endl;
        cout << "[5] Checkout" << endl;
        cout << "[6] Clear Order" << endl;
        cout << "[0] Back To Main Menu" << endl;
        return handleUserSelection(6);
    }

    void displayMenuHeader(string header) {
        cout << '+' << string(menuHeaderWidth, '-') << '+' << endl;
        cout << "| " << setw(menuHeaderWidth - 2) << left << header << " |" << endl;
        cout << '+' << string(menuHeaderWidth, '-') << '+' << endl;
    }

    private:

    int handleUserSelection(int numOfOptions, bool hasBack = true) {
        int option;
        int min = hasBack ? 0:1;
        bool isValid = false;
        cout << endl;
        do {
            cout << "Please select: ";
            cin >> option;
            if (cin.good() && option >= min && option <= numOfOptions) {
                isValid = true;
            } else {
                cout << "Invalid selection. ";
            }
            cin.clear();
            cin.ignore(10000, '\n');
        } while (isValid == false);
        cout << endl;
        return option;
    }
};

#endif