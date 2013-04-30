#include "fighttable-db.h"

#include <QMessageBox>

#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QToolButton>
#include <QPushButton>

#include <QTableWidget>
#include <QHeaderView>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVariant>

#include <QDebug>
#include <QSettings>

namespace Melampig
{

    FightTable::FightTable(Keeper *keeper, QWidget *parent) :
        QDialog(parent),  keeper(keeper), parentWidget(parent)
    {
        setWindowTitle( tr("Select fight") );
        setObjectName( QString("select fight") );

        compLabel = new QLabel(tr("Competition"), this );
        compCombo = new QComboBox(this );
        compCombo->addItem( tr("Select competition"), 0);

        TQueryMap opts;
        opts.insert("order", QStringList() << "id desc");
        QList<QVariant> list = keeper->getListOfFields(OCompetition, QStringList() << "title" << "id", opts );

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            compCombo->addItem( item.at(0), item.at(1) );
        }

        connect(compCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(compCombo_changed(int)));

        styleLabel = new QLabel(tr("Style"), this );
        styleCombo = new QComboBox(this );
        styleCombo->addItem( tr("Select style"), 0);

        weightLabel = new QLabel(tr("Weight"), this );
        weightCombo =  new QComboBox(this );
        weightCombo->setEnabled(false);

        matchLabel = new QLabel(tr("Match"), this );
        matchCombo =  new QComboBox(this);
        matchCombo->setEnabled(false);

        opts.clear();
        opts.insert("order", QStringList() << "id");
        tourHash = keeper->getFieldMap(OCTour, "title", opts );

        opts.clear();
        opts.insert("order", QStringList() << "id");
        roundHash = keeper->getFieldMap(OCRound, "title", opts );

        matLabel = new QLabel(tr("Mat"), this );
        matCombo =  new QComboBox(this );
        matCombo->addItem( tr("Select mat"), 0 );
//        matCombo->setEnabled(false);

        opts.clear();
        opts.insert("order", QStringList() << "id");
        matHash = keeper->getFieldMap(OMat, "title", opts );
        QHash<int,QVariant>::iterator it = matHash.begin();
        for(; it != matHash.end(); ++it) {
            matCombo->addItem( it.value().toString(), QString::number(it.key()) );
        }

        tableWidget = new QTableWidget(this);
        tableWidget->setColumnCount(10);

        tableWidget->setHorizontalHeaderItem( 0, new QTableWidgetItem ( tr("Id") ));
        tableWidget->setHorizontalHeaderItem( 1, new QTableWidgetItem ( tr("Competition") ));
        tableWidget->setHorizontalHeaderItem( 2, new QTableWidgetItem ( tr("Style") ));
        tableWidget->setHorizontalHeaderItem( 3, new QTableWidgetItem ( tr("Weight") ));

        tableWidget->setHorizontalHeaderItem( 4, new QTableWidgetItem ( tr("Num") ));
        tableWidget->setHorizontalHeaderItem( 5, new QTableWidgetItem ( tr("Round") ));
        tableWidget->setHorizontalHeaderItem( 6, new QTableWidgetItem ( tr("Tour") ));
        tableWidget->setHorizontalHeaderItem( 7, new QTableWidgetItem ( tr("Mat") ));
        tableWidget->setHorizontalHeaderItem( 8, new QTableWidgetItem ( tr("Red wrestler") ));
        tableWidget->setHorizontalHeaderItem( 9, new QTableWidgetItem ( tr("Blue wrestler") ));

        tableWidget->horizontalHeader()->hideSection(0);
        tableWidget->horizontalHeader()->hideSection(1);
        tableWidget->horizontalHeader()->hideSection(2);
        tableWidget->horizontalHeader()->hideSection(3);

        tableWidget->horizontalHeader()->setStretchLastSection(true);

        tableWidget->verticalHeader()->hide();

        tableWidget->setEnabled(false);

        searchButton = new QToolButton(this);
        searchButton->setIcon( QIcon(":icons/zoom.png"));
        searchButton->setEnabled(false);

        QGridLayout *layout = new QGridLayout(this);

        layout->addWidget(compLabel, 0, 0, 1, 5);
        layout->addWidget(compCombo, 1, 0, 1, 5);

        layout->addWidget(styleLabel, 2, 0);
        layout->addWidget(weightLabel, 2, 1);
        layout->addWidget(matchLabel, 2, 1);
        layout->addWidget(matLabel, 2, 2);
        layout->addItem(new QSpacerItem(100, 1, QSizePolicy::Maximum), 2, 3, 1, 2);

        layout->addWidget(styleCombo, 3, 0);
        layout->addWidget(weightCombo, 3, 1);
        layout->addWidget(matchCombo, 3, 1);
        layout->addWidget(matCombo, 3, 2);
        layout->addItem(new QSpacerItem(100, 1, QSizePolicy::Maximum), 3, 3 );
        layout->addWidget(searchButton, 3, 4);

