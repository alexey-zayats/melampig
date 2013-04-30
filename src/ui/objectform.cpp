#include "objectform.h"
#include "objectselector.h"
#include "svgeditor.h"
#include "imageeditor.h"
#include "intarrayeditor.h"

#include <QtGui>

namespace Melampig
{
    ObjectForm::ObjectForm(Object *obj, Keeper *keeper, QWidget *parent) :
        QMainWindow(parent),
        keeper(keeper),
        parentWidget(parent)
    {
        object = keeper->objectByType(obj->getType());
        object->restore(obj->get("id").toInt());

        setObjectName( object->objectName() + object->get("id") );

        actionBar = addToolBar(tr("Actions"));
        saveAction = actionBar->addAction( QIcon( ":/icons/disk.png" ), tr("Save") );
        connect(saveAction, SIGNAL(triggered()), this, SLOT(saveObject()));

        innerWidget = new QWidget(this);
        scrollArea = new QScrollArea();

        scrollArea->setWidget( innerWidget );
        scrollArea->setWidgetResizable( true );

		innerLayout = new QGridLayout( innerWidget );
		innerLayout->setSpacing(2);
		innerLayout->setMargin(2);

		setCentralWidget(scrollArea);

		QStringList list = object->nameSchema();

		QWidget *w = 0;
		QLabel *l = 0;

		int row = 0;
		for( int i = 0; i < list.size(); i++ )
		{
			QApplication::processEvents();

			Attr *a = object->attribute( list.at(i) );
			if ( !a->isVisible() ) continue;

			l = new QLabel( innerWidget );
			l->setText( QString("<b>%1</b> [%2]").arg( a->getTitle() ).arg( a->getName() ) );
			l->setAlignment( Qt::AlignTop | Qt::AlignLeft );

			innerLayout->addWidget( l, row, 0 );

			w = this->getAttrWidget( a, innerWidget );
			if ( w )
			{
				innerLayout->addWidget( w, row, 1 );
				widgetMap.insert( a->getName(), w );
			}
			innerLayout->addItem( new QSpacerItem(10, 10 ), row++, 0 );
		}

		innerLayout->addItem( new QSpacerItem(20, 1200, QSizePolicy::Maximum, QSizePolicy::Maximum), row, 0, 1, 2 );
	}

    ObjectForm::~ObjectForm()
    {
        delete object;
    }

	void ObjectForm::saveObject()
	{
		QStringList list = object->nameSchema();

		Attr *a = 0;
		for( int i = 0; i < list.size(); i++ )
		{
			QApplication::processEvents();

			a = object->attribute( list.at(i) );

			if ( !a->isVisible() ) continue;
			if ( a->isReadOnly() ) continue;

			if ( widgetMap.contains( a->getName() )  )
				object->set( a->getName(), getWidgetData( a, widgetMap[a->getName() ] ) );
		}

		try
		{
			object->store();

			emit objectSaved(object->get("id").toInt());

			object->restore( object->get("id").toInt() );

			for( int i = 0; i < list.size(); i++ )
			{
				a = object->attribute( list.at(i) );
				if ( !a->isVisible() ) continue;

				if ( widgetMap.contains( a->getName() )  )
					setWidgetData( a, widgetMap[a->getName()] );
			}
		}
		catch ( SqlError &er  )
		{
			QMessageBox::critical( this, tr("Error"), er.getText() + "\n" + er.getQuery() );
		}
	}

