#ifndef INTARRAYEDITOR_H
#define INTARRAYEDITOR_H

#include <QWidget>

class QLabel;
class QToolButton;
class QGridLayout;
class QListWidget;
class QSpinBox;

namespace Melampig
{

    class IntArrayEditor : public QWidget
    {
        Q_OBJECT
        public:
            IntArrayEditor(QWidget *parent = 0);
            QStringList data();

        signals:

        public slots:
            void setData(const QStringList &list);
            void updateListWidgetItem(const QString &);
            void setSpinBoxText(const QString &);
            void addListWidgetItem();
            void removeListWidgetItem();

        private:
            QListWidget *listWidget;

            QSpinBox *spinBox;

            QToolButton *addButton;
            QToolButton *removeButton;

            QGridLayout *layout;
    };

}
#endif // INTARRAYEDITOR_H
