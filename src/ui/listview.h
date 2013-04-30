#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>
#include <orm.h>

namespace Melampig
{

class MELAMPIG_UI_EXPORT ListView : public QListView
{
    Q_OBJECT
public:
    ListView(QWidget *parent = 0);

signals:

public slots:

};

}

#endif // LISTVIEW_H
