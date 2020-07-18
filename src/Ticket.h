//
// Created by vanilla on 2020/7/17.
//

#include "string"
#include "boost/lexical_cast.hpp"
#include "iomanip"
#include "json/json.h"

using namespace std;

class Ticket {
public:
    int id = -1;

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


    /**
     * convert a ticket object to json object.
     * Also, C++ do not have the real reflection.
     *
     * @return json object
     * @see https://stackoverflow.com/questions/17549906/c-json-serialization
     */
    Json::Value toJson() const {
        Json::Value ticketJson;

        ticketJson["id"] = to_string(id);
        ticketJson["number"] = to_string(number);
        ticketJson["startCity"] = startCity;
        ticketJson["reachCity"] = reachCity;
        ticketJson["takeOffTime"] = takeOffTime;
        ticketJson["receiveTime"] = receiveTime;
        ticketJson["price"] = to_string(price);
        ticketJson["ticketNumber"] = to_string(ticketNumber);

        return ticketJson;
    }
};
