#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
#include "Customer.h"
#include "Volunteer.h"
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;
enum class ActionStatus{
    COMPLETED, ERROR, initailized
};

enum class CustomerType{
    Soldier, Civilian
};

class WareHouse;


class BaseAction{
    public:
        BaseAction();
        BaseAction(const BaseAction &other);
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;
        virtual ~BaseAction() = default;
    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;
        //set the status of the action
        void setActionStatus(ActionStatus status);

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int numOfSteps);
        SimulateStep(const SimulateStep &other);
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;
        

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id);
        AddOrder(const AddOrder &other);
        void act(WareHouse &wareHouse) override;
        string toString() const override;
        AddOrder *clone() const override;
        
    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
        AddCustomer(const AddCustomer &other);
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override;
        string toString() const override;
        
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};

class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        PrintOrderStatus(const PrintOrderStatus &other);
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        string toString() const override;
        
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        PrintCustomerStatus(const PrintCustomerStatus &other);
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        string toString() const override;
        
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        PrintVolunteerStatus(const PrintVolunteerStatus &other);
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
        
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        PrintActionsLog(const PrintActionsLog &other);
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;
        
    private:
};

class Close : public BaseAction {
    public:
        Close();
        Close(const Close &other);
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        string toString() const override;
        
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        BackupWareHouse(const BackupWareHouse &other);
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;
        
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        RestoreWareHouse(const RestoreWareHouse &other);
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;
        
    private:
};