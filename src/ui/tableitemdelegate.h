#ifndef TABLEITEMDELEGATE_H
#define TABLEITEMDELEGATE_H

#include <QItemDelegate>
#include <orm.h>

namespace Melampig
{
    class MELAMPIG_UI_EXPORT TableItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        TableItemDelegate(Object *o, QObject *parent = 0);

    signals:

    public slots:
    private:
        Object *obj;

    };
}

#endif // TABLEITEMDELEGATE_H
