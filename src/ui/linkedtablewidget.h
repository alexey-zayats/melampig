#ifndef MELAMPIG_LINKEDTABLEWIDGET_H
#define MELAMPIG_LINKEDTABLEWIDGET_H

#include <QMainWindow>
#include <orm.h>


#include <QModelIndex>

namespace Melampig
{

    class LinkedTableModel;
    class TableView;

    class MELAMPIG_UI_EXPORT LinkedTableWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        LinkedTableWidget(QList<Object*> &list, Keeper *keeper, QWidget *parent = 0);
        ~LinkedTableWidget();

        LinkedTableModel *getModel() const { return model; }

    public slots:
        void actionAdd_clicked();
        void actionDelete_clicked();

        void contextMenu(const QPoint &point);
        void updateActions();

    protected:
        QList<Object*> olist;
        Keeper *keeper;

        TableView *view;
        LinkedTableModel *model;

    private:
        QToolBar *actionBar;
        QWidget *parentWidget;

        QAction *addAction;
        QAction *deleteAction;
    };

}

#endif // MELAMPIG_LINKEDTABLEWIDGET_H
