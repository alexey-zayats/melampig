#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include <QAbstractItemModel>
#include <QAbstractItemView>

#include <QDialog>
#include <orm.h>

class QBoxLayout;
class QPushButton;
class QMainWindow;

namespace Melampig
{

    class MELAMPIG_UI_EXPORT ReferenceDialog : public QDialog
    {
        Q_OBJECT
    public:
        ReferenceDialog(Attr *a, Keeper * keeper, QWidget *parent = 0);
        ~ReferenceDialog();

        Object *getObject();

    public slots:
        void updateActions();

    signals:
        void itemSelected(Object*);

    private:
        Attr *attr;
        Object *ref;
        Keeper *keeper;

        QMainWindow *widget;

        QPushButton *okButton;
        QPushButton *cancelButton;
        QBoxLayout *vbox;
        QBoxLayout *hbox;
    };

}

#endif // REFERENCEDIALOG_H
