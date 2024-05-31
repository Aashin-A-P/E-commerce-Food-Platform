#ifndef MENU_HPP  // Header guard to prevent multiple inclusions
#define MENU_HPP
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept> 
#include <map>
#include <algorithm>
#include <set>
#include <unistd.h>

using namespace std;
struct Dish{
    int hid;
    string dishname;
    string description;
    double cost;
};
struct MenuItem {
    int pid;
    int hid;
    string itemName;
    string cuisine;
    string description;
    double cost;
};
struct Location {
  string hotelname;
  double latitude;
  double longitude;
};
class Menu {
public:
  // Function to read menu items from a CSV file
  map<int, vector<Dish>> readMenu(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
      throw runtime_error("Error opening file!"); // Use runtime_error for potential file issues
    }

    map<int, vector<Dish>> menuItems;
    string line;

    // Read and discard the header row
    getline(infile, line);

    while (getline(infile, line)) {
      stringstream ss(line);
      string field,field1,f;
      MenuItem item;

      getline(ss, field1, ',');

      // Read hotel name
      getline(ss, f, ',');

      // Read item name
      getline(ss, item.itemName, ',');

      // Read description (without quotes)
      getline(ss, item.description, ',');

      // Read cost
      getline(ss, field, ',');
      try {
        item.hid = stoi(f);
        item.pid = stoi(field1);
        item.cost = stod(field);
      } catch (const invalid_argument& e) {
        cerr << "Invalid cost value in line: " << line << endl;
        continue; // Skip this item and move to the next one
      }
      Dish z = {item.hid,item.itemName,item.description,item.cost};
      menuItems[item.pid].push_back(z);
    }

    infile.close();
    return menuItems;
  }
  map<int, vector<Location>> readHotel(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
      throw runtime_error("Error opening file!"); // Use runtime_error for potential file issues
    }

    map<int, vector<Location>> Hotel;
    string line;

    // Read and discard the header row
    getline(infile, line);
    Location a;
    while (getline(infile, line)) {
      stringstream ss(line);
      string field,field1,field2;
      MenuItem item;

      getline(ss, field, ',');

      // Read hotel name
      getline(ss, a.hotelname, ',');

      // Read item name
      getline(ss, field1, ',');

      // Read description (without quotes)
      getline(ss, field2, ',');

      // Read cost
      getline(ss, field, ',');
      try {
        int x = stoi(field);
        a.latitude = stod(field1);
        a.longitude = stod(field2);
      } catch (const invalid_argument& e) {
        cerr << "Invalid cost value in line: " << line << endl;
        continue; // Skip this item and move to the next one
      }
      
      Hotel[stoi(field)].push_back(a);
    }

    infile.close();
    return Hotel;
  }
  // Getter methods to access private members
  int getPID(const MenuItem& item) const { return item.pid; }
  int getHotelName(const MenuItem& item) const { return item.hid; }
  string getItemName(const MenuItem& item) const { return item.itemName; }
  string getCuisineName(const MenuItem& item) const { return item.cuisine; }
  string getDescription(const MenuItem& item) const { return item.description; }
  double getCost(const MenuItem& item) const { return item.cost; }
  

};

#endif