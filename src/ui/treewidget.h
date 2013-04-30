#ifndef MELAMPIG_TREEWIDGET_H
#define MELAMPIG_TREEWIDGET_H

#include <QMainWindow>
#include <orm.h>
#include <QModelIndex>
#include <QHash>

class QWorkspace;

namespace Melampig
{

    class MainWindow;
    class TreeModel;
    class TreeView;

    class MELAMPIG_UI_EXPORT TreeWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        TreeWidget(ObjectType type,  Keeper *keeper, TQueryMap &opt, QWidget *parent = 0);
        ~TreeWidget();
        void setMultiSelect(bool b);
        Object* getSelectedObject();

    signals:
        void selectionChanged();

    public slots:
        void actionAdd_clicked();
        void actionEdit_clicked();
        void actionDelete_clicked();
        void actionChild_clicked();
        void contextMenu(const QPoint&);
        void updateActions();
        void objectForm_saved(int id);

    private:
        ObjectType otype;
        Object *obj;
        Keeper *keeper;
        QWidget *parentWidget;

        QHash<int,QModelIndex*> formIndex;

        TreeModel *model;
        TreeView *view;

        QToolBar *actionBar;
        QAction *addAction;
        QAction *editAction;
        QAction *deleteAction;
        QAction *childAction;

        MainWindow *mainWindow;
        QWorkspace *workspace;
    };

}

#endif // MELAMPIG_TREEWIDGET_H
