#include "headers/sortmodel.h"

SortModel::SortModel(QObject *parent): QSortFilterProxyModel(parent)
{
}

void SortModel::filteringListView(QString string)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterFixedString(string);
}

void SortModel::sortingListView(bool checked)
{

    if (checked)
    {
        sort(0, Qt::AscendingOrder);
    }
    else
    {
        sort(0, Qt::DescendingOrder);
    }

}
