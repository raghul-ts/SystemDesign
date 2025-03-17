#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class User {
    
private:
    int userId = 1111;
    string name, user_name, password;
    
public:
    
    int type = 0;
    User (string name, string user_name, string password, int uid) : name(name), user_name(user_name), password(password), userId(uid) {}
    string getUserName() { return user_name; }
    string getName() { return name; }
    string getPassword() { return password; }
    int getUserId() { return userId; }
    User() {}
};

class Payment {
    
};

class Passenger {
protected:
    string name;
    int age;
    string seatAllocated;
public:
    Passenger (string name, int age, string sa) : name(name), age(age), seatAllocated(sa) {}
    Passenger () {}
};


class Train {
private:
    int train_no;
    string source;
    string destination;
    int fare;
    string dateOfJourney;
    int berth = 3, rac = 2, wt = 1;
    vector<Passenger*> berthLocation;
    queue<Passenger*> racQ;
    queue<Passenger*> wtQ;
public:
    Train(int tn, string src, string dest, int f, string doj) : train_no(tn), source(src), destination(dest), fare(f), dateOfJourney(doj) {
        //        berthLocation.assign(3, nullptr);
    }
    int getTrainNo() {return train_no;}
    string getTrainSource() {return source;}
    string getTrainDestination() {return destination;}
    int getTrainFare() {return fare;}
    string getDOJ() {return dateOfJourney;}
    int getBerth() {return berth;}
    int getRAC() {return rac;}
    int getWT() {return wt;}
    void setBerth(int val) {
        berth += val;
    }
    void setRac(int val) {
        rac += val;
    }
    void setWt(int val) {
        wt += val;
    }
    vector<Passenger*> getBerthLocation() {return berthLocation;}
    void pushRacQ(Passenger* p) {
        racQ.push(p);
    }

};




class Railway : public User, public Passenger {
    
public:
    User* loggedInUser = nullptr;
    int userIdItr = 1;
    unordered_map<int, User*> userHashMap;
    unordered_map<int, Train*> allTrains;
    
    void registerUser() {
        string name, user_name, password;
        cout << "Enter name, user_name, password: "; cin >> name >> user_name >> password;
        
        User* newUser = new User(name, user_name, password, getUserId() + userIdItr++);
        userHashMap[getUserId()+userIdItr-1] = newUser;
    }
    
    bool findUser(string UN, string P) {
        for (auto user:userHashMap) {
            if (user.second->getUserName() == UN && user.second->getPassword() == P) {
                loggedInUser = user.second;
                return true;
            }
        }
        return false;
    }
    
    void loginUser() {
        if (loggedInUser) {
            cout << "Logout First\n";
            return;
        }
        string user_name, password;
        cout << "Enter user_name & password: "; cin >> user_name >> password;

        if (user_name == "admin" && password == "abcde") {
            loggedInUser = new User();
            loggedInUser -> type = 1;
            cout << "Logged In as Admin\n";
            return;
        }
        if (findUser(user_name, password)) {
            cout << "Logged In - " << loggedInUser -> getUserName() << endl;
        } else cout << "Invalid Credentials\n";
    }
    
    void logoutUser() {
        if (loggedInUser) {
            if (loggedInUser -> type == 1) {
                cout << "Admin Logged out\n";
                loggedInUser = NULL;
                return;
            }
            cout << "Logged out - " << loggedInUser -> getUserName() << endl;
            loggedInUser = NULL;
        } else cout << "No User Logged In!" << endl;
    }
    

    void allocateTicket(char pref, string name, int age, int train_no) {
        if (allTrains[train_no]->getBerth()==0) {
            if (allTrains[train_no]->getRAC() != 0) {
//                Passenger* p = new Passenger(name, age, )
            }
        }
    }
    void bookTickets() {
        if (!loggedInUser) {
            cout << "Login First\n";
            return;
        }
        string doj, source, destination;
        int noOfPass;
        cout << "Enter DOJ(dd-mm-yyyy), No of Passengers, source and destination: "; cin >> doj >> noOfPass >> source >> destination;

        cout << "Available Trains: \n";
        for (auto train:allTrains) {
            if (doj == train.second->getDOJ() && source == train.second->getTrainSource() && destination == train.second->getTrainDestination()) {
                cout << "Train No: " << train.first << ", Source: " << train.second->getTrainSource() << ", Destination: " << train.second->getTrainDestination() << ", Travel Fare: " << train.second->getTrainFare() << endl;
            }
        }
        int bookTrainNo;
        cout << "Enter Train No. to book - "; cin >> bookTrainNo;
        int wt = allTrains[bookTrainNo]->getWT();
        int rac = allTrains[bookTrainNo]->getRAC();
        int berth = allTrains[bookTrainNo]->getBerth();
        if (wt+rac+berth == 0) {
            cout << "No tickets available\n"; return;
        }
        
        cout << "Availability => \n BERTH: " << berth << " || RAC: " << rac << " || WL: " << wt << endl;
        
        for (int i = 0; i < noOfPass; i++) {
            string name; int age; char pref;
            cout << "Enter name and age of Passenger " << i+1 << ": "; cin >> name >> age;
            cout << "Enter preference (L|U|M): "; cin >> pref;
            allocateTicket(pref, name, age, bookTrainNo);

        }
        
        
    }
    
    void addNewTrain() {
        if (!loggedInUser) {
            cout << "Login as Admin first\n"; return;
        }
        if (loggedInUser -> type != 1) {
            cout << "You are not an Admin\n";
            return;
        }
        cout << "Enter Train No, Source, Destination, Travel Fare, date of journey: ";
        int tn, f; string src, dest, doj; cin >> tn >> src >> dest >> f >> doj;
        allTrains[tn] = new Train(tn, src, dest, f, doj);
        
    }
    
    void showAllUser() {
        if (!loggedInUser) {cout << "Login as Admin first\n"; return;}
        if (loggedInUser -> type == 1) {
            for (auto user:userHashMap) {
                cout << "UserId: " << user.second->getUserId() << " UserName: " << user.second->getUserName() << " Name: " << user.second->getName() << endl;
            }
        } else cout << "You are not Admin\n";
    }
    
    void menu() {
        bool qFlag = true;
        while (qFlag) {
            cout << "\n\nSOURCES: CBE, MADRAS, BLR|| DESTINATIONS: CBE, MADRAS, BLR" << endl;
            cout<<"1.Register\n2.Login\n3.Book Tickets\n4.Check PNR status\n5.Logout\n6.Exit\n7.Add train\n8.Show all users =>> ";
            int choice; cin >> choice;
            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loginUser();
                    break;
                case 3:
                    bookTickets();
                    break;
                case 5:
                    logoutUser();
                    break;
                case 6:
                    cout << "Bye Bye!" << endl;
                    qFlag = false;
                    break;
                case 7:
                    addNewTrain();
                    break;
                case 8:
                    showAllUser();
                    break;
                default:
                    break;
            }
        }
    }
    
    Railway() {}
};


int main() {
    
    Railway r = Railway();
    r.menu();
    

    return 0;
}


// signin
// login
// check pnr status or refund -> pnr no, show refundable amount
// date of journey
// no of passengers, name, age
// origin, destination
// class type
// display train-list, seat availability
// select train
// (payment)
// status, pnr -> payment details to admin
// 63 berths 18 rac 10 wt
