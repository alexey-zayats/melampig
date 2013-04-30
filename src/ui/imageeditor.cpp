#include "imageeditor.h"

#include <QtGui>
#include <QBuffer>

namespace Melampig
{

    ImageEditor::ImageEditor(QWidget *parent) :
        QWidget(parent)
    {
        imageLabel = new QLabel(this);
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);

        scrollArea = new QScrollArea(this);
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidget(imageLabel);

        chooseFile = new QToolButton(this);
        chooseFile->setIcon( QIcon(":/icons/magnifier.png") );

        clear = new QToolButton(this);
        clear->setIcon( QIcon(":/icons/delete.png") );

        layout = new QGridLayout(this);
        layout->addWidget(scrollArea,  0, 0, 3, 1);
        layout->addWidget(chooseFile, 0, 1);
        layout->addWidget(clear,      1, 1);
        layout->addItem(new QSpacerItem(10, 10),      2, 1);

		layout->setSpacing(2);
		layout->setMargin(2);

		setLayout(layout);

        connect(chooseFile, SIGNAL(clicked()), this, SLOT(chooseFile_clicked()));
        connect(clear, SIGNAL(clicked()), this, SLOT(clearButton_clicked()));

        //setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
        resize(100, 120);
    }

    void ImageEditor::chooseFile_clicked()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QCoreApplication::applicationDirPath() + "/../share",
                                                        tr("Image (*.gif *.jpeg *jpg *.png)"));
        if ( fileName.isEmpty() )
            return;

        QImage image(fileName);

        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        setData(image.scaledToHeight(100, Qt::SmoothTransformation));
    }

    void ImageEditor::setData(const QImage &image)
    {
        QBuffer buffer( &bytes );
        buffer.open( QBuffer::WriteOnly );
        image.save( &buffer, "PNG" );

        imageLabel->setPixmap(QPixmap::fromImage(image));
        imageLabel->adjustSize();

        setMinimumSize(imageLabel->sizeHint());
        resize(image.size());
    }

    void ImageEditor::setData(const QByteArray &d)
    {
        setData(QImage::fromData(d));
    }

    void ImageEditor::clearButton_clicked()
    {
        setData(QByteArray());
    }

}
