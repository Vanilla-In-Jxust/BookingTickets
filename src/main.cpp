#include "string"

#include "Interactive.h"
#include "Database.h"

using namespace std;

int main() {
/*    auto storage = initDatabase();

    Ticket ticket = *new Ticket("a", "b", "1:00", "2:00", 13.00, 13);
    ticket.id = insertTicket(storage, ticket);
    printf("%s", ticket.toJson().dump().c_str());*/

    Ticket ticket = requestTicket();
    printf("%s", ticket.toString().c_str());

    return 0;
}
