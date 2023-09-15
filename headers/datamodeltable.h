#ifndef DATAMODELTABLE_H
#define DATAMODELTABLE_H

#include <QAbstractItemModel>
#include <QDebug>

#include "connector.h"
#include "datamodellist.h"
#include "datamodeltree.h"
#include "sortmodel.h"
#include "singleton.h"

class DataModelTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    QObjectList data_table;
    int row_Count = 0;

    DataModelTable(int rows = 0, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int, QByteArray> roleNames() const;

    void takeObjects(QObjectList object_from_list);
    void setAddress(Singleton &rootItem);


protected:
    QStringList property_column;
    QMap <QModelIndex, QVariant> cell_data;


public slots:
    void slot_realiseTable(SortModel *model_proxy_sort,
                           DataModelList *model_list,
                           DataModelTree *model_tree);
};

#endif // DATAMODELTABLE_H
