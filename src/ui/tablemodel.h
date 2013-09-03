#ifndef MELAMPIG_TABLEMODEL_H
#define MELAMPIG_TABLEMODEL_H

#include <QtGui>
#include <orm.h>

#include "ormmodel.h"
//class QTableView;

namespace Melampig
{
	class MELAMPIG_UI_EXPORT TableModel : public QAbstractTableModel
	{
        Q_OBJECT
    public:
        TableModel(Object *obj, Keeper *keeper, QObject *parent = 0);
        ~TableModel();

        QVariant data(const QModelIndex &index, int role) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole);

        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

        bool canFetchMore ( const QModelIndex & parent ) const;
        void fetchMore ( const QModelIndex & parent );

        void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

        void initModel();
        Object *getObject(const QModelIndex &index);
        QItemDelegate *getItemDelegate(int);
        QList<Attr*> attrList() const { return attr; }

    signals:
    public slots:
        void setQueryOptions(const TQueryMap &opt);
        void changeData( const QModelIndex &from, const QModelIndex &to );

    private:
        void fillRefMap(const ObjectType &type, const QList<QVariant> &idlist);

        Object *obj;
        Keeper *keeper;

        QList<Attr*> attr;
        QStringList views;

        int rows;

        QList<Object*> record;
        int limit;
        int offset;

        QHash<ObjectType,QList<QVariant> > refId;
        QHash<ObjectType,QHash<int,QVariant> > refs;

        QHash<ObjectType,QString> refName;
        QWidget *parentWidget;

        TQueryMap options;
	};

}

#endif // MELAMPIG_TABLEMODEL_H
