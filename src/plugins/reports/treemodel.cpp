
#include <QtGui>

#include "treeitem.h"
#include "treemodel.h"

namespace Melampig
{
	RepTreeModel::RepTreeModel(const QString &data, QObject *parent)
		: QAbstractItemModel(parent)
	{
		QList<QVariant> rootData;
		rootData << "Title";
		rootItem = new TreeItem(rootData);
		setupModelData(data.split(QString("\n")), rootItem);
	}

	RepTreeModel::~RepTreeModel()
	{
		delete rootItem;
	}

	int RepTreeModel::columnCount(const QModelIndex &parent) const
	{
		if (parent.isValid())
			return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
		else
			return rootItem->columnCount();
	}

	QVariant RepTreeModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role != Qt::DisplayRole)
			return QVariant();

		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

		return item->data(index.column());
	}

	Qt::ItemFlags RepTreeModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return 0;

		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}

	QVariant RepTreeModel::headerData(int section, Qt::Orientation orientation,
								   int role) const
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
			return rootItem->data(section);

		return QVariant();
	}

	QModelIndex RepTreeModel::index(int row, int column, const QModelIndex &parent)
				const
	{
		if (!hasIndex(row, column, parent))
			return QModelIndex();

		TreeItem *parentItem;

		if (!parent.isValid())
			parentItem = rootItem;
		else
			parentItem = static_cast<TreeItem*>(parent.internalPointer());

		TreeItem *childItem = parentItem->child(row);
		if (childItem) {
			return createIndex(row, column, childItem);
		} else {
			return QModelIndex();
		}
	}

	QModelIndex RepTreeModel::parent(const QModelIndex &index) const
	{
		if (!index.isValid())
			return QModelIndex();

		TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
		TreeItem *parentItem = childItem->parent();

		if (parentItem == rootItem)
			return QModelIndex();

		return createIndex(parentItem->row(), 0, parentItem);
	}

	int RepTreeModel::rowCount(const QModelIndex &parent) const
	{
		TreeItem *parentItem;
		if (parent.column() > 0)
			return 0;

		if (!parent.isValid())
			parentItem = rootItem;
		else
			parentItem = static_cast<TreeItem*>(parent.internalPointer());

		return parentItem->childCount();
	}

	void RepTreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
	{
		QList<TreeItem*> parents;
		QList<int> indentations;
		parents << parent;
		indentations << 0;

		int number = 0;

		while (number < lines.count()) {
			int position = 0;
			while (position < lines[number].length()) {
				if (lines[number].mid(position, 1) != "\t")
					break;
				position++;
			}

			QString lineData = lines[number].mid(position).trimmed();

			if (!lineData.isEmpty()) {
				// Read the column data from the rest of the line.
				QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
				QList<QVariant> columnData;
				for (int column = 0; column < columnStrings.count(); ++column)
					columnData << columnStrings[column];

				if (position > indentations.last()) {
					// The last child of the current parent is now the new parent
					// unless the current parent has no children.

					if (parents.last()->childCount() > 0) {
						parents << parents.last()->child(parents.last()->childCount()-1);
						indentations << position;
					}
				} else {
					while (position < indentations.last() && parents.count() > 0) {
						parents.pop_back();
						indentations.pop_back();
					}
				}

				// Append a new item to the current parent's list of children.
				parents.last()->appendChild(new TreeItem(columnData, parents.last()));
			}

			number++;
		}
	}
}
