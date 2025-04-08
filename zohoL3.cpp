#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <set>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

class CustomerBooking {
public:
    char src, dest;
    string d_name;
    char d_gender;   
    int fare_amount;
    
    CustomerBooking(char s, char d, string name, char gender, int f) : src(s), dest(d), d_name(name), d_gender(gender), fare_amount(f) {}
};

class DriverBooking {
public:
    char src, dest;
    int fare_amount;
    float zulu_commission;
    float final_profit;
    DriverBooking(char s, char d, int f, int zc, int fp) : src(s), dest(d), fare_amount(f), zulu_commission(zc), final_profit(fp) {}
    
};


class Customer {
public:
    int type = 1;
    int customer_id = 4;
    string customer_name;
    int customer_age;
    string customer_password;
    char customer_gender;
    vector<CustomerBooking*> customer_bookings;
    Customer(int id, string name, int age, string pass, char gender):customer_id(id), customer_name(name), customer_age(age), customer_password(pass), customer_gender(gender) {}
    Customer();
};


class Driver {
public:
    int type = 2;
    int trips = 0;
    int available = 1;
    int driver_id;
    string driver_name;
    int driver_age;
    string driver_password;
    char driver_gender;
    vector<DriverBooking*> driver_bookings;
    Driver(int id, string name, int age, string pass, char gender):driver_id(id), driver_name(name), driver_age(age), driver_password(pass), driver_gender(gender) {}
    
};

class Admin {
public:
    int type = 3;
    
};



class TaxiApp {
    
public:
    Customer* logged_in_customer = NULL;
    Driver* logged_in_driver = NULL;
    Admin* logged_in_admin = NULL;
    float total_revenue = 0;
    unordered_map<char, int> location_fair_map;
    unordered_map<int, char> initial_cabId_pos;
    vector<Driver*> drivers;
    vector<Customer*> customers;
    
    vector<char> loc_name = {'A', 'C', 'D', 'F', 'B', 'G', 'H', 'E'};
    vector<int> fair_from_origin = {0, 4, 7, 9, 15, 18, 20, 23};
    
    void bookCab() {
        cout << "Cab Ids and its locations" << endl;
        for (auto cp:initial_cabId_pos) {
            cout << "Cab Id: " << cp.first << " Cab position: " << cp.second << endl;
        }
        cout << "Enter source and Destination A - H => ";
        char src, dest; cin >> src >> dest;
        int fare = abs(location_fair_map[src] - location_fair_map[dest]) * 10;
        
        // search for nearest;
        cout << "Fare amount: " << fare << endl;
        int minimum_distance = INT_MAX;
        int least_trips = INT_MAX;
        Driver* pickupDriver = NULL;
        char pickupDriverLoc = 'A';
        for (Driver* d:drivers) {
            if (d -> available == 1) {
                char dPos = 'A';
                for (auto cp:initial_cabId_pos) {
                    if (cp.first == d -> driver_id) {
                        dPos = cp.second;
                        break;
                    }
                }
                
                int drInd = -1, cuInd = -1;
                for (int i = 0; i < loc_name.size(); i++) {
                    if (loc_name[i] == src) {
                        cuInd = i;
                    }
                    if (loc_name[i] == dPos) {
                        drInd = i;
                    }
                }
                
                int dist_dr_cu = abs(fair_from_origin[drInd] - fair_from_origin[cuInd]);
                if (dist_dr_cu < minimum_distance) {
                    least_trips = d -> trips;
                    minimum_distance = dist_dr_cu;
                    pickupDriver = d;
                    pickupDriverLoc = dPos;
                } else if (dist_dr_cu == minimum_distance) {
                    if (d -> trips <= pickupDriver -> trips) {
                        pickupDriver = d;
                        pickupDriverLoc = dPos;
                    }
                }
                
            }
        }
        if (pickupDriver == NULL) {
            cout << "Sorry! No Drivers Available currently!" << endl;
            return;
        }
        pickupDriver -> available = -1;
        cout << "Driver arriving from location: " << pickupDriverLoc << endl;
        cout << "Pick up Driver:\n Name -> " << pickupDriver -> driver_name << " Id -> " << pickupDriver -> driver_id << " Age -> " << pickupDriver -> driver_age << " Gender -> " << pickupDriver -> driver_gender << endl;
        
        cout << "Journey Confirmed! Happy ride!" << endl;
        
        for (Driver* d:drivers) {
            if (d -> available != 1) {
                d -> available++;
            }
        }
        pickupDriver -> trips++;
        
        initial_cabId_pos[pickupDriver->driver_id] = dest;
        
        logged_in_customer -> customer_bookings.push_back(new CustomerBooking(src, dest, pickupDriver->driver_name, pickupDriver->driver_gender, fare));
        
        float driver_zulu_c = (fare * (30/100.0));
        float driver_final_profit = (fare - driver_zulu_c);
        pickupDriver -> driver_bookings.push_back(new DriverBooking(src, dest, fare, driver_zulu_c, driver_final_profit));
        
        total_revenue += driver_zulu_c;
        
    }
    
    
    
    
    void CustomerSummary() {
        cout << "All bookings -> \n";
        for (CustomerBooking* cs:logged_in_customer->customer_bookings) {
            cout << "Source -> " << cs -> src << endl;
            cout << "Destination -> " << cs -> dest << endl;
            cout << "Driver name -> " << cs -> d_name << endl;
            cout << "Gender -> " << cs -> d_gender << endl;
            cout << "Fare amount -> " << cs -> fare_amount << endl;
        }
    }
    
    
    void customerLogin() {
        cout << "Enter Id and Password: ";
        int id; string p; cin >> id >> p;
        for (Customer* c:customers) {
            if (c -> customer_id == id && c -> customer_password == p) {
                cout << "Customer found! Logged in id " << id << endl;
                logged_in_customer = c;
                break;
            }
        }
        if (logged_in_customer == NULL) {
            cout << "No customer found!" << endl; return;
        }
        
        cout << "-------1. Book cab\n2. Summary => ";
        int choice; cin >> choice;
        switch (choice) {
            case 1:
                bookCab();
                break;
            case 2:
                CustomerSummary();
                break;
            default:
                break;
        }
        
        
        
    }
    
