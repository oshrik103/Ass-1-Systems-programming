#include "../include/Volunteer.h"

//--------------------------------------------------- class Volunteer ---------------------------------------------------
// defualt constructor
Volunteer::Volunteer(int id, const string &name): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}
// copy constructor
Volunteer::Volunteer(Volunteer& other): completedOrderId(other.getCompletedOrderId()),
activeOrderId(other.getActiveOrderId()), id(other.getId()), name(other.getName()) {}
int Volunteer::getId() const { return id; }
const string& Volunteer::getName() const { return name; }
int Volunteer::getActiveOrderId() const { return activeOrderId; }
int Volunteer::getCompletedOrderId() const { return completedOrderId; }
bool Volunteer::isBusy() const // Signal whether the volunteer is currently processing an order    
{
    if (activeOrderId != NO_ORDER)
    {
        return true;
    }
    return false;
}



//--------------------------------------------------- class CollectorVolunteer ---------------------------------------------------
// defualt constructor
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown): Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}
// copy constructor
CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other): Volunteer(other.getId(), other.getName()), coolDown(other.coolDown), timeLeft(other.timeLeft) 
{
    activeOrderId = other.getActiveOrderId();
    completedOrderId = other.getCompletedOrderId();
}
int CollectorVolunteer::getCoolDown() const {return coolDown; }
int CollectorVolunteer::getTimeLeft() const {return timeLeft; }

    bool CollectorVolunteer::decreaseCoolDown() //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
{
    timeLeft--;
    if (timeLeft == 0)
    {
        return true;
    }
    return false;
}

void CollectorVolunteer::step() 
{
    if (isBusy())
    {
        if (decreaseCoolDown())
        {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }                    
}

bool CollectorVolunteer::hasOrdersLeft() const { return true; }
bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return order.getStatus() == OrderStatus::PENDING && ! isBusy(); 
}
void CollectorVolunteer::acceptOrder(const Order &order)
{
    activeOrderId = order.getId();
    timeLeft = coolDown;
}


string CollectorVolunteer::toString() const
{
    string isBusyStr= (isBusy()) ? "True" : "False";
    string collector = "VolunteerID: " + to_string(getId()) + "\n" 
    + "isBusy: " +  isBusyStr + "\n" + "OrderId: ";
    string active_orderID = (getActiveOrderId() == NO_ORDER) ? "None" : to_string(getActiveOrderId());
    string timeLeft = (getTimeLeft() == 0) ? "None" : to_string(getTimeLeft());
    collector += active_orderID + "\n" + "TimeLeft: "+ timeLeft + "\n" + "OrdersLeft: No Limit"; 
    return collector;
}

CollectorVolunteer* CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}
string CollectorVolunteer::getVolunteerType() const
{
    return "Collector";
}



//--------------------------------------------------- class LimitedCollectorVolunteer ---------------------------------------------------
// defualt constructor
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders): CollectorVolunteer(id, name, coolDown),
maxOrders(maxOrders), ordersLeft(maxOrders)  {}

// copy constructor
LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer& other): CollectorVolunteer(other),
maxOrders(other.getMaxOrders()), ordersLeft(other.getNumOrdersLeft()) 
{}
LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const { return ordersLeft > 0;}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{ return CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft(); }

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}


int LimitedCollectorVolunteer::getMaxOrders() const { return maxOrders; }
int LimitedCollectorVolunteer::getNumOrdersLeft() const { return ordersLeft; }
string LimitedCollectorVolunteer::toString() const
{
    string isBusyStr= (isBusy()) ? "True" : "False";
    string limitedCollector = "VolunteerID: " + to_string(getId()) + "\n" + "isBusy: " +  isBusyStr + "\n" + "OrderId: ";
    string active_orderID = (getActiveOrderId() == NO_ORDER) ? "None" : to_string(getActiveOrderId());
    string timeLeft = (getTimeLeft() == 0) ? "None" : to_string(getTimeLeft());
    limitedCollector += active_orderID + "\n" + "TimeLeft: "+ timeLeft + "\n" + "OrdersLeft: " + to_string(getNumOrdersLeft());
    return limitedCollector;
}


