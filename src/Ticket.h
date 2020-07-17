//
// Created by vanilla on 2020/7/17.
//

#include "string"
#include "../include/json.hpp"

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
};
