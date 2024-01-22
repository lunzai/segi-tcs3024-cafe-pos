/*
Assignment: TCS3024 - Introductory Programming
Author: Leong Hean Luen
Student ID: SUKD2301060
Email: sukd2301060@segi4u.my
Contact: +6013 344 6652
*/

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include "menu.h"
#include "config.h"

using namespace std;

struct orderItem
{
    string name;
    char type;
    char size;
    int qty;
    double unitPrice;
    double subTotal;
};

class Order {
    private:

    const char* ordersFile = "orders/all.txt";
    const char CHAR_COMMENT = '#';
    Config config;

    public: 

    const char TYPE_FOOD = 'F';
    const char TYPE_BEVERAGE = 'B';
    const char SIZE_MEDIUM = 'M';
    const char SIZE_LARGE = 'L';
    const char SIZE_NONE = 'N';

    string id, datetime;
    int numOfItem = 0;
    bool 
        hasItem = false,
        isCreated = false;
    double 
        serviceTax = 0, 
        serviceTaxPercent = 0,
        salesTax = 0, 
        salesTaxPercent = 0,
        rounding = 0, 
        subtotal = 0, 
        total = 0,
        amountReceivable = 0, 
        amountReturned = 0;
    vector<orderItem> items;

    Order() {
        
    }

    void setConfig(Config config) {
        salesTaxPercent = config.salesTaxPercent;
        serviceTaxPercent = config.serviceTaxPercent;
    }
 
    void flush() {
        id = "";
        datetime = "";
        serviceTax = 0; 
        salesTax = 0;
        rounding = 0; 
        subtotal = 0; 
        total = 0;
        amountReceivable = 0; 
        amountReturned = 0;
        isCreated = false;
        clearItems();
    }

    void create() {
        flush();
        id = getOrderId();
        datetime = getDatetimeString();
        isCreated = true;
    }

    void updateItem(int index, int qty) {
        if (index < items.size() && qty > 0 && qty < 99) {
            items[ index ].qty = qty;
            items[ index ].subTotal = items[ index ].unitPrice * qty;
            recalculateInvoice();
        }
    }

