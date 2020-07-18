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
regex chinaLongIdRegex = regex("^\\d{15}|\\d{18}$");

/**
 *  or "^\d{8,18}|[0-9x]{8,18}|[0-9X]{8,18}?$",
 *  see also https://tojohnonly.github.io/63-C++%E4%BD%BF%E7%94%A8%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F.html
 */
regex chinaShortIdRegex = regex("^([0-9]){7,18}(x|X)?$");

bool isTime(const string &timeString) {
    return regex_match(timeString, timeRegex);
}

bool isPositiveInteger(const string &integerString) {
    return regex_match(integerString, integerRegex);
}

bool isPositiveDouble(const string &doubleString) {
    return regex_match(doubleString, doubleRegex);
}

bool isChinaId(const string &idString) {
    // see also https://my.oschina.net/anziguoer/blog/1780511
    return regex_match(idString, chinaLongIdRegex) ||
           regex_match(idString, chinaShortIdRegex);
}
