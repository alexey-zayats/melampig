#ifndef SVGITEMDELEGATE_H
#define SVGITEMDELEGATE_H

#include <QItemDelegate>
#include <orm.h>

namespace Melampig
{
    class SvgEditor;
    class MELAMPIG_UI_EXPORT SvgItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        SvgItemDelegate(QObject *parent);

        QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
        void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    };

}

#endif // SVGITEMDELEGATE_H
