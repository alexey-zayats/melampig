#ifndef MELAMPIG_IMAGEEDITOR_H
#define MELAMPIG_IMAGEEDITOR_H

#include <QWidget>
#include <QByteArray>

class QLabel;
class QToolButton;
class QGridLayout;
class QPushButton;
class QScrollArea;

namespace Melampig
{

    class ImageEditor : public QWidget
    {
        Q_OBJECT
    public:
        ImageEditor(QWidget *parent = 0);
        QByteArray data() { return bytes; }

    signals:

    public slots:
        void chooseFile_clicked();
        void clearButton_clicked();
        void setData( const QByteArray &bytes);
        void setData(const QImage &image);

    private:
        QByteArray bytes;
        QLabel *imageLabel;
        QToolButton *chooseFile;
        QGridLayout *layout;
        QToolButton *clear;
        QScrollArea *scrollArea;
    };

}

#endif // IMAGEEDITOR_H
