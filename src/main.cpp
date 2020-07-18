#include "Interactive.h"

using namespace std;

int main() {
    auto storage = initDatabase();

    insertTicket(storage, Ticket(123, "", "", "", "", 123.1, 123));
    bookTicket(storage);

    return 0;
}
