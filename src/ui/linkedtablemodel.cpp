#include "linkedtablemodel.h"
#include "mainwindow.h"

#include "referencedialog.h"

#include "svgitemdelegate.h"
#include "dateitemdelegate.h"
#include "datetimeitemdelegate.h"
#include "objectrefitemdelegate.h"
#include "integeritemdelegate.h"

#include <QSqlQuery>
#include <QSqlDatabase>

namespace Melampig
{
    LinkedTableModel::LinkedTableModel(QList<Object*> &data, Keeper *keeper, QObject *parent)
        : QAbstractTableModel(parent), keeper(keeper), parentWidget((QWidget*)parent)
    {
        oSrc = data.at(0);
        oLink = data.at(1);
        oDst = data.at(2);

        initModel();
        sortColumn = QString("id");
    }

    LinkedTableModel::~LinkedTableModel()
    {
        delete oLink;
        delete oDst;
    }

	void LinkedTableModel::sort ( int column, Qt::SortOrder order )
	{
		sortColumn = attr.at(column)->getName();
		options.insert("order", QStringList() << QString("%1 %2").arg(attr.at(column)->getName()).arg(order == Qt::AscendingOrder ? "asc" : "desc") );

		fetchMore(QModelIndex());

		reset();
	}

	QVariant LinkedTableModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

        int col = index.column();
        if ( col < 0 || col >= attr.size() )
            return QVariant();

        int row = index.row();
        if ( row < 0 || row >= linkRecord.size() )
            return QVariant();

        if ( row < 0 || row >= dstRecord.size() )
            return QVariant();

        Attr *a = attr[col];
        if ( a == 0 )
            return QVariant();

        Object *link = linkRecord[row];
        Object *dst = dstRecord[row];

        QString value;
        if ( link->attribute( a->getName()) ) {
            value = link->get( a->getName() );
        } else {
            value = dst->get( a->getName() );
        }