	QWidget *ObjectForm::getAttrWidget( Attr *a, QWidget *parent )
	{
		QWidget *w = 0;
		switch( a->getType() )
		{
			case Attr::Int:
				w = new QSpinBox(parent);
				((QSpinBox*)w)->setMinimum( -2147483647 );
				((QSpinBox*)w)->setMaximum( 2147483647 );
				((QSpinBox*)w)->setSingleStep( 1 );
				((QSpinBox*)w)->setValue( object->get( a->getName() ).toInt() );
				((QSpinBox*)w)->setReadOnly ( a->isReadOnly() );
				w->setMinimumHeight( 20 );
				break;
			case Attr::Double:
				w = new QDoubleSpinBox(parent);
				((QDoubleSpinBox*)w)->setValue( object->get( a->getName() ).toDouble() );
				((QDoubleSpinBox*)w)->setReadOnly( a->isReadOnly() );
				w->setMinimumHeight( 20 );
				break;
			case Attr::String:
			case Attr::LTree:
				w = new QLineEdit(parent);
				((QLineEdit*)w)->setText( object->get( a->getName() ) );
				((QLineEdit*)w)->setReadOnly( a->isReadOnly() );
				w->setMinimumHeight( 20 );
				break;
			case Attr::Text:
				w = new QTextEdit( parent );
				((QTextEdit*)w)->setPlainText( object->get( a->getName() ) );
				w->setMinimumHeight(400);
				w->setEnabled( !a->isReadOnly() );
				break;
			case Attr::Html:
				w = new QTextEdit( parent );
				((QTextEdit*)w)->setHtml( object->get( a->getName() ) );
				w->setMinimumHeight(500);
				w->setEnabled ( !a->isReadOnly() );
				break;
			case Attr::Password:
				w = new QLineEdit(parent);
				((QLineEdit*)w)->setText( object->get( a->getName() ) );
				((QLineEdit*)w)->setEchoMode( QLineEdit::Password );
				w->setMinimumHeight( 20 );
				((QLineEdit*)w)->setReadOnly( a->isReadOnly() );
				break;
			case Attr::Email: // FIXME: NEED VALIDATOR FOR EMAIL!
				w = new QLineEdit(parent);
				((QLineEdit*)w)->setText( object->get( a->getName() ) );
				w->setMinimumHeight( 20 );
				((QLineEdit*)w)->setReadOnly( a->isReadOnly() );
				break;
			case Attr::Bool:
				w = new QCheckBox(parent);
				((QCheckBox*)w)->setChecked( object->get( a->getName() ).compare("true") == 0  );
				w->setMinimumHeight( 20 );
				((QCheckBox*)w)->setCheckable( !a->isReadOnly() );
				break;
			case Attr::Date:
				w = new QDateEdit(parent);
				((QDateEdit*)w)->setCalendarPopup ( true );
				if ( object->get( a->getName() ).isEmpty() ) {
					((QDateEdit*)w)->setDate( QDate::currentDate() );
				} else {
					((QDateEdit*)w)->setDate( QDate::fromString( object->get( a->getName() ), "yyyy-MM-dd" ) );
				}
				w->setMinimumHeight( 20 );
				((QDateEdit*)w)->setReadOnly ( a->isReadOnly() );
				break;
			case Attr::Time:
				w = new QTimeEdit(parent);
				if ( object->get( a->getName() ).isEmpty() ) {
					((QTimeEdit*)w)->setTime( QTime::currentTime() );
				} else {
					((QTimeEdit*)w)->setTime( QTime::fromString( object->get( a->getName() ), "hh:mm:ss" ) );
				}
				w->setMinimumHeight( 20 );
				((QTimeEdit*)w)->setReadOnly ( a->isReadOnly() );
				break;
			case Attr::DateTime:
				w = new QDateTimeEdit(parent);
				((QDateTimeEdit*)w)->setCalendarPopup( true );
				if ( object->get( a->getName() ).isEmpty() ) {
					((QDateTimeEdit*)w)->setDateTime( QDateTime::currentDateTime() );
				} else {
					((QDateTimeEdit*)w)->setDateTime( QDateTime::fromString( object->get( a->getName() ), "yyyy-MM-ddThh:mm:ss" ) );
				}
				w->setMinimumHeight( 20 );
				((QDateTimeEdit*)w)->setReadOnly ( a->isReadOnly() );
				break;
			case Attr::ObjectRef: {
				w = new ObjectSelector(a, keeper, parent);
				((ObjectSelector*)w)->setParentWidget(parentWidget);
				if ( !object->get(a->getName()).isEmpty() )
					((ObjectSelector*)w)->setReference(object->get(a->getName()));
				w->setMinimumHeight( a->isArray() ? 100 : 20 );
				break;
			}
			case Attr::Svg: {
				w = new SvgEditor(parentWidget);
				QByteArray svgData;
				svgData += object->get(a->getName());
				((SvgEditor*)w)->setData(QByteArray::fromBase64(svgData));
				w->setMinimumHeight( 100 );
				break;
			}
			case Attr::Image: {
				w = new ImageEditor(parentWidget);
				QByteArray imageData;
				imageData += object->get(a->getName());
				((ImageEditor*)w)->setData(QByteArray::fromBase64(imageData));
				w->setMinimumHeight( 100 );
				break;
			}
            case Attr::IntArray:
                w = new IntArrayEditor(parentWidget);
                ((IntArrayEditor*)w)->setData( Object::toList(object->get(a->getName())) );
                w->setMinimumHeight( 100 );
                break;
			default:
				w = new QLabel( QString("not implemented editor for: %1").
						arg( a->getType() ), parent);
				break;
		}
		return w;
	}

