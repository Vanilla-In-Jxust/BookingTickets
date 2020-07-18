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
    int number = -1;
    string startCity = "undefined";
    string reachCity = "undefined";
    string takeOffTime = "undefined";
    string receiveTime = "undefined";
    double price = 0.0;
    int ticketNumber = -1;

    // @NoArgsConstructor.
    Ticket() = default;

    // @AllArgsConstructor.
    Ticket(int number, string startCity, string reachCity, string takeOffTime, string receiveTime,
           double price, int ticketNumber) :
            number(number), startCity(move(startCity)), reachCity(move(reachCity)),
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
        return json{{"number",       number},
                    {"startCity",    startCity},
                    {"reachCity",    reachCity},
                    {"takeOffTime",  takeOffTime},
                    {"receiveTime",  receiveTime},
                    {"price",        price},
                    {"ticketNumber", ticketNumber}};
    }

    /**
     * compare two ticket info, return true of they are same.
     *
     * @param anotherTicket to compare
     * @return true if the are same trip.
     */
    bool compare(const Ticket &anotherTicket) const {
        return this->number == anotherTicket.number;
    }
};
