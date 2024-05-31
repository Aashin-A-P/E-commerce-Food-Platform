#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <map>
#include <stack>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <list>
#include <queue>
#include <limits.h>
#include <ctime>
#include <cmath>
#include <unordered_map>
#include <unistd.h>

#include "user.hpp"
#include "menu.hpp"
#include "graph.hpp"

using namespace std;
void keepHeader(const string &filename)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string header;
    getline(inputFile, header);
    inputFile.close();

    ofstream outputFile(filename, ofstream::out | ofstream::trunc);
    if (!outputFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    outputFile << header << endl;
    outputFile.close();
}
void appendToCSV(int cid, const string &customer, double lat, double lon, double tamt, const string &filename)
{
    ofstream outputFile(filename, ios_base::app); // Open file in append mode
    if (!outputFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    // Write data to the CSV file
    outputFile << cid << "," << customer << "," << lat << "," << lon << "," << tamt << endl;

    outputFile.close();
}
int main()
{
    map<int, vector<Dish>> MenuList;
    map<int, vector<Location>> Hotel;
    Menu menu; // Create an instance of the Menu class
    string fn = "hotels.csv";
    Hotel = menu.readHotel(fn);
    map<int, vector<Customer>> Customers;
    vector<Order> Orders;
    int cid = 1;
    char choice;
    User user;
    string filename = "menu.csv";
    vector<pair<string, string>> users = user.readUsersFromFile();
    try
    {
        MenuList = menu.readMenu(filename);
    }
    catch (const runtime_error &e)
    {
        cerr << e.what() << endl;
        return 1; // Indicate failure in main
    }

    while (true)
    {
        cout << "===================================================" << endl;
        int retcode = system("figlet CULINARY");
        int retcode2 = system("figlet CLOUD");
        cout << "Choose an option:" << endl;
        cout << "1. User Sign up" << endl;
        cout << "2. User Log in" << endl;
        cout << "3. Delivery Guy Login" << endl;
        cout << "4. Exit" << endl;
        cout << "===================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        string ch;
        pair<string, bool> loguser = {"", false};
        switch (choice)
        {
        case '1':
            user.signup();
            cout << "Do you want to login now? (Y/N):";
            cin >> ch;
            if (!(ch == "Y" || ch == "yes" || ch == "y" || ch == "Yes" || ch == "YES" || ch == "yES" || ch == "yeS" || ch == "YeS"))
            {
                cout << "Thanks for Visiting..." << endl;
                cout << "===================================================" << endl;
                break;
            }
        case '2':
            loguser = user.login();
            break;
        case '3':
        {
            string username, password;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            if (username == "." && password == ".")
            {
                loguser = {".", true};
            }
            break;
        }
        default:
            cout << "Thanks for Visiting..." << endl;
            exit(1);
        }
        
        if (loguser.first != "." && loguser.second)
        {
            int ch1;
            double amt = 0.0;
            while (true)
            {

                cout << "===================================================" << endl;
                int a = system("figlet Food is Medicine");
                cout << "===================================================" << endl;
                int b = system("figlet HOTELS");
                cout << endl;
                cout << "ID\t Hotel Name" << endl;
                for (int i = 1; i <= 35; i++)
                {
                    cout << i << "\t" << Hotel[i][0].hotelname << endl;
                }
                cout << "===================================================" << endl;
                cout << "Enter Hotel ID to select Hotel:";
                cin >> ch1;
                cout << "You have selected " << Hotel[ch1][0].hotelname << endl;
                int c = system("figlet ITEMS");
                for (int i = 1; i < 351; i++)
                {
                    if (MenuList[i][0].hid == ch1)
                    {
                        cout << "===================================================" << endl;
                        cout << "Dish Name : " << MenuList[i][0].dishname << "\tCost: Rs." << MenuList[i][0].cost << endl;
                        cout << MenuList[i][0].description << endl;
                        cout << "Dish ID:" << i << endl;
                        cout << "===================================================" << endl;
                    }
                }
                int did, quantity;
                cout << "Enter Dish ID to select Dish:" << endl;
                cin >> did;
                cout << "Enter Quantity:" << endl;
                cin >> quantity;
                string customer = loguser.first;
                amt += MenuList[did][0].cost * quantity;
                Orders.push_back({cid, ch1, did, quantity});
                int choice2;
                cout << "What do you want to do?" << endl;
                cout << "1.Proceed to Confirmation of Order." << endl;
                cout << "2.Order more." << endl;
                cout << "Enter your choice:";
                cin >> choice2;
                if (choice2 == 1)
                {
                    double lat, lon;
                    cout << "Enter your location in terms of latitude and longitude:";
                    cin >> lat >> lon;
                    cout << "Enter User name to confirm order:";
                    string username;
                    cin >> username;
                    cout << "Total Amount:Rs. " << amt << endl;
                    cout << "Tax:Rs. " << 0.020 * amt << endl;
                    cout << "Amount Payable:Rs. " << amt + 0.020 * amt << endl;
                    double tamt = amt + 0.20 * amt;
                    Customers[cid].push_back({username, lat, lon, tamt});
                    appendToCSV(cid, username, lat, lon, tamt, "customers.csv");
                    cout << "Amount to be paid on delivery." << endl;
                    cid++;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }

        if (loguser.first == "." && loguser.second)
        {

            cout << "===================================================" << endl;
            cout << "Delivery Guy Interface" << endl;
            cout << "Are you ready to take on the orders?(y/n):";
            string ch;
            cin >> ch;
            cout << "===================================================" << endl;
            if (ch == "y")
            {   
                if(Orders.size()==0){
                    cout << "No Orders to be Serviced."<<endl;
                    continue;
                }
                set<int> GraphHotel;
                set<int> GraphCustomer;
                Hotel = menu.readHotel(fn);
                Hotel[0].push_back({"delivery", 13.051748, 80.2232});
                GraphHotel.insert(0);
                
                for (int i = 0; i < Orders.size(); ++i)
                {
                    int hid = Orders[i].hid;
                    if (Hotel.find(hid) != Hotel.end())
                    {
                        GraphHotel.insert(hid);
                    }
                    else
                    {
                        cout << "Hotel with ID " << hid << " not found." << endl;
                    }
                }                
                vector<vector<double>> HotelGraph = createHotelGraph(GraphHotel);
                vector<Edge> mst = findMST(HotelGraph);
                int destinationhotel = printTravelDetails(mst, 75);
                Customers[0].push_back({"delivery", Hotel[destinationhotel][0].latitude, Hotel[destinationhotel][0].longitude, 0.0});
                GraphCustomer.insert(0);
                for (int i = 0; i < Orders.size(); ++i)
                {
                    int cid = Orders[i].cid;
                    if (Customers.find(cid) != Customers.end())
                    {
                        GraphCustomer.insert(cid);
                    }
                    else
                    {
                        cout << "Customer with ID" << cid << " not found." << endl;
                    }
                }
                vector<vector<double>> CustomerGraph = createCustomerGraph(GraphCustomer);
                vector<Edge> mst2 = findMST(CustomerGraph);
                int dest = printOrderDetails(mst2, 75);
                keepHeader("customers.csv");
                int abc = system("figlet ORDERS SERVICED.");
                cout << "All Orders Serviced" << endl;
                GraphCustomer.clear();
                GraphHotel.clear();
                Orders.clear();
                cout << "Exiting Delivery Guy Interface..." << endl;
                cout << "Well done, my boy!!" << endl;
                cout << "===================================================" << endl;
            }
        }
    }
    return 0;
}