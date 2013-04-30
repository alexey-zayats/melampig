#include "treeitemdelegate.h"

namespace Melampig
{
    TreeItemDelegate::TreeItemDelegate(Object *o, QObject *parent) :
        QItemDelegate(parent), obj(o)
    {
    }
}
