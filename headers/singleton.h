#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>
#include <QMap>
#include <QSqlQuery>
#include <QDebug>

#include "connector.h"

class City;
class Street;
class Home;
class People;

class General : public QObject
{
    Q_OBJECT
public:
   QString name_object = "";
   QMap <int, QStringList> class_property;
   QString generated_property;

   int id_object, id_parent;

   General();
   template <typename OBJECT> void generateProperty(OBJECT *object, int key);

   int counter_names[ AMOUNT_TYPE ]= {1, 1, 1, 1};
};

class Singleton : public General
{
public:
    static Singleton& get_instance();

    int counter_names_city = 1;

private:
    Singleton();
    Singleton(Singleton const&) = delete;
    Singleton(Singleton const&&) = delete;
    Singleton& operator= (Singleton const&) = delete;
    Singleton& operator= (Singleton const&&) = delete;
    ~Singleton() {};
};

class City : public General{
public:
    City(Singleton *parent);
};

class Street : public General{
public:
     Street(City *parent = nullptr);
};

class  Home : public General{
public:
     Home(Street *parent=nullptr);
};

class People : public General{
public:
    People(Home *parent=nullptr);
};

template <typename PARENT, typename CHILD>
CHILD* generateDefaultObject(General *parent_object, int id_child, int id_parent)
{
        PARENT *prt = dynamic_cast<PARENT*>( parent_object );
        CHILD *object = new CHILD( prt );
        object->id_object = id_child;
        object->id_parent = id_parent;
        QSqlQuery query_to_dbobject( "UPDATE object SET property = \'" + object->generated_property + "\'" +
                                     " WHERE id_object = " + QString::number(object->id_object));
        return object;
}

template<typename OBJECT>
void General::generateProperty(OBJECT *object, int key)
{
    srand( time( 0 ) );
    int random_key = rand() % 3;
    object->generated_property = class_property[ key ][ random_key ];
    //QSqlDatabase db = openDB();
    //db.close();
}

#endif // SINGLETON_H
