#ifndef WINNERDIALOG_H
#define WINNERDIALOG_H

#include <QDialog>

namespace Ui {
    class WinnerDialogDB;
}

namespace Melampig
{
    class Object;
    class Keeper;
    class WinnerDialog : public QDialog
    {
        Q_OBJECT

    public:
        WinnerDialog(Object *o, Keeper *k, QWidget *parent = 0);
        ~WinnerDialog();

    public slots:
        void accept();
        void checkOkButton();

    private:
        Ui::WinnerDialogDB *ui;
        Object *object;
        Keeper *keeper;
    };
}

#endif // WINNERDIALOG_H
