#include <QtGui>

#include "treemodel.h"
#include "keeper.h"

#include "svgitemdelegate.h"
#include "dateitemdelegate.h"
#include "datetimeitemdelegate.h"
#include "objectrefitemdelegate.h"
#include "integeritemdelegate.h"

namespace Melampig
{
    TreeModel::TreeModel( Tree *item, Keeper *keeper, QObject *oparent )
        : QAbstractItemModel(oparent), keeper(keeper), rootItem(item), parentWidget((QWidget*)oparent)
    {

        rootItem->populateChilds();

        QStringList views = rootItem->getViewItem();
        for( int i = 0; i < views.size(); i++ ) {
            attr.append( rootItem->attribute(views.at(i)) );
        }

        reset();
    }

    TreeModel::~TreeModel()
    {

    }

    int TreeModel::columnCount(const QModelIndex &/*parent*/) const
    {

        return attr.size();
    }

    QVariant TreeModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        Tree *item = static_cast<Tree*>(index.internalPointer());

        int col = index.column();
        if ( col < 0 || col >= attr.size() )
            return QVariant();

        Attr *a = attr[col];
        if ( a == 0 )
            return QVariant();

        QString value = item->get( a->getName() );

        switch (role)
        {
            case Qt::DisplayRole:
            {
                if ( a->getType() == Attr::DateTime ) {
                    return QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
                } else if ( a->getType() == Attr::Svg ) {
                    return value.isEmpty() ? QVariant() : QString("[x]");
                } else {
                    return value;
                };
                break;
            }
            case Qt::EditRole: {
                if ( a->getType() == Attr::DateTime )
                    return QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
                else
                    return value;
                break;
            }
            case Qt::CheckStateRole:
                if ( a->getType() == Attr::Bool )
                    return value.compare("true") == 0 ? Qt::Checked : Qt::Unchecked;
                break;
            case Qt::UserRole:
                return item->get( "id" );
            default:
                break;
        }
        return QVariant();
    }

    Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
    {


        if (!index.isValid())
            return Qt::ItemIsEnabled;

        Qt::ItemFlags flags = Qt::ItemIsEnabled |  Qt::ItemIsSelectable
                | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

        int col = index.column();

        if ( attr[col]->isReadOnly() == false ) {
            flags |= Qt::ItemIsEditable;
        };

        if ( attr[col]->isVisible() == true ) {
            flags |= Qt::ItemIsEnabled;
        };

        if ( attr[col]->getType() == Attr::Bool ) {
            flags |= Qt::ItemIsUserCheckable;
            flags |= Qt::ItemIsTristate;
        };

        if ( attr[col]->getType() == Attr::Svg ) {
            flags ^= Qt::ItemIsEditable;
            flags ^= Qt::ItemIsEnabled;
        }


        return flags;
    }


    QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {


        if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
            return attr[section]->getTitle();

        return QAbstractItemModel::headerData( section, orientation, role );
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
    {


        if (parent.isValid() && parent.column() != 0)
                 return QModelIndex();

        Tree *parentItem;

        if (!parent.isValid())
               parentItem = rootItem;
           else
               parentItem = static_cast<Tree*>(parent.internalPointer());

        Tree *childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();
    }

    bool TreeModel::insertRows(int position, int, const QModelIndex &parent)
    {


        beginInsertRows(parent, position, position);

        Tree *parentItem;

        if (!parent.isValid())
            parentItem = rootItem;
        else
            parentItem = static_cast<Tree*>(parent.internalPointer());

        if (position < 0 || position > parentItem->childCount())
            return false;

        parentItem->insertChild(position);

        endInsertRows();

        emit layoutChanged();

        return true;
    }

    QModelIndex TreeModel::parent(const QModelIndex &index) const
    {


        if (!index.isValid())
            return QModelIndex();

        Tree *childItem = static_cast<Tree*>(index.internalPointer());
        Tree *parentItem = childItem->parent();

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->childIndex(childItem), 0, parentItem);
    }

    bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
    {


        Tree *parentItem;

        if (!parent.isValid())
            parentItem = rootItem;
        else
            parentItem = static_cast<Tree*>(parent.internalPointer());

        if (position < 0 || position > parentItem->childCount())
            return false;

        beginRemoveRows(parent, position, position + rows - 1);

        for (int row = 0; row < rows; ++row) {
            parentItem->deleteChild(position);
        }

        endRemoveRows();
        return true;
    }

    int TreeModel::rowCount(const QModelIndex &parent) const
    {


        Tree *parentItem;

        if (!parent.isValid())
            parentItem = rootItem;
        else
            parentItem = static_cast<Tree*>(parent.internalPointer());

        return parentItem->childCount();
    }


    bool TreeModel::setData(const QModelIndex &index,
                            const QVariant &value, int role)
    {


        if (!index.isValid() || role != Qt::EditRole)
            return false;

        Tree *item = static_cast<Tree*>(index.internalPointer());
        Attr *a = attr[index.column()];

        if( role == Qt::CheckStateRole ) {
            bool isChecked = ( value.toInt() == Qt::Checked );
            item->set(a->getName(), QString(isChecked ? "true":"false"));
        } else {
            item->set(a->getName(), value.toString());
        }

        try
        {
            item->store();
        } catch ( SqlError &err ) {
            QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        emit dataChanged(index, index);

        return true;
    }

    Object *TreeModel::getObject(const QModelIndex &index)
    {


        if (!index.isValid())
            return 0;

        return static_cast<Object*>(index.internalPointer());
    }

    QItemDelegate *TreeModel::getItemDelegate(int idx)
    {


        Attr *a = attr.at(idx);
        switch(a->getType())
        {
        case Attr::ObjectRef: {
            ObjectRefItemDelegate *w = new ObjectRefItemDelegate(a, keeper, this);
            w->setParentWidget(parentWidget);
            return w;
        }
        case Attr::DateTime:
            return new DateTimeItemDelegate(this);
        case Attr::Date:
            return new DateItemDelegate(this);
        case Attr::Svg:
            return new SvgItemDelegate(this);
        case Attr::Int:
            return new IntegerItemDelegate(this);
        default:
            return new QItemDelegate(this);
        }
    }

    void TreeModel::changeData( const QModelIndex &from, const QModelIndex &to )
    {


        Tree *item = static_cast<Tree*>(from.internalPointer());
        if ( item ) {
            item->restore( item->get("id").toInt() );
            emit dataChanged(from, to);
        }
    }
}

