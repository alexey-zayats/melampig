#include <QtGui>

#include "dragdropmodel.h"

namespace Melampig
{

    DragDropModel::DragDropModel(Tree *item, Keeper *keeper, QObject *parent)
        : TreeModel(item, keeper, parent), keeper(keeper)
    {
    }

    bool DragDropModel::dropMimeData(const QMimeData *data,
                                     Qt::DropAction /*action*/, int row, int /*column*/, const QModelIndex &parent)
    {
        if (!data->hasFormat("text/plain"))
            return false;

        QByteArray encodedData = data->data("text/plain");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        qint64 _id;
        int _row;
        int _column;

        stream >> _id >> _row >> _column;

        QModelIndexList pers = persistentIndexList();
        foreach(const QModelIndex &idx, pers)
        {
            if (idx.row() == _row && idx.column() == _column && idx.internalId() == _id )
            {
                if ( idx.parent().internalId() == parent.internalId() )
                    return false;

                Tree *child = static_cast<Tree*>(idx.internalPointer());
                if ( child == 0 )
                    return false;

                Tree *oldParent = child->parent();
                if ( oldParent == 0 )
                    return false;

                Tree *newParent = static_cast<Tree*>(parent.internalPointer());
                if ( newParent == 0 )
                    return false;

                beginMoveRows(idx.parent(), idx.row(), idx.row(), parent, parent.row());

                try
                {
                    oldParent->removeChild(idx.row());
                    newParent->addChild(child, row);
                    child->setParent(newParent);
                }
                catch (SqlError&){}

                endMoveRows();

                return true;
            }
        }

        return true;
    }

    Qt::ItemFlags DragDropModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags defaultFlags = TreeModel::flags(index);

        if (index.isValid())
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
        else
            return Qt::ItemIsDropEnabled | defaultFlags;
    }

    QMimeData *DragDropModel::mimeData(const QModelIndexList &indexes) const
    {
        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;

        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        foreach (QModelIndex index, indexes)
            if (index.isValid())
                stream << index.internalId() << index.row() << index.column() ;

        mimeData->setData("text/plain", encodedData);
        return mimeData;
    }

    QStringList DragDropModel::mimeTypes() const
    {
        QStringList types;
        types << "text/plain";
        return types;
    }

    Qt::DropActions DragDropModel::supportedDropActions() const
    {
        return /*Qt::CopyAction |*/ Qt::MoveAction;
    }

}
