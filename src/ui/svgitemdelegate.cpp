#include "svgitemdelegate.h"
#include "svgeditor.h"
#include <QtGui>
#include <QSvgRenderer>

namespace Melampig
{
    SvgItemDelegate::SvgItemDelegate(QObject *parent)
        : QItemDelegate(parent)
    {
    }

    QWidget *SvgItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/ ) const
    {
        SvgEditor *editor = new SvgEditor(parent);
        editor->installEventFilter( const_cast<SvgItemDelegate*>(this) );
        editor->focusWidget();
        return editor;
    }

    void SvgItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
    {
        SvgEditor *edit = static_cast<SvgEditor*>(editor);
        edit->setData( index.model()->data(index, Qt::EditRole).toByteArray() );
    }

    void SvgItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /*index*/ ) const
    {
        SvgEditor *edit = static_cast<SvgEditor*>(editor);
        QRect r(option.rect.x(), option.rect.y(),
                option.rect.width(), editor->height());
         edit->setGeometry(r);
    }

    void SvgItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
    {
        SvgEditor *edit = static_cast<SvgEditor*>(editor);
        model->setData(index, edit->data(), Qt::EditRole );
    }

}


