#include "tablewidget.h"

#include <QtGui>

#include <orm.h>
#include <ui.h>

#include <QtSql>

#include "mainwindow.h"
#include "filterwidget.h"

namespace Melampig
{
TableWidget::TableWidget(ObjectType otype, Keeper *keeper, TQueryMap &opt, QWidget *parent) :
    QMainWindow(parent), otype(otype), keeper(keeper), parentWidget(parent), baseOpt(opt)
{
    obj = keeper->objectByType(otype);
    setObjectName(obj->objectName());

    model = new TableModel(obj, keeper, parentWidget);
    model->setQueryOptions(baseOpt);

    QWidget *centralWidget = new QWidget(this);

    view = new TableView( centralWidget );
//        view->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
//        view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->setModel(model);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    view->setAlternatingRowColors(true);

    model->initModel();
    for(int i = 0; i < model->columnCount(); i++ ) {
        view->setItemDelegateForColumn(i, model->getItemDelegate(i) );
    }

//        view->resizeColumnsToContents();

    QGridLayout *layout = new QGridLayout(centralWidget);
    layout->addWidget(view, 0,0);

    layout->setMargin(0);

    setCentralWidget( centralWidget );

    actionBar = addToolBar(tr("Manage"));

    addAction = actionBar->addAction( QIcon( ":/icons/page_add.png" ), tr("Add") );
    addAction->setShortcut(tr("Insert"));

    editAction = actionBar->addAction( QIcon( ":/icons/page_edit.png" ), tr("Edit") );
    editAction->setShortcut(tr("F4"));

    deleteAction = actionBar->addAction( QIcon( ":/icons/page_delete.png" ), tr("Delete") );
    deleteAction->setShortcut(tr("Delete"));

    actionBar->addSeparator();

    refreshAction = actionBar->addAction(QIcon(":/icons/arrow_refresh.png"), tr("Refresh") );
    refreshAction->setShortcut(tr("F5"));


    connect(addAction, SIGNAL(triggered()), this, SLOT(actionAdd_clicked()));
    connect(editAction, SIGNAL(triggered()), this, SLOT(actionEdit_clicked()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(actionDelete_clicked()));

    connect(refreshAction, SIGNAL(triggered()), this, SLOT(actionRefresh_clicked()));

    connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

    connect(view->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &,
                                    const QItemSelection &)),
            this, SLOT(updateActions()));

    // connect(view, SIGNAL(activated(QModelIndex)), view, SLOT(resizeRowsToContents()));

    mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
    workspace = (QWorkspace*)parentWidget;

    initFilters();

    updateActions();

//        statusBar()->showMessage(tr("Ready"));
}

TableWidget::~TableWidget()
{
    delete obj;
}

void TableWidget::actionRefresh_clicked()
{
    model->initModel();
}

void TableWidget::setModelQueryOptions(const TQueryMap &opt)
{
    if (opt.size() == 0 )
        model->setQueryOptions(baseOpt);
    else
        model->setQueryOptions(opt);
}

void TableWidget::initFilters()
{
    QDockWidget *dockWidget = new QDockWidget(tr("Filters"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                     Qt::RightDockWidgetArea);

    QList<Attr*> list = model->attrList();

    filterWidget = new FilterWidget(baseOpt, list, keeper, parentWidget);

    connect(filterWidget, SIGNAL(filterActivated(const TQueryMap&)), this, SLOT(filterWidget_activated(const TQueryMap &)));

    dockWidget->setWidget(filterWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    dockWidget->hide();

    dockWidget->toggleViewAction()->setIcon( QIcon(":/icons/magnifier.png"));
    actionBar->addAction(dockWidget->toggleViewAction());
}

void TableWidget::filterWidget_activated(const TQueryMap&opt)
{
    if (opt.size() == 0 )
        model->setQueryOptions(baseOpt);
    else
        model->setQueryOptions(opt);
    model->initModel();
}

void TableWidget::actionEdit_clicked()
{
    QModelIndex index = view->selectionModel()->currentIndex();

    Object *o = model->getObject(index);
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

    formIndex.insert(o->get("id").toInt(), index.row());
    connect(form, SIGNAL(objectSaved(int)), this, SLOT(objectForm_saved(int)) );

    connect( form, SIGNAL( destroyed() ), mainWindow, SLOT( closeTab() ) );
}

void TableWidget::objectForm_saved(int id)
{
    if ( formIndex.contains(id) ) {
        int row = formIndex.value(id);
        model->changeData( model->index(row, 0), model->index(row, view->horizontalHeader()->count()));
    }
}

void TableWidget::actionAdd_clicked()
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

    try
    {
        Attr *a;
        QList<Attr*> attr = obj->attrSchema();
        QStringList views = obj->getViewItem();
        for( int i = 0; i < attr.size(); i++ )
        {
            a = attr.at(i);
            QString name = a->getName();

            if ( !views.contains(name) )
                continue;

            QModelIndex child = model->index(row, i);
            model->setData(child, QVariant(), Qt::EditRole);
        }

        int firstCol = 0;
        for( int i = 0; i < attr.size(); i++ ) {
            a = attr.at(i);
            if ( views.contains(a->getName()) ) {
                firstCol = i;
                break;
            }
        }

        QModelIndex index( model->index(row, firstCol) );
        QItemSelection selection(index, index);
        view->selectionModel()->select(selection, QItemSelectionModel::Select);
        view->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);

//            actionEdit_clicked();
    }
    catch ( SqlError &err )
    {
        QMessageBox::critical(this, tr("Sql error"), err.getText().append( err.getQuery() ) );
    }

}

void TableWidget::actionDelete_clicked()
{
    int result = QMessageBox::question(this, tr("Remove rows"), tr("Are you shure to remove selected rows?"), QMessageBox::Yes, QMessageBox::No);
    if ( QMessageBox::No == result) return;

    try
    {
        QModelIndexList indexList = view->selectionModel()->selectedIndexes();
        for( int i = (indexList.size()-1); i >= 0;  i-- ) {
            QModelIndex index = indexList.at(i);
            model->removeRow(index.row(), index.parent());
        };

        updateActions();

    } catch (SqlError &err) {
        QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
    }

}

void TableWidget::contextMenu(const QPoint &point)
{
    QMenu menu(this);
    menu.addAction( addAction );
    menu.addAction( deleteAction );

    menu.exec( view->mapToGlobal( point ) );
}

void TableWidget::updateActions()
{

    bool hasSelection = !view->selectionModel()->selection().isEmpty();
    deleteAction->setEnabled(hasSelection);
    editAction->setEnabled(hasSelection);

    emit selectionChanged();
}

Object* TableWidget::getSelectedObject()
{
    Object *data = 0;

    QModelIndexList list = view->selectionModel()->selectedIndexes();
//        qDebug() << Q_FUNC_INFO << list.size();
    if (list.size() > 0 )
        data = model->getObject(list.at(0));

    return data;
}
}
