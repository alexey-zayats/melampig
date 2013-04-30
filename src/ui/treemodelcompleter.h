#ifndef TREEMODELCOMPLETER_H
#define TREEMODELCOMPLETER_H

#include <QCompleter>
#include <orm.h>

namespace Melampig
{
    class MELAMPIG_UI_EXPORT TreeModelCompleter : public QCompleter
     {
         Q_OBJECT

     public:
         TreeModelCompleter(QObject *parent = 0);
         TreeModelCompleter(QAbstractItemModel *model, QObject *parent = 0);

         QString separator() const;
     public slots:
         void setSeparator(const QString &separator) { sep = separator; }

     protected:
         QStringList splitPath(const QString &path) const;
         QString pathFromIndex(const QModelIndex &index) const;

     private:
         QString sep;
     };
}

#endif // TREEMODELCOMPLETER_H
