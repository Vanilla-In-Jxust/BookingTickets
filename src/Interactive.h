//
// Created by vanilla on 2020/7/17.
//

#include "iostream"
#include "Ticket.h"
#include "boost/algorithm/string.hpp"
#include "Validate.h"

using namespace std;

Ticket requestTicket() {
    cout << "Please input tickets info to insert, format like " << endl;
    cout << "\"Start City - Reach City - Take Off - Time Receive - Price - Ticket Number\": " << endl;

    start:
    string userInput;
    getline(cin, userInput);

    vector<string> splitResult;
    boost::split(splitResult, userInput, boost::is_any_of("-"));

    while (splitResult.size() != 6) {
        cout << "Input field length is illegal, please try again: ";

        getline(cin, userInput);
        boost::split(splitResult, userInput, boost::is_any_of("-"));
    }

    string priceString = splitResult[4];
    string ticketNumberString = splitResult[5];

    // when args are illegal.
    while (!(isTime(splitResult[2]) && isTime(splitResult[3])
             && isPositiveDouble(priceString) && isPositiveInteger(ticketNumberString))) {
        cout << "time format, price or Ticket Number is illegal, please try again: ";
        goto start;
    }

    double price = stod(priceString);
    int ticketNumber = stoi(ticketNumberString);

    return Ticket(splitResult[0], splitResult[1], splitResult[2], splitResult[3], price, ticketNumber);
}
