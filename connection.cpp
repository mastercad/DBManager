#include "connection.h"

#include <QString>
#include <QElapsedTimer>

#include <QDebug>

Connection::Connection(QObject* parent)
    :parent(parent) {}

QSqlQuery Connection::sendQuery(QSqlQuery query) {
    QElapsedTimer timer;
    timer.start();
    query.exec();
    lastQueryTime = timer.elapsed();

    if (query.isActive()) {
        informationView->append(QString(query.lastQuery()+" in %1ms.").arg(lastQueryTime));
    } else {
        informationView->append(query.lastError().text());
    }
    return query;
}

QSqlQuery Connection::sendQuery(QString queryString) {
    QSqlQuery query(database);
    query.prepare(queryString);
    return sendQuery(query);
}

void Connection::setDatabaseListView(QTreeView *databaseListView) {
    this->databaseListView = databaseListView;
}

QTreeView* Connection::getDatabaseListView() const {
    return this->databaseListView;
}

void Connection::setQueryResultView(QTableView *queryResultView) {
    this->queryResultView = queryResultView;
}

QTableView* Connection::getQueryResultView() const {
    return this->queryResultView;
}
/*
void Connection::setQueryRequestView(TextEdit *queryRequestView) {
    this->queryRequestView = queryRequestView;
}

TextEdit* Connection::getQueryRequestView() const {
    return this->queryRequestView;
}
*/
void Connection::setInformationView(QTextEdit *informationView) {
    this->informationView = informationView;
}

QTextEdit* Connection::getInformationView() const {
    return this->informationView;
}

void Connection::setConnectionInfo(ConnectionInfo *connectionInfo) {
    this->connectionInfo = connectionInfo;
}

ConnectionInfo* Connection::getConnectionInfo() const {
    return this->connectionInfo;
}

void Connection::setKeywords(QMap<QString, bool>* keywords) {
    this->keywords = keywords;
}

QMap<QString, bool>* Connection::getKeywords() const {
    return this->keywords;
}

void Connection::setDatabaseCollection(QStandardItemModel* databaseCollection) {
    this->databaseCollection = databaseCollection;
}

QStandardItemModel* Connection::getDatabaseCollection() {
    return this->databaseCollection;
}

void Connection::setTableNames(QMap<QString, QString>* tableNameCollection) {
    this->tableNameCollection = tableNameCollection;
}

QMap<QString, QString>* Connection::getTableNames() {
    return this->tableNameCollection;
}

QString Connection::generateLastExecutedQuery(const QSqlQuery& query) {
    /*
    QString sql = query.executedQuery();
    int nbBindValues = query.boundValues().size();

    for(int i = 0, j = 0; j < nbBindValues;) {
        int s = sql.indexOf(QLatin1Char('\''), i);
        i = sql.indexOf(QLatin1Char('?'), i);
        if (i < 1) {
            break;
        }

        if (s < i && s > 0) {
            i = sql.indexOf(QLatin1Char('\''), s + 1) + 1;
            if(i < 2) {
                break;
            }
        } else {
            const QVariant &var = query.boundValue(j);
            QSqlField field(QLatin1String(""), var.type());
            if (var.isNull()) {
                field.clear();
            } else {
                field.setValue(var);
            }
            QString formatV = query.driver()->formatValue(field);
            sql.replace(i, 1, formatV);
            i += formatV.length();
            ++j;
        }
    }

    return sql;
    */
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());

    while (it.hasNext()) {
        it.next();
        str.replace(it.key(), it.value().toString());
    }
    return str;
}

QSqlError Connection::lastError() {
    return database.lastError();
}

void Connection::close() {
    return database.close();
}

bool Connection::open() {
    return database.open();
}

bool Connection::isOpen() {
    return database.isOpen();
}

Connection::~Connection() {
    database.close();
}
