#ifndef MELAMPIG_SVGEDITOR_H
#define MELAMPIG_SVGEDITOR_H

#include <QWidget>
#include <QByteArray>

class QSvgWidget;
class QToolButton;
class QGridLayout;

namespace Melampig
{
    class SvgEditor : public QWidget
    {
        Q_OBJECT
    public:
        SvgEditor(QWidget *parent = 0);
         QByteArray data() const { return bytes; }

    signals:

    public slots:
        void chooseFile_clicked();
        void clearButton_clicked();
        void setData(const QByteArray &d);

    private:
        QByteArray bytes;
        QSvgWidget *svgWidget;

        QToolButton *chooseFile;
        QGridLayout *layout;
        QToolButton *clear;
    };

}

#endif // MELAMPIG_SVGEDITOR_H
