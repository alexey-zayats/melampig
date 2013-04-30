#ifndef MELAMPIG_DRAGDROPMODEL_H
#define MELAMPIG_DRAGDROPMODEL_H

#include "treemodel.h"

namespace Melampig
{

    class MELAMPIG_UI_EXPORT DragDropModel : public TreeModel
    {
        Q_OBJECT

    public:
        DragDropModel(Tree *item, Keeper *keeper, QObject *parent = 0);

        Qt::ItemFlags flags(const QModelIndex &index) const;

        bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                          int row, int column, const QModelIndex &parent);
        QMimeData *mimeData(const QModelIndexList &indexes) const;
        QStringList mimeTypes() const;
        Qt::DropActions supportedDropActions() const;
    private:
        Keeper *keeper;
    };

}

#endif // MELAMPIG_DRAGDROPMODEL_H