    void addCab() {
        cout << "Enter driver name, age, password, gender, cab position: " << endl;
        string name, p; int age; char g; char pos;
        cin >> name >> age >> p >> g >> pos;
        Driver* newDriver = new Driver(5, name, age, p, g);
        drivers.push_back(newDriver);
        initial_cabId_pos[5] = pos;
        
        
    }
    

    void printCustomers() {
        cout << "All customers" << endl;
        for (Customer* c:customers) {
            cout << "Customer Name: " << c->customer_name << " Age: " << c->customer_age << " Id: " << c -> customer_id << endl;
            cout << "Customer summary: " << endl;
            for (CustomerBooking* b:c->customer_bookings) {
                cout << "Source: " << b->src << " Destination: " << b->dest << " Fare amount: " << b->fare_amount << endl;
            }
        }
    }
    void printDrivers() {
        cout << "All Drivers" << endl;
        for (Driver* d:drivers) {
            cout << "Driver Name: " << d->driver_name << " Age: " << d->driver_age << " Id: " << d -> driver_id << endl;
            for (DriverBooking* b:d->driver_bookings) {
                cout << "Source: " << b->src << " Destination: " << b->dest << " Fare amount: " << b->fare_amount << endl;
            }
        }
    }
    
    
    void addLoc() {
        cout << "Enter Location point and distance from origin: ";
        char point; int d;
        cin >> point >> d;
        
    }
    void adminLogin() {
        cout << "Enter password: ";
        string p;
        cin >> p;
        if (p == "zulu") {
            cout << "\n1 Add cab\n2 All customers\n3 All drivers\n4 Total revenue\n5 Add location";
            int choice; cin >> choice;
            switch (choice) {
                case 1:
                    addCab();
                    break;
                case 2:
                    printCustomers();
                    break;
                case 3:
                    printDrivers();
                    break;
                case 4:
                    cout << "Zulu Total revenue: " << total_revenue << endl;
                    break;
                case 5:
                    addLoc();
                    break;
                default:
                    break;
            }

        } else {
            cout << "Incorrect Password!";
        }
    }
    
    void driverLogin() {
        int id; string p;
        cout << "Enter driver id and password: ";
        cin >> id >> p;
        for (Driver* d:drivers) {
            if (d -> driver_id == id && d -> driver_password == p) {
                cout << "Driver logged in -> id " << id << endl;
                logged_in_driver = d;
                break;
            }
        }
        if (logged_in_driver == NULL) {
            cout << "No driver id found" << endl;
            return;
        }
        cout << "Driver Summary: ";
        for (DriverBooking* db:logged_in_driver->driver_bookings) {
            cout << "Source: " << db->src << " Destination: " << db->dest << " Fare amount: " << db->fare_amount << " Zulu Commission: " << db->zulu_commission << " Final Profit: " << db->final_profit << endl;
        }
        
    }
    
    void login() {
        cout << "Login as 1. Customer\n2. Admin\n3. Driver => ";
        int choice; cin >> choice;
        switch (choice) {
            case 1:
                customerLogin();
                break;
            case 2:
                adminLogin();
                break;
            case 3:
                driverLogin();
                break;
                
            default:
                break;
        }
    }
    
    void CustomerSignin() {
        cout << "Sign in\nEnter name, age, gender, password: ";
        string name, password; int age; char gender;
        cin >> name >> age >> gender >> password;
        
        Customer* newCustomer = new Customer(5, name, age, password, gender);
        
        customers.push_back(newCustomer);
        
    }
    
    
    void menu() {

        
        drivers.push_back(new Driver(1, "aaa", 43, "111", 'M'));
        drivers.push_back(new Driver(2, "bbb", 31, "222", 'M'));
        drivers.push_back(new Driver(3, "ccc", 38, "333", 'F'));
        drivers.push_back(new Driver(4, "ddd", 28, "444", 'F'));
        
        customers.push_back(new Customer(1, "zz", 25, "99", 'F'));
        customers.push_back(new Customer(2, "yy", 61, "88", 'M'));
        customers.push_back(new Customer(3, "xx", 22, "77", 'M'));
        customers.push_back(new Customer(4, "ww", 36, "66", 'F'));
        
        initial_cabId_pos[1] = 'D';initial_cabId_pos[2] = 'G';initial_cabId_pos[3] = 'H';initial_cabId_pos[4] = 'A';
        
        for (int i = 0; i < loc_name.size(); i++) {
            location_fair_map[loc_name[i]] = fair_from_origin[i];
        }
        
        
        while (1) {
            cout << "----------\n1 Sign in\n2 Login\n3 Exit -> ";
            int choice; cin >> choice;
            switch (choice) {
                case 1:
                    cout << "Sign in as customer => ";
                    CustomerSignin();
                    break;
                case 2:
                    login();
                    break;
                case 3:
                    cout << "Bye";
                    break;
                default:
                    break;
            }
            
        }
        
    }
    TaxiApp () {}
};



int main() {
    

    TaxiApp t = TaxiApp();
    t.menu();
    
    
    
    
    cout << endl;
    return 0;
}

