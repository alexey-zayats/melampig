#ifndef MELAMPIG_OBJECTSELECTOR_H
#define MELAMPIG_OBJECTSELECTOR_H

#include <QWidget>
#include <orm.h>

class QLineEdit;
class QToolButton;
class QBoxLayout;
class QListWidget;
class QWorkspace;

namespace Melampig
{
    class MainWindow;
    class MELAMPIG_UI_EXPORT ObjectSelector : public QWidget
    {
        Q_OBJECT
    public:
        ObjectSelector(Attr *a, Keeper *keeper, QWidget *parent = 0);
        ~ObjectSelector();

        bool isArray() { return attr->isArray(); }
        QString value();

    signals:
        void objectSelected(Object *);

    public slots:
        void setReference(const QString &);
        void addReference(const QString &id);

        void findReference();
        void removeReference();
        void clearReferences();

        void lineEdit_textChanged(QString);
        void referenceSelected(Object*);
        void setParentWidget(QWidget*);

    private:
        QList<Object*>ref;

        Attr *attr;
        Keeper *keeper;

        QLineEdit *lineEdit;
        QToolButton *findButton;
        QToolButton *clearButton;

        QListWidget *listWidget;
        QToolButton *deleteButton;
        QWidget *parentWidget;

        MainWindow *mainWindow;
        QWorkspace *workspace;
    };

}

#endif // MELAMPIG_OBJECTSELECTOR_H
