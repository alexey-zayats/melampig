#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QItemDelegate>
#include <orm.h>

namespace Melampig
{
    class MELAMPIG_UI_EXPORT TreeItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        TreeItemDelegate(Object *obj, QObject *parent = 0);

    signals:

    public slots:
    private:
        Object *obj;

    };
}

#endif // TREEITEMDELEGATE_H
