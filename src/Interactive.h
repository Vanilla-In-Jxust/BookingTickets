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
 * return no ticket info if list is empty.
 *
 * @param ticketList to generate string.
 * @return printable list string of ticket list.
 * @see https://github.com/seleznevae/libfort
 */
string printableList(const vector<Ticket> &ticketList) {
    if (ticketList.empty()) return "There is no ticket to show, consider insert first. ";

    using namespace fort;

    utf8_table ticketTable;

    for (int i = 1; i < 5; ++i)
        ticketTable.column(i).set_cell_text_align(text_align::center);

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
    cout << "Ticket of number " + to_string(ticket.ticketNumber) +  " saved. " << endl;

    return ticket;
}

/**
 * query tickets by given field and value, and return printable string.
 *
 * @param storage need to query.
 * @return printable string by using printableList(const vector<Ticket> &) method.
 */
template<class ...Ts>
string searchTickets(sqlite_orm::internal::storage_t<Ts...> storage) {
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
        queryField = "reachCity";
        cout << "Input the city: ";

        getline(cin, userInput);
    }

    vector<Ticket> result = queryByField(storage, queryField, userInput);
    return printableList(result);
}

/**
 * let user book some ticket(s), update ticket info and return bookNum.
 *
 * @param storage need to query ticket
 * @return bookNum, number of user booked.
 */
template<class ...Ts>
int bookTicket(sqlite_orm::internal::storage_t<Ts...> storage) {
    cout << "Please input the ticket number you want: ";

    string userInput;
    getline(cin, userInput);
    while (!isPositiveInteger(userInput)) {
        cout << "Input is not number, please try again: ";
        getline(cin, userInput);
    }

    vector<Ticket> result = queryByField(storage, "number", userInput);
    if (result.empty()) {
        cout << "There is no ticket of number " + userInput + ", please query first. ";
        return -1;
    }

    Ticket ticket = result[0];
    cout << printableList(vector<Ticket>{ticket});

    cout << "Input your bookNum: ";

    start:
    getline(cin, userInput);
    while (!isPositiveInteger(userInput) || userInput == "0") {
        cout << "Input is not number or illegal, please try again: ";
        getline(cin, userInput);
    }

    int bookNum = stoi(userInput);
    while (bookNum > ticket.ticketNumber) {
        cout << "bookNum " + userInput + " > stock ticket number " +
                to_string(ticket.ticketNumber) + ", please try again: ";
        goto start;
    }

    cout << "Input your name: ";
    getline(cin, userInput);

    cout << "Input your id: ";
    getline(cin, userInput);
    while (!isChinaId(userInput)) {
        cout << "Your input is not legal id, please try again: ";
        getline(cin, userInput);
    }

    ticket.ticketNumber -= bookNum;
    storage.update(ticket);
    cout << "Lucky! You have booked " + to_string(bookNum) + " ticket(s)! ";

    return bookNum;
}

/**
 * allow user to modify a ticket

 * @param storage need to query ticket
 * @return modified ticket
 */
template<class ...Ts>
Ticket modifyTicket(sqlite_orm::internal::storage_t<Ts...> storage) {
    cout << "Please input ticket number to modify: ";

    string userInput;
    getline(cin, userInput);
    while (!isPositiveInteger(userInput)) {
        cout << "Input is not number, please try again: ";
        getline(cin, userInput);
    }

    vector<Ticket> result = queryByField(storage, "number", userInput);
    if (result.empty()) {
        cout << "There is no ticket of number " + userInput + ", please query first. ";
        return Ticket();
    }

    Ticket resultTicket = result[0];
    cout << "This is ticket " + to_string(resultTicket.ticketNumber) + "'s info. " << endl;
    cout << printableList(vector<Ticket>{resultTicket});

    cout << "Do you want to modify it? input y / n: ";
    getline(cin, userInput);
    while (userInput != "y" && userInput != "n") {
        cout << "Only can input y / n, please try again: ";
        getline(cin, userInput);
    }

    if (userInput == "n") return Ticket();

    Ticket ticket = requestTicket(storage);
    storage.template remove<Ticket>(resultTicket.id);
    return ticket;
}

/**
 * Just show the usage dialog.
 */
void showUsage() {
    cout << "+-----------------------------------------------+" << endl;
    cout << "|                Booking Tickets                |" << endl;
    cout << "+-----------------------------------------------+" << endl;
    cout << "|       0: quit the system.                     |" << endl;
    cout << "|       1: Insert a train information.          |" << endl;
    cout << "|       2: Search a train information.          |" << endl;
    cout << "|       3: Book a train ticket.                 |" << endl;
    cout << "|       4. Modify the train information.        |" << endl;
    cout << "|       5: Show the train information.          |" << endl;
    cout << "|       6: Save information to file.            |" << endl;
    cout << "+-----------------------------------------------+" << endl;
}

string queryWeather() {
    return "";
}