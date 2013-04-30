#ifndef SVGDIALOG_H
#define SVGDIALOG_H

#include <QDialog>
#include <QByteArray>
#include <orm.h>

class QSvgWidget;
class QToolButton;
class QGridLayout;
class QPushButton;

namespace Melampig
{
    class MELAMPIG_UI_EXPORT SvgDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit SvgDialog(const QByteArray &data, QWidget *parent = 0);
        QByteArray value() const { return data; }

    signals:

    public slots:
        void chooseFile_clicked();
        void clearButton_clicked();

    private:
        QSvgWidget *svg;
        QToolButton *chooseFile;
        QGridLayout *layout;
        QByteArray data;
        QPushButton *accept;
        QPushButton *decline;
        QPushButton *clear;
    };

}

#endif // SVGDIALOG_H
