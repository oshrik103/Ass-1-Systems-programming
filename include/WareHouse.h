#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Action.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

using std::string;
class BaseAction;
class Volunteer;
// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order); //gets a pointer need to take care of it
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const; 
        Volunteer &getVolunteer(int volunteerId) const; 
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void parser(string input);
        //Rule of 5
        ~WareHouse();
        WareHouse(const WareHouse &other);
        WareHouse(WareHouse &&other);
        WareHouse &operator=(const WareHouse &other);
        WareHouse &operator=(WareHouse &&other);
        int getOrderCounter() const;
        void restore(WareHouse &other);
        WareHouse *clone() const;
        
        void addVolunteer(Volunteer* volunteer);
        //gengerators of ids
        int genCustomerId();
        int genOrderId();
        int genVolunteerId();
        void addCostumer(Customer* customer);
        void step();

        // auxiliary functions
        int getCustomerCounter() const;
        void releaseMemory();
        void moveOrderFromInProcess(int orderId); // move from in process to completed/pending
        // find available volunteer
        Volunteer* findAvailableVolunteer(string role, Order* order ) const;
        // remove pointers from vectors
        void removePointers(vector<BaseAction*>& pointers);
        void removePointers(vector<Volunteer*>& pointers);
        void removePointers(vector<Order*>& pointers);
        void removePointers(vector<Customer*>& pointers);
        // copy pointers from source to destination
        void copyPointers (const vector<Customer*>& source, vector<Customer*>& destination);
        void copyPointers (const vector<Order*>& source, vector<Order*>& destination);
        void copyPointers (const vector<Volunteer*>& source, vector<Volunteer*>& destination);
        void copyPointers (const vector<BaseAction*>& source, vector<BaseAction*>& destination);
        // copy primitive fields
        void copyPrimitiveFields(const WareHouse& other);

        // move pointers from source to destination
        void movePointers(vector<BaseAction*>& source, vector<BaseAction*>& destination);
        void movePointers(vector<Volunteer*>& source, vector<Volunteer*>& destination);
        void movePointers(vector<Order*>& source, vector<Order*>& destination);
        void movePointers(vector<Customer*>& source, vector<Customer*>& destination);
        void setDefaultVolunteer(Volunteer* volunteer);

        void test();//need to deleteee!!!!!!!!!!!!!!!!!!!1
    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
        Volunteer* defualtVolunteer;// create a default volunteer // so we can return a reference to a volunteer if the requested volunteer does not exist
};