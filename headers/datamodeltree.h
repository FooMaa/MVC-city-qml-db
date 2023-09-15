#ifndef DATAMODELTREE_H
#define DATAMODELTREE_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QObjectList>
#include <QMimeData>
#include <QDataStream>
#include <QCoreApplication>
#include <QDebug>

#include "connector.h"
#include "singleton.h"

class DataModelTree : public QAbstractItemModel
{
  Q_OBJECT
public:
    Singleton &rootItem = Singleton::get_instance();
    QModelIndex currentIndex;
    QModelIndex parent_idx;
    QModelIndex child_idx;
    QObject *for_repeat_objects;
    QModelIndex checkIdx;

    DataModelTree(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    template <typename OBJECT, typename PARENT> void addValue(OBJECT *item, const QModelIndex &parentIdx);
    template <typename OBJECT> OBJECT *objByIndex(const QModelIndex &index) const;
    template <typename CHILD, typename PARENT> void insertObjectInTree(QModelIndex currentIndex, int class_object);
    int seeParent(QObject *child = nullptr, QModelIndex index = QModelIndex());


public slots:
    void slot_insertValueFromLineEdit(QModelIndex currentIndex);
    QModelIndex seeObjectByName(QString name);
    void setCurrentIndex(QModelIndex index);
    QModelIndex getCurrentIndex();
    bool changeParent(QModelIndex currentIndex, QModelIndex currentParent);
    bool checkDoubleNames(QModelIndex parent, QModelIndex child);
    bool changeObjectName(QString text);
    bool checkAdd(QModelIndex curidx);
    bool checkIndex(QModelIndex index, QModelIndex styleData);

signals:
    void signal_dragAndDrop();
    void signal_addItem();  
};

template <typename OBJECT,typename PARENT>
void DataModelTree::addValue(OBJECT *item, const QModelIndex &parentIdx)
{
    beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));

    if (objByIndex<PARENT>(parentIdx) == nullptr)
    {
        item->setParent(&rootItem);
    }
    else
    {
        item->setObjectName(item->names_obj);
    }

    endInsertRows();
    emit layoutChanged();
}

template <typename OBJECT>
OBJECT *DataModelTree::objByIndex(const QModelIndex &index) const
{
    if (!index.isValid())
    {
       return 0;
    }

   return static_cast<OBJECT*>(index.internalPointer());
}

#endif // DATAMODELTREE_H
