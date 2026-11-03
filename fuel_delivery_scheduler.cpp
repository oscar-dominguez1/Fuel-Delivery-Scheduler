#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

class ForecastRecord
{
private:
    string month;
    int day;
    int tHigh;
    int tLow;

public:
    ForecastRecord(string = "January", int = 0, int = 0, int = 0);
    float getHeatingDegreeDays();
    string getMonth();
    int getDay();
    int getTHigh();
    int getTLow();
};

ForecastRecord::ForecastRecord(string month, int day, int tHigh, int tLow){
    this->month = month;
    this->day = day;
    this->tHigh = tHigh;
    this->tLow = tLow;
}

float ForecastRecord::getHeatingDegreeDays(){
    // HDD = 65 - average temperature
    float hdd = 65 - (this->tHigh + this->tLow) / 2.0;
    if(hdd > 0) {
        return hdd;
    } else {
        return 0;
    }
}

string ForecastRecord::getMonth(){
    return this->month;
}

int ForecastRecord::getDay(){
    return this->day;
}

int ForecastRecord::getTHigh(){
    return this->tHigh;
}

int ForecastRecord::getTLow(){
    return this->tLow;
}

class CustomerRecord
{
private:
    int buildingNumber;
    string streetName;
    int fuelCapacity;
    double fuelLevel;
    double kFactor;

public:
    CustomerRecord(int = 0, string = "empty", int = 0, double = 0.0, double = 0.0);
    int doesCustNeedDelivery();
    void updateCustEstFuelInTank(float, float);
    int getBuildingNumber();
    string getStreetName();
    int getFuelCapacity();
    double getFuelLevel();
    double getKFactor();
};

CustomerRecord::CustomerRecord(int buildingNumber, string streetName, int fuelCapacity, double fuelLevel, double kFactor){
    this->buildingNumber = buildingNumber;
    this->streetName = streetName;
    this->fuelCapacity = fuelCapacity;
    this->fuelLevel = fuelLevel;
    this->kFactor = kFactor;
}

int CustomerRecord::doesCustNeedDelivery(){
    if(this->fuelLevel / (double) this->fuelCapacity < 0.20){
        return 1;
    } else {
        return 0;
    }
}

void CustomerRecord::updateCustEstFuelInTank(float deliveryAmount, float hdd){
    this->fuelLevel += deliveryAmount - this->kFactor * hdd;
}

int CustomerRecord::getBuildingNumber(){
    return this->buildingNumber;
}

string CustomerRecord::getStreetName(){
    return this->streetName;
}

int CustomerRecord::getFuelCapacity(){
    return this->fuelCapacity;
}

double CustomerRecord::getFuelLevel(){
    return this->fuelLevel;
}

double CustomerRecord::getKFactor(){
    return this->kFactor;
}

int main(){
    //Declare variables to open forecast
    string fileNameForecast;
    ifstream fileInputForecast;

    //open forecast file
    cout << "Enter the name of the forecast file (or drag it here):" << endl;
    cin >> fileNameForecast;

    fileInputForecast.open(fileNameForecast);
    if(fileInputForecast.fail()){
        cout << "File could not be opened." << endl;
        return 1;
    }

    //store the number of forecast days and create an array
    int numForecastDays;
    fileInputForecast >> numForecastDays;

    ForecastRecord* forecast = nullptr;
    forecast = new ForecastRecord[numForecastDays];
    if(forecast == nullptr){
        cout << "Memory failed to allocate." << endl;
        return 1;
    }

    for(int i = 0; i < numForecastDays; i++){
        string month;
        int day, tHigh, tLow;
        fileInputForecast >> month >> day >> tHigh >> tLow;
        forecast[i] = ForecastRecord(month, day, tHigh, tLow);
    }

    fileInputForecast.close();

    cout << "Data for " << numForecastDays << " forecast days were read in.\n" << endl;

    //declare variables to open customers file
    string fileNameCustomers;
    ifstream inputFileCustomers;

    cout << "Enter the name of the customers file (or drag it here):" << endl;
    cin >> fileNameCustomers;

    inputFileCustomers.open(fileNameCustomers);
    if(inputFileCustomers.fail()){
        cout << "File could not be opened." << endl;
        delete[] forecast;
        forecast = nullptr;
        return 1;
    }

    //store the number of customers there are and create an array
    int numCustomers;
    inputFileCustomers >> numCustomers;

    CustomerRecord* customer = nullptr;
    customer = new CustomerRecord[numCustomers];
    if(customer == nullptr){
        cout << "Memory failed to allocate." << endl;
        delete[] forecast;
        forecast = nullptr;
        return 1;
    }

    for(int i = 0; i < numCustomers; i++){
        int buildingNumber, fuelCapacity;
        double fuelLevel;
        string streetName;
        double kFactor;

        inputFileCustomers >> buildingNumber >> streetName >> fuelCapacity >> fuelLevel >> kFactor;
        customer[i] = CustomerRecord(buildingNumber, streetName, fuelCapacity, fuelLevel, kFactor);
    }

    inputFileCustomers.close();

    cout << "Data for " << numCustomers << " customers were read in.\n\n" << endl;

    for(int iDay = 0; iDay < numForecastDays; iDay++){
        int deliveriesToday = 0;
        float hdd = forecast[iDay].getHeatingDegreeDays();

        cout << forecast[iDay].getMonth() << " " << forecast[iDay].getDay() << ":" << endl;

        for(int iCustomer = 0; iCustomer < numCustomers; iCustomer++){
            float delivery = 0;

            if(customer[iCustomer].doesCustNeedDelivery() == 1){
                delivery = 0.70 * customer[iCustomer].getFuelCapacity();
                cout << delivery << " gallons -> " << customer[iCustomer].getBuildingNumber()
                     << " " << customer[iCustomer].getStreetName() << endl;
                deliveriesToday++;
            }

            customer[iCustomer].updateCustEstFuelInTank(delivery, hdd);
        }

        if(deliveriesToday == 0){
            cout << "No fuel deliveries scheduled for this day." << endl;
        }

        cout << endl;
    }

    cout << "ESTIMATED FUEL IN TANK AT THE END OF FORECAST PERIOD\n" << endl;

    cout << fixed << setprecision(1);
    for(int iCustomer = 0; iCustomer < numCustomers; iCustomer++){
        cout << customer[iCustomer].getBuildingNumber() << " " << customer[iCustomer].getStreetName()
             << ": " << customer[iCustomer].getFuelLevel() << " gallons" << endl;
    }

    delete[] customer;
    delete[] forecast;

    customer = nullptr;
    forecast = nullptr;
    return 0;
}