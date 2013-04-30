#ifndef INTEGERITEMDELEGATE_H
#define INTEGERITEMDELEGATE_H

#include <QItemDelegate>
#include <orm.h>

namespace Melampig
{
    class MELAMPIG_UI_EXPORT IntegerItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        IntegerItemDelegate(QObject *parent = 0);

        QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
        void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

    signals:

    public slots:

    };
}
#endif // INTEGERITEMDELEGATE_H
