#ifndef MELAMPIG_REFERENCE_SELECTOR_H
#define MELAMPIG_REFERENCE_SELECTOR_H

#include <QAbstractItemModel>
#include <QAbstractItemView>

#include <QMainWindow>
#include <orm.h>

class QBoxLayout;
class QPushButton;

namespace Melampig
{

    class MELAMPIG_UI_EXPORT ReferenceSelector : public QMainWindow
    {
        Q_OBJECT
    public:
        ReferenceSelector(Attr *a, Keeper * keeper, QWidget *parent = 0);
        ~ReferenceSelector();

    public slots:
        void updateActions();
        void okButtonClicked();

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

#endif // MELAMPIG_REFERENCE_SELECTOR_H
