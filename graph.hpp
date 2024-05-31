#ifndef GRAPH_HPP // Header guard to prevent multiple inclusions
#define GRAPH_HPP
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
#include <stdexcept>
#include <random>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "menu.hpp"
using namespace std;
struct Edge
{
    int src;
    int dest;
    double weight;
};
struct CompareEdge
{
    bool operator()(const Edge &a, const Edge &b)
    {
        return a.weight > b.weight;
    }
};
struct Customer
{
    string username;
    double lat;
    double lon;
    double amt;
};
struct Order
{
    int cid;
    int hid;
    int did;
    int quantity;
};
map<int, vector<Customer>> readCustomersFromCSV(const string &filename)
{
    map<int, vector<Customer>> Customers;

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return Customers;
    }

    string line;
    // Skip the header line
    getline(inputFile, line);

    // Read data from each line and store it in the map
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string token;

        int cid;
        string name;
        double lat, lon, amt;

        // Read each comma-separated value from the line
        getline(iss, token, ',');
        cid = stoi(token);

        getline(iss, name, ',');
        getline(iss, token, ',');
        lat = stod(token);

        getline(iss, token, ',');
        lon = stod(token);

        getline(iss, token, ',');
        amt = stod(token);

        // Create a Customer object and add it to the map
        Customers[cid].push_back({name, lat, lon, amt});
    }

    inputFile.close();
    return Customers;
}
double degreesToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}
double calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371e3 / 1000; // Earth's radius in km
    double dLat = degreesToRadians(lat2 - lat1);
    double dLon = degreesToRadians(lon2 - lon1);
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(degreesToRadians(lat1)) * cos(degreesToRadians(lat2)) *
                   sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c * 7.5;
}
vector<vector<double>> createCustomerGraph(set<int> &GraphCust)
{
    map<int, vector<Customer>> Customers = readCustomersFromCSV("customers.csv");
    Customers[0].push_back({"delivery", 13.051748, 80.2232, 0});
    vector<vector<double>> result(100, vector<double>(100, 0.0));
    for (int hid1 : GraphCust)
    {
        if (GraphCust.find(hid1) == GraphCust.end())
        {
            // Handle case where HotelLoc does not contain entry for hid1
            continue;
        }
        double lat1 = Customers[hid1][0].lat;
        double long1 = Customers[hid1][0].lon;
        for (int hid2 : GraphCust)
        {
            if (GraphCust.find(hid2) == GraphCust.end())
            {
                // Handle case where HotelLoc does not contain entry for hid2
                continue;
            }
            double lat2 = Customers[hid2][0].lat;
            double long2 = Customers[hid2][0].lon;
            double distance = calculateDistance(lat1, long1, lat2, long2);
            result[hid1][hid2] = distance;
            result[hid2][hid1] = distance;
        }
    }

    return result;
}
vector<vector<double>> createHotelGraph(set<int> &Hotel)
{
    Menu menu;
    map<int, vector<Location>> HotelLoc = menu.readHotel("hotels.csv");
    HotelLoc[0].push_back({"delivery", 13.051748, 80.2232});
    vector<vector<double>> result(36, vector<double>(36, 0.0));
    for (int hid1 : Hotel)
    {
        if (HotelLoc.find(hid1) == HotelLoc.end())
        {
            // Handle case where HotelLoc does not contain entry for hid1
            continue;
        }
        double lat1 = HotelLoc[hid1][0].latitude;
        double long1 = HotelLoc[hid1][0].longitude;
        for (int hid2 : Hotel)
        {
            if (HotelLoc.find(hid2) == HotelLoc.end())
            {
                // Handle case where HotelLoc does not contain entry for hid2
                continue;
            }
            double lat2 = HotelLoc[hid2][0].latitude;
            double long2 = HotelLoc[hid2][0].longitude;
            double distance = calculateDistance(lat1, long1, lat2, long2);
            result[hid1][hid2] = distance;
            result[hid2][hid1] = distance;
        }
    }

    return result;
}

vector<Edge> findMST(const vector<vector<double>> &graph)
{
    int n = graph.size();
    priority_queue<Edge, vector<Edge>, CompareEdge> pq;
    unordered_set<int> visited;

    for (int j = 1; j < n; ++j)
    {
        if (graph[0][j] > 0)
        {
            pq.push({0, j, graph[0][j]});
        }
    }

    visited.insert(0);

    vector<Edge> mst;
    while (!pq.empty() && visited.size() < n)
    {
        Edge e = pq.top();
        pq.pop();
        if (visited.find(e.dest) != visited.end())
        {
            continue;
        }
        visited.insert(e.dest);
        mst.push_back(e);

        for (int j = 0; j < n; ++j)
        {
            if (graph[e.dest][j] > 0 && visited.find(j) == visited.end())
            {
                pq.push({e.dest, j, graph[e.dest][j]});
            }
        }
    }
    return mst;
}

int printTravelDetails(const vector<Edge> &mst, int speed)
{
    Menu menu;
    map<int, vector<Location>> HotelLoc = menu.readHotel("hotels.csv");
    HotelLoc[0].push_back({"delivery point", 13.051748, 80.2232});
    double total_distance = 0.0;
    double elapsed_time = 0.0;
    int a;
    cout << "Delivery Guy is on the way to pickup." << endl;
    for (const Edge &edge : mst)
    {
        double distance = edge.weight;
        total_distance += distance;
        double timet = distance / speed;
        elapsed_time += timet;
        cout << "===================================================" << endl;
        cout << "Delivery Picked Up From " << HotelLoc[edge.dest][0].hotelname << endl;

        sleep(2);

        cout << "Distance: " << distance << " km, Time: " << timet * 60 << " mins." << endl;
        cout << "===================================================" << endl;
        sleep(2);

        a = edge.dest;
    }
    return a;
}
int printOrderDetails(const vector<Edge> &mst, int speed)
{
    map<int, vector<Customer>> Customers = readCustomersFromCSV("customers.csv");
    Customers[0].push_back({"delivery", 13.051748, 80.2232, 0});
    double total_distance = 0.0;
    double elapsed_time = 0.0;
    int a;
    cout << "Delivery on the Way." << endl;
    for (const Edge &edge : mst)
    {
        double distance = edge.weight;
        total_distance += distance;
        double timet = distance / speed;
        elapsed_time += timet;
        cout << "===================================================" << endl;
        cout << "Order Delivered to " << Customers[edge.dest][0].username << endl;
        cout << "Amount for Dish:" << Customers[edge.dest][0].amt <<endl;
        cout << "Delivery Charges : " << total_distance*0.3 <<endl;
        cout << "Total Amount to be paid: " << total_distance*0.3+ Customers[edge.dest][0].amt<<endl;


        sleep(2);

        cout << "Distance: " << distance << " km, Time: " << timet * 60 << " mins." << endl;
        cout << "===================================================" << endl;
        cout<<endl;
        
        sleep(2);

        a = edge.dest;
    }
    return a;
}

#endif