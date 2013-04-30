#ifndef MELAMPIG_TREEMODEL_H
#define MELAMPIG_TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <orm.h>

class QItemDelegate;

namespace Melampig
{
	class MELAMPIG_UI_EXPORT TreeModel : public QAbstractItemModel
	{
		Q_OBJECT

		public:
			TreeModel( Tree *root, Keeper *keeper, QObject *parent = 0);
			~TreeModel();

			QVariant data(const QModelIndex &index, int role) const;
			Qt::ItemFlags flags(const QModelIndex &index) const;
			QVariant headerData(int section, Qt::Orientation orientation,
								int role = Qt::DisplayRole) const;
			QModelIndex index(int row, int column,
							  const QModelIndex &parent = QModelIndex()) const;
			QModelIndex parent(const QModelIndex &index) const;
			int rowCount(const QModelIndex &parent = QModelIndex()) const;
			int columnCount(const QModelIndex &parent = QModelIndex()) const;

			bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
			bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
			bool setData(const QModelIndex &index, const QVariant &value,
						 int role = Qt::EditRole);

			Object *getObject(const QModelIndex &index);
			QItemDelegate *getItemDelegate(int);

		public slots:
//            void viewItemActivated(QModelIndex);
            void setQueryOptions( TQueryMap &opt) { options = opt; }
            void changeData( const QModelIndex &from, const QModelIndex &to );

        private:
            Keeper *keeper;
            Tree *rootItem;
            QList<Attr*> attr;
            QWidget *parentWidget;
            TQueryMap options;
    };
}

#endif // _CMS_TREE_MODEL_H_
