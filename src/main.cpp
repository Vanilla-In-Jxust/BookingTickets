#include "Interactive.h"

using namespace std;

/**
 * Just show "press EHTER to back" info.
 */
void pressToBack() {
    cout << endl << "Press ENTER key to back... ";
    getchar();
}

int main() {
    auto storage = initDatabase();

    start:
    system("clear");
    showUsage();
    cout << "Please choose: (0 ~ 6) ";

    string userInput;
    getline(cin, userInput);
    while (!isPositiveInteger(userInput)) {
        cout << "Only can input number, please try again: ";
        getline(cin, userInput);
    }

    int inputNumber = stoi(userInput);
    switch (inputNumber) {
        case 0: {
            system("clear");
            cout << "Thanks for using, bye! " << endl;
            exit(0);
        }
        case 1: {
            requestTicket(storage);
            pressToBack();
            goto start;
        }
        case 2: {
            searchTickets(storage);
            pressToBack();
            goto start;
        }
        case 3: {
            bookTicket(storage);
            pressToBack();
            goto start;
        }
        case 4: {
            modifyTicket(storage);
            pressToBack();
            goto start;
        }
        case 5: {
            vector<Ticket> tickets = storage.template get_all<Ticket>();
            cout << printableList(tickets) << endl;

            pressToBack();
            goto start;
        }
        case 6: {
            storage.sync_schema();
            cout << "Sync schema successfully. " << endl;
            pressToBack();
            goto start;
        }

        default: {
            system("clear");
            cout << "Whoops, something is wrong... " << endl;
            exit(-1);
        }
    }
}
