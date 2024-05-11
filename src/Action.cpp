#include "../include/Action.h"
    //golbal variable for backup initailized to nullptr
    extern WareHouse* backup;

    //  BaseAction
    BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::initailized) {}
    BaseAction::BaseAction(const BaseAction &other): errorMsg(other.errorMsg), status(other.status){}
    ActionStatus BaseAction::getStatus() const{
        return status;
    }
    void BaseAction::complete(){
        status = ActionStatus::COMPLETED;}
    void BaseAction::error(string errorMsg){
        status = ActionStatus::ERROR;
        this->errorMsg = errorMsg;
        cout << "Error: " << errorMsg << endl; 
        } 
    string BaseAction::getErrorMsg() const{
        return errorMsg;
    }
    void BaseAction::setActionStatus(ActionStatus status){
        this->status = status;
    }

    //SimulateStep
    SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){}
    SimulateStep::SimulateStep(const SimulateStep &other):BaseAction::BaseAction(other), numOfSteps(other.numOfSteps){}
    void SimulateStep::act(WareHouse &wareHouse) {
        for (int i=0; i<numOfSteps; i++){
            wareHouse.step();
        }
        setActionStatus(ActionStatus::COMPLETED);
    }
    std::string SimulateStep::toString() const {
        //template:  step <number_of_steps>
        return "step "+std::to_string(numOfSteps);
    }
    SimulateStep *SimulateStep::clone() const {return new SimulateStep(*this);}



    //AddOrder
    AddOrder::AddOrder(int id): customerId(id){}
    AddOrder::AddOrder(const AddOrder &other):BaseAction::BaseAction(other), customerId(other.customerId){}
    void AddOrder::act(WareHouse &wareHouse){
        //save it in a pointer to apply the polymorphism
        if (customerId<wareHouse.getCustomerCounter() && customerId>=0)
        {
            Customer *customer = &wareHouse.getCustomer(customerId);
            if (!customer->canMakeOrder())
                {error("Cannot place this order");}
            else{
                int order_id=wareHouse.genOrderId();
                customer->addOrder(order_id);
                //save it in a pointer to pass the responsibility of the order to the warehouse
                Order* order = new Order(order_id, customerId, customer->getCustomerDistance());
                order->setStatus(OrderStatus::PENDING);
                wareHouse.addOrder(order); //passing pointer !!
                setActionStatus(ActionStatus::COMPLETED);
                }
        }
        else  {error("Customer does not exist");}
        
        
        
    }
    string AddOrder::toString() const {
        //template: order <customer_id>
        return "order "+std::to_string(customerId);
    }
    AddOrder *AddOrder::clone() const{ return new AddOrder(*this);}



    //AddCustomer
    AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) 
    : customerName(customerName), customerType(customerType == "Soldier" ? CustomerType::Soldier : CustomerType::Civilian),
     distance(distance), maxOrders(maxOrders){} // need to check if we need to change the customerType (in case its not soldier or civilian)
    AddCustomer::AddCustomer(const AddCustomer &other):BaseAction::BaseAction(other), customerName(other.customerName), customerType(other.customerType), distance(other.distance), maxOrders(other.maxOrders){}
    void AddCustomer::act(WareHouse &wareHouse) {
        int customerId=wareHouse.genCustomerId(); // this function is not implemented yet
        Customer* customer=nullptr;
        if (customerType == CustomerType::Soldier){
            customer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
        }
        else
            customer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCostumer(customer); // need to implement this function passing pointer!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        setActionStatus(ActionStatus::COMPLETED);
    }
    AddCustomer *AddCustomer::clone() const{ return new AddCustomer(*this);}
    string AddCustomer::toString() const {
        //template: customer <customer_name> <customer_type> <customer_distance> <max_orders>
        return "customer "+customerName+" "+(customerType == CustomerType::Soldier ? "Soldier " : "Civilian ")+std::to_string(distance)+" "+std::to_string(maxOrders);
    }


    //PrintOrderStatus
    PrintOrderStatus::PrintOrderStatus(int id): orderId(id){}
    PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other):
    BaseAction::BaseAction(other),orderId(other.orderId){}
    void PrintOrderStatus::act(WareHouse &wareHouse) {
        if (0 <= orderId && orderId < wareHouse.getOrderCounter())
        {
            Order order = wareHouse.getOrder(orderId);
            cout << order.toString() << endl; //usisng the toString function from Order
            setActionStatus(ActionStatus::COMPLETED);
        }   
        else
            error("Order does not exist");
    }
    PrintOrderStatus *PrintOrderStatus::clone() const {return new PrintOrderStatus(*this);}
    string PrintOrderStatus::toString() const {
        //template: orderStatus <order_id>
        return "orderStatus "+std::to_string(orderId);
    }


    //PrintCustomerStatus
    PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}
    PrintCustomerStatus::PrintCustomerStatus(const PrintCustomerStatus &other):BaseAction::BaseAction(other), customerId(other.customerId){}
    void PrintCustomerStatus::act(WareHouse &wareHouse) {
        if (0 <= customerId && customerId < wareHouse.getCustomerCounter()) 
        {
            //save it in a pointer to apply the polymorphism
            Customer *customer = &wareHouse.getCustomer(customerId);
            cout << "CusomerID: "+ std::to_string(customer->getId()) << endl; // customer id
            vector<int> orders = customer->getOrdersIds();
            for (int i=0; static_cast<std::size_t>(i) <orders.size(); i++){ //printing all the orders
                Order order = wareHouse.getOrder(orders[i]);
                string output="OrderID: "+std::to_string(order.getId())+
                "\nOrderStatus: "+order.getStatusString();
                cout << output << endl; //orders
            }
            cout << "numOrdersLeft: "+ std::to_string(customer->getLeftOrders()) << endl; // numOrderLeft
            setActionStatus(ActionStatus::COMPLETED);
        }           
        else
            error("Customer does not exist");

    }
    PrintCustomerStatus *PrintCustomerStatus::clone() const {return new PrintCustomerStatus(*this);}
    string PrintCustomerStatus::toString() const {
        //template: customerStatus <customer_id>
        return "customerStatus "+std::to_string(customerId);
    }

    //PrintVolunteerStatus::
    PrintVolunteerStatus::PrintVolunteerStatus(int id): volunteerId(id){}
    PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other): BaseAction::BaseAction(other),volunteerId(other.volunteerId){}
    void PrintVolunteerStatus::act(WareHouse &wareHouse) {
        //save it in a pointer to apply the polymorphism
        Volunteer *volunteer = &wareHouse.getVolunteer(volunteerId);
        //if there is no volunteer with this id the default id is -1
        if (volunteer->getId() == -1) 
            error("Volunteer does not exist");
        else
        {
            cout << volunteer->toString() << endl; // volunteer id
            setActionStatus(ActionStatus::COMPLETED);
        }

    }
    PrintVolunteerStatus *PrintVolunteerStatus::clone() const {return new PrintVolunteerStatus(*this);}
    string PrintVolunteerStatus::toString() const {
        //template: volunteerStatus <volunteer_id>
        return "volunteerStatus "+std::to_string(volunteerId);
    }

    //PrintActionsLog
    PrintActionsLog::PrintActionsLog(){}
    PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction::BaseAction(other){}
    void PrintActionsLog::act(WareHouse &wareHouse){
        //template: <action_1_name> <action_1_args> <action_1_status>
        vector<BaseAction*> actions = wareHouse.getActions();
        //printing all the actions excluding the last one(log itself)
        for (int i=0; static_cast<std::size_t>(i)<actions.size()-1; i++){
            //casting the status to string
            string status=(actions[i]->getStatus() == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
            cout << actions[i]->toString() + " "+ status << endl;
        }
        setActionStatus(ActionStatus::COMPLETED);
    }
    PrintActionsLog *PrintActionsLog::clone() const {return new PrintActionsLog(*this);}
    string PrintActionsLog::toString() const {
        //template: log
        return "log";   
    }

    //Close
    Close::Close(){}
    Close::Close(const Close &other):BaseAction::BaseAction(other){}
    void Close::act(WareHouse &wareHouse) {
        //prints all the orders by this template: OrderID: <order_1_id> , CustomerID<customer_1_id> , OrderStatus: <order_1_status>
        int num_orders = wareHouse.getOrderCounter();
        for(int i=0; i<num_orders; i++){
            Order order = wareHouse.getOrder(i);
            cout << "OrderID: "+std::to_string(order.getId())+" , CustomerID: "+std::to_string(order.getCustomerId())+" , OrderStatus: "+order.getStatusString() << endl;
        }
        setActionStatus(ActionStatus::COMPLETED);
        wareHouse.close(); //needs to take care of freeing the memory
        
    }
    Close *Close::clone() const{ return new Close(*this);}
    string Close::toString() const {
        //template: close
        return "close";
    }


    //BackupWareHouse
    BackupWareHouse::BackupWareHouse(){}
    BackupWareHouse::BackupWareHouse(const BackupWareHouse &other):BaseAction::BaseAction(other){}
    void BackupWareHouse::act(WareHouse &wareHouse) {
        if (backup != nullptr)
            delete backup;
        setActionStatus(ActionStatus::COMPLETED); 
        backup=wareHouse.clone(); 
           
    }
    BackupWareHouse *BackupWareHouse::clone() const {return new BackupWareHouse(*this);}
    string BackupWareHouse::toString() const {
        //template: backup
        return "backup";
    }

    //RestoreWareHouse
    RestoreWareHouse::RestoreWareHouse(){}
    RestoreWareHouse::RestoreWareHouse(const RestoreWareHouse &other):BaseAction::BaseAction(other){}
    void RestoreWareHouse::act(WareHouse &wareHouse) {
        if (backup == nullptr)
            error("No backup available");
        else
        {
            setActionStatus(ActionStatus::COMPLETED);
            wareHouse.restore(*backup); // need to implement the restore function in WareHouse
        }
    }
    RestoreWareHouse *RestoreWareHouse::clone() const {return new RestoreWareHouse(*this);}
    string RestoreWareHouse::toString() const {
        //template: restore
        return "restore";
    }
