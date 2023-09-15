#include "headers/datamodeltree.h"

DataModelTree::DataModelTree(QObject *parent) : QAbstractItemModel(parent)
{

}

int DataModelTree::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return 1;
}

int DataModelTree::rowCount(const QModelIndex &parent) const
{
    if ( objByIndex<Singleton>( parent ) == nullptr ) {
        return rootItem.children().count();
    }

    return static_cast<QObject*>( parent.internalPointer() )->children().size();
}

QModelIndex DataModelTree::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex( row, column, parent ) ) {
        return QModelIndex();
    }

    if ( objByIndex<Singleton>( parent ) == nullptr ) {
        return createIndex( row, column, rootItem.children().at( row ) );
    }

    return createIndex( row, column, static_cast<QObject*>( parent.internalPointer() )->children()[ row ] );

}

QModelIndex DataModelTree::parent(const QModelIndex &child) const
{
    int row = 0;

    if ( objByIndex<Singleton>( child )->parent() == &rootItem ) {
        return QModelIndex();
    }

    if ( objByIndex<Singleton>( child )->parent() != nullptr ) {
        QObject *children = objByIndex<QObject>( child );
        QObject *father = children->parent();
        QObject *grandfather = father->parent();

        if ( grandfather == nullptr ) {
            grandfather = &rootItem;
        }

        row = grandfather->children().indexOf( father );
        return createIndex( row, 0, father );
    }

    return QModelIndex();
}



QVariant DataModelTree::data(const QModelIndex &index, int role) const
{

    if ( !index.isValid() ) {
        return QVariant();
    }

    if( role == ROLE_NAME ) {
        return objByIndex<QObject>( index )->property( QString( PROPERTY_NAME ).toUtf8() );
    }

    if ( role == Qt::DisplayRole ) {
        return objByIndex<QObject>( index )->property( QString( PROPERTY_NAME ).toUtf8() );
    }

    return QVariant();
}

int DataModelTree::seeParent(QObject *child, QModelIndex index)
{
    int amount_parent = 0;

    if ( child == nullptr ) {
        child = objByIndex<QObject>( index );
    }

    while( child != &rootItem && child != nullptr ) {
        child = child->parent();
        amount_parent++;
    }

    return amount_parent;
}

void DataModelTree::slot_insertValueFromLineEdit(QModelIndex currentIndex)
{
    emit beginInsertRows( currentIndex, rowCount( currentIndex ), rowCount( currentIndex ) );

        switch ( seeParent( nullptr, currentIndex ) )
        {
        case type_City:
            insertObjectInTree<City, Singleton>( currentIndex, type_city_drop );
            break;

        case type_Street:
            insertObjectInTree<Street,City>( currentIndex, type_street_drop );
            break;

        case type_Home:
            insertObjectInTree<Home,Street>( currentIndex, type_home_drop );
            break;

        case type_People:
            insertObjectInTree<People,Home>( currentIndex, type_people_drop );
            break;
        }

    emit endInsertRows();
}

void DataModelTree::setCurrentIndex(QModelIndex index)
{
    currentIndex = index;
}

QModelIndex DataModelTree::getCurrentIndex()
{
    return currentIndex;
}

bool DataModelTree::changeParent(QModelIndex currentIndex, QModelIndex currentParent)
{
    QModelIndex parentIndex = parent( currentIndex );

    if ( objByIndex<QObject>(parentIndex) != nullptr ) {

        QObjectList list_children_parent = objByIndex<QObject>( parentIndex )->children();
        General *children = objByIndex<General>( currentIndex );
        General *parent_object = objByIndex<General>( currentParent );
        int metka_current_index = list_children_parent.indexOf( objByIndex<QObject>( currentIndex ) );

        if ( seeParent( children ) == seeParent( parent_object ) + 1 ) {

            emit beginRemoveRows( parentIndex, metka_current_index, metka_current_index );
            emit endRemoveRows();
            emit beginInsertRows( currentParent, rowCount( currentParent ), rowCount( currentParent ) );
            children->setParent( parent_object );
            children->id_parent = parent_object->id_object;
            QSqlDatabase db = openDB();

            QSqlQuery query_to_dbobject( "UPDATE hierarchy SET id_parent = " +
                                         QString::number(parent_object->id_object) + " WHERE id_child = " + QString::number(children->id_object));

            db.close();
            emit endInsertRows();
        }
//        else {

//          if ( seeParent(children) == type_people_drop && seeParent(parent_object) + 1 == type_home_drop )
//          {
//              emit beginRemoveRows( parentIndex, metka_current_index, metka_current_index );
//              emit endRemoveRows();
//              emit beginInsertRows( currentParent, rowCount( currentParent ), rowCount( currentParent ) );
//              children->setObjectName("Бомж");
//              children->setParent( parent_object );
//              children->id_parent = parent_object->id_object;
//              emit endInsertRows();
//          }

//          if ( children->objectName() == "Бомж" && seeParent(parent_object) + 1 == type_people_drop )
//          {
//              emit beginRemoveRows( parentIndex, metka_current_index, metka_current_index );
//              emit endRemoveRows();
//              emit beginInsertRows( currentParent, rowCount( currentParent ), rowCount( currentParent ) );
//              children->setObjectName( "People из бомжа" ) ;
//              children->setParent( parent_object );
//              children->id_parent = parent_object->id_object;
//              emit endInsertRows();
//          }

//        }

        emit layoutChanged();


     }

     parent_idx = currentParent;
     child_idx = currentIndex;

     if ( checkDoubleNames( currentParent, currentIndex ) ) {
        return true;
     }

     return false;
}