	void ObjectForm::setWidgetData( Attr *a, QWidget *w )
	{
		switch( a->getType() )
		{
			case Attr::Int:
				((QSpinBox*)w)->setValue( object->get( a->getName() ).toInt() );
				break;
			case Attr::Double:
				((QDoubleSpinBox*)w)->setValue( object->get( a->getName() ).toDouble() );
				break;
			case Attr::String:
			case Attr::Password:
			case Attr::Email:
				((QLineEdit*)w)->setText( object->get( a->getName() ) );
				break;
			case Attr::Text:
				((QTextEdit*)w)->setText( object->get( a->getName() ) );
				break;
			case Attr::Html:
				((QTextEdit*)w)->setHtml( object->get( a->getName() ) );
				break;
			case Attr::Bool:
				((QCheckBox*)w)->setChecked(
							object->get( a->getName() ).compare("true") == 0  );
				break;
			case Attr::Date:
				((QDateEdit*)w)->setDate(
							QDate::fromString( object->get( a->getName() ),
							"yyyy-MM-dd" ) );
				break;
			case Attr::Time:
				((QTimeEdit*)w)->setTime(
							QTime::fromString( object->get( a->getName() ),
							"hh:mm:ss" ) );
				break;
			case Attr::DateTime:
				((QDateTimeEdit*)w)->setDateTime(
							QDateTime::fromString( object->get( a->getName() ),
							"yyyy-MM-ddThh:mm:ss" ) );
				break;
			case Attr::ObjectRef:
				((ObjectSelector*)w)->setReference( object->get( a->getName() ) );
				break;
			case Attr::Svg: {
				QByteArray svgData;
				svgData += object->get( a->getName() );
				((SvgEditor*)w)->setData(QByteArray::fromBase64(svgData));
				break;
			}
			case Attr::Image: {
				QByteArray imageData;
				imageData += object->get( a->getName() );
				((ImageEditor*)w)->setData(QByteArray::fromBase64(imageData));
				break;
			}
            case Attr::IntArray:
                ((IntArrayEditor*)w)->setData( Object::toList(object->get(a->getName())) );
                break;
			default:
				break;
		}
	}

	QString ObjectForm::getWidgetData( Attr *a, QWidget *editor )
	{
		QString out;
		switch( a->getType() )
		{
			case Attr::Int:
				out = ((QSpinBox*)editor)->text();
				break;
			case Attr::Double:
				out = QString::number( ((QDoubleSpinBox*)editor)->value(), 'f' );
				break;
			case Attr::String:
			case Attr::Password:
			case Attr::Email:
				out = ((QLineEdit*)editor)->text();
				break;
			case Attr::Text:
				out = ((QTextEdit*)editor)->toPlainText();
				break;
			case Attr::Html:
				out = ((QTextEdit*)editor)->toHtml();
				break;
			case Attr::Bool:
				out = ((QCheckBox*)editor)->isChecked() ? "true" : "false";
				break;
			case Attr::Date:
				out = ((QDateEdit*)editor)->date().toString("yyyy-MM-dd" );
				break;
			case Attr::Time:
				out = ((QTimeEdit*)editor)->time().toString("hh:mm:ss" );
				break;
			case Attr::DateTime:
				out = ((QDateTimeEdit*)editor)->dateTime().toString( "yyyy-MM-ddThh:mm:ss" );
				break;
			case Attr::ObjectRef:
				out = ((ObjectSelector*)editor)->value();
				break;
			case Attr::Svg:
				out = ((SvgEditor*)editor)->data().toBase64();
				break;
			case Attr::Image:
				out = ((ImageEditor*)editor)->data().toBase64();
				break;
            case Attr::IntArray:
                out = Object::toArray( ((IntArrayEditor*)editor)->data() );
                break;
			default:
				out = QString("");
		}
		return out;
	}
}
