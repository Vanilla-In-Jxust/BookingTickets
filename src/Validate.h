//
// Created by vanilla on 2020/7/17.
//

#include "string"
#include "regex"

using namespace std;

// @see https://stackoverflow.com/questions/7536755/regular-expression-for-matching-hhmm-time-format/7536768
#pragma ide diagnostic ignored "cert-err58-cpp"
regex timeRegex("^(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$");

bool isTime(const string &timeString) {
    return regex_match(timeString, timeRegex);
}
