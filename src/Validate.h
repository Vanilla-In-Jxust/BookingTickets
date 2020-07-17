//
// Created by vanilla on 2020/7/17.
//

#include "string"
#include "regex"

using namespace std;

/**
 * @see https://stackoverflow.com/questions/7536755/regular-expression-for-matching-hhmm-time-format/7536768
 * @see https://stackoverflow.com/questions/33128258/c-check-if-string-is-valid-integer-or-decimal-both-negative-and-positive-case
 */
#pragma ide diagnostic ignored "cert-err58-cpp"
regex timeRegex = regex("^(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$");
regex integerRegex = regex("[0-9]+");
regex doubleRegex = regex("[0-9]*\\.[0-9]+|[0-9]+");

bool isTime(const string &timeString) {
    return regex_match(timeString, timeRegex);
}

bool isPositiveInteger(const string &integerString) {
    return regex_match(integerString, integerRegex);
}

bool isPositiveDouble(const string &doubleString) {
    return regex_match(doubleString, doubleRegex);
}
