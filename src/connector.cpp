#include <headers/connector.h>
QSqlDatabase openDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("forcity");
    db.setUserName("astra");
    db.setPassword("12345678");
    db.open();

    return db;
}
