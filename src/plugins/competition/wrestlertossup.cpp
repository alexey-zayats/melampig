#include "wrestlertossup.h"

#include <mainwindow.h>

#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>

#include "errors.h"

namespace Melampig
{
    WrestlerTossup::WrestlerTossup(Object *o, Keeper *keeper, QWidget *parent) :
        QMainWindow(parent), keeper(keeper), object(o), parentWidget(parent)
    {
        setWindowTitle( tr("Toss-up") );
        setObjectName( QString("Toss-up") + o->get("id") );

        QToolBar *actionBar = addToolBar( tr("Actions") );

        saveAction = actionBar->addAction(
                         QIcon(":/icons/disk.png"),
                         tr("Save") );
        autoAction = actionBar->addAction(
                         QIcon(":/icons/asterisk_orange.png"),
                         tr("Automatic") );
        fightsAction = actionBar->addAction(
                           QIcon(":/icons/shield.png"),
                           tr("Create fights") );

        centralWidget = new QWidget(this);

        styleLabel = new QLabel(tr("Style"), centralWidget );
        styleCombo = new QComboBox(centralWidget );
        styleCombo->addItem( tr("Select style"), 0);

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, o->get("id")));

        QList<QVariant> styleId = keeper->getFieldList(
                                   OCompetitionStyle, "style",
                                   opts );

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, styleId));

        QList<QVariant> list = keeper->getListOfFields(
                                   OStyle,
                                   QStringList() << "title" << "id",
                                   opts );

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            styleCombo->addItem( item.at(0), item.at(1) );
        }

        fightsAction->setEnabled(false);

        weightLabel = new QLabel(tr("Weight"), centralWidget );
        weightCombo =  new QComboBox(centralWidget );
        weightCombo->setEnabled(false);

        tableWidget = new QTableWidget(centralWidget );
        tableWidget->setColumnCount(4);

        tableWidget->setHorizontalHeaderItem( 0,
                                              new QTableWidgetItem ( tr("Id") ));
        tableWidget->setHorizontalHeaderItem( 1,
                                              new QTableWidgetItem ( tr("WresterlId") ));
        tableWidget->setHorizontalHeaderItem( 2,
                                              new QTableWidgetItem ( tr("Position") ));
        tableWidget->setHorizontalHeaderItem( 3,
                                              new QTableWidgetItem ( tr("Wrestler") ));

        tableWidget->horizontalHeader()->hideSection(0);
        tableWidget->horizontalHeader()->hideSection(1);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->setSortingEnabled(true);

        tableWidget->verticalHeader()->hide();
        tableWidget->setEnabled(false);

        QGridLayout *layout = new QGridLayout(centralWidget);
        layout->addWidget(styleLabel, 0, 0);
        layout->addWidget(styleCombo, 0, 1);
        layout->addItem(new QSpacerItem(1,1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Minimum), 0, 2);
        layout->addWidget(weightLabel, 1, 0);
        layout->addWidget(weightCombo, 1, 1);
        layout->addItem(new QSpacerItem(1,1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Minimum), 1, 2);
        layout->addItem(new QSpacerItem(1,200,
                                        QSizePolicy::Minimum,
                                        QSizePolicy::Expanding), 2, 0, 1, 3);

        setCentralWidget(tableWidget);

        QDockWidget *dockWidget = new QDockWidget(tr("Filters"), this);
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                         Qt::RightDockWidgetArea);

        dockWidget->setWidget(centralWidget);
        addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
        actionBar->addAction(dockWidget->toggleViewAction());


        connect(styleCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(styleCombo_changed(int)));
        connect(weightCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(weightCombo_changed(int)));

        connect(saveAction, SIGNAL(triggered()),
                this, SLOT(saveAction_triggerd()));
        connect(autoAction, SIGNAL(triggered()),
                this, SLOT(autoAction_triggerd()));
        connect(fightsAction, SIGNAL(triggered()),
                this, SLOT(fightsAction_triggerd()));

        autoAction->setEnabled(false);

        mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        workspace = (QWorkspace*)parentWidget;
    }

    void WrestlerTossup::styleCombo_changed(int /*index*/)
    {
        disconnect(weightCombo, SIGNAL(currentIndexChanged(int)), 0, 0);

        weightCombo->clear();
        weightCombo->addItem( tr("Select weight"), 0 );

        autoAction->setEnabled(false);

        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

        TQueryMap opts;
        opts.insert("competition",
                    keeper->prepareParam(Equal, object->get("id")) );

        QList<QVariant> weightId = keeper->getFieldList(
                                       OCompetitionWeight,
                                       "weight",
                                       opts);

        if ( weightId.size() == 0 )
            return;

        weightCombo->setEnabled(true);

        opts.clear();

        opts.insert("id", keeper->prepareParam(InSet, weightId));
        QList<QVariant> list = keeper->getListOfFields(
                                   OWeight,
                                   QStringList() << "title" << "id",
                                   opts );

        if ( list.size() == 0 )
            return;

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            weightCombo->addItem( item.at(0), item.at(1) );
        }
        connect(weightCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(weightCombo_changed(int)));
    }

    void WrestlerTossup::weightCombo_changed(int idx)
    {
        disconnect(tableWidget,
                   SIGNAL(itemChanged(QTableWidgetItem*)), 0, 0);
        disconnect(tableWidget,
                   SIGNAL(currentItemChanged(QTableWidgetItem*,
                                             QTableWidgetItem*)), 0, 0);

        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

        tableWidget->setEnabled(true);
        int style = styleCombo->itemData (styleCombo->currentIndex() ).toInt();
        int weight = weightCombo->itemData(idx).toInt();

        if ( style == 0 || weight == 0 )
            return;

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("style",       keeper->prepareParam(And | Equal, style));
        opts.insert("weight",      keeper->prepareParam(And | Equal, weight));
        opts.insert("order",       QStringList() << "sorder" );

        QStringList flist;
        flist.append("id");
        flist.append("wrestler");
        flist.append("sorder");

        QList<QVariant> idList = keeper->getListOfFields(OCompetitionWrestler, flist, opts);

        QStringList list;
        QList<QVariant> linkedId;

        for (int i = 0; i < idList.size(); i++ )
        {
            list = idList.at(i).toStringList();
            linkedId.append(list.at(1));
        };
        list.clear();

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, linkedId));

        QHash<int,QVariant> names = keeper->getFieldMap(
                                        OWrestler,
                                        QString("title"), opts);

        bool by_title = true;
        int hasNumber = -1;

        for (int i = 0; i < idList.size(); i++ )
        {
            list = idList.at(i).toStringList();

            if (by_title && list.at(2).toInt() > 0)
                by_title = false;

            tableWidget->insertRow(i);

            QTableWidgetItem *item1 = new QTableWidgetItem( list.at(0) );
            tableWidget->setItem(i, 0, item1);

            QTableWidgetItem *item2 = new QTableWidgetItem( list.at(1) );
            tableWidget->setItem(i, 1, item2 );

            QTableWidgetItem *item3 = new QTableWidgetItem( list.at(2) );
            tableWidget->setItem(i, 2, item3 );

            if ( hasNumber < 0 || hasNumber > 0 ) {
                hasNumber = list.at(2).toInt();
            }

            QTableWidgetItem *item4 = new QTableWidgetItem(
                                          names[ list.at(1).toInt() ].toString() );
            tableWidget->setItem(i, 3, item4 );
        }

