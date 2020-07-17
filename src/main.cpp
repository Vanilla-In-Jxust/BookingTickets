#include "string"

#include "Interactive.h"

using namespace std;

int main() {
    auto storage = initDatabase();

    Ticket ticket = requestTicket(storage);
    printf("%s", ticket.toString().c_str());

    return 0;
}
