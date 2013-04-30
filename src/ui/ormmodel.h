#ifndef ORMMODEL_H
#define ORMMODEL_H

#include <QModelIndex>
#include <QVariant>
#include <QObject>

#include <orm.h>

namespace Melampig
{
    class OrmModel : public QObject
    {
        Q_OBJECT
    public:
        OrmModel(QObject *parent = 0);

    signals:

    public slots:

    };
}

#endif // ORMMODEL_H
