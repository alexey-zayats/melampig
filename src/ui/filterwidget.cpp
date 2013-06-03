#include "filterwidget.h"
#include "objectselector.h"

#include "mainwindow.h"

namespace Melampig
{
    FilterWidget::FilterWidget(TQueryMap opts, QList<Attr*> list, Keeper *keeper, QWidget *parent) :
        QWidget(parent), keeper(keeper), attrList(list), parentWidget(parent), opts(opts)
    {
        innerWidget = new QWidget(this);

        scrollArea = new QScrollArea(this);
        scrollArea->setWidget( innerWidget );
        scrollArea->setWidgetResizable( true );

		innerLayout = new QGridLayout( innerWidget );
		innerLayout->setSpacing(2);
		innerLayout->setMargin(2);

		QWidget *w = 0;
		int row = 0;
		for( int i = 0; i < attrList.size(); i++ )
		{
			QApplication::processEvents();

			Attr *a = list.at(i);

			w = getAttrWidget( a, innerWidget );
			if ( !w ) continue;

            QCheckBox *box = new QCheckBox( a->getTitle(), innerWidget);

            if ( opts.contains(a->getName()) ) {
                box->setChecked(true);
                setWidgetData(a, w, opts[a->getName()]);
            } else {
                w->setEnabled(false);
            }
			widgetMap.insert( a->getName(), w );


			connect(box, SIGNAL(toggled(bool)), w, SLOT(setEnabled(bool)));

			innerLayout->addWidget( box, row++, 0 );
			innerLayout->addWidget( w, row++, 0);
		}

		filterButton = new QPushButton(tr("Filter"), innerWidget);
		filterButton->setIcon(QIcon(":/icons/magnifier.png"));

		clearButton = new QPushButton(tr("Clear"), innerWidget);
		clearButton->setIcon(QIcon(":/icons/cancel.png"));

		connect(filterButton, SIGNAL(clicked()), this, SLOT(filterButton_clicked()));
		connect(clearButton, SIGNAL(clicked()), this, SLOT(clearButton_clicked()));

		QBoxLayout *hlayout = new QBoxLayout(QBoxLayout::LeftToRight);
		hlayout->addWidget(filterButton);
		hlayout->addWidget(clearButton);
		hlayout->addItem(new QSpacerItem(10, 10 ));

		innerLayout->addLayout(hlayout, row++, 0);
		innerLayout->addItem( new QSpacerItem(20, 1200, QSizePolicy::Maximum, QSizePolicy::Maximum), row, 0);

        QGridLayout *glayout = new QGridLayout(this);
        glayout->addWidget(scrollArea,  0, 0);

		glayout->setSpacing(2);
		glayout->setMargin(2);

        setLayout(glayout);
    }

    FilterWidget::~FilterWidget()
    {
    }

    void FilterWidget::clearButton_clicked()
    {
        TQueryMap opts;
        emit filterActivated(opts);
    }

    void FilterWidget::filterButton_clicked()
    {
        TQueryMap opts;

		Attr *a = 0;
		for( int i = 0; i < attrList.size(); i++ )
		{
			QApplication::processEvents();

			a = attrList.at(i);

			if ( widgetMap.contains( a->getName() )  )
			{
				QWidget *w = widgetMap[a->getName() ];
				if ( false == w->isEnabled() )
					continue;

				QString v = getWidgetData( a, w );
				if ( !v.isEmpty() ) {

                    int flag = Like | Case;

					bool ok;
					v.toInt(&ok, 10);

					if ( ok ) {
						flag = Equal;
					}

                    if (a->getType() == Attr::Bool) {
                        flag = Equal;
                    }

					if ( opts.size() > 0 ) {
						flag |= And;
					}

					opts.insert(a->getName(),keeper->prepareParam(flag, v));
				}
			}
		}

        emit filterActivated(opts);
    }

