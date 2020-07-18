//
// Created by vanilla on 2020/7/17.
//

#include "../include/sqlite_orm.h"

using namespace sqlite_orm::internal;

/**
 * create database, and sync schema.
 * @return database storage
 * @see https://github.com/fnc12/sqlite_orm
 */
auto initDatabase() {
    using namespace sqlite_orm;
    auto storage = make_storage(
            "tickets.sqlite",
            make_table(
                    "tickets",
                    make_column("id", &Ticket::id, autoincrement(), primary_key()),
                    make_column("number", &Ticket::number),
                    make_column("startCity", &Ticket::startCity),
                    make_column("reachCity", &Ticket::reachCity),
                    make_column("takeOffTime", &Ticket::takeOffTime),
                    make_column("receiveTime", &Ticket::receiveTime),
                    make_column("price", &Ticket::price),
                    make_column("ticketNumber", &Ticket::ticketNumber)));

    storage.sync_schema();
    return storage;
}

/**
 * Insert ticket to database, and modify ticket.id.
 *
 * @param storage the database storage, should call after initDatabase() function.
 * @param ticketInfo the ticket to insert.
 */
template<class ...Ts>
void insertTicket(storage_t<Ts...> storage, Ticket ticket) {
    auto insertedId = storage.insert(ticket);
    ticket.id = insertedId;
}

template<class ...Ts>
vector<Ticket> queryByField(storage_t<Ts...> storage, const string &field, const string &value) {
    auto allTickets = storage.template get_all<Ticket>();
    vector<Ticket> result;

    for (const Ticket &ticket: allTickets) {
        if (ticket.toJson()[field].asString() == value)
            result.push_back(ticket);
    }

    return result;
}
