/*
Assignment: TCS3024 - Introductory Programming
Author: Leong Hean Luen
Student ID: SUKD2301060
Email: sukd2301060@segi4u.my
Contact: +6013 344 6652
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "config.h"
#include "order.h"
#include "navigation.h"
#include "menu.h"

using namespace std;

// init core modules
Config config;
Order order;
Navigation navi;
Menu menu(config);

// function declaration for aesthetic purposes
void displayHelp();
void displayConfig();
void displayContinue();
void displayOrderHistory();
void displayFoodMenu();
void displayBeverageMenu();
void handleOrderBackToMainMenu();
void handleOrderClear();
void handleOrderCreate();
void handleOrderAddItem();
void handleOrderRemoveItem();
void handleOrderCheckout();
void handleOrderUpdateItem();
void handleOrderAddFood();
void handleOrderAddBeverage();
int handleOrderAddItemSelection();
void displayOrderItemSelection();
int handleOrderItemSelection();
bool confirm();
void displayOrder();
void displayContinue();

int main() {
    if (config.isLoaded == false) {
        cout << "[WARNING] Config file not found or invalid. Contact system administrator.";
        return 0;
    }
    order.setConfig(config);
    int menuSelection, orderSelection;
    
    cout << endl;
    cout << "WELCOME TO " + config.cafeName << endl;
    do {
        menuSelection = navi.main();
        switch (menuSelection) {
            // show menu
            case 1:
                menu.displayFoodMenu();
                cout << endl;
                menu.displayBeverageMenu();
                displayContinue();
                break;
            // show / create order
            case 2: 
                do {
                    if (!order.isCreated) {
                        order.create();
                    }
                    orderSelection = navi.manageOrder();
                    switch (orderSelection) {
                        // show order
                        case 1:
                            displayOrder();
                            displayContinue();
                            break;
                        // add item
                        case 2:
                            handleOrderAddItem();
                            displayContinue();
                            break;
                        // update item
                        case 3:
                            handleOrderUpdateItem();
                            displayContinue();
                            break;
                        // remove item
                        case 4:
                            handleOrderRemoveItem();
                            displayContinue();
                            break;
                        // checkout
                        case 5:
                            handleOrderCheckout();
                            displayContinue();
                            break;
                        // clear order
                        case 6:
                            handleOrderClear();
                            orderSelection = 0;
                            displayContinue();                            
                            break;
                        // back to main menu
                        case 0:
                            handleOrderBackToMainMenu();
                            break;
                    }
                } while (orderSelection != 0);
                break;
            // show config
            case 3: 
                displayConfig();
                displayContinue();
                break;
            // order history
            case 4: 
                displayOrderHistory();
                displayContinue();
                break;
            // help
            case 5: 
                displayHelp();
                displayContinue();
                break;
            case 0:
                cout << "Thank you and goodbye." << endl;
                cout << endl;
        }
    } while (menuSelection != 0);
    return 0;
}

bool confirm(string message) {
    char answer;
    cout << message;
    cout << "(Y) Yes / (N) No: ";
    do {
        cin >> answer;
        if (cin.good()) {
            if (answer == 'y' || answer == 'Y') {
                return true;
            } else if (answer == 'n' || answer == 'N') {
                return false;
            }
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please input (Y) Yes / (N) No:";
    } while (true);
}

void handleOrderBackToMainMenu() {
    cout << endl << "Returning to main menu" << endl;
}

void handleOrderClear() {
    string confirmation = "Are you sure? Once cleared the order cannot be retrieved. ";
    if (!confirm(confirmation)) {
        return;
    }
    order.flush();
    cout << endl << "[INFO] Order cleared." << endl;
}

void handleOrderAddItem() {
    navi.displayMenuHeader("Order: Add Item");
    bool isValid = false;
    char type;
    cout << endl << "What do you want to add? (F) Food / (B) Beverage: ";
    do {
        cin >> type;
        if (cin.good()) {
            if (type == 'f' || type == 'F') {
                type = order.TYPE_FOOD;
                cout << endl;
                handleOrderAddFood();
                isValid = true;
            } else if (type == 'b' || type == 'B') {
                type = order.TYPE_BEVERAGE;
                cout << endl;
                handleOrderAddBeverage();
                isValid = true;
            }
        }
        cin.clear();
        cin.ignore(10000, '\n');
        if (isValid == false) {
            cout << "Invalid input. What do you want to add? (F) Food / (B) Beverage: ";
        }
    } while (isValid == false);
}

int handleOrderAddItemSelection(int itemCount) {
    bool isValid = false;
    int selection;
    cout << "Input item number to add to order (1 - " << itemCount << "): ";
    do {
        cin >> selection;
        if (cin.good() && selection > 0 && selection <= itemCount) {
            isValid = true;
            selection--;
        } else {
            cout << "Invalid selection. Input item number to add to order (1 - " << itemCount << "): ";
        }
        cin.clear();
        cin.ignore(10000, '\n');
    } while(isValid == false);
    return selection;
}

int handleItemInputQty(int max = 99) {
    bool isValid = false;
    int qty;
    cout << "Input item quantity to add to order (1 - " << max << "): ";
    do {
        cin >> qty;
        if (cin.good() && qty > 0 && qty <= max) {
            isValid = true;
        } else {
            cout << "Invalid selection. Input item quantity to add to order (1 - " << max << "): ";
        }
        cin.clear();
        cin.ignore(10000, '\n');
    } while(isValid == false);
    return qty;
}

void handleOrderAddFood() {
    if (!menu.hasFoodItem) {
        cout << "[INFO] Food menu is empty. Please contact system administrator." << endl;
        return;
    }
    navi.displayMenuHeader("Order: Add Food Item");
    cout << endl;
    menu.displayFoodItemSelection();
    cout << endl;
    int itemCount = menu.foodItemCount;
    int itemIndex = handleOrderAddItemSelection(itemCount);
    cout << endl;
    foodItem item = menu.foodItems[ itemIndex ];
    int qty = handleItemInputQty();
    cout << endl;
    order.addFoodItem(item, qty);
    cout << "[INFO] " << item.name << " x " << qty << " added to order." << endl;
}

void handleOrderAddBeverage() {
    if (!menu.hasBeverageItem) {
        cout << "[INFO] Food menu is empty. Please contact system administrator." << endl;
        return;
    }
    navi.displayMenuHeader("Order: Add Beverage Item");
    cout << endl;
    menu.displayBeverageItemSelection();
    cout << endl;
    char size;
    int itemCount = menu.beverageItemCount;
    int itemIndex = handleOrderAddItemSelection(itemCount);
    cout << endl;
    beverageItem item = menu.beverageItems[ itemIndex ];

    // user input size
    bool isValid = false;
    char input;
    cout << "Select beverage size. (M) Medium / (L) Large: ";
    do {
        cin >> input;
        if (cin.good()) {
            if (input == 'm' || input == 'M') {
                size = order.SIZE_MEDIUM;
                isValid = true;
            } else if (input == 'l' || input == 'L') {
                size = order.SIZE_LARGE;
                isValid = true;
            }
        }
        if (!isValid) {
            cout << "Invalid size. Select beverage size. (M) Medium / (L) Large: ";
        }
        cin.clear();
        cin.ignore(10000, '\n');
    } while(isValid == false);
    cout << endl;
    // quantity
    int qty = handleItemInputQty();
    cout << endl;
    order.addBeverageItem(item, qty, size);
    cout << "[INFO] " << item.name << " (" << size << ")" << " x " << qty << " added to order." << endl;
}

void displayOrderItemSelection() {
    for (int i = 0; i < order.numOfItem; i++) {
        cout << "[" << i + 1 << "] " << order.items[ i ].name;
        if (order.items[ i ].type == order.TYPE_BEVERAGE) {
            cout << "(" << order.items[ i ].size << ")";
        }
        cout << " x " << order.items[ i ].qty;
        cout << " - " << config.currency << fixed << setprecision(2) << order.items[ i ].subTotal << endl;
    }
}

int handleOrderItemSelection() {
    bool isValid = false;
    int selection;
    cout << "Input order item number (1 - " << order.numOfItem << "): ";
    do {
        cin >> selection;
        if (cin.good() && selection > 0 && selection <= order.numOfItem) {
            isValid = true;
            selection--;
        } else {
            cout << "Invalid selection. Input order item number (1 - " << order.numOfItem << "): ";
        }
        cin.clear();
        cin.ignore(10000, '\n');
    } while(isValid == false);
    return selection;
}

void handleOrderUpdateItem() {
    if (order.hasItem == false) {
        cout << "[INFO] Nothing to update. Please add item to order first." << endl;
        return;
    }
    navi.displayMenuHeader("Order: Update Item");
    cout << endl;
    displayOrderItemSelection();
    cout << endl;
    int index = handleOrderItemSelection();
    cout << endl;
    int qty = handleItemInputQty();
    cout << endl;
    order.updateItem(index, qty);
    cout << endl << "Item quantity updated to " << qty << '.' << endl;
}

void handleOrderRemoveItem() {
    if (order.hasItem == false) {
        cout << "[INFO] Nothing to remove. Please add item to order first." << endl;
        return;
    }
    navi.displayMenuHeader("Order: Remove Item");
    cout << endl;
    displayOrderItemSelection();
    cout << endl;
    int index = handleOrderItemSelection();
    cout << endl;
    string itemName = order.items[ index ].name;
    string confirmation = "Are you sure you want to remove " + itemName + "? ";
    if (!confirm(confirmation)) {
        return;
    }
    order.removeItem(index);
    cout << endl << itemName << " removed from order." << endl;
}

void handleOrderCheckout() {
    if (order.hasItem == false) {
        cout << "[INFO] Nothing to checkout. Please add item to order first." << endl;
        return;
    }
    string confirmation = "Are you sure? ";
    double amountReceived = 0;
    bool isValid = false;
    if (!confirm(confirmation)) {
        return;
    }
    order.recalculateInvoice();
    cout << endl << "Order total amount: " << config.currency << fixed << setprecision(2)  << order.total << endl << endl;
    cout << "Please input amount collected from customer: ";
    do {
        cin >> amountReceived;
        if (cin.good()) {
            if (amountReceived >= order.total) {
                isValid = true;
                order.setAmountReceivable(amountReceived);
            } else {
                cout << "Please collect more than or equal to: " << config.currency << fixed << setprecision(2)  << order.total << endl << endl;
            }
        }
        cin.clear();
        cin.ignore(10000, '\n');
        if (isValid == false) {
            cout << "Please input amount collected from customer: ";
        }
    } while (isValid == false);
    cout << endl << endl;
    cout << "You've collected: " << config.currency << fixed << setprecision(2) << amountReceived << endl << endl;
    if (order.amountReturned > 0) {
        cout << "Please return to customer: " << config.currency << fixed << setprecision(2)  << order.amountReturned << endl;
    } else {
        cout << "No change required." << endl;
    }
    
    displayContinue();

    // pretenting to be a serious action =)
    cout << endl << "[INFO] Checking out in progress, please wait..." << endl;
    cout << endl << "[INFO] Generating receipt: orders/receipts/" + order.id + ".txt..." << endl;

    order.displayCurrent();
    order.checkout();
}

void displayOrder() {
    cout << endl;
    navi.displayMenuHeader("Current Order");
    cout << endl;
    order.displayCurrent();
}

void displayContinue() {
    cout << endl << "Press any key to continue...";
    cout << endl;
    cin.ignore(10000, '\n');
}

void displayOrderHistory() {
    cout << endl;
    navi.displayMenuHeader("Order History");
    cout << endl;
    order.displayHistory();
}

void displayConfig() {
    cout << endl;
    navi.displayMenuHeader("Configurations");
    cout << endl;
    config.display();
    cout << endl;
}

void displayHelp() {
    cout << endl;
    navi.displayMenuHeader("Help");
    cout << endl;
    cout << "Navigating the system:" << endl;
    cout << string(40, '-') << endl;
    cout << "To navigation, please input corresponding number in the menu." << endl;
    cout << "You can find the menu number enclosed in [ ] square braket." << endl;
    cout << "If you input is invalid, you will be prompted to input again." << endl;
    cout << endl;
    cout << "Manage your menu:" << endl;
    cout << string(40, '-') << endl;
    cout << "You can manually manage your menu by editing the menu config files." << endl;
    cout << "You can find the menu config files in [menu] folder within project folder." << endl;
    cout << "In menu folder, you will find [beverage.txt] and [food.txt]" << endl;
    cout << "Open the text file with your preferred text editor to manage menu." << endl;
    cout << endl;
    cout << "Configurating your cafe management system:" << endl;
    cout << string(40, '-') << endl;
    cout << "You can manually update the configurations for cafe management system." << endl;
    cout << "You can find the configration file in project folder." << endl;
    cout << "Open [config.ini] with your preferred text editor to update configurations." << endl;
    cout << "To disable service or sales tax, change value to [0]." << endl;
    cout << endl;
    cout << "Where to find receipts:" << endl;
    cout << string(40, '-') << endl;
    cout << "You can retrieve order receipts from [orders/receipts] folder within project folder." << endl;
    cout << endl;
    cout << "End of help. Please contact system administrator if you have more questions." << endl;
}
