#ifndef USER_HPP  // Header guard to prevent multiple inclusions
#define USER_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> 
using namespace std;
class User{
    private:
        string username;
        string password;

    public:

        vector<pair<string, string>> readUsersFromFile() {
        vector<pair<string, string>> users; // Vector of pairs to store usernames and passwords

        ifstream infile("users.txt");
        if (!infile) {
            cerr << "Error opening file!" << endl;
            exit(1);
        }

        string fileusername, filePassword;
        while (infile >> fileusername >> filePassword) {
            users.push_back(make_pair(fileusername, filePassword));
        }

        infile.close();

        return users;
        }

        bool isUniquePhoneNo(vector<pair<string,string>> &a , string username){
                for(const auto& i : a){
                    if(i.first==username){
                        return false;
                    }
                }
                return true;
        }

        void signup() {
            ofstream outfile("users.txt", ios::app); // Append mode
            if (!outfile) {
                cerr << "Error opening file!" << endl;
                exit(1);
            }
            cout << "===================================================" <<endl;
            cout << "Signup Page" <<endl;
            cout << "Enter Username:";
            cin >> username;

            vector<pair<string, string>> a = readUsersFromFile();
            if (!isUniquePhoneNo(a, username)) {
                cout << "Username already exists. Please choose a different one or Login." << endl;
                cout << "===================================================" <<endl;
                return;
            }

            cout << "Enter password: ";
            cin >> password;
            cout << "===================================================" <<endl;

            outfile << username << " " << password <<endl;

            outfile.close();
            cout << "User signed up successfully!" << endl;
            cout << "===================================================" <<endl;
        }

        pair<string,bool> login() {
            ifstream infile("users.txt");
            if (!infile) {
                cerr << "Error opening file!" << endl;
                exit(1);
            }

            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;

            string username, password;
            while (infile >> username >> password) {
                if (username == username && password == password) {
                    cout << "Login successful!" << endl;
                    infile.close();
                    return {username,true};
                }
            }

            infile.close();
            cout << "Invalid username or password!" << endl;
            return {"",false};
        }

};


#endif