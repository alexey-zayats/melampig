#ifndef MELAMPIG_LINKEDTABLEMODEL_H
#define MELAMPIG_LINKEDTABLEMODEL_H

#include <QtGui>
#include <orm.h>

namespace Melampig
{
	class MELAMPIG_UI_EXPORT LinkedTableModel : public QAbstractTableModel
	{
		Q_OBJECT
	public:
		LinkedTableModel(QList<Object*> &data, Keeper *keeper, QObject *parent = 0);
		~LinkedTableModel();

		QVariant data(const QModelIndex &index, int role) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;

		Qt::ItemFlags flags(const QModelIndex &index) const;
		bool setData(const QModelIndex &index, const QVariant &value,
					 int role = Qt::EditRole);

		bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
		bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

//		bool canFetchMore ( const QModelIndex & parent ) const;
		void fetchMore ( const QModelIndex & parent );

		void changeData( const QModelIndex &from, const QModelIndex &to );
		void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

		QList<Attr*> attrList() const { return attr; }
		QItemDelegate *getItemDelegate(int);

	signals:
	public slots:
		void referenceSelected(int, QList<Object*> value);

	private:
		void fillRefMap(const ObjectType &type, const QList<QVariant> &idlist);
		void setupSchema(Object *o);
		void initModel();

		Object *oSrc;
		Object *oDst;
		Object *oLink;

		Keeper *keeper;

		QList<Attr*> attr;
		QList<int> hiddenCols;

		QList<QVariant> rows;
		int count;
		int linkFieldsCount;

		QList<Object*> linkRecord; // link objects
		QList<Object*> dstRecord;
		int limit;

		QHash<ObjectType,QList<QVariant> > refId;
		QHash<ObjectType,QHash<int,QVariant> > refs;

		QHash<ObjectType,QString> refName;
		QWidget *parentWidget;

		QString srcName;
		QString dstName;
		QStringList views;

		TQueryMap options;
		QString sortColumn;
	};

}

#endif // MELAMPIG_LINKEDTABLEMODEL_H
