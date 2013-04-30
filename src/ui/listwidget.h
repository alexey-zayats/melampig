#ifndef MELAMPIG_LISTWIDGET_H
#define MELAMPIG_LISTWIDGET_H

#include <ui.h>
#include <orm.h>

#include <QMainWindow>

namespace Melampig
{

    class MELAMPIG_UI_EXPORT ListWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        ListWidget(ObjectType type,  Keeper *keeper, TQueryMap &opt, QWidget *parent = 0);
        ~ListWidget();

         Object* getSelectedObject();

    signals:
         void selectionChanged();

    public slots:
        void actionAdd_clicked();
        void actionEdit_clicked();
        void actionDelete_clicked();
        void updateActions();
        void contextMenu(const QPoint &point);
        void objectForm_saved(int id);

    private:
        ObjectType otype;
        Object *obj;
        Keeper *keeper;

        QWidget *parentWidget;
        QHash<int,int> formIndex;

        ListView *view;
        ListModel *model;

        QToolBar *actionBar;

        QAction *addAction;
        QAction *deleteAction;
        QAction *editAction;

        MainWindow *mainWindow;
        QWorkspace *workspace;
    };

}

#endif // MELAMPIG_LISTWIDGET_H
