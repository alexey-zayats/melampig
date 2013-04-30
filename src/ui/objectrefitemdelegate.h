#ifndef OBJECTREFITEMDELEGATE_H
#define OBJECTREFITEMDELEGATE_H

#include <QItemDelegate>
#include <orm.h>

class QCompleter;

namespace Melampig
{
    class ObjectSelector;
    class MELAMPIG_UI_EXPORT ObjectRefItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        ObjectRefItemDelegate(Attr *a, Keeper *keeper, QObject *parent = 0);
        ~ObjectRefItemDelegate();

        QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
        void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
        bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index );

    signals:

    public slots:
        void setParentWidget(QWidget*w) { parentWidget = w;}

    private:
        Attr *attr;
        Keeper *keeper;
        QWidget *parentWidget;
    };

}

#endif // OBJECTREFITEMDELEGATE_H