        switch (role)
        {
        case Qt::DisplayRole:
        {
            if ( a->getType() == Attr::Text ) {
                return value;
            } else if ( a->getType() == Attr::DateTime )
            {
                return QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
            }
            else if ( a->getType() == Attr::ObjectRef )
            {
                QStringList idList;
                if ( a->isArray() )
                    idList << Object::toList(value);
                else
                    idList << value;

                ObjectType type = a->getRef();
                if ( !refs.contains(type) )
                    return value;

                QStringList out;
                QString id;
                foreach (id, idList) {
                    int  i = id.toInt();
                    if ( refs[type].contains(i) )
                        out.append(refs[type][i].toString());
                }
                return out.join(",");
            } else if ( a->getType() == Attr::Svg ) {
                return QString("[SVG]");
            } else {
                return value;
            };
            break;
        }
        case Qt::EditRole: {
            if ( a->getType() == Attr::DateTime )
                return QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
            else if ( a->getType() == Attr::Date )
                return QDate::fromString( value, "yyyy-MM-dd" );
            else if ( a->getType() == Attr::Time )
                return QTime::fromString( value, "HH:mm:ss" );
            else
                return value;
            break;
        }
        case Qt::CheckStateRole:
            if ( a->getType() == Attr::Bool )
                return value.compare("true") == 0 ? Qt::Checked : Qt::Unchecked;
            break;
        default:
            break;
        }
        return QVariant();
    }

	int LinkedTableModel::rowCount(const QModelIndex &/*parent*/) const
	{
		return rows.size();
	}

	int LinkedTableModel::columnCount(const QModelIndex &/*parent*/) const
	{
		return attr.size();
	}

	QVariant LinkedTableModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
			return attr[section]->getTitle();

		return QAbstractTableModel::headerData( section, orientation, role );
	}

	Qt::ItemFlags LinkedTableModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return Qt::ItemIsEnabled;

		Qt::ItemFlags flags = Qt::ItemIsSelectable;

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
        };

		return flags;
	}

	bool LinkedTableModel::setData(const QModelIndex &index, const QVariant &value,
				 int role)
	{
		if (!index.isValid() || role != Qt::EditRole)
			return false;

        int col = index.column();
        if ( col < 0 || col >= attr.size() )
            return false;

        int row = index.row();
        if ( row < 0 || row >= linkRecord.size() )
            return false;

        Attr *a = attr[col];
        if ( a == 0 )
            return false;


        Object *link = linkRecord[row];
        Object *dst = dstRecord[row];

        Object *o = ( link->attribute(a->getName()) ) ? link : dst;
        QString aname = a->getName();

        switch(a->getType())
        {
        case Attr::DateTime:
            o->set(aname, value.toDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
            break;
        case Attr::Date:
            o->set(aname, value.toDate().toString("yyyy-MM-dd"));
            break;
        case Attr::Time:
            o->set(aname, value.toTime().toString("HH:mm:ss"));
            break;
        case Attr::Bool:
            o->set(aname, QString(value.toBool() ? "true" : "false"));
            break;
        default: {
            o->set(aname, a->isArray() ? Object::toArray(value.toStringList()) :  value.toString());
            break;
        }
        }

        try {
            o->store();
        } catch (SqlError &err) {
            QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        rows[index.row()] = o->get("id").toInt();

        emit dataChanged(index, index);

		return true;
	}

	bool LinkedTableModel::insertRows(int position, int count, const QModelIndex &parent)
	{
		beginInsertRows(parent, position, position + count - 1 );


        QString title ( tr("Choose %1 from list").arg( oLink->attribute(dstName)->getTitle() ) );

        MainWindow *mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        QWorkspace *workspace = (QWorkspace*)parentWidget;

		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j )
			if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
				workspace->setActiveWindow(windows.at(j));
				endInsertRows();
				return true;
			}

        ReferenceDialog *dialog = new ReferenceDialog(oLink->attribute(dstName), keeper, parentWidget);
        dialog->setWindowTitle( title );
        dialog->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(dialog);
        dialog->show();

		connect(dialog, SIGNAL(destroyed()), mainWindow, SLOT(closeTab()) );
		connect(dialog, SIGNAL(itemSelected(int,QList<Object*>)), this, SLOT(referenceSelected(int,QList<Object*>)));

		endInsertRows();
		return true;
	}

	void LinkedTableModel::referenceSelected(int position, QList<Object*> value)
	{
		if ( value.size() == 0 ) return;


		Object *o = value.at(0);

		Object *no = keeper->objectByType( oLink->getType() );
		no->set(srcName, oSrc->get("id"));
		no->set(dstName, o->get("id"));

		try
		{
			no->store();

			linkRecord.insert(position, no);
			dstRecord.insert(position, o);

			QHash<ObjectType,QString>::iterator it;
			for( it = refName.begin(); it != refName.end(); it++)
			{
				ObjectType t = it.key();
				QString name = it.value();
				Attr *a = oDst->attribute(name);

				if ( a == 0 )
					continue;

				QStringList id;
				if ( a->isArray() ) {
					id << Object::toList(o->get(name));
				} else {
					id << o->get(name);
				}
				foreach (QString x, id) {
					if ( x.toInt() == 0 ) continue;
					if ( refId.contains(t) ) {
						refId[t].append(x.toInt());
					} else {
						QList<QVariant> a;
						a.append(x.toInt());
						refId.insert(t, a);
					}
				}
			}

			QHash<ObjectType, QList<QVariant> >::const_iterator it1;
			for( it1 = refId.begin(); it1 != refId.end(); ++it1 )
			{
				fillRefMap(it1.key(), it1.value());
			}

			rows.insert(position, no->get("id"));

		} catch (SqlError &err) {
			QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
		}


	}

	bool LinkedTableModel::removeRows(int position, int /*rows*/, const QModelIndex &parent)
	{
		beginRemoveRows(parent, position, position );

        Object *o = dstRecord.takeAt(position);
        delete o;

        o = linkRecord.takeAt(position);
        o->remove();
        delete o;

        endRemoveRows();

		return true;
	}

	void LinkedTableModel::fetchMore ( const QModelIndex & /*parent*/ )
	{

		TQueryMap opt;
		if ( oLink->attribute( sortColumn ) ) {
			opt = options;
		} else {
			opt.insert("order", QStringList() << "id");
		}
		opt.insert(srcName, keeper->prepareParam(Equal, oSrc->get("id")));
		linkRecord = keeper->getObjectList(oLink->getType(), opt);

		if ( linkRecord.size() == 0 ) return;

		opt.clear();

		QList<QVariant> idList;

		Object *o = 0;
		for(int i = 0; i < linkRecord.size(); i++ )
		{
			o = linkRecord.at(i);
			idList.append( o->get(dstName) );

			QHash<ObjectType,QString>::iterator it;
			for( it = refName.begin(); it != refName.end(); it++)
			{
				ObjectType t = it.key();
				QString name = it.value();
				Attr *a = o->attribute(name);
				if ( a == 0 )
					continue;

				QStringList id;
				if ( a->isArray() ) {
					id << Object::toList(o->get(name));
				} else {
					id << o->get(name);
				}
				foreach (QString x, id) {
					if ( x.toInt() == 0 ) continue;
					if ( refId.contains(t) ) {
						refId[t].append(x.toInt());
					} else {
						QList<QVariant> a;
						a.append(x.toInt());
						refId.insert(t, a);
					}
				}
			}
		}

		opt.insert("id", keeper->prepareParam(InSet, idList));

		if ( oDst->attribute( sortColumn ) ) {
			opt = options;
		} else {
			opt.insert("order", QStringList() << "id" );
		}

		dstRecord = keeper->getObjectList(oDst->getType(), opt);

		for(int i = 0; i < dstRecord.size(); i++ )
		{
			o = dstRecord.at(i);
			QHash<ObjectType,QString>::iterator it;
			for( it = refName.begin(); it != refName.end(); it++)
			{
				ObjectType t = it.key();
				QString name = it.value();

				Attr *a = o->attribute(name);
				if ( a == 0 )
					continue;

				QStringList id;
				if ( a->isArray() ) {
					id << Object::toList(o->get(name));
				} else {
					id << o->get(name);
				}
				foreach ( QString x, id) {
					if ( x.toInt() == 0 ) continue;

					if ( refId.contains(t) ) {
						refId[t].append(x.toInt());
					} else {
						QList<QVariant> a;
						a.append(x.toInt());
						refId.insert(t, a);
					}
				}
			}
		}

		QHash<ObjectType, QList<QVariant> >::const_iterator it;
		for( it = refId.begin(); it != refId.end(); ++it )
		{
			fillRefMap(it.key(), it.value());
		}
	}

	void LinkedTableModel::fillRefMap(const ObjectType &type, const QList<QVariant> &idlist)
	{
		QList<QVariant> data;
		for( int i = 0; i < idlist.size(); i++ )
		{
			if ( !data.contains(idlist.at(i))) {
				data.append(idlist.at(i));
			}
		}

		TQueryMap opts;
		opts.insert("id", keeper->prepareParam(InSet, data) );
		opts.insert("order", QStringList() << "id" );

		QStringList flist;
		flist << "id";
		flist << "title";

		QList<QVariant> list = keeper->getListOfFields(type, flist, opts);

		QHash<int, QVariant> map;
		for( int i = 0; i < list.size(); i++) {
			QStringList fd = list.at(i).toStringList();
			map.insert(fd.at(0).toInt(), fd.at(1));
		}


		refs.insert( type, map );
	}

	void LinkedTableModel::initModel()
	{
		qDeleteAll(linkRecord);
		qDeleteAll(dstRecord);

        dstRecord.clear();
        linkRecord.clear();

        attr.clear();
        rows.clear();

		views = oLink->getViewItem();
		setupSchema(oLink);

		linkFieldsCount = views.size();

		views << oDst->getViewItem();
		setupSchema(oDst);

		TQueryMap opt;
		opt.insert(srcName, keeper->prepareParam(Equal, oSrc->get("id")));
		rows = keeper->getFieldList(oLink->getType(), "id", opt);

		fetchMore(QModelIndex());
	}

	void LinkedTableModel::setupSchema(Object *o)
	{
		QList<Attr*> list = o->attrSchema();

		Attr *a = 0;

        for( int i = 0; i < list.size(); i++)
        {
            a = list.at(i);

            QString name = a->getName();

			if ( a->getRef() == oSrc->getType() ) {
				srcName = name;
				views.removeAt(views.indexOf(srcName));
			} else if ( a->getRef() == oDst->getType()) {
				dstName = name;
				views.removeAt(views.indexOf(dstName));
			}


			if ( !views.contains(name) )
				continue;

            if ( a->getType() == Attr::ObjectRef ) {
                refName.insert(a->getRef(), name);
            }
            setHeaderData(i, Qt::Horizontal, a->getTitle() );
            attr.append(a);
        }
    }

    void LinkedTableModel::changeData( const QModelIndex &from, const QModelIndex &to )
    {
        if (!from.isValid()) return;
        if (!to.isValid()) return;

        Object *o = 0;
        if ( from.column() > linkFieldsCount )
            o = dstRecord[from.row()];
        else
            o = linkRecord[from.row()];

        try {
            o->restore( o->get("id").toInt() );
        } catch (SqlError &err) {
            QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

		QHash<ObjectType,QString>::iterator it;
		for( it = refName.begin(); it != refName.end(); it++)
		{
			ObjectType t = it.key();
			QString name = it.value();

			QStringList id;
			Attr *alink = oLink->attribute(name);
			if ( alink ) {
				if ( alink->isArray() ) {
					id << Object::toList(o->get(name));
				} else {
					id << o->get(name);
				}
			}

			Attr *dlink = oDst->attribute(name);
			if ( dlink ) {

				if ( dlink->isArray() ) {
					id << Object::toList(o->get(name));
				} else {
					id << o->get(name);
				}
			}
			foreach (QString x, id) {
				if ( x.toInt() == 0 ) continue;
				if ( refId.contains(t) ) {
					refId[t].append(x.toInt());
				} else {
					QList<QVariant> a;
					a.append(x.toInt());
					refId.insert(t, a);
				}
			}
		}

		QHash<ObjectType, QList<QVariant> >::const_iterator it1;
		for( it1 = refId.begin(); it1 != refId.end(); ++it1 )
		{
			fillRefMap(it1.key(), it1.value());
		}

        emit dataChanged(from, to);
    }


    QItemDelegate *LinkedTableModel::getItemDelegate(int idx)
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
}
