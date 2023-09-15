#include "headers/singleton.h"

Singleton& Singleton::get_instance()
{
    static Singleton instance;
    return instance;
}

Singleton::Singleton()
{
    QSqlDatabase db = openDB();

    QMap <int, General*> child_parent;

    QSqlQuery query_to_dbobject( "SELECT * FROM object" );

    int counter_object[ AMOUNT_TYPE ]= {0, 0, 0, 0};

    while ( query_to_dbobject.next() ) {

        int class_dbobject = query_to_dbobject.value( 1 ).toInt();
        int id_child = query_to_dbobject.value( 0 ).toInt();
        int id_parent = -1;

        QSqlQuery query_to_hierarchy( "SELECT * FROM hierarchy WHERE id_child = " + QString::number( id_child ) );

        while ( query_to_hierarchy.next() ) {

            id_parent = query_to_hierarchy.value( 1 ).toInt();

       }

        switch ( class_dbobject )
        {
        case CITY_OBJ:
            counter_object[ type_City ] ++;
            child_parent.insert(id_child, generateDefaultObject<Singleton, City>(this, id_child, id_parent));
            break;

        case STREET_OBJ:
            counter_object[ type_Street ] ++;
            child_parent.insert( id_child, generateDefaultObject<City, Street>( child_parent[ id_parent ], id_child, id_parent ) );
            break;

        case HOME_OBJ:
            counter_object[ type_Home ] ++;
            child_parent.insert( id_child, generateDefaultObject<Street, Home>( child_parent[ id_parent ], id_child, id_parent ) );
            break;

        case PEOPLE_OBJ:
            counter_object[ type_People ] ++;
            child_parent.insert( id_child, generateDefaultObject<Home, People>( child_parent[ id_parent ], id_child, id_parent ) );
            break;

        }

    }
    db.close();

}

City::City(Singleton *parent)
{
    name_object = CITY + QString::number( parent->counter_names_city );
    parent->counter_names_city++;
    setObjectName( name_object );
    setParent( parent );
    generateProperty( this, type_City );
}

Street::Street(City *parent)
{
    name_object = STREET + QString::number( parent->counter_names[ type_Street ] );
    parent->counter_names[ type_Street ]++;
    setObjectName( name_object );

    if (parent != nullptr) {
       setParent( parent );
    }

    generateProperty( this, type_Street );
}

Home::Home(Street *parent)
{
    name_object = HOME + QString::number( parent->counter_names[ type_Home ] );
    parent->counter_names[ type_Home ]++;
    setObjectName( this->name_object );
    if (parent!= nullptr)
    {
       setParent( parent );
    }

    generateProperty( this, type_Home );
}

People::People(Home *parent)
{
    name_object = PEOPLE + QString::number( parent->counter_names[ type_People ] );
    parent->counter_names[ type_People ]++;
    setObjectName( this->name_object );

    if (parent!= nullptr) {
       setParent( parent );
    }

    generateProperty( this, type_People );
}

General::General()
{
    class_property[ type_City ] << "For live" << "For work" << "For relax";
    class_property[ type_Street ] << "Long" << "Short" << "Middle";;
    class_property[ type_Home ] << "Gray" << "Black" << "White" << "Blue" << "Green";
    class_property[ type_People ] << "Clever" << "Atletic" << "Beautiful";
}
