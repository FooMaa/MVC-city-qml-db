#ifndef CONNECTOR_H
#define CONNECTOR_H

#define NAME_STRING "name"
#define ADDRESS_STRING "address"
#define PROPERTY_STRING "property"

#define CITY "City"
#define STREET "Street"
#define HOME "Home"
#define PEOPLE "People"

#define PROPERTY_NAME "objectName"

#define AMOUNT_TYPE 4

#include <QSqlDatabase>
#include <QObject>

enum Type {
   type_City = 0,
   type_Street,
   type_Home,
   type_People
};

enum ROLES {
    ROLE_NAME = Qt::UserRole + 1,
    ROLE_ADDRESS,
    ROLE_PROPERTY
};

enum TypeForTree {
   type_city_drop = 1,
   type_street_drop,
   type_home_drop,
   type_people_drop
};

enum Type_from_DB {
   ROOT_OBJ = 0,
   CITY_OBJ,
   STREET_OBJ,
   HOME_OBJ,
   PEOPLE_OBJ
};

QSqlDatabase openDB();

#endif // CONNECTOR_H
