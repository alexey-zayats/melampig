#include "tableitemdelegate.h"

namespace Melampig
{
    TableItemDelegate::TableItemDelegate(Object *o, QObject *parent) :
        QItemDelegate(parent), obj(o)
    {
    }

}