    void removeItem(int index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
            recalculateInvoice();
        }
    }

    void checkout() {
        writeOrderToFile();
        appendOrderHistory();
        flush();
    }

    void setAmountReceivable(double amount) {
        if (amount >= total) {
            amountReceivable = amount;
            amountReturned = amount - total;
        }
    }

    void recalculateInvoice() {
        hasItem = false;
        numOfItem = 0;
        serviceTax = 0; 
        salesTax = 0;
        rounding = 0; 
        subtotal = 0; 
        total = 0;
        if (items.size() == 0) {
            return;
        } else {
            hasItem = true;
        }
        for (int i = 0; i < items.size(); i++) {
            numOfItem++;
            subtotal += items[ i ].subTotal;
        }
        if (serviceTaxPercent > 0) {
            serviceTax = subtotal * serviceTaxPercent;
        }
        if (salesTaxPercent > 0) {
            salesTax = subtotal * salesTaxPercent;
        }
        total = subtotal + serviceTax + salesTax;
        rounding = total - (floor(total * 10) / 10);
        total -= rounding;
    }

    void addFoodItem(foodItem item, int qty) {
        if (qty < 0) {
            return;
        }
        hasItem = true;
        items.push_back({
            item.name,
            TYPE_FOOD,
            SIZE_NONE,
            qty,
            item.price,
            item.price * qty
        });
        recalculateInvoice();
    }

    void addBeverageItem(beverageItem item, int qty, char size) {
        if (qty < 0) {
            return;
        }
        double price;
        if (size == SIZE_MEDIUM) {
            price = item.mediumPrice;
        } else if (size == SIZE_LARGE) {
            price = item.largePrice;
        } else {
            return;
        }
        hasItem = true;
        items.push_back({
            item.name,
            TYPE_BEVERAGE,
            size,
            qty,
            price,
            price * qty 
        });
        recalculateInvoice();
    }

    void clearItems() {
        items.clear();
        recalculateInvoice();
    }

    /*
    Please update writeOrderToFile() when you edit the output
    */
    void displayCurrent() {
        string footerText = "THANK YOU, PLEASE COME AGAIN";
        int width = 50;
        int datetimeWidth = 20;
        int priceWidth = 10;
        int currencyWidth = config.currency.size();
        int summaryWidth = width - priceWidth - currencyWidth;
        int qtyWidth = 6;
        int itemWidth = summaryWidth - qtyWidth;
        int cafeNamePadding = (int)floor((width - config.cafeName.size()) / 2);
        int footerPadding = (int)floor((width - footerText.size()) / 2);
        string itemName;
        // header
        cout << endl << endl << string(width, '-') << endl << endl;
        cout << string(cafeNamePadding, ' ') << config.cafeName << endl << endl;
        cout << string(width, '-') << endl << endl;
        cout << setw(width - datetimeWidth) << left << "RECEIPT" << setw(datetimeWidth) << id << endl;
        cout << setw(width - datetimeWidth) << left << "DATE" << setw(datetimeWidth) << datetime << endl << endl;
        cout << string(width, '=') << endl;
        // items
        if (numOfItem > 0) {
            cout << setw(itemWidth) << "ITEM" << setw(qtyWidth) << "QTY" << setw(priceWidth) << "AMOUNT" << endl;
            cout << string(width, '-') << endl;
            for (int i = 0; i < numOfItem; i++) {
                itemName = items[ i ].name;
                if (items[ i ].type == TYPE_BEVERAGE) {
                    itemName = itemName + " (" + items[ i ].size + ")";
                }
                cout << setw(itemWidth) << itemName << setw(qtyWidth) << items[ i ].qty << config.currency;
                cout << setw(priceWidth) << fixed << setprecision(2) << items[ i ].subTotal << endl;
            }
        } else {
            cout << endl << "NO ITEM ADDED" << endl << endl;
        }
        // summary
        cout << string(width, '-') << endl;
        cout << setw(summaryWidth) << left << "Sub-total" << config.currency;
        cout << setw(priceWidth) << fixed << setprecision(2) << subtotal << endl;
        if (config.salesTaxPercent > 0) {
            string salesTaxLabel = "Sales Tax (" + config.salesTaxPercentDisplay + ")";
            cout << setw(summaryWidth) << left << salesTaxLabel << config.currency;
            cout << setw(priceWidth) << fixed << setprecision(2) << salesTax << endl;
        }
        if (config.serviceTaxPercent > 0) {
            string serviceTaxLabel = "Service Tax (" + config.serviceTaxPercentDisplay + ")";
            cout << setw(summaryWidth) << left << serviceTaxLabel << config.currency;
            cout << setw(priceWidth) << fixed << setprecision(2) << serviceTax << endl;
        }
        if (rounding > 0) {
            cout << setw(summaryWidth - 1) << left << "Rounding" << '-' << config.currency;
            cout << setw(priceWidth) << fixed << setprecision(2) << rounding << endl;
        }
        cout << setw(summaryWidth - 2) << left << " " << string(priceWidth + currencyWidth + 2, '-') << endl;
        cout << setw(summaryWidth) << left << "Grand Total" << config.currency;
        cout << setw(priceWidth) << fixed << setprecision(2) << total << endl;
        
        cout << setw(summaryWidth - 2) << left << " " << string(priceWidth + currencyWidth + 2, '=') << endl;
        
        if (amountReceivable) {
            cout << setw(summaryWidth) << left << "Amount Received" << config.currency;
            cout << setw(priceWidth) << fixed << setprecision(2) << amountReceivable << endl;
            cout << setw(summaryWidth) << left << "Amount Returned" << config.currency;
            cout << setw(priceWidth) << fixed << setprecision(2) << amountReturned << endl;
        } else {
            cout << endl;
        }

        cout << string(width, '-') << endl << endl;
        cout << string(footerPadding, ' ') << footerText << endl << endl;
        cout << string(width, '-') << endl << endl << endl;
    }

    /*
    Please update displayCurrent() when you edit the output
    */
    void writeOrderToFile() {
        ofstream fout;
        string filename = "orders/receipts/" + id + ".txt";
        fout.open(filename, ios::trunc);

        string footerText = "THANK YOU, PLEASE COME AGAIN";
        int width = 50;
        int datetimeWidth = 20;
        int priceWidth = 10;
        int currencyWidth = config.currency.size();
        int summaryWidth = width - priceWidth - currencyWidth;
        int qtyWidth = 6;
        int itemWidth = summaryWidth - qtyWidth;
        int cafeNamePadding = (int)floor((width - config.cafeName.size()) / 2);
        int footerPadding = (int)floor((width - footerText.size()) / 2);
        string itemName;
        // header
        fout << string(width, '-') << endl << endl;
        fout << string(cafeNamePadding, ' ') << config.cafeName << endl << endl;
        fout << string(width, '-') << endl << endl;
        fout << setw(width - datetimeWidth) << left << "RECEIPT" << setw(datetimeWidth) << id << endl;
        fout << setw(width - datetimeWidth) << left << "DATE" << setw(datetimeWidth) << datetime << endl << endl;
        fout << string(width, '=') << endl;
        // items
        if (numOfItem > 0) {
            fout << setw(itemWidth) << "ITEM" << setw(qtyWidth) << "QTY" << setw(priceWidth) << "AMOUNT" << endl;
            fout << string(width, '-') << endl;
            for (int i = 0; i < numOfItem; i++) {
                itemName = items[ i ].name;
                if (items[ i ].type == TYPE_BEVERAGE) {
                    itemName = itemName + " (" + items[ i ].size + ")";
                }
                fout << setw(itemWidth) << itemName << setw(qtyWidth) << items[ i ].qty << config.currency;
                fout << setw(priceWidth) << fixed << setprecision(2) << items[ i ].subTotal << endl;
            }
        } else {
            fout << "NO ITEM ADDED" << endl;
        }
        // summary
        fout << string(width, '-') << endl;
        fout << setw(summaryWidth) << left << "Sub-total" << config.currency;
        fout << setw(priceWidth) << fixed << setprecision(2) << subtotal << endl;
        if (config.salesTaxPercent > 0) {
            string salesTaxLabel = "Sales Tax (" + config.salesTaxPercentDisplay + ")";
            fout << setw(summaryWidth) << left << salesTaxLabel << config.currency;
            fout << setw(priceWidth) << fixed << setprecision(2) << salesTax << endl;
        }
        if (config.serviceTaxPercent > 0) {
            string serviceTaxLabel = "Service Tax (" + config.salesTaxPercentDisplay + ")";
            fout << setw(summaryWidth) << left << serviceTaxLabel << config.currency;
            fout << setw(priceWidth) << fixed << setprecision(2) << serviceTax << endl;
        }
        if (rounding > 0) {
            fout << setw(summaryWidth - 1) << left << "Rounding" << '-' << config.currency;
            fout << setw(priceWidth) << fixed << setprecision(2) << rounding << endl;
        }
        fout << setw(summaryWidth - 2) << left << " " << string(priceWidth + currencyWidth + 2, '-') << endl;
        fout << setw(summaryWidth) << left << "Grand Total" << config.currency;
        fout << setw(priceWidth) << fixed << setprecision(2) << total << endl;
        
        fout << setw(summaryWidth - 2) << left << " " << string(priceWidth + currencyWidth + 2, '=') << endl;
        
        if (amountReceivable) {
            fout << setw(summaryWidth) << left << "Amount Received" << config.currency;
            fout << setw(priceWidth) << fixed << setprecision(2) << amountReceivable << endl;
            fout << setw(summaryWidth) << left << "Amount Returned" << config.currency;
            fout << setw(priceWidth) << fixed << setprecision(2) << amountReturned << endl;
        } else {
            fout << endl;
        }

        fout << string(width, '-') << endl << endl;
        fout << string(footerPadding, ' ') << footerText << endl << endl;
        fout << string(width, '-') << endl;

        fout.close();
    }

    void appendOrderHistory() {
        ofstream fout;
        fout.open(ordersFile, ios::app);
        fout << id << '|';
        fout << datetime << '|';
        fout << numOfItem << '|';
        fout << fixed << setprecision(2) << subtotal << '|';
        fout << fixed << setprecision(2) << serviceTax << '|';
        fout << fixed << setprecision(2) << salesTax << '|';
        fout << fixed << setprecision(2) << rounding << '|';
        fout << fixed << setprecision(2) << total << '|';
        fout << fixed << setprecision(2) << amountReceivable << '|';
        fout << fixed << setprecision(2) << amountReturned << endl;
        fout.close();
    }

    void displayHistory() {
        char const *columnFormat = "|%-15s|%-21s|%-14s|%-11s|%-13s|%-11s|%-10s|%-10s|%-18s|%-17s|";
        ifstream cFile(ordersFile);
        bool isFirst = true;
        if (cFile.is_open()) {
            string line;
            while (getline(cFile, line)) {
                if (line[0] == CHAR_COMMENT || line.empty()) {
                    continue;
                }
                vector<string> columns = split(line);
                if (isFirst) {
                    displayTableDivider();
                    isFirst = false;
                }
                printf(
                    columnFormat, 
                    columns[0].c_str(),
                    columns[1].c_str(),
                    columns[2].c_str(),
                    columns[3].c_str(),
                    columns[4].c_str(),
                    columns[5].c_str(),
                    columns[6].c_str(),
                    columns[7].c_str(),
                    columns[8].c_str(),
                    columns[9].c_str(),
                    columns[10].c_str()
                );
                cout << endl;
                displayTableDivider();
            }
            if (isFirst) {
                cout << "[INFO] Order history - no record found." << endl;
            }
        } else {
            cout << "[WARNING] File not found or invalid. Please contact system administrator." << endl;
        }
        
    }
    private:

    string formatDatetime(char* format) {
        auto t = time(nullptr);
        auto tm = *localtime(&t);
        ostringstream oss;
        oss << put_time(&tm, format);
        return oss.str();
    }

    string getOrderId() {
        return formatDatetime((char*)"%Y%m%d-%H%M%S");
    }

    string getDatetimeString() {
        return formatDatetime((char*)"%Y-%m-%d %H:%M:%S");
    }

    void displayTableDivider() {
        cout << '+' << string(15, '-') << '+' << string(21, '-') << '+' << string(14, '-');
        cout << '+' << string(11, '-') << '+' << string(13, '-') << '+' << string(11, '-');
        cout << '+' << string(10, '-') << '+' << string(10, '-') << '+' << string(18, '-');
        cout << '+' << string(17, '-') << '+' << endl;
    }

    /*
    https://www.geeksforgeeks.org/cpp-string-to-vector-using-delimiter/
    */
    vector<string> split(string str, string delimiter = "|") {
        vector<string> v;
        if (!str.empty()) {
            int start = 0;
            do {
                // Find the index of occurrence
                int idx = str.find(delimiter, start);
                if (idx == string::npos) {
                    break;
                }
                // If found add the substring till that
                // occurrence in the vector
                int length = idx - start;
                v.push_back(str.substr(start, length));
                start += (length + delimiter.size());
            } while (true);
            v.push_back(str.substr(start));
        }
        return v;
    }    
};

#endif