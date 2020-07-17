//
// Created by vanilla on 2020/7/17.
//

#include "string"
#include "../include/json.hpp"
#include "boost/lexical_cast.hpp"
#include "iomanip"

using namespace std;

class Ticket {
public:
    // id is autoincrement primary key, so it is -1.
    int id = -1;
    string startCity;
    string reachCity;
    string takeOffTime;
    string receiveTime;
    double price;
    int ticketNumber;

    // All args constructor.
    Ticket(string startCity, string reachCity, string takeOffTime, string receiveTime, double price, int ticketNumber) :
            startCity(move(startCity)), reachCity(move(reachCity)),
            takeOffTime(move(takeOffTime)), receiveTime(move(receiveTime)),
            price(price), ticketNumber(ticketNumber) {}

    using json = nlohmann::json;

    /**
     * convert a ticket object to json object.
     * Also, C++ do not have the real reflection.
     *
     * @return json object
     * @see https://stackoverflow.com/questions/17549906/c-json-serialization
     */
    json toJson() {
        return json{{"id",           id},
                    {"startCity",    startCity},
                    {"reachCity",    reachCity},
                    {"takeOffTime",  takeOffTime},
                    {"receiveTime",  receiveTime},
                    {"price",        price},
                    {"ticketNumber", ticketNumber}};
    }

    /**
     * a printable format of ticket object, splitter is " || ".
     *
     * @return printable string of ticket.
     */
    string toString() const {
        ostringstream pricePrecisionStream;
        pricePrecisionStream << fixed << setprecision(2) << price;

        return to_string(id) + " || " + startCity + " || " + reachCity + " || " +
               takeOffTime + " || " + receiveTime + " || " +
               pricePrecisionStream.str() + " || " + to_string(ticketNumber);
    }

    /**
     * compare two ticket info, return true of they are same.
     *
     * @param anotherTicket to compare
     * @return true if the are same trip.
     */
    bool compare(const Ticket &anotherTicket) const {
        return this->startCity == anotherTicket.startCity && this->reachCity == anotherTicket.reachCity
               && this->takeOffTime == anotherTicket.takeOffTime && this->receiveTime == anotherTicket.receiveTime;
    }
};