    void FilterWidget::setWidgetData( Attr *a, QWidget *editor, QVariant value )
    {
        QList<QVariant> list = value.toList();
        QList<QVariant>::Iterator it = list.begin();
        *it++;

        switch( a->getType() )
        {
            case Attr::Int:
                ((QSpinBox*)editor)->setValue((*it).toInt());
                break;
            case Attr::Double:
                ((QDoubleSpinBox*)editor)->setValue((*it).toDouble());
                break;
            case Attr::String:
            case Attr::Password:
            case Attr::Email:
                ((QLineEdit*)editor)->setText((*it).toString());
                break;
            case Attr::Bool:
                ((QCheckBox*)editor)->setChecked((*it).toBool());
                break;
            case Attr::Date:
                ((QDateEdit*)editor)->setDate( (*it).toDate() );
                break;
            case Attr::Time:
                ((QTimeEdit*)editor)->setTime( (*it).toTime() );
                break;
            case Attr::DateTime:
                ((QDateTimeEdit*)editor)->setDateTime( (*it).toDateTime() );
                break;
            case Attr::ObjectRef:
            {
                ((ObjectSelector*)editor)->setReference( (*it).toString() );
                break;
            }
            default:
                break;
        }
    }

	QString FilterWidget::getWidgetData( Attr *a, QWidget *editor )
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
			case Attr::Bool:
				out = ((QCheckBox*)editor)->isChecked() ? "true" : "false";
				break;
			case Attr::Date:
				out = ((QDateEdit*)editor)->date() != QDate(0,0,0) ?
						((QDateEdit*)editor)->date().toString("yyyy-MM-dd" ) :
							QString();
				break;
			case Attr::Time:
				out = ((QTimeEdit*)editor)->time() != QTime(0,0) ?
							((QTimeEdit*)editor)->time().toString("hh:mm:ss" ) :
							QString();
				break;
			case Attr::DateTime:
				out = ((QDateTimeEdit*)editor)->dateTime() != QDateTime(QDate(0,0,0),QTime(0,0)) ?
							((QDateTimeEdit*)editor)->dateTime().toString( "yyyy-MM-ddThh:mm:ss" ) :
							QString();
				break;
			case Attr::ObjectRef: {
				out = ((ObjectSelector*)editor)->value();
				QStringList list = Object::toList(out);
				if ( list.size() == 0 )
					out = QString();
				break;
			 }
			default:
				out = QString();
		}
		return out;
	}

	QWidget *FilterWidget::getAttrWidget( Attr *a, QWidget *parent )
	{
		QWidget *w = 0;
		switch( a->getType() )
		{
			case Attr::Int:
				w = new QSpinBox(parent);
				((QSpinBox*)w)->setMinimum( -2147483647 );
				((QSpinBox*)w)->setMaximum( 2147483647 );
				((QSpinBox*)w)->setSingleStep( 1 );
				((QSpinBox*)w)->setReadOnly ( a->isReadOnly() );
				break;
			case Attr::Double:
				w = new QDoubleSpinBox(parent);
				w->setMinimumHeight( 20 );
				break;
			case Attr::String:
			case Attr::LTree:
				w = new QLineEdit(parent);
				w->setMinimumHeight( 20 );
				break;
			case Attr::Password:
				w = new QLineEdit(parent);
				((QLineEdit*)w)->setEchoMode( QLineEdit::Password );
				w->setMinimumHeight( 20 );
				break;
			case Attr::Email: // FIXME: NEED VALIDATOR FOR EMAIL!
				w = new QLineEdit(parent);
				w->setMinimumHeight( 20 );
				break;
			case Attr::Bool:
				w = new QCheckBox(parent);
				w->setMinimumHeight( 20 );
				break;
			case Attr::Date:
				w = new QDateEdit(QDate(), parent);
				((QDateEdit*)w)->setCalendarPopup ( true );
				w->setMinimumHeight( 20 );
				break;
			case Attr::Time:
				w = new QTimeEdit(QTime(), parent);
				w->setMinimumHeight( 20 );
				break;
			case Attr::DateTime:
				w = new QDateTimeEdit(QDateTime(), parent);
				((QDateTimeEdit*)w)->setCalendarPopup( true );
				w->setMinimumHeight( 20 );
				break;
			case Attr::ObjectRef: {
				w = new ObjectSelector(a, keeper, this);
				((ObjectSelector*)w)->setParentWidget(parentWidget);
				w->setMinimumHeight( a->isArray() ? 100 : 20 );
				break;
			}
			default:
				w = new QLabel( QString("not implemented editor for: %1").
						arg( a->getType() ), parent);
				break;
		}
		return w;
	}
}
