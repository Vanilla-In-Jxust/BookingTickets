//
// Created by vanilla on 2020/7/17.
//

#include "iostream"
#include "Ticket.h"
#include "boost/algorithm/string.hpp"
#include "Validate.h"
#include "Database.h"
#include <cctype>

#include "Weather.h"

using namespace std;

template<class ...Ts>
string getUserInput(storage_t<Ts...> storage) {
    string userInput;

    if (cin.eof()) {
        storage.sync_schema();
        using namespace fort;

        utf8_table eolTable;
        eolTable.set_border_style(FT_DOUBLE_STYLE);

        eolTable << "Detected EOL, stdin closed. " << endr;
        eolTable << "Database synced successfully. " << endr;
        eolTable << "Exit with code 255. " << endr;

        cout << endl << eolTable.to_string() << endl;

        exit(255);
    }

    getline(cin, userInput);
    return userInput;
}

/**
 * request user input a ticket's info, insert and return this ticket.
 *
 * @return the built ticket object
 */
template<class ...Ts>
Ticket requestTicket(sqlite_orm::internal::storage_t<Ts...> storage) {
    cout << "Please input the number of the train, 0 is return: ";

    string userInput = getUserInput(storage);

    vector<Ticket> queryResult = queryByField(storage, "number", userInput);
    if (userInput == "0") return Ticket();
    else if (queryResult.size() == 1) {
        cout << "Ticket of number " + userInput + " is already exist: " << endl;
        cout << printableTicketList(vector<Ticket>{queryResult[0]});
        cout << "Consider using modify function to change it. ";

        return Ticket();
    }
    while (!isPositiveInteger(userInput)) {
        cout << "number of the train is illegal, please try again: ";
        userInput = getUserInput(storage);

        queryResult = queryByField(storage, "number", userInput);
        if (userInput == "0") return Ticket();
        else if (queryResult.size() == 1) {
            cout << "Ticket of number " + userInput + " is already exist: " << endl;
            cout << printableTicketList(vector<Ticket>{queryResult[0]});
            cout << "Consider using modify function to change it. ";

            return Ticket();
        }
    }
    int number = stoi(userInput);

    cout << "Input the city where the train will start: ";
    userInput = getUserInput(storage);
    while (userInput.find_first_not_of(' ') == string::npos) {
        cout << "Start city is empty, please try again: ";
        userInput = getUserInput(storage);
    }
    string startCity = userInput;

    cout << "Input the city where the train will reach: ";
    userInput = getUserInput(storage);
    while (userInput.find_first_not_of(' ') == string::npos) {
        cout << "Reach city is empty, please try again: ";
        userInput = getUserInput(storage);
    }
    string reachCity = userInput;

    cout << "Input the time which the the train take off: (HH:MM) ";
    userInput = getUserInput(storage);
    while (!isTime(userInput)) {
        cout << "Time format is invalid, please try again: (HH:MM) ";
        userInput = getUserInput(storage);
    }
    string takeOffTime = userInput;

    cout << "Input the time which the the train receive: (HH:MM) ";
    userInput = getUserInput(storage);
    while (!isTime(userInput)) {
        cout << "Time format is invalid, please try again: (HH:MM) ";
        userInput = getUserInput(storage);
    }
    string receiveTime = userInput;

    cout << "Input the price of ticket (￥): ";
    userInput = getUserInput(storage);
    while (!isPositiveDouble(userInput)) {
        cout << "Input is illegal, please try again: ";
        userInput = getUserInput(storage);
    }
    double price = stod(userInput);

    cout << "Input the number of booked ticket(s): ";
    userInput = getUserInput(storage);
    while (!isPositiveInteger(userInput)) {
        cout << "Input is illegal, please try again: ";
        userInput = getUserInput(storage);
    }
    int ticketNumber = stoi(userInput);

    Ticket ticket = Ticket(number, startCity, reachCity, takeOffTime, receiveTime, price, ticketNumber);
    insertTicket(storage, ticket);
    cout << endl << "Ticket of number " + to_string(ticket.number) + " saved. " << endl;

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
    using namespace fort;
    char_table chooseTable;
    chooseTable.set_border_style(FT_DOUBLE_STYLE);

    chooseTable << header << "1 - query by the number of train. " << endr;
    chooseTable << header << "2 - query by the city. " << endr;

    cout << endl << chooseTable.to_string();
    cout << "Choose the way to query: ";

    string userInput, queryField;
    userInput = getUserInput(storage);
    while (userInput != "1" && userInput != "2") {
        cout << R"(Input can only be "1" or "2", please try again: )";
        userInput = getUserInput(storage);
    }

    if (userInput == "1") {
        queryField = "number";
        cout << "Input the number of train: ";

        userInput = getUserInput(storage);
        while (!isPositiveInteger(userInput)) {
            cout << "Input is invalid, please try again: ";
            userInput = getUserInput(storage);
        }
    } else if (userInput == "2") {
        queryField = "reachCity";
        cout << "Input the city: ";

        userInput = getUserInput(storage);
    }

    vector<Ticket> result = queryByField(storage, queryField, userInput);
    return printableTicketList(result);
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
    userInput = getUserInput(storage);
    while (!isPositiveInteger(userInput)) {
        cout << "Input is not number, please try again: ";
        userInput = getUserInput(storage);
    }

    vector<Ticket> result = queryByField(storage, "number", userInput);
    if (result.empty()) {
        cout << "There is no ticket of number " + userInput + ", please query first. ";
        return -1;
    }

    Ticket ticket = result[0];
    cout << printableTicketList(vector<Ticket>{ticket});

    cout << "Input your bookNum: ";

    start:
    userInput = getUserInput(storage);
    while (!isPositiveInteger(userInput) || userInput == "0") {
        cout << "Input is not number or illegal, please try again: ";
        userInput = getUserInput(storage);
    }

    int bookNum = stoi(userInput);
    while (bookNum > ticket.ticketNumber) {
        cout << "bookNum " + userInput + " > stock ticket number " +
                to_string(ticket.ticketNumber) + ", please try again: ";
        goto start;
    }

    cout << "Input your name: ";
    userInput = getUserInput(storage);

    cout << "Input your id: ";
    userInput = getUserInput(storage);
    while (!isChinaId(userInput)) {
        cout << "Your input is not legal id, please try again: ";
        userInput = getUserInput(storage);
    }

    ticket.ticketNumber -= bookNum;
    storage.update(ticket);
    cout << "Lucky! You have booked " + to_string(bookNum) + " ticket(s)! " << endl;

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
    userInput = getUserInput(storage);
    while (!isPositiveInteger(userInput)) {
        cout << "Input is not number, please try again: ";
        userInput = getUserInput(storage);
    }

    vector<Ticket> result = queryByField(storage, "number", userInput);
    if (result.empty()) {
        cout << "There is no ticket of number " + userInput + ", please query first. ";
        return Ticket();
    }

    Ticket resultTicket = result[0];
    cout << endl << "This is ticket " + to_string(resultTicket.ticketNumber) + "'s info: ";
    cout << printableTicketList(vector<Ticket>{resultTicket});

    cout << "Do you want to modify it? input y / n: ";
    userInput = getUserInput(storage);
    while (userInput != "y" && userInput != "n") {
        cout << "Only can input y / n, please try again: ";
        userInput = getUserInput(storage);
    }

    if (userInput == "n") return Ticket();
    cout << endl;

    storage.template remove<Ticket>(resultTicket.id);
    Ticket ticket = requestTicket(storage);
    if (ticket.id == -1) insertTicket(storage, resultTicket);

    return ticket;
}

/**
 * Just show the usage dialog.
 */
void showUsage() {
    using namespace fort;

    utf8_table usageTable;
    usageTable.set_border_style(FT_DOUBLE_STYLE);

    usageTable << header << "Booking Tickets System" << endr;

    usageTable << "0: quit the system. " << endr;
    usageTable << "1: Insert a train information. " << endr;
    usageTable << "2: Search a train information. " << endr;
    usageTable << "3: Book a train ticket. " << endr;
    usageTable << "4. Modify the train information. " << endr;
    usageTable << "5: Show the train information. " << endr;
    usageTable << "6: Save information to file. " << endr;
    usageTable << header << "7: Query weather condition." << endr;

    cout << usageTable.to_string();
}

template<class ...Ts>
string queryWeather(storage_t<Ts...> storage) {
    cout << "Input place name to find: ";

    string userInput;
    userInput = getUserInput(storage);

    cout << "querying... please wait..." << endl;

    vector<Weather> weatherList = getWeatherList(userInput);
    if (weatherList.empty()) return "Cannot query any weather info of \"" + userInput + "\". \n";

    string location = getLocation(getCoordinate(userInput));
    cout << endl << "Here is the weather info of \"" + location + "\": ";

    return printableWeatherList(weatherList);
}
