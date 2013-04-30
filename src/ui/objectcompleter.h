#ifndef OBJECTCOMPLETER_H
#define OBJECTCOMPLETER_H

#include <QLineEdit>
#include <QModelIndex>
#include <orm.h>

class QAbstractItemModel;
class QCompleter;
class QToolButton;
class QWorkspace;

namespace Melampig
{
    class MainWindow;
    class ReferenceDialog;

    class MELAMPIG_UI_EXPORT ObjectCompleter : public QLineEdit
    {
            Q_OBJECT
        public:
            ObjectCompleter(Attr *a, Keeper *keeper, QWidget *pw, QWidget *parent = 0);
            ~ObjectCompleter();
            int getValue();

        public slots:
            void setCompletedIndex(QModelIndex);
            void setModelIndex(QModelIndex);
            void findReference();
            void clearReference();
            void setReferenceData(Object*);
            void objectForm_saved(int);

        protected:
            void resizeEvent(QResizeEvent *);
            bool event(QEvent *event);

        private:
            Attr *attr;
            Keeper *keeper;
            Object *obj;

            QAbstractItemModel *model;
            QCompleter *completer;

            QToolButton *findButton;
            QToolButton *clearButton;

            QWidget *parentWidget;

            MainWindow *mainWindow;
            QWorkspace *workspace;

            QString data;
            int id;

//            ReferenceDialog *dialog;
    };
}
#endif // OBJECTCOMPLETER_H