bool DataModelTree::checkDoubleNames(QModelIndex parent, QModelIndex child)
{
    QObject *parent_object = static_cast<QObject*>( parent.internalPointer() );
    QObject *child_object = static_cast<QObject*>( child.internalPointer() );

    QString name_object = child_object->objectName();

    QObjectList childrens = parent_object->children();

    int counter_all_common = 0;
    for ( int i = 0; i < childrens.size(); i++ )
    {

        if ( childrens[i]->objectName() == name_object )
        {
           counter_all_common++;
           for_repeat_objects = childrens[i];
        }

    }

    if ( counter_all_common > 1 )
    {
        return true;
    }

    return false;
}

bool DataModelTree::changeObjectName(QString text)
{
    if( text != "" ) {
    qobject_cast<General*>( for_repeat_objects )->name_object = text;
    for_repeat_objects->setObjectName( text );
    layoutChanged();
    }

    if ( checkDoubleNames( parent_idx, child_idx ) ) {
        return false;
    }

    if (checkAdd(checkIdx)) {
        return false;
    }

    return true;

}

bool DataModelTree::checkAdd(QModelIndex curidx)
{

    checkIdx = curidx;
    QObject *parent_object = objByIndex<QObject>( curidx );

    if (parent_object == nullptr) {
        return false;
    }

    int size_parent = parent_object->children().size();

    if (size_parent == 0) {
        return false;
    }

    QObject *child_object = parent_object->children().at( size_parent - 1 );

    QString name_object = child_object->objectName();

    QObjectList childrens = parent_object->children();

    int counter_all_common = 0;
    for ( int mark_childrens = 0; mark_childrens < childrens.size(); mark_childrens++ ) {

        if ( childrens[ mark_childrens ]->objectName() == name_object ) {
           counter_all_common++;
           for_repeat_objects = childrens[ mark_childrens ];
        }

    }

    if ( counter_all_common > 1 ) {
        return true;
    }

    return false;

}

bool DataModelTree::checkIndex(QModelIndex index, QModelIndex styleData)
{
    QObject *parent = objByIndex<QObject>( index );
    QObject *styleParent = objByIndex<QObject>( styleData );

    while( parent != static_cast<QObject*>( &rootItem ) ) {
        if ( parent == styleParent ) {
            return true;
        }
        parent = parent->parent();

    }

    return false;

}

QHash<int, QByteArray> DataModelTree::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    roles.insert( ROLE_NAME, NAME_STRING );

    return roles;
}

QModelIndex DataModelTree::seeObjectByName(QString name)
{
    QObjectList childrens = objByIndex<QObject>( currentIndex )->children();

    for ( int row = 0; row < childrens.length(); row++ ) {

        if ( childrens[ row ]->objectName() == name ) {
            currentIndex = createIndex( row, currentIndex.column() + 1, childrens[ row ] );
            return currentIndex;
        }

    }

    return QModelIndex();
}

template<typename CHILD, typename PARENT>
void DataModelTree::insertObjectInTree(QModelIndex currentIndex, int class_object)
{
    General *parent = objByIndex<General>( currentIndex );
    if (parent == nullptr)
    {
        parent = &rootItem;
    }

    CHILD *object = new CHILD( dynamic_cast<PARENT*>( parent )  );
    QSqlDatabase db = openDB();
    QSqlQuery query_to_dbobject( "INSERT INTO object (number_classes, property) VALUES (" + QString::number(class_object) + ", NULL)" );
    QSqlQuery q ("SELECT * FROM object");
    int id_obj = -1;
    while ( q.next() ) {
        id_obj = q.value( 0 ).toInt();
    }
    object->id_object = id_obj;
    object->id_parent = parent->id_object;
    QSqlQuery q1("INSERT INTO hierarchy VALUES(" + QString::number(object->id_object) + ", " + QString::number(object->id_parent) +")");
    QSqlQuery query_to_dbobject1( "UPDATE object SET property = \'" + object->generated_property + "\'" +
                                 " WHERE id_object = " + QString::number(object->id_object));
    db.close();

}
