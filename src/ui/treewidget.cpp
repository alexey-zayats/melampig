#include "treewidget.h"

#include <QtGui>
#include <QtSql>

#include <orm.h>
#include <ui.h>

#include <QModelIndex>

#include "errors.h"

#include "dragdropmodel.h"
#include "treeview.h"
#include "mainwindow.h"

namespace Melampig
{
    TreeWidget::TreeWidget(ObjectType otype, Keeper *keeper, TQueryMap &opt, QWidget *parent) :
        QMainWindow(parent), otype(otype), keeper(keeper), parentWidget(parent)
    {
        obj = keeper->objectByType(otype);
        setObjectName(obj->objectName());

        model = new DragDropModel ( (Tree*)obj, keeper, this);
        model->setQueryOptions(opt);

        view = new TreeView( this );
        view->setModel( model );
        view->setRootIsDecorated(true);
        view->setContextMenuPolicy(Qt::CustomContextMenu);

        view->setAcceptDrops(true);

        view->setDragEnabled(true);
        view->viewport()->setAcceptDrops(true);
        view->setDropIndicatorShown(true);
        view->setDragDropMode(QAbstractItemView::InternalMove);
        view->setDragDropOverwriteMode(true);
        view->setAlternatingRowColors(true);
        view->resizeColumnToContents(0);

        // connect(view, SIGNAL(activated(QModelIndex)), model, SLOT(viewItemActivated(QModelIndex)));

        for(int i = 0; i < model->columnCount(); i++ ) {
            view->setItemDelegateForColumn(i, model->getItemDelegate(i) );
        }
        setCentralWidget( view );

        actionBar = addToolBar(tr("Manage"));

        addAction = actionBar->addAction( QIcon( ":/icons/page_add.png" ), tr("Add") );
        addAction->setShortcut(tr("Insert"));

        childAction = actionBar->addAction( QIcon( ":/icons/chart_organisation.png" ), tr("Child") );
        childAction->setShortcut(tr("Ctrl+Insert"));

        editAction = actionBar->addAction( QIcon( ":/icons/page_edit.png" ), tr("Edit") );
        editAction->setShortcut(tr("Insert"));

        deleteAction = actionBar->addAction( QIcon( ":/icons/page_delete.png" ), tr("Delete") );
        deleteAction->setShortcut(tr("Delete"));

        connect(addAction, SIGNAL(triggered()), this, SLOT(actionAdd_clicked()));
        connect(editAction, SIGNAL(triggered()), this, SLOT(actionEdit_clicked()));
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(actionDelete_clicked()));
        connect(childAction, SIGNAL(triggered()), this, SLOT(actionChild_clicked()));

        connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

        connect(view->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &,
                                        const QItemSelection &)),
                this, SLOT(updateActions()));

        mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        workspace = (QWorkspace*)parentWidget;

        updateActions();
    }

    TreeWidget::~TreeWidget()
    {
        delete obj;
        qDeleteAll(formIndex);
    }

    void TreeWidget::actionAdd_clicked()
    {
        QModelIndex index = view->selectionModel()->currentIndex();

        try
        {
            if (!model->insertRow(index.row()+1, index.parent()))
                return;
        } catch (SqlError &err) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        updateActions();

        try
        {
            for (int column = 0; column < model->columnCount(index.parent()); ++column) {
                QModelIndex child = model->index(index.row()+1, column, index.parent());
                model->setData(child, QVariant(), Qt::EditRole);
            }
        }
        catch ( SqlError &err )
        {
            QMessageBox::critical(this, tr("Sql error"), err.getText().append( err.getQuery() ) );
        }
    }

    void TreeWidget::actionEdit_clicked()
    {
        QModelIndex *index = new QModelIndex( view->selectionModel()->currentIndex() );
        Object *o = model->getObject(*index);

        QString title( tr("%1(%2)").arg(o->get("title"), o->get("id")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j )
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }

        ObjectForm *form = new ObjectForm(o, keeper, parentWidget);
        form->setWindowTitle(title);
        form->setAttribute(Qt::WA_DeleteOnClose, true);
        form->setWindowIcon( windowIcon() );

        workspace->addWindow(form);
        form->show();

        connect( form, SIGNAL( destroyed() ), mainWindow, SLOT( closeTab() ) );

        formIndex.insert( o->get("id").toInt(), index );
        connect(form, SIGNAL(objectSaved(int)), this, SLOT(objectForm_saved(int)) );
    }

    void TreeWidget::objectForm_saved(int id)
    {
        if ( formIndex.contains(id) ) {
            QModelIndex *index = formIndex.value(id);
            model->changeData( *index, model->index( index->row(), model->columnCount() - 1, index->parent() ) );
        }
    }

    void TreeWidget::actionDelete_clicked()
    {
        QModelIndex index = view->selectionModel()->currentIndex();

        try
        {
            if (model->removeRow(index.row(), index.parent()))
                updateActions();
        } catch (SqlError &err) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }
    }

    void TreeWidget::actionChild_clicked()
    {
        QModelIndex index = view->selectionModel()->currentIndex();

        try
        {
            if (!model->insertRow(0, index))
                return;
        } catch (SqlError &err) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        try
        {
            for (int column = 0; column < model->columnCount(index); ++column) {
                QModelIndex child = model->index(0, column, index);
                model->setData(child, QVariant(), Qt::EditRole);
            }
        } catch (SqlError &err) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                QItemSelectionModel::ClearAndSelect);
        updateActions();
    }

    void TreeWidget::contextMenu(const QPoint &point)
    {
        QMenu menu(this);
        menu.addAction( addAction );
        menu.addAction( childAction );
        menu.addAction( editAction );
        menu.addAction( deleteAction );

        menu.exec( view->mapToGlobal( point ) );
    }

    void TreeWidget::updateActions()
    {
        bool hasSelection = !view->selectionModel()->selection().isEmpty();
        deleteAction->setEnabled(hasSelection);
        editAction->setEnabled(hasSelection);

        bool hasCurrent = view->selectionModel()->currentIndex().isValid();
        addAction->setEnabled(hasCurrent);
        childAction->setEnabled(hasCurrent);

        emit selectionChanged();
    }

    void TreeWidget::setMultiSelect(bool b) {
        view->setSelectionMode( b ? QAbstractItemView::ExtendedSelection :
                                    QAbstractItemView::SingleSelection );
    }

    Object* TreeWidget::getSelectedObject()
    {
        Object* data = 0;

        QModelIndexList list = view->selectionModel()->selectedRows();
        if (list.size() > 0 )
            data = model->getObject(list.at(0));

        return data;
    }

}
