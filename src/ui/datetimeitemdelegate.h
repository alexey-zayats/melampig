#ifndef DATETIMEITEMDELEGATE_H
#define DATETIMEITEMDELEGATE_H

#include <QItemDelegate>
#include <orm.h>

namespace Melampig
{

    class MELAMPIG_UI_EXPORT DateTimeItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        DateTimeItemDelegate(QObject *parent = 0);

        QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
        void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

    signals:

    public slots:
    private:

    };

}

#endif // DATETIMEITEMDELEGATE_H
