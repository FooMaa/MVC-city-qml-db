#ifndef DATAMODELLIST_H
#define DATAMODELLIST_H

#include <QSortFilterProxyModel>

#include "singleton.h"
#include "connector.h"

class DataModelList : public QAbstractItemModel
{
    Q_OBJECT

public:
    QObjectList data_list;
    QModelIndex currentIndexTree;

    DataModelList(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void sort(int column, Qt::SortOrder order = Qt::DescendingOrder);
    QHash<int, QByteArray> roleNames() const;

public slots:
    void addValue(const QModelIndex &index);
    void slot_insertToLineFromTree(QModelIndex currentIndex);
};

#endif // DATAMODELLIST_H
