#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QtGui>
#include <orm.h>

namespace Melampig
{

    class MELAMPIG_UI_EXPORT ListModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        ListModel(Object *obj, Keeper *keeper, QObject *parent = 0);
        ~ListModel();

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole);
        QVariant data(const QModelIndex &index, int role) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;

        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
        void initModel();

        void fetchMore ( const QModelIndex &/*parent*/);
        bool canFetchMore ( const QModelIndex &/*parent*/) const;

        Object *getObject(const QModelIndex &index);

        void setCheckable(const bool &a) { checkable = a; }
        void setMulticheck(const bool &a) { multicheck = a; }

        bool isCheckable () const { return checkable; }
        bool isMulticheck () const { return multicheck; }

    signals:

    public slots:
        void changeData( const QModelIndex &from, const QModelIndex &to );

    private:
        Object *obj;
        Keeper *keeper;

        QList<Attr*> attr;
        QList<QVariant> rows;
        int count;

        QList<Object*> record;
        int limit;
        int titleIndex;

        bool checkable;
        bool multicheck;

    };

}

#endif // LISTMODEL_H
