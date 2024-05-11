#include "../include/WareHouse.h"

    

    WareHouse::WareHouse(const string &configFilePath):isOpen(false), actionsLog(),volunteers(),
    pendingOrders(),inProcessOrders(),completedOrders(),customers(), customerCounter(0),
    volunteerCounter(0),orderCounter(0),defualtVolunteer(new CollectorVolunteer(-1 , "default", 0))
    {
        std::ifstream file(configFilePath);
        string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;  // Skip empty lines and comments
            parser(line);
        }
        //free the actions from actionsLog to prevent them from shoing in the log
        removePointers(actionsLog);
       
    }
    
    void WareHouse::parser(string line){
        //braek the line into words
        std::istringstream iss(line);
        std::vector<string> words((std::istream_iterator<string>(iss)),
                                        std::istream_iterator<string>());
        // Parse customer
        if (words[0] == "customer") {

            string name = words[1];
            string type = words[2];
            //stoi convert string to int
            int distance = std::stoi(words[3]);
            int maxOrders = std::stoi(words[4]);
            addAction(new AddCustomer(name,type,distance,maxOrders));
        // Parse volunteer
        } else if (words[0] == "volunteer") {
            
            string name = words[1];
            string role = words[2];
            int coolDown_or_maxDistance = std::stoi(words[3]);
            if (role == "collector"){
                addVolunteer(new CollectorVolunteer(genVolunteerId(),name, coolDown_or_maxDistance));
            } else if (role == "limited_collector")
            {
                int max_orders = std::stoi(words[4]);
                addVolunteer(new LimitedCollectorVolunteer(genVolunteerId(),name, coolDown_or_maxDistance,max_orders));
            } else if (role == "driver")
            {
                int distancePerStep = std::stoi(words[4]); 
                addVolunteer(new DriverVolunteer(genVolunteerId(),name, coolDown_or_maxDistance,distancePerStep));
            } else if (role == "limited_driver")
            {
                int distancePerStep = std::stoi(words[4]);
                int max_orders = std::stoi(words[5]);
                addVolunteer(new LimitedDriverVolunteer(genVolunteerId(),name, coolDown_or_maxDistance,distancePerStep,max_orders));
            }
        } else if (words[0] == "order") {
            int customerId = std::stoi(words[1]);
            //use Action AddOrder to add the order
            addAction(new AddOrder(customerId));
        } else if(words[0]=="orderStatus")
        {
            int orderId = std::stoi(words[1]);
            //use Action PrintOrderStatus to print the order status
            addAction(new PrintOrderStatus(orderId));
        } else if(words[0]=="step")
        {
            int numOfSteps = std::stoi(words[1]);
            //use Action SimulateStep to simulate the steps
            addAction(new SimulateStep(numOfSteps));
        } else if(words[0]=="customerStatus")
        {
            int customerId = std::stoi(words[1]);
            //use Action PrintCustomerStatus to print the customer status
            addAction(new PrintCustomerStatus(customerId));
        } else if (words[0] == "volunteerStatus"){
            int volunteerId = std::stoi(words[1]);
            //use Action PrintVolunteerStatus to print the volunteer status
            addAction(new PrintVolunteerStatus(volunteerId));
        } else if (words[0] == "log"){
            //use Action PrintActionsLog to print the actions log
            addAction(new PrintActionsLog());
        } else if (words[0] == "close"){
            //use Action Close to close the warehouse
            addAction(new Close());
        } else if (words[0] == "backup"){
            //use Action BackupWareHouse to backup the warehouse
            addAction(new BackupWareHouse());
        } else if (words[0] == "restore"){
            //use Action RestoreWareHouse to restore the warehouse
            addAction(new RestoreWareHouse());
        }
        
    }
    // Start the warehouse
    void WareHouse::start(){
        open();
        cout << "Warehouse is open!" << endl; 
        while (isOpen){
            string input;
            std::getline(std::cin, input); // Get string input from the user
            parser(input); // Pass the input to the parser function
            this.isOpen = false; // close the warehouse
            (*this).isOpen = true; // open the warehouse
        }
    }

    // -------------------------- auxiliary functions --------------------------

    void WareHouse::removePointers(vector<BaseAction*>& pointers) {
        for (BaseAction* pointer : pointers) {
            delete pointer;
        }
        pointers.clear(); 
    }

    void WareHouse::removePointers(vector<Volunteer*>& pointers) {
        for (Volunteer* pointer : pointers) {
            delete pointer;
        }
        pointers.clear();
    }

    void WareHouse::removePointers(vector<Customer*>& pointers) {
        for (Customer* pointer : pointers) {
            delete pointer;
        }
        pointers.clear();
    }

    void WareHouse::removePointers(vector<Order*>& pointers) {
        for (Order* pointer : pointers) {
            delete pointer;
        }
        pointers.clear();
    }

    void WareHouse::copyPointers(const vector<Customer*>& source, vector<Customer*>& destination) {
        for (const Customer* customer : source) {
            destination.push_back(customer->clone());
        }
    }

    void WareHouse::copyPointers(const vector<Volunteer*>& source, vector<Volunteer*>& destination) {
        for (const Volunteer* volunteer : source) {
            destination.push_back(volunteer->clone());
        }
    }

    void WareHouse::copyPointers(const vector<BaseAction*>& source, vector<BaseAction*>& destination) {
        for (const BaseAction* action : source) {
            destination.push_back(action->clone());
        }
    }

    void WareHouse::copyPointers(const vector<Order*>& source, vector<Order*>& destination) {
        for (const Order* order : source) {
            destination.push_back(order->clone());
        }
    }

    void WareHouse::copyPrimitiveFields(const WareHouse& other) 
    {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

    }

    void WareHouse::movePointers(vector<BaseAction*>& source, vector<BaseAction*>& destination) {
        for (BaseAction* action : source) 
        {
            destination.push_back(action); // move the pointer to the new warehouse
            action = nullptr; // so the destructor of other won't delete the action
        }
    }

    void WareHouse::movePointers(vector<Volunteer*>& source, vector<Volunteer*>& destination) {
        for (Volunteer* volunteer : source) 
        {
            destination.push_back(volunteer); // move the pointer to the new warehouse
            volunteer = nullptr; // so the destructor of other won't delete the volunteer
        }
    }

    void WareHouse::movePointers(vector<Customer*>& source, vector<Customer*>& destination) {
        for (Customer* customer : source) 
        {
            destination.push_back(customer); // move the pointer to the new warehouse
            customer = nullptr; // so the destructor of other won't delete the customer
        }
    }

    void WareHouse::movePointers(vector<Order*>& source, vector<Order*>& destination) {
        for (Order* order : source) 
        {
            destination.push_back(order); // move the pointer to the new warehouse
            order = nullptr; // so the destructor of other won't delete the order
        }
    }

    // --------------------------- rule of five ---------------------------

    // -------------------------------------- Destructor --------------------------------------
    WareHouse::~WareHouse() 
    {
        releaseMemory();

    }

    void WareHouse::releaseMemory()
    {
        removePointers(actionsLog);
        removePointers(volunteers);
        removePointers(customers);
        removePointers(pendingOrders);
        removePointers(inProcessOrders);
        removePointers(completedOrders);
        delete defualtVolunteer;
    } 

    // ------------------------------------------------- Copy constructor -------------------------------------------------
    WareHouse::WareHouse(const WareHouse& other)
    :isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),
    customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter),defualtVolunteer(other.defualtVolunteer->clone()) 
    {
        copyPointers(other.customers, customers);
        copyPointers(other.volunteers, volunteers);
        copyPointers(other.actionsLog, actionsLog);
        copyPointers(other.pendingOrders, pendingOrders);
        copyPointers(other.inProcessOrders, inProcessOrders);
        copyPointers(other.completedOrders, completedOrders);
    }

    // -------------------------------------- Move constructor --------------------------------------
    WareHouse::WareHouse(WareHouse&& other)
    :isOpen(other.isOpen), actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(), 
    customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter),defualtVolunteer(other.defualtVolunteer)
    { 
        movePointers(other.customers, customers);
        movePointers(other.volunteers, volunteers);
        movePointers(other.actionsLog, actionsLog);
        movePointers(other.pendingOrders, pendingOrders);
        movePointers(other.inProcessOrders, inProcessOrders);
        movePointers(other.completedOrders, completedOrders);
        other.setDefaultVolunteer(nullptr);
    }

    // ------------------------------------- Copy assignment operator -------------------------------------
    WareHouse& WareHouse::operator=(const WareHouse& other) 
    {
        if (this != &other) 
        {
            // Clear existing data
            removePointers(actionsLog);
            removePointers(volunteers);
            removePointers(customers);
            removePointers(pendingOrders);
            removePointers(inProcessOrders);
            removePointers(completedOrders);
            // Copy data
            copyPointers(other.customers, customers);
            copyPointers(other.volunteers, volunteers);
            copyPointers(other.actionsLog, actionsLog);
            copyPointers(other.pendingOrders, pendingOrders);
            copyPointers(other.inProcessOrders, inProcessOrders);
            copyPointers(other.completedOrders, completedOrders);
            copyPrimitiveFields(other);
        }
        return *this;
        // no need to copy the defualtVolunteer, because it is a pointer to a constant object
    }
        
    // --------------------------------------- Move assignment operator ---------------------------------------
    WareHouse& WareHouse::operator=(WareHouse&& other) 
    {
        if (this != &other) 
        {
            // Clear existing data
            removePointers(actionsLog);
            removePointers(volunteers);
            removePointers(customers);
            removePointers(pendingOrders);
            removePointers(inProcessOrders);
            removePointers(completedOrders);

            // move data
            movePointers(other.customers, customers);
            movePointers(other.volunteers, volunteers);
            movePointers(other.actionsLog, actionsLog);
            movePointers(other.pendingOrders, pendingOrders);
            movePointers(other.inProcessOrders, inProcessOrders);
            movePointers(other.completedOrders, completedOrders);
            copyPrimitiveFields(other);
            // no need to copy the defualtVolunteer, because it is a pointer to a constant object
        }
        
        return *this;
        

    }


    // Add an order to the warehouse
    void WareHouse::addOrder(Order* order) 
    {
        pendingOrders.push_back(order);
    }

    // Add an action to the warehouse
    void WareHouse::addAction(BaseAction* action) 
    {
        actionsLog.push_back(action);
        // actionStatus alerady updated in the action
        action->act(*this);
    }


    // Add a customer to the warehouse
    void WareHouse::addCostumer(Customer* customer) 
    {
        customers.push_back(customer);
    }

    // Get a customer by ID
    Customer& WareHouse::getCustomer(int customerId) const 
    {
        for (Customer* customer : customers) 
        {
            if (customer->getId() == customerId) 
            {
                return *customer;
            }
        }
        throw std::runtime_error("Customer does not exist");
        // already checked that the customer exists
        
    }

    // Get a volunteer by ID
    Volunteer& WareHouse::getVolunteer(int volunteerId) const 
    {
        for (Volunteer* volunteer : volunteers) 
        {
            if (volunteer->getId() == volunteerId) 
            {
                return *volunteer;
            }
        }
        // if the volunteer does not exist, return the default volunteer to mark it as not found
        return *defualtVolunteer; 
    }
    

    // Get an order by ID
    Order& WareHouse::getOrder(int orderId) const 
    {
        for (Order* order : pendingOrders) 
        {
            if (order->getId() == orderId) 
            {
                return *order;
            }
        }
        for (Order* order : inProcessOrders) 
        {
            if (order->getId() == orderId) 
            {
                return *order;
            }
        }
        for (Order* order : completedOrders) 
        {
            if (order->getId() == orderId) 
            {
                return *order;
            }
        }
        // already checked that the order exists
        throw std::runtime_error("Order does not exist");
        
    }



    // Get the actions log
    const vector<BaseAction*>& WareHouse::getActions() const 
    {
        return actionsLog; 
    }
    int WareHouse::getCustomerCounter() const 
    {
        return customerCounter;
    }

    // Close the warehouse
    void WareHouse::close() 
    {
        cout << "Warehouse is closed!" << endl; 
        isOpen = false;
    }

    // Open the warehouse
    void WareHouse::open() 
    {
        isOpen = true;
    }
    // Get the order counter
    int WareHouse::getOrderCounter() const 
    {
        return orderCounter;    
    }

    // Restore the warehouse from another warehouse
    void WareHouse::restore(WareHouse& other) 
    {
        *this = other; // using the copy assignment operator
    }

    // Clone the warehouse
    WareHouse* WareHouse::clone() const 
    {
        return new WareHouse(*this);
    }

    // Generate a new customer ID
    int WareHouse::genCustomerId() 
    {
        // not incrementing the counter, because the counter is incremented when you add a new customer
        return customerCounter++;
    }

    // Generate a new order ID
    int WareHouse::genOrderId() 
    {
        // not incrementing the counter, because the counter is incremented when you add a new order
        return orderCounter++;
    }

    // Generate a new volunteer ID
    int WareHouse::genVolunteerId() 
    {
        // not incrementing the counter, because the counter is incremented when you add a new volunteer
        return volunteerCounter++;
    }




    // Auxiliary function to find an available volunteer
    Volunteer* WareHouse::findAvailableVolunteer(string role, Order* order ) const // not changing the warehouse so no need to delete pointers
    {
        for (Volunteer* volunteer : volunteers) 
        {
            if (volunteer->getVolunteerType() == role && volunteer->canTakeOrder(*order))
            {
                return volunteer;
            }
        }
        return nullptr;
    }
    // Perform a step in the warehouse
    void WareHouse::step() 
    {
        // ---------------------------------------- part 1 ---------------------------------------- 
        // Iterate through the pendingOrders list
        for (vector<Order*>::iterator orderIter = pendingOrders.begin(); orderIter != pendingOrders.end();) 
        {
            // if the order status is pending
            if ((*orderIter)->getStatus() == OrderStatus::PENDING )
            {
                // Search for an available collector volunteer
                Volunteer* collector = findAvailableVolunteer("Collector", *orderIter);

                if (collector != nullptr) // there is a collector available
                {
                    // Assign the order to the collector
                    collector->acceptOrder(**orderIter); // the volunteer accepts the order
                    (*orderIter)->setStatus(OrderStatus::COLLECTING); // now the order is collecting
                    (*orderIter)->setCollectorId(collector->getId()); // set the collector id
                    inProcessOrders.push_back(*orderIter);
                    orderIter = pendingOrders.erase(orderIter); // remove the order from the pending orders and move the iterator to the next order
                
                    collector = nullptr; // to avoid alliasing
                    
                }
                else //  no collector available
                {
                    orderIter++; // incrementing iterator
                    continue; // Move to the next order if no collector is available
                }
            }
            // the order status is collecting
            else if ((*orderIter)->getStatus() == OrderStatus::COLLECTING) 
            {
                // Search for an available driver volunteer
                Volunteer* driver = findAvailableVolunteer("Driver", *orderIter);

                if (driver != nullptr) // there is a driver available
                {
                    // Assign the order to the driver
                    driver->acceptOrder(**orderIter); // the volunteer accepts the order
                    (*orderIter)->setStatus(OrderStatus::DELIVERING); // now the order is delivering
                    (*orderIter)->setDriverId(driver->getId()); // set the driver id
                    inProcessOrders.push_back(*orderIter);
                    orderIter = pendingOrders.erase(orderIter); // remove the order from the pending orders and move the iterator to the next order
                    driver = nullptr; // to avoid memory leak
                }
                else //  no suitable driver is available
                {
                    orderIter++; // incrementing iterator
                    continue; // Move to the next order if no driver is available
                }
            }
        }
        // ---------------------------------------- part 2+3+4 ---------------------------------------- 
        for (vector<Volunteer*>::iterator volIter = volunteers.begin(); volIter != volunteers.end();) 
        {
            if ((*volIter)->isBusy()) // if the volunteer did not have an order before the step, there is nothing to do
            {
                (*volIter)->step(); // perform a step for each occupied volunteer - decrease time left for collectors and distance left for drivers
                // the volunteer finsihed his order if now he is not busy
                if (! ((*volIter)->isBusy()))
                {
                    moveOrderFromInProcess((*volIter)->getCompletedOrderId()); // send to move the order to the completed/pending orders
                    if (!((*volIter)->hasOrdersLeft())) // if the volunteer reached the limit of orders
                    {
                        delete *volIter; // free the memory
                        volIter = volunteers.erase(volIter); // erase the volunteer from the vector and move the iterator to the next element
                        continue; // skip the rest of the loop iteration
                    }
                }     
            }
            volIter++; // move to the next element in the vector
        }
    }
    void WareHouse::moveOrderFromInProcess(int orderId)
    {
        for (vector<Order*>::iterator orderIter = inProcessOrders.begin(); orderIter != inProcessOrders.end(); orderIter++) 
        {
            if ((*orderIter)->getId() == orderId) 
            {
                if ((*orderIter)->getStatus() == OrderStatus::COLLECTING)
                {
                    pendingOrders.push_back(*orderIter); 
                }
                else if ((*orderIter)->getStatus() == OrderStatus::DELIVERING)
                {
                    (*orderIter)->setStatus(OrderStatus::COMPLETED);
                    completedOrders.push_back(*orderIter);
                }
                inProcessOrders.erase(orderIter);
                return;
            }
        }
    }
    void WareHouse::setDefaultVolunteer(Volunteer* volunteer)
    {
        defualtVolunteer = volunteer;
    }
    void WareHouse::addVolunteer(Volunteer* volunteer)
    {
        volunteers.push_back(volunteer);
    }  