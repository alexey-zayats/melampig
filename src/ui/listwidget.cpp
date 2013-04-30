#include "listwidget.h"

#include "mainwindow.h"
#include <QWorkspace>

namespace Melampig
{

    ListWidget::ListWidget(ObjectType type,  Keeper *keeper, TQueryMap &/*opt*/, QWidget *parent) :
        QMainWindow(parent), otype(type), keeper(keeper), parentWidget(parent)
    {
        obj = keeper->objectByType(otype);
        setObjectName(obj->objectName());

        model = new ListModel(obj, keeper, this);

        view = new ListView( this );
        view->setModel(model);
        view->setContextMenuPolicy(Qt::CustomContextMenu);
        view->setAlternatingRowColors(true);

        model->initModel();

        setCentralWidget( view );

        actionBar = addToolBar(tr("Manage"));

        addAction = actionBar->addAction( QIcon( ":/icons/table_add.png" ), tr("Add") );
        editAction = actionBar->addAction( QIcon( ":/icons/table_edit.png" ), tr("Edit") );
        deleteAction = actionBar->addAction( QIcon( ":/icons/table_delete.png" ), tr("Delete") );

        connect(addAction, SIGNAL(triggered()), this, SLOT(actionAdd_clicked()));
        connect(editAction, SIGNAL(triggered()), this, SLOT(actionEdit_clicked()));
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(actionDelete_clicked()));

        connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

        connect(view->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &,
                                        const QItemSelection &)),
                this, SLOT(updateActions()));

        mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        workspace = (QWorkspace*)parentWidget;

        updateActions();
    }

    ListWidget::~ListWidget()
    {
            delete obj;
    }

    void ListWidget::actionEdit_clicked()
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
        connect( form, SIGNAL( destroyed() ), mainWindow, SLOT( closeTab() ) );
        connect(form, SIGNAL(objectSaved(int)), this, SLOT(objectForm_saved(int)) );
    }

    void ListWidget::objectForm_saved(int id)
    {
        if ( formIndex.contains(id) ) {
            int row = formIndex.value(id);
            model->changeData( model->index(row, 0), model->index(row, 0));
        }
    }

    void ListWidget::actionAdd_clicked()
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

                QModelIndex child = model->index(row, i, index.parent());
                model->setData(child, QVariant(), Qt::EditRole);
            }
        }
        catch ( SqlError &err )
        {
            QMessageBox::critical(this, tr("Sql error"), err.getText().append( err.getQuery() ) );
        }

    }

    void ListWidget::actionDelete_clicked()
    {
        QModelIndex index = view->selectionModel()->currentIndex();
        try
        {
            if ( model->removeRow(index.row(), index.parent()) )
                updateActions();
        }
        catch ( SqlError &err )
        {
            QMessageBox::critical(this, tr("Sql error"), err.getText().append( err.getQuery() ) );
        }
    }

    void ListWidget::contextMenu(const QPoint &point)
    {
        QMenu menu(this);
        menu.addAction( addAction );
        menu.addAction( deleteAction );

        menu.exec( view->mapToGlobal( point ) );
    }

    void ListWidget::updateActions()
    {
        bool hasSelection = !view->selectionModel()->selection().isEmpty();
        deleteAction->setEnabled(hasSelection);
        editAction->setEnabled(hasSelection);

        emit selectionChanged();
    }

    Object* ListWidget::getSelectedObject()
    {
        Object *data = 0;

        QModelIndexList list = view->selectionModel()->selectedRows();
        if (list.size() > 0 )
            data = model->getObject(list.at(0));

        return data;
    }

}
