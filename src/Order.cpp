#include "../include/Order.h"

    Order::Order(int id, int customerId, int distance)
    :id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}
    Order::Order(const Order &other):
    id(other.id), customerId(other.customerId), distance(other.distance), status(other.status), collectorId(other.collectorId), driverId(other.driverId){}
    int Order::getId() const{return id;}
    int Order::getCustomerId() const{return customerId;} 
    void Order::setStatus(OrderStatus status){this->status = status;}
    void Order::setCollectorId(int collectorId){ this->collectorId=collectorId;}
    void Order::setDriverId(int driverId){this->driverId=driverId;}
    int Order::getCollectorId() const {return collectorId;}
    int Order::getDriverId() const {return driverId;}
    OrderStatus Order::getStatus() const { return status;}
    const string Order::toString() const{
        string collector_str = (collectorId == NO_VOLUNTEER) ? "None" : std::to_string(collectorId);
        string driver_str = (driverId == NO_VOLUNTEER) ? "None" : std::to_string(driverId);
        string status_str= getStatusString();
        return "OrderId: " + std::to_string(id) + "\n"
            + "OrderStatus: " + status_str + "\n"
            + "CustomerID: " + std::to_string(customerId) + "\n"
            + "Collector: " + collector_str + "\n"
            + "Driver: " + driver_str;            
    }
    int Order::getDistance() const {return distance;}
    string Order::getStatusString() const{
        // transform enum value to it's name
        switch (status) {
            case OrderStatus::PENDING:
                return "Pending";
            case OrderStatus::COLLECTING:
                return "Collecting";
            case OrderStatus::DELIVERING:
                return "Delivering";
            case OrderStatus::COMPLETED:
                return "Completed";
        }
        //invalid
        return "invalid Status";
    }
    Order* Order::clone() const{
        return new Order(*this);
    }
    