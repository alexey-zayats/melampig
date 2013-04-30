#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <orm.h>
#include <QtGui>

namespace Melampig
{
    class MELAMPIG_UI_EXPORT FilterWidget : public QWidget
    {
        Q_OBJECT
    public:
        FilterWidget(TQueryMap opts, QList<Attr*> list, Keeper *keeper, QWidget *parent = 0);
        ~FilterWidget();

        QWidget *getAttrWidget( Attr *a, QWidget *parent );
        QString getWidgetData( Attr *a, QWidget *editor );
        void setWidgetData( Attr *a, QWidget *editor, QVariant value );

    signals:
        void filterActivated(const TQueryMap &);

    public slots:
        void filterButton_clicked();
        void clearButton_clicked();

    private:
        Keeper *keeper;
        QList<Attr*> attrList;
        QWidget *innerWidget;
        QScrollArea *scrollArea;
        QGridLayout *innerLayout;
        QHash<QString, QWidget*> widgetMap;
        QWidget *parentWidget;

        QPushButton *filterButton;
        QPushButton *clearButton;

        TQueryMap opts;
    };

}
#endif // FILTERWIDGET_H
