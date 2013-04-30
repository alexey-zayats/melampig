#include "tableview.h"
#include <errors.h>
#include <QHeaderView>

namespace Melampig
{
    TableView::TableView( QWidget *parent )
    : QTableView( parent )
    {
//        horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
//        horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//        horizontalHeader()->setStretchLastSection(true);
        setSortingEnabled(true);
        setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed );
        setSelectionBehavior(QAbstractItemView::SelectItems);
//        verticalHeader()->hide();
    }

    TableView::~TableView()
    {}

}
