//
// Created by vanilla on 2020/7/17.
//

#include "iostream"
#include "Ticket.h"
#include "boost/algorithm/string.hpp"
#include "Validate.h"
#include "Database.h"
#include "fort.hpp"
#include <cctype>

using namespace std;

/**
 * generate a list string of given tickets,
 * return "There is no ticket as result. " if list is empty.
 *
 * @param ticketList to generate string.
 * @return printable list srting of ticket list.
 * @see https://github.com/seleznevae/libfort
 */
string printableList(const vector<Ticket> &ticketList) {
    if (ticketList.empty()) return "There is no ticket as result. ";

    using namespace fort;

    char_table ticketTable;
    ticketTable << header << "Number" << "Start city" << "Reach city" <<
                "Take off time" << "Receive time" << "Price" << "Ticket number" << endr;

    for (const Ticket &ticket : ticketList)
        ticketTable << ticket.number << ticket.startCity << ticket.reachCity <<
                    ticket.takeOffTime << ticket.receiveTime << ticket.price << ticket.ticketNumber << endr;

    return ticketTable.to_string();
}

/**
 * request user input a ticket's info, insert and return this ticket.
 *
 * @return the built ticket object
 */
template<class ...Ts>
Ticket requestTicket(sqlite_orm::internal::storage_t<Ts...> storage) {
    cout << "Please input the number of the train, 0 is return: ";

    string userInput;
    getline(cin, userInput);

    vector<Ticket> queryResult = queryByField(storage, "number", userInput);
    if (userInput == "0") return Ticket();
    else if (queryResult.size() == 1) {
        cout << "Ticket of number " + userInput + " is already exist: " << endl;
        cout << printableList(vector<Ticket>{queryResult[0]});
        cout << "Consider using modify function to change it. ";

        return Ticket();
    }
    while (!isPositiveInteger(userInput)) {
        cout << "number of the train is illegal, please try again: ";
        getline(cin, userInput);

        queryResult = queryByField(storage, "number", userInput);
        if (userInput == "0") return Ticket();
        else if (queryResult.size() == 1) {
            cout << "Ticket of number " + userInput + " is already exist: " << endl;
            cout << printableList(vector<Ticket>{queryResult[0]});
            cout << "Consider using modify function to change it. ";

            return Ticket();
        }
    }
    int number = stoi(userInput);

    cout << "Input the city where the train will start: ";
    getline(cin, userInput);
    while (userInput.find_first_not_of(' ') == string::npos) {
        cout << "Start city is empty, please try again: ";
        getline(cin, userInput);
    }
    string startCity = userInput;

    cout << "Input the city where the train will reach: ";
    getline(cin, userInput);
    while (userInput.find_first_not_of(' ') == string::npos) {
        cout << "Reach city is empty, please try again: ";
        getline(cin, userInput);
    }
    string reachCity = userInput;

    cout << "Input the time which the the train take off: (HH:MM) ";
    getline(cin, userInput);
    while (!isTime(userInput)) {
        cout << "Time format is invalid, please try again: (HH:MM) ";
        getline(cin, userInput);
    }
    string takeOffTime = userInput;

    cout << "Input the time which the the train receive: (HH:MM) ";
    getline(cin, userInput);
    while (!isTime(userInput)) {
        cout << "Time format is invalid, please try again: (HH:MM) ";
        getline(cin, userInput);
    }
    string receiveTime = userInput;

    cout << "Input the price of ticket: ";
    getline(cin, userInput);
    while (!isPositiveDouble(userInput)) {
        cout << "Input is illegal, please try again: ";
        getline(cin, userInput);
    }
    double price = stod(userInput);

    cout << "Input the number of booked ticket(s): ";
    getline(cin, userInput);
    while (!isPositiveInteger(userInput)) {
        cout << "Input is illegal, please try again: ";
        getline(cin, userInput);
    }
    int ticketNumber = stoi(userInput);

    Ticket ticket = Ticket(number, startCity, reachCity, takeOffTime, receiveTime, price, ticketNumber);
    insertTicket(storage, ticket);
    return ticket;
}

/**
 * query tickets by given field and value, and retuen printable string.
 *
 * @param storage need to query.
 * @return printable string by using printableList(const vector<Ticket> &) method.
 */
template<class ...Ts>
string queryTickets(sqlite_orm::internal::storage_t<Ts...> storage) {
    cout << "1 - query by the number of train. " << endl
         << "2 - query by the city. " << endl
         << "Choose the way to query: ";

    string userInput, queryField;
    getline(cin, userInput);
    while (userInput != "1" && userInput != "2") {
        cout << R"(Input can only be "1" or "2", please try again: )";
        getline(cin, userInput);
    }

    if (userInput == "1") {
        queryField = "ticketNumber";
        cout << "Input the number of train: ";

        getline(cin, userInput);
        while (!isPositiveInteger(userInput)) {
            cout << "Input is invalid, please try again: ";
            getline(cin, userInput);
        }
    } else if (userInput == "2") {
        queryField = "ticketNumber";
        cout << "Input the city: ";

        getline(cin, userInput);
    }

    vector<Ticket> result = queryTickets(storage, queryField, userInput);
    return printableList(result);
}
