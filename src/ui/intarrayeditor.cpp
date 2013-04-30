#include "intarrayeditor.h"

#include <QtGui>

namespace Melampig
{
    IntArrayEditor::IntArrayEditor(QWidget *parent) :
        QWidget(parent)
    {
        spinBox = new QSpinBox(this);
        listWidget = new QListWidget(this);

        addButton = new QToolButton(this);
        addButton->setIcon( QIcon( ":/plus") );

        removeButton = new QToolButton(this);
        removeButton->setIcon( QIcon( ":/minus") );

        layout = new QGridLayout(this);
        layout->addWidget(spinBox, 0, 0, 1, 2);
        layout->addWidget(listWidget, 1, 0, 3, 1);
        layout->addWidget(addButton, 1, 1, 1, 1);
        layout->addWidget(removeButton, 2, 1, 1, 1);
        layout->addItem( new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Maximum), 3, 1, 1, 1 );

        layout->setSpacing(2);
        layout->setMargin(2);

        connect(listWidget, SIGNAL(currentTextChanged(const QString&)), this, SLOT(setSpinBoxText(const QString&)));
        connect(spinBox, SIGNAL(valueChanged(QString)), this, SLOT(updateListWidgetItem(const QString&)));
        connect(addButton, SIGNAL(clicked()), this, SLOT(addListWidgetItem()));
        connect(removeButton, SIGNAL(clicked()), this, SLOT(removeListWidgetItem()));
    }

    void IntArrayEditor::setSpinBoxText(const QString &text)
    {
        spinBox->setValue( text.toInt() );
    }

    QStringList IntArrayEditor::data()
    {
        QStringList out;
        for( int i = 0; i < listWidget->count(); i++ ) {
            out << listWidget->item(i)->text();
        }
        return out;
    }

    void IntArrayEditor::setData(const QStringList &list)
    {
        listWidget->clear();
        listWidget->addItems(list);
    }

    void IntArrayEditor::updateListWidgetItem(const QString &txt)
    {
        QListWidgetItem *item = listWidget->currentItem();
        if ( !item ) return;

        item->setText(txt);
    }

    void IntArrayEditor::addListWidgetItem()
    {
        listWidget->addItem( QString("0") );
    }

    void IntArrayEditor::removeListWidgetItem()
    {
        QListWidgetItem *item = listWidget->currentItem();
        if ( !item ) return;

        int row = listWidget->row(item);
        item = listWidget->takeItem(row);
        delete item;
    }

}
