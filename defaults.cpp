#include "defaults.h"

QString Defaults::Resolver::resolve(QString key, QString currentValue) {
    if (currentValue.isEmpty()) {
        if ("user" == key) {
            return Defaults::MYSQL::USER;
        }
        if ("host" == key) {
            return Defaults::MYSQL::HOST;
        }
        if ("password" == key) {
            return Defaults::MYSQL::PASSWORD;
        }
    }
    return currentValue;
}

int Defaults::Resolver::resolve(QString key, int currentValue) {
    if (0 == currentValue) {
        return Defaults::MYSQL::PORT;
    }
    return currentValue;
}
