#include "headers/datamodeltable.h"

DataModelTable::DataModelTable(int rows, QObject *parent)
    : QAbstractTableModel(parent),
      row_Count( rows )
{
    property_column.append( "Address" );
    property_column.append( "Property" );
}

int DataModelTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return row_Count;
}

int DataModelTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return property_column.length();
}

QVariant DataModelTable::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

    switch ( role ) {
    case ROLE_NAME:
        return data_table.at( index.row() )->objectName();

    case ROLE_ADDRESS:
        return cell_data.value( index, QVariant() );

    case ROLE_PROPERTY:
        return qobject_cast<General*>( data_table.at( index.row() ) )->generated_property;
    }

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        return cell_data.value( index, QVariant() );
    }

    return QVariant();
}

QVariant DataModelTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
         return property_column.at(section);
    }
    else {
        return data_table.at(section)->objectName();
    }

}

bool DataModelTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }


    if (role == Qt::DisplayRole || role == Qt::EditRole) {
       cell_data.insert(index, value);

       emit dataChanged(index, index);
    }

    return false;
}

void DataModelTable::takeObjects(QObjectList object_from_list)
{
    data_table = object_from_list;
}

void DataModelTable::setAddress(Singleton &rootItem)
{
    QString rule = "/";
    QObject *object;
    QVector<int> count_columns(this->data_table.length());

    int count_parent = 0;

    for ( int row = 0; row < this->data_table.length(); row++ ) {
        object = this->data_table.at( row );

        while ( object->parent() != static_cast<QObject*>( &rootItem ) ) {
             rule = "/" + object->parent()->objectName() + rule;
             object = object->parent();
             count_parent++;
        }

        count_columns[ row ] = count_parent;
        count_parent = 0;
        setData( index( row, 0 ), rule, Qt::EditRole );
        rule = "/";
    }

    for ( int row = 0 ; row < this->data_table.length(); row++ ) {

        switch( count_columns[row] ) {
            case type_City:
              setData( index( row, 2 ),
                      static_cast<City*>( this->data_table.at( row ) )->generated_property,
                      Qt::EditRole );
              break;

            case type_Street:
              setData( index( row, 2 ),
                      static_cast<Street*>( this->data_table.at( row ) )->generated_property,
                      Qt::EditRole );
              break;

            case type_Home:
              setData( index( row, 2 ),
                      static_cast<Home*>( this->data_table.at( row ) )->generated_property,
                      Qt::EditRole );
              break;

            case type_People:
              setData( index( row, 2 ),
                      static_cast<People*>(this->data_table.at(row))->generated_property,
                      Qt::EditRole );
              break;
        }

    }
}

void DataModelTable::slot_realiseTable(SortModel *model_proxy_sort, DataModelList *model_list, DataModelTree *model_tree)
{
    QObjectList objects_from_list;

    row_Count = model_proxy_sort->rowCount();

    for ( int row_proxy = 0; row_proxy < model_proxy_sort->rowCount(); row_proxy++ ) {

        for ( int row_list = 0; row_list < model_list->data_list.length(); row_list++ ) {

           if ( model_proxy_sort->index( 0, 0 ).internalPointer() != nullptr ) {

                 if ( model_proxy_sort->data( model_proxy_sort->index(row_proxy, 0) ).toString() ==
                      model_list->data( model_list->index( row_list, 0 ), ROLE_NAME ).toString() ) {
                     objects_from_list.append( static_cast<QObject*>( model_list->index( row_list, 0 ).internalPointer() ) );
                 }

           }

        }

    }

    takeObjects( objects_from_list );
    setAddress( model_tree->rootItem );

    emit layoutChanged();
}

QHash<int, QByteArray> DataModelTable::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    roles.insert( ROLE_NAME, NAME_STRING );
    roles.insert( ROLE_ADDRESS, ADDRESS_STRING );
    roles.insert( ROLE_PROPERTY, PROPERTY_STRING );

    return roles;
}