//--------------------------------------------------- class DriverVolunteer ---------------------------------------------------
// defualt constructor
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep): Volunteer(id, name), maxDistance(maxDistance),
distancePerStep(distancePerStep), distanceLeft(0) {}

// copy constructor
DriverVolunteer::DriverVolunteer(const DriverVolunteer& other): Volunteer(other.getId(), other.getName()), maxDistance(other.getMaxDistance()),
distancePerStep(other.getDistancePerStep()), distanceLeft(other.getDistanceLeft()) 
{
    activeOrderId = other.getActiveOrderId();
    completedOrderId = other.getCompletedOrderId();
}


DriverVolunteer* DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}
int DriverVolunteer::getDistanceLeft() const { return distanceLeft; }
int DriverVolunteer::getMaxDistance() const { return maxDistance; }
int DriverVolunteer::getDistancePerStep() const { return distancePerStep; }

//Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
bool DriverVolunteer::decreaseDistanceLeft() 
{
    distanceLeft -= distancePerStep;
    if (distanceLeft <= 0)
    {
        distanceLeft = 0;
        return true;
    }
    return false; 
}

bool DriverVolunteer::hasOrdersLeft() const { return true; }
// Signal if the volunteer is not busy and the order is within the maxDistance, the order is pending and has a collector
bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    return order.getStatus() == OrderStatus::COLLECTING && ! isBusy() && order.getCollectorId() != NO_VOLUNTEER &&
    order.getDistance() <= maxDistance;
}

// Assign distanceLeft to order's distance
void DriverVolunteer::acceptOrder(const Order &order)
{
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
} 
// Decrease distanceLeft by distancePerStep
void DriverVolunteer::step()
{
    if (isBusy())
    {
        if (decreaseDistanceLeft())
        {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
} 
string DriverVolunteer::toString() const
{
    string isBusyStr= (isBusy()) ? "True" : "False";
    string driver = "VolunteerID: " + to_string(getId()) + "\n" + "isBusy: " +  isBusyStr + "\n" + "OrderId: ";
    string active_orderID = (getActiveOrderId() == NO_ORDER) ? "None" : to_string(getActiveOrderId());
    string distanceLeft = (getDistanceLeft() == 0) ? "None" : to_string(getDistanceLeft());
    // according to the template DistanceLeft should be called TimeLeft, but it does not make sense for a driver 
    driver += active_orderID + "\n" + "DistanceLeft: "+ distanceLeft + "\n" + "OrdersLeft: No Limit"; 
    return driver;
}

string DriverVolunteer::getVolunteerType() const
{
    return "Driver";
}


//--------------------------------------------------- class LimitedDriverVolunteer ---------------------------------------------------
// defualt constructor
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}
// copy constructor
LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer& other): DriverVolunteer(other), maxOrders(other.getMaxOrders()),
ordersLeft(other.getNumOrdersLeft()) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const { return maxOrders; }
int LimitedDriverVolunteer::getNumOrdersLeft() const { return ordersLeft; }
bool LimitedDriverVolunteer::hasOrdersLeft() const { return ordersLeft > 0; }
/* Signal if the volunteer is not busy, the order is within the maxDistance, the order is pending and has a collector,
and the volunteer has orders left */
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}
// Assign distanceLeft to order's distance and decrease ordersLeft
void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
} 

string LimitedDriverVolunteer::toString() const
{
    string isBusyStr= (isBusy()) ? "True" : "False";
    string limitedDriver = "VolunteerID: " + to_string(getId()) + "\n" + "isBusy: " +  isBusyStr + "\n" + "OrderId: ";
    string active_orderID = (getActiveOrderId() == NO_ORDER) ? "None" : to_string(getActiveOrderId());
    string distanceLeft = (getDistanceLeft() == 0) ? "None" : to_string(getDistanceLeft());
        // according to the template DistanceLeft should be called TimeLeft, but it does not make sense for a driver 
    limitedDriver += active_orderID + "\n" + "DistanceLeft: "+ distanceLeft + "\n" + "OrdersLeft: " + to_string(getNumOrdersLeft());
    return limitedDriver;
}

