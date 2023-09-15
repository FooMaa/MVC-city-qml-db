#ifndef SORTMODEL_H
#define SORTMODEL_H

#include <QSortFilterProxyModel>

class SortModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SortModel(QObject *parent = nullptr);


public slots:
    void sortingListView(bool checked);
    void filteringListView(QString string);
};

#endif // SORTMODEL_H
