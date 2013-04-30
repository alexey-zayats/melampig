#include "svgeditor.h"

#include <QSvgWidget>
#include <QToolButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QCoreApplication>

namespace Melampig
{

    SvgEditor::SvgEditor(QWidget *parent) :
        QWidget(parent)
    {
        svgWidget = new QSvgWidget(this);

        chooseFile = new QToolButton(this);
        chooseFile->setText(tr("Choose file"));

        clear = new QToolButton(this);
        clear->setText(tr("Clear"));

        layout = new QGridLayout(this);
        layout->addWidget(chooseFile, 0, 0);
        layout->addWidget(clear,      0, 1);
        layout->addWidget(svgWidget,  1, 0, 1, 2);

		layout->setSpacing(2);
		layout->setMargin(2);

		setMinimumHeight(100);
		setAutoFillBackground(true);

        connect(chooseFile, SIGNAL(clicked()), this, SLOT(chooseFile_clicked()));
        connect(clear, SIGNAL(clicked()), this, SLOT(clearButton_clicked()));

        setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    }

    void SvgEditor::chooseFile_clicked()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QCoreApplication::applicationDirPath() + "/../share",
                                                        tr("Svg (*.svg)"));
        if ( fileName.isEmpty() )
            return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QByteArray array;

        while (!file.atEnd()) {
            array += file.readLine();
        }

        if ( !array.isEmpty() ) {
            setData(array);
        }
    }

    void SvgEditor::clearButton_clicked()
    {
        setData(QByteArray());
    }

    void SvgEditor::setData( const QByteArray &d)
    {
        bytes = d;
        svgWidget->load(bytes);
        updateGeometry();
    }

}
