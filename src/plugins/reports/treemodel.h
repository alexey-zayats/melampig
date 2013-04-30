#ifndef MELAMPIG_REPTREEMODEL_H
#define MELAMPIG_REPTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace Melampig
{
	class TreeItem;

	//! [0]
	class RepTreeModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		RepTreeModel(const QString &data, QObject *parent = 0);
		~RepTreeModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation,
							int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column,
						  const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

	private:
		void setupModelData(const QStringList &lines, TreeItem *parent);

		TreeItem *rootItem;
	};
	//! [0]
}

#endif // MELAMPIG_REPTREEMODEL_H
