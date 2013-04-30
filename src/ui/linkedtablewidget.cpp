#include "linkedtablewidget.h"

#include <QtGui>

#include <orm.h>
#include <ui.h>

#include <QtSql>
#include "referencedialog.h"

namespace Melampig
{
	LinkedTableWidget::LinkedTableWidget(QList<Object*> &list, Keeper *keeper, QWidget *parent) :
		QMainWindow(parent), olist(list), keeper(keeper), parentWidget(parent)
	{
		setObjectName(QString("LinkedTable%1").arg(olist.at(1)->objectName()));

		model = new LinkedTableModel(olist, keeper, parentWidget);

        view = new TableView( this );
        view->setModel(model);
        view->setContextMenuPolicy(Qt::CustomContextMenu);
        view->setAlternatingRowColors(true);
//        view->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
//        view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

        for(int i = 0; i < model->columnCount(); i++ ) {
            view->setItemDelegateForColumn(i, model->getItemDelegate(i) );
        }

        setCentralWidget( view );

        actionBar = addToolBar(tr("Manage"));

        addAction = actionBar->addAction( QIcon( ":/icons/table_row_insert.png" ), tr("Add") );
        deleteAction = actionBar->addAction( QIcon( ":/icons/table_row_delete.png" ), tr("Delete") );

        connect(addAction, SIGNAL(triggered()), this, SLOT(actionAdd_clicked()));
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(actionDelete_clicked()));

        connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

        connect(view->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &,
                                        const QItemSelection &)),
                this, SLOT(updateActions()));

        updateActions();
    }

    LinkedTableWidget::~LinkedTableWidget()
    {
    }

    void LinkedTableWidget::actionAdd_clicked()
    {
        QModelIndex index = view->selectionModel()->currentIndex();

        int row = model->rowCount();
        try
        {
            if (!model->insertRow(row, index.parent() ))
                return;
        } catch (SqlError &err) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        updateActions();
    }

    void LinkedTableWidget::actionDelete_clicked()
    {
        QModelIndex index = view->selectionModel()->currentIndex();
        try
        {
            if ( model->removeRow(index.row(), index.parent()) )
                updateActions();
        } catch (SqlError &err) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }
    }

    void LinkedTableWidget::contextMenu(const QPoint &point)
    {
        QMenu menu(this);
        menu.addAction( addAction );
        menu.addAction( deleteAction );

        menu.exec( view->mapToGlobal( point ) );
    }

    void LinkedTableWidget::updateActions()
    {
        bool hasSelection = !view->selectionModel()->selection().isEmpty();
        deleteAction->setEnabled(hasSelection);
    }

}
