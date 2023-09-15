#include "headers/datamodellist.h"

DataModelList::DataModelList(QObject* parent): QAbstractItemModel(parent)
{

}

QModelIndex DataModelList::index(int row, int column, const QModelIndex &parent) const
{

    if ( !hasIndex( row, column, parent ) ) {
        return QModelIndex();
    }

    return createIndex( row, column, ( void* ) data_list[ row ] );
}

QModelIndex DataModelList::parent(const QModelIndex &child) const
{
    Q_UNUSED( child );

    return QModelIndex();
}

int DataModelList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return data_list.size();
}

int DataModelList::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return 1;
}

QVariant DataModelList::data(const QModelIndex &index, int role) const
{
    if ( role == ROLE_NAME ) {
        return static_cast<QObject*>( index.internalPointer() )->property(QString( PROPERTY_NAME ).toUtf8());
    }

    if ( role == Qt::DisplayRole ) {
        return static_cast<QObject*>( index.internalPointer() )->property(QString( PROPERTY_NAME ).toUtf8());
    }

    return QVariant();
}

void DataModelList::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED( column );

    emit beginResetModel();

    if (order == Qt::AscendingOrder) {
        std::sort( data_list.begin(), data_list.end(), std::less<QObject*>() );
    }
    else {
        std::sort( data_list.begin(), data_list.end(), std::greater<QObject*>() );
    }

    emit endResetModel();
}

void DataModelList::addValue(const QModelIndex &index)
{
    emit beginResetModel();

    if ( index.internalPointer() != nullptr ) {
       data_list = static_cast<QObject*>( index.internalPointer() )->children();
       currentIndexTree = index;
    }

    emit endResetModel();
}

void DataModelList::slot_insertToLineFromTree(QModelIndex currentIndex)
{
    addValue( currentIndex );
}


QHash<int, QByteArray> DataModelList::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    roles.insert( ROLE_NAME, NAME_STRING );

    return roles;
}


