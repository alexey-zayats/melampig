#ifndef MELAMPIG_SETTINGSFORM_H
#define MELAMPIG_SETTINGSFORM_H

#include <QWidget>

namespace Ui {
class SettingsForm;
}

namespace Melampig
{
class Keeper;

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(Keeper *keeper, QWidget *parent = 0);
    ~SettingsForm();

public slots:
    void useCompTitle_checked(bool);
    void setOpenProgram();
    void setSaveFolder();
    void save();

private:
    Ui::SettingsForm *ui;
    Keeper *keeper;
};

#endif // MELAMPIG_SETTINGSFORM_H

}