        layout->addWidget(tableWidget, 4, 0, 1, 5);

        okButton = new QPushButton(tr("Ok"), this);
        cancelButton = new QPushButton(tr("Cancel"), this);
        okButton->setEnabled(false);

        QBoxLayout *box = new QBoxLayout(QBoxLayout::LeftToRight);
        box->addWidget(okButton);
        box->addWidget(cancelButton);

        layout->addLayout(box, 5, 0, 1, 5);

        matchLabel->hide();
        matchCombo->hide();

        connect(styleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(styleCombo_changed(int)));
        connect(weightCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateActions()));
        connect(matchCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateActions()));
        connect(matCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateActions()));

        connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(okButton, SIGNAL(clicked()), this, SLOT(lockFight()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
        connect(tableWidget, SIGNAL(itemSelectionChanged ()), this, SLOT(tableWidget_itemSelectionChanged ()));

        comp = 0;
    }

    FightTable::~FightTable()
    {
    }

    void FightTable::tableWidget_itemSelectionChanged ()
    {
        QList<QTableWidgetItem *> list = tableWidget->selectedItems();
        if ( list.size() == 0 ) return;

        okButton->setEnabled(true);
    }

    void FightTable::updateActions()
    {
        int style  = styleCombo->itemData(  styleCombo->currentIndex()  ).toInt();
        int weight = weightCombo->itemData( weightCombo->currentIndex() ).toInt();
        int match = matchCombo->itemData( matchCombo->currentIndex() ).toInt();
        int mat   = matCombo->itemData(  matCombo->currentIndex()   ).toInt();

        if ( style && (weight||match) && mat ) {
            searchButton->setEnabled(true);
        }

        okButton->setEnabled(false);
    }

    void FightTable::compCombo_changed(int index)
    {
        disconnect(styleCombo, SIGNAL(currentIndexChanged(int)));

        int id = compCombo->itemData( index ).toInt();

        if (comp) delete comp;
        comp = new Competition(id, keeper);

        if ( comp->get("byteam").compare("true") == 0 ) {
            weightLabel->hide();
            weightCombo->hide();
            matchLabel->show();
            matchCombo->show();
        } else {
            matchLabel->hide();
            matchCombo->hide();
            weightLabel->show();
            weightCombo->show();
        }

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        QList<QVariant> styleId = keeper->getFieldList(OCompetitionStyle, "style", opts);

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, styleId));
        opts.insert("order", QStringList() << "id");

        QList<QVariant> list = keeper->getListOfFields(OStyle, QStringList() << "title" << "id", opts );


        styleCombo->clear();
        styleCombo->addItem( tr("Select style"), 0 );

        weightCombo->clear();
        weightCombo->addItem( tr("Select weight"), 0 );

        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            styleCombo->addItem( item.at(0), item.at(1) );
        }
        connect(styleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(styleCombo_changed(int)));
    }

    void FightTable::styleCombo_changed(int index)
    {
        weightCombo->clear();
        weightCombo->addItem( tr("Select weight"), 0 );

        matchCombo->clear();
        matchCombo->addItem( tr("Select match"), 0 );

        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

        int style = styleCombo->itemData( index ).toInt();

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        opts.insert("order", QStringList() << "id");
        QList<QVariant> weightId = keeper->getFieldList(OCompetitionWeight, "weight", opts);

        if ( weightId.size() ) {

            weightCombo->setEnabled(true);

            opts.clear();

            opts.insert("id", keeper->prepareParam(InSet, weightId));
            opts.insert("order", QStringList() << "id");
            QList<QVariant> list = keeper->getListOfFields(OWeight,
                                                           QStringList() << "title" << "id", opts );

            if ( list.size() == 0 )
                return;

            QStringList item;
            for (int i = 0; i < list.size(); i++ ) {
                item = list.at(i).toStringList();
                weightCombo->addItem( item.at(0), item.at(1) );
            }
        }

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        opts.insert("style", keeper->prepareParam(And|Equal,style));
        opts.insert("order", QStringList() << "id");
        QList<QVariant> matchId = keeper->getFieldList(OMatch, "id", opts);

        if ( matchId.size() ) {

            matchCombo->setEnabled(true);

            opts.clear();

            opts.insert("id", keeper->prepareParam(InSet, matchId));
            opts.insert("order", QStringList() << "id");
            QList<QVariant> list = keeper->getListOfFields(OMatch,
                                                           QStringList() << "title" << "id", opts );

            if ( list.size() == 0 )
                return;

            QStringList item;
            for (int i = 0; i < list.size(); i++ ) {
                item = list.at(i).toStringList();
                matchCombo->addItem( item.at(0), item.at(1) );
            }
        }

    }

    void FightTable::searchButton_clicked()
    {
        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

        tableWidget->setEnabled(true);

        int style  = styleCombo->itemData(  styleCombo->currentIndex()  ).toInt();
        int weight = weightCombo->itemData( weightCombo->currentIndex() ).toInt();
        int match = matchCombo->itemData( matchCombo->currentIndex() ).toInt();
        int mat  = matCombo->itemData( matCombo->currentIndex()   ).toInt();

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        opts.insert("style",       keeper->prepareParam(And|Equal, style));
        if ( comp->get("byteam").compare("true") == 0 ) {
            opts.insert("match",      keeper->prepareParam(And|Equal, match));
        } else {
            opts.insert("weight",      keeper->prepareParam(And|Equal, weight));
        }
        opts.insert("mat",         keeper->prepareParam(And|Equal, mat));
        opts.insert("winner",      keeper->prepareParam(And|Is|Field, "NULL"));
        opts.insert("on_fight",    keeper->prepareParam(And|Equal|Field, "false"));
        opts.insert("order",       QStringList() << "num");

        QStringList fields = QStringList() << "id" << "competition" << "style"
                                           << "weight" << "num"
                                           << "cround" << "ctour"
                                           << "mat" << "red" << "blue";
        QList<QVariant> fightList = keeper->getListOfFields(OFight, fields, opts);

        QStringList list;
        QList<QVariant> wrestlerId;
        wrestlerId.append(0);
        QHash<int, QList<int> > wrestlerPos;

        QTableWidgetItem *item = 0;
        Qt::ItemFlags flags = Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        for (int i = 0; i < fightList.size(); i++ )
        {
            list = fightList.at(i).toStringList();
            tableWidget->insertRow(i);

            item = new QTableWidgetItem(list.at(0));
            item->setFlags(flags);
            tableWidget->setItem(i, 0, item ); // id

            item = new QTableWidgetItem(list.at(1));
            item->setFlags(flags);
            tableWidget->setItem(i, 1, item );  // competition

            item = new QTableWidgetItem(list.at(2));
            item->setFlags(flags);
            tableWidget->setItem(i, 2, item);  // style

            item = new QTableWidgetItem(list.at(3));
            item->setFlags(flags);
            tableWidget->setItem(i, 3, item);  // weight

            item = new QTableWidgetItem(list.at(4));
            item->setFlags(flags);
            tableWidget->setItem(i, 4, item);  // num

            item = new QTableWidgetItem(roundHash.value(list.at(5).toInt()).toString());
            item->setFlags(flags);
            tableWidget->setItem(i, 5, item);  // round

            item = new QTableWidgetItem(tourHash.value(list.at(6).toInt()).toString());
            item->setFlags(flags);
            tableWidget->setItem(i, 6, item);  // tour

            item = new QTableWidgetItem(matHash.value(list.at(7).toInt()).toString());
            item->setFlags(flags|Qt::ItemIsEditable);
            tableWidget->setItem(i, 7, item);  // mat

            item = new QTableWidgetItem(list.at(8));
            item->setFlags(flags);
            tableWidget->setItem(i, 8, item);  // red

            item = new QTableWidgetItem(list.at(9));
            item->setFlags(flags);
            tableWidget->setItem(i, 9, item);  // blue

            wrestlerId.append(list.at(8));
            wrestlerId.append(list.at(9));

            wrestlerPos.insertMulti(list.at(8).toInt(), QList<int>() << i << 8 );
            wrestlerPos.insertMulti(list.at(9).toInt(), QList<int>() << i << 9 );
        }

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, wrestlerId));

        QHash<int,QVariant> wrestlers = keeper->getFieldMap(OWrestler,
                                                         QString("title"), opts);

        QHash<int,QVariant>::iterator it = wrestlers.begin();
        for( ; it != wrestlers.end(); it++ )
        {
            QList<QList<int> > posList = wrestlerPos.values(it.key());
            for( int i = 0 ; i < posList.size(); i++ )
            {
                QList<int> pos = posList.at(i);
                item = tableWidget->item(pos.at(0), pos.at(1));
                if ( item )
                    item->setText(it.value().toString());
            }
        }
    }

    int FightTable::getFightId()
    {
        int row = tableWidget->currentRow();
        QTableWidgetItem *item = tableWidget->item(row, 0);

        return item->text().toInt();
    }

    void FightTable::lockFight()
    {
        int row = tableWidget->currentRow();
        QTableWidgetItem *item = tableWidget->item(row, 0);

        Fight *f = new Fight(item->text().toInt(), keeper);
        f->set("on_fight", "true");
        f->store();

        delete f;
    }
}
