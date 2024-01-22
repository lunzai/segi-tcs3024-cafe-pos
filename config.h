/*
Assignment: TCS3024 - Introductory Programming
Author: Leong Hean Luen
Student ID: SUKD2301060
Email: sukd2301060@segi4u.my
Contact: +6013 344 6652
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Config {
    private:

    const char* configFile = "config.ini";
    const char CHAR_COMMENT = '#';
    const char CHAR_DELIMITER = '=';

    public:

    string cafeName, currency, salesTaxPercentDisplay, serviceTaxPercentDisplay;
    double salesTaxPercent = 0;
    double serviceTaxPercent = 0;
    bool isLoaded = false;

    Config() {
        try {
            ifstream cFile(configFile);
            if (cFile.is_open()) {
                string line;
                while (getline(cFile, line)) {
                    if (line[0] == CHAR_COMMENT || line.empty()) {
                        continue;
                    }
                    auto delimiterPosition = line.find(CHAR_DELIMITER);
                    auto key = line.substr(0, delimiterPosition);
                    auto value = line.substr(delimiterPosition + 1);
                    if (key == "cafe_name") {
                        cafeName = value;
                    } else if (key == "sales_tax_percent") {
                        salesTaxPercent = stod(value);
                        if (salesTaxPercent < 0) {
                            salesTaxPercent = 0;
                        }
                    } else if (key == "service_tax_percent") {
                        serviceTaxPercent = stod(value);
                        if (serviceTaxPercent < 0) {
                            serviceTaxPercent = 0;
                        }
                    } else if (key == "currency") {
                        currency = value;
                    }
                }
                isLoaded = true;
            }
            ostringstream streamObj;
            streamObj.precision(2);
            if (salesTaxPercent > 0) {
                streamObj << fixed << salesTaxPercent * 100;
                salesTaxPercentDisplay = streamObj.str() + '%';
            } else {
                salesTaxPercentDisplay = "Disabled";
            }
            streamObj.str("");
            if (serviceTaxPercent > 0) {
                streamObj << fixed << serviceTaxPercent * 100;
                serviceTaxPercentDisplay = streamObj.str() + '%';
            } else {
                serviceTaxPercentDisplay = "Disabled";
            }
        }
        catch(const std::exception& e) {
            return;
        }
    }

    void display() {
        int headerWidth = 15;
        cout << setw(headerWidth) << left << "Cafe name:" << cafeName << endl; 
        cout << setw(headerWidth) << left << "Currency:" << currency << endl;
        cout << setw(headerWidth) << left << "Sales tax:" << salesTaxPercentDisplay << endl;
        cout << setw(headerWidth) << left << "Service tax:" << serviceTaxPercentDisplay << endl;
    }

};

#endif