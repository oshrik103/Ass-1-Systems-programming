#include "../include/Customer.h"


    Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    :id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {}
    // copy constructor
    Customer::Customer(const Customer &other): id(other.getId()), name(other.getName()), locationDistance(other.getCustomerDistance()),
    maxOrders(other.getMaxOrders()), ordersId(other.getOrdersIds()){}

    const string &Customer::getName() const{return name;}
    int Customer::getId() const{return id;}
    int Customer::getCustomerDistance() const {return locationDistance;}
    int Customer::getMaxOrders() const {return maxOrders;} //Returns maxOrders
    int Customer::getNumOrders() const {return ordersId.size();} //Returns num of orders the customer has made so far
    bool Customer::canMakeOrder() const { return getNumOrders() < maxOrders;} //Returns true if the customer didn't reach max orders
    const vector<int> &Customer::getOrdersIds() const {return ordersId;}
    int Customer::addOrder(int orderId){
        if (canMakeOrder()){
            ordersId.push_back(orderId);
            return orderId;
        }
        else{
            return -1;
        }
    } //return OrderId if order was added successfully, -1 otherwise
    int Customer::getLeftOrders() const {return maxOrders - getNumOrders();} //Returns the number of orders the customer can still make


    SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer{id,name,locationDistance,maxOrders} {}
    SoldierCustomer::SoldierCustomer(const SoldierCustomer &other): Customer::Customer(other) {}
    SoldierCustomer *SoldierCustomer::clone() const {
        return new SoldierCustomer(*this);
    }
    

    CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders): Customer{id,name,locationDistance,maxOrders} {}
    CivilianCustomer::CivilianCustomer(const CivilianCustomer &other): Customer::Customer(other) {}
    CivilianCustomer *CivilianCustomer::clone() const {
            return new CivilianCustomer(*this);
    }
