#include "Interactive.h"
#include "../hash-library/sha256.h"

using namespace std;

/**
 * Just show "press ENTER to back" info.
 */
void pressToBack() {
    cout << endl << "Press ENTER key to back... ";
    getchar();
}

/**
 * Insert some example data, it should be called after delete db.
 *
 * @param storage to insert data.
 */
template<class ...Ts>
void insertExampleData(sqlite_orm::internal::storage_t<Ts...> storage) {
    storage.template remove_all<Ticket>();

    storage.insert(Ticket(2292, "南昌", "武汉", "20:53", "23:05", 171.0, 17));
    storage.insert(Ticket(90, "武昌", "新乡", "01:15", "06:50", 86.0, 20));
    storage.insert(Ticket(2036, "南昌", "汉口", "17:55", "20:34", 360.5, 10));
    storage.insert(Ticket(336, "武昌", "新乡", "23:00", "04:33", 152.0, 5));
    storage.insert(Ticket(1329, "南昌", "长沙", "17:29", "19:14", 264.5, 17));
}

int main() {
    auto storage = initDatabase();
    insertExampleData(storage);

    start:
    system("clear");
    showUsage();
    cout << endl << "Please choose (0 ~ 7): ";

    string userInput;

    input:
    userInput = getUserInput(storage);
    while (!isPositiveInteger(userInput)) {
        cout << "Only can input positive number, please try again: ";
        userInput = getUserInput(storage);
    }

    int inputNumber = stoi(userInput);
    if (inputNumber > 7) {
        cout << "Input number can only be 0 ~ 6, please try again: ";
        goto input;
    }

    switch (inputNumber) {
        case 0: {
            cout << endl;

            using namespace fort;
            char_table byeTable;
            byeTable.set_border_style(FT_DOUBLE_STYLE);

            byeTable << header << "   Thanks for using, bye!   " << endr;
            cout << byeTable.to_string();

            cout << endl;
            exit(0);
        }
        case 1: {
            requestTicket(storage);
            pressToBack();
            goto start;
        }
        case 2: {
            cout << searchTickets(storage);
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
            cout << printableTicketList(tickets);

            pressToBack();
            goto start;
        }
        case 6: {
            storage.sync_schema();
            cout << "Sync schema successfully. " << endl;
            pressToBack();
            goto start;
        }

        case 7: {
            cout << queryWeather(storage);
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
