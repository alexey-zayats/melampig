#ifndef MELAMPIG_TABLEWIDGET_H
#define MELAMPIG_TABLEWIDGET_H

#include <QMainWindow>
#include <orm.h>


#include <QModelIndex>
#include <QHash>

class QSqlRelationalTableModel;
class QAbstractItemDelegate;
class QWorkspace;

namespace Melampig
{

    class TableModel;
    class TableView;
    class ObjectFrom;
    class FilterWidget;
    class MainWindow;

    class MELAMPIG_UI_EXPORT TableWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        TableWidget(ObjectType type,  Keeper *keeper, TQueryMap &opt, QWidget *parent = 0);
        ~TableWidget();
        Object* getSelectedObject();

    signals:
        void selectionChanged();

    public slots:
        void actionAdd_clicked();
        void actionEdit_clicked();
        void actionDelete_clicked();
        void actionRefresh_clicked();

        void contextMenu(const QPoint &point);
        void updateActions();

        void objectForm_saved(int);
        void setModelQueryOptions(const TQueryMap &opt);
        void filterWidget_activated(const TQueryMap &opt);

    protected:
        ObjectType otype;
        Object *obj;
        Keeper *keeper;

        TableView *view;
        TableModel *model;

        QHash<int,int> formIndex;

    private:
        void initFilters();

        QToolBar *actionBar;
        QWidget *parentWidget;

        QAction *addAction;
        QAction *deleteAction;
        QAction *editAction;
        QAction *refreshAction;

        MainWindow *mainWindow;
        QWorkspace *workspace;

        FilterWidget *filterWidget;
        TQueryMap baseOpt;

    };

}

#endif // MELAMPIG_TABLEWIDGET_H
