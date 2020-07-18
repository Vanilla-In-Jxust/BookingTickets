cmake_minimum_required(VERSION 3.16)
project(BookingTickets)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookingTickets src/main.cpp include/sqlite_orm.h src/Ticket.h src/Database.h src/Validate.h src/Interactive.h)

# orm framework need sqlite3 library.
find_package(libfort)
target_link_libraries(BookingTickets sqlite3 libfort::fort jsoncpp)