//        if ( by_title )
//            tableWidget->sortByColumn( 3, Qt::AscendingOrder );

        autoAction->setEnabled(true);
        fightsAction->setEnabled(hasNumber>0);

        connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
                this, SLOT(tableWidget_itemChanged(QTableWidgetItem*)));
        connect(tableWidget, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
                this, SLOT(tableWidget_currentItemChanged()));
    }

    void WrestlerTossup::saveAction_triggerd()
    {
        int rows = tableWidget->rowCount();

        fightsAction->setEnabled(true);

        for( int i = 0; i < rows; i++ )
        {
            CompetitionWrestler *cw = new CompetitionWrestler(tableWidget->item(i, 0)->text().toInt(), keeper);
            cw->set("sorder", tableWidget->item(i, 2)->text() );

            try {
                cw->store();
            } catch ( SqlError &err ) {
                QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
            }
            delete cw;
        }
    }

    void WrestlerTossup::fightsAction_triggerd()
    {
        QDialog *dialog = new QDialog(this);

        QLabel *label = new QLabel(tr("Select mat"), dialog);
        QComboBox *box = new QComboBox(dialog);
        box->addItem( tr("Select mat"), 0);

        TQueryMap opts;
        opts.insert("order", QStringList() << "id");
        QHash<int,QVariant> list = keeper->getFieldMap(OMat, "title", opts );

        QHash<int,QVariant>::Iterator it = list.begin();
        for( ; it != list.end(); ++it ) {
            box->addItem( it.value().toString(), it.key() );
        }

        QPushButton *ok = new QPushButton(tr("Ok"), dialog);
        QPushButton *cancel = new QPushButton(tr("Cancel"), dialog);

        connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
        connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

//        QCheckBox *isClear = new QCheckBox(tr("Clean before create?"), dialog);

        QBoxLayout *vLayout = new QBoxLayout(QBoxLayout::TopToBottom, dialog);
        vLayout->addWidget(label);
        vLayout->addWidget(box);
//        vLayout->addWidget(isClear);

        QBoxLayout *hLayout = new QBoxLayout(QBoxLayout::LeftToRight, dialog);
        hLayout->addWidget(ok);
        hLayout->addWidget(cancel);

        vLayout->addLayout(hLayout);

        if ( dialog->exec() != QDialog::Accepted )
            return;

        if ( box->currentIndex() == 0 )
            return;

        int competition = object->get("id").toInt();
        int mat = box->itemData( box->currentIndex() ).toInt();
        int style  = styleCombo->itemData(  styleCombo->currentIndex()  ).toInt();
        int weight = weightCombo->itemData( weightCombo->currentIndex() ).toInt();

        // competition, style, weight, mat,
        QString sql("SELECT build_fight_list(?,?,?,?)");

        QSqlQuery query(keeper->database());
        query.prepare(sql);
        query.addBindValue(competition);
        query.addBindValue(style);
        query.addBindValue(weight);
        query.addBindValue(mat);

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.lastError().text() + "\n" + query.executedQuery() );
            return;
        }

        if ( query.next() ) {
            QMessageBox::information(this, tr("Result"), tr("Created %1 fights").arg(query.value(0).toInt()));
        }

        qDebug() << "SELECT build_fight_list(" << competition << ","<< style <<","<< weight <<","<< mat <<")";
    }

    void WrestlerTossup::autoAction_triggerd()
    {
        fightsAction->setEnabled(false);

        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);


        tableWidget->setEnabled(true);
        int style = styleCombo->itemData (styleCombo->currentIndex()).toInt();
        int weight = weightCombo->itemData(weightCombo->currentIndex()).toInt();

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("style",       keeper->prepareParam(And | Equal, style));
        opts.insert("weight",    keeper->prepareParam(And | Equal, weight));
        opts.insert("order",       QStringList() << "random()" );

        QStringList flist;
        flist.append("id");
        flist.append("wrestler");

        QList<QVariant> idList = keeper->getListOfFields(OCompetitionWrestler, flist, opts);

        QStringList list;
        QList<QVariant> itemId;

        for (int i = 0; i < idList.size(); i++ )
        {
            list = idList.at(i).toStringList();
            itemId.append(list.at(1));
        };
        list.clear();

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, itemId));

        QHash<int,QVariant> names = keeper->getFieldMap(OWrestler, QString("title"), opts);

        for (int i = 0; i < idList.size(); i++ )
        {
            list = idList.at(i).toStringList();

            tableWidget->insertRow(i);
            tableWidget->setItem(i, 0, new QTableWidgetItem(list.at(0)));
            tableWidget->setItem(i, 1, new QTableWidgetItem(list.at(1)));
            tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(i+1)));
            tableWidget->setItem(i, 3, new QTableWidgetItem(names[list.at(1).toInt()].toString()));
//            qDebug() << names[list.at(1).toInt()].toString();
        }

        // tableWidget->sortByColumn( 2, Qt::AscendingOrder );
    }

    void WrestlerTossup::tableWidget_currentItemChanged()
    {
        QTableWidgetItem *item = tableWidget->currentItem();
        if (!item) return;

        currentItemText = tableWidget->currentItem()->text();
    }

    void WrestlerTossup::tableWidget_itemChanged(QTableWidgetItem*item)
    {
        if (!item) return;

        QString txt = item->text();

        int pos = txt.toInt();
        int rows = tableWidget->rowCount();

        if ( pos > rows && pos < rows )
        {
            item->setText(currentItemText);
            return;
        }

        QList<QTableWidgetItem*> list = tableWidget->findItems(txt, Qt::MatchExactly);

        for( int i = 0; i < list.size(); i++ )
            if ( list.at(i) != item )
                list.at(i)->setText(currentItemText);

        currentItemText = txt;
    }
}
