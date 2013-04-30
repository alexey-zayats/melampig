#include "svgdialog.h"

#include <QSvgWidget>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QCoreApplication>
#include <QPushButton>

namespace Melampig
{
    SvgDialog::SvgDialog(const QByteArray &data, QWidget *parent) :
        QDialog(parent), data(data)
    {
        svg = new QSvgWidget(this);
        if ( !data.isEmpty() ) {
            svg->load(data);
            setMinimumSize(svg->sizeHint());
            svg->adjustSize();
        }

        chooseFile = new QToolButton(this);
        chooseFile->setText(tr("Choose file"));

        accept = new QPushButton(this);
        accept->setText(tr("Ok"));

        decline = new QPushButton(this);
        decline->setText(tr("Canel"));

        clear = new QPushButton(this);
        clear->setText(tr("Clear"));

        layout = new QGridLayout(this);
        layout->addWidget(chooseFile, 0, 0);

        layout->addWidget(clear,     0, 4);
        layout->addWidget(accept,     0, 5);
        layout->addWidget(decline,    0, 6);

        layout->addWidget(svg,        1, 0, 1, 7);

        layout->addItem(new QSpacerItem(100, 10, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 1, 1, 2);

        connect(chooseFile, SIGNAL(clicked()), this, SLOT(chooseFile_clicked()));
        connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
        connect(decline, SIGNAL(clicked()), this, SLOT(close()));
        connect(clear, SIGNAL(clicked()), this, SLOT(clearButton_clicked()));
    }

    void SvgDialog::chooseFile_clicked()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QCoreApplication::applicationDirPath() + "/../share",
                                                        tr("Svg (*.svg)"));
        if ( fileName.isEmpty() )
            return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        data.clear();

        while (!file.atEnd()) {
            data += file.readLine();
        }

        if ( !data.isEmpty() ) {
            svg->load(data);
            setMinimumSize(svg->sizeHint());
            svg->adjustSize();
        }
    }

    void SvgDialog::clearButton_clicked()
    {
        data.clear();
        svg->load(data);
        setMinimumSize(svg->sizeHint());
        svg->adjustSize();
    }

}
