//
// Created by vanilla on 2020/7/17.
//

#include "iostream"
#include "Ticket.h"
#include "boost/algorithm/string.hpp"
#include "Validate.h"
#include "Database.h"

using namespace std;

/**
 * request user input a ticket's info, and return this ticket.
 *
 * @return the built ticket object
 */
template<class ...Ts>
Ticket requestTicket(sqlite_orm::internal::storage_t<Ts...> storage) {
    cout << "Please input tickets info to insert, format like " << endl;
    cout << "\"Start City || Reach City || Take Off || Time Receive || Price || Ticket Number\": " << endl;

    start:
    string userInput;
    getline(cin, userInput);

    vector<string> splitResult;
    boost::split(splitResult, userInput, boost::is_any_of("||"));

    while (splitResult.size() != 11) {
        cout << "Input field length is illegal, please try again: ";

        getline(cin, userInput);
        boost::split(splitResult, userInput, boost::is_any_of("||"));
    }

    string priceString = splitResult[8];
    string ticketNumberString = splitResult[10];

    // when args are illegal.
    while (!(isTime(splitResult[4]) && isTime(splitResult[6])
             && isPositiveDouble(priceString) && isPositiveInteger(ticketNumberString))) {
        cout << "time format, price or Ticket Number is illegal, please try again: ";
        goto start;
    }

    double price = stod(priceString);
    int ticketNumber = stoi(ticketNumberString);

    Ticket ticket = Ticket(splitResult[0], splitResult[2], splitResult[4], splitResult[6], price, ticketNumber);
    insertTicket(storage, ticket);

    return ticket;
}
