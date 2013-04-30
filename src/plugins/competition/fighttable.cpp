#include "fighttable.h"

#include <QMessageBox>

#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QToolButton>

#include <QTableWidget>
#include <QHeaderView>
#include <QGridLayout>

#include <QDebug>

namespace Melampig
{

    FightTable::FightTable(Object *o, Keeper *keeper, QWidget *parent) :
        QMainWindow(parent),  keeper(keeper), parentWidget(parent), comp(o)
    {
        setWindowTitle( tr("%1 - create fights ").arg(comp->get("title")) );
        setObjectName( QString("create-fights-") + comp->get("id") );

        centralWidget = new QWidget(this);

        styleLabel = new QLabel(tr("Style"), centralWidget );
        styleCombo = new QComboBox(centralWidget );
        styleCombo->addItem( tr("Select style"), 0);

        QStringList styleId = Object::toList(o->get("style"));

        QList<QVariant> idlist;
        for ( int i = 0; i < styleId.size(); i++ )
            idlist.append( styleId.at(i) );

        TQueryMap opts;
        opts.insert("id", keeper->prepareParam(InSet, idlist));

        QList<QVariant> list = keeper->getListOfFields(OStyle, QStringList() << "title" << "id", opts );

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            styleCombo->addItem( item.at(0), item.at(1) );
        }

        weightLabel = new QLabel(tr("Weight"), centralWidget );
        weightCombo =  new QComboBox(centralWidget );
        weightCombo->setEnabled(false);

        roundLabel = new QLabel(tr("Round"), centralWidget );
        roundCombo =  new QComboBox(centralWidget );
        roundCombo->addItem( tr("Select round"), 0);

        opts.clear();
        list = keeper->getListOfFields(OCompetitionRound, QStringList() << "title" << "id", opts );

        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            roundCombo->addItem( item.at(0), item.at(1) );
        }

        opts.clear();
        matHash = keeper->getFieldMap(OMat, "title", opts );

        tableWidget = new QTableWidget(centralWidget );
        tableWidget->setColumnCount(11);

        tableWidget->setHorizontalHeaderItem( 0, new QTableWidgetItem ( tr("Id") ));
        tableWidget->setHorizontalHeaderItem( 1, new QTableWidgetItem ( tr("Competition") ));
        tableWidget->setHorizontalHeaderItem( 2, new QTableWidgetItem ( tr("Style") ));
        tableWidget->setHorizontalHeaderItem( 3, new QTableWidgetItem ( tr("Weightca") ));
        tableWidget->setHorizontalHeaderItem( 4, new QTableWidgetItem ( tr("Round") ));
        tableWidget->setHorizontalHeaderItem( 5, new QTableWidgetItem ( tr("Tour") ));

        tableWidget->setHorizontalHeaderItem( 6, new QTableWidgetItem ( tr("Num") ));
        tableWidget->setHorizontalHeaderItem( 7, new QTableWidgetItem ( tr("Mat") ));
        tableWidget->setHorizontalHeaderItem( 8, new QTableWidgetItem ( tr("Red wrestler") ));
        tableWidget->setHorizontalHeaderItem( 9, new QTableWidgetItem ( tr("Blue wrestler") ));
        tableWidget->setHorizontalHeaderItem( 10, new QTableWidgetItem ( tr("Winner wrestler") ));

        tableWidget->horizontalHeader()->hideSection(0);
        tableWidget->horizontalHeader()->hideSection(1);
        tableWidget->horizontalHeader()->hideSection(2);
        tableWidget->horizontalHeader()->hideSection(3);
        tableWidget->horizontalHeader()->hideSection(4);
        tableWidget->horizontalHeader()->hideSection(5);

        tableWidget->horizontalHeader()->setStretchLastSection(true);

        tableWidget->verticalHeader()->hide();

        tableWidget->setEnabled(false);

        searchButton = new QToolButton(this);
        searchButton->setIcon( QIcon(":/share/icons/search.png"));
        searchButton->setEnabled(false);

        QGridLayout *layout = new QGridLayout(centralWidget);

        layout->addWidget(styleLabel, 0, 0);
        layout->addWidget(weightLabel, 0, 1);
        layout->addWidget(roundLabel, 0, 2);
        layout->addItem(new QSpacerItem(100, 1, QSizePolicy::Maximum), 0, 3, 1, 2);

        layout->addWidget(styleCombo, 1, 0);
        layout->addWidget(weightCombo, 1, 1);
        layout->addWidget(roundCombo, 1, 2);
        layout->addItem(new QSpacerItem(100, 1, QSizePolicy::Maximum), 1, 3);
        layout->addWidget(searchButton, 1, 4);

        layout->addWidget(tableWidget, 2, 0, 1, 5);

        setCentralWidget(centralWidget);

        connect(styleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(styleCombo_changed(int)));
        connect(weightCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateActions()));
        connect(roundCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateActions()));

        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
    }

    FightTable::~FightTable()
    {
    }

    void FightTable::updateActions()
    {
        int style  = styleCombo->itemData(  styleCombo->currentIndex()  ).toInt();
        int weight = weightCombo->itemData( weightCombo->currentIndex() ).toInt();
        int round   = roundCombo->itemData(   roundCombo->currentIndex()   ).toInt();

        if ( style && weight && round ) {
            searchButton->setEnabled(true);
        }
    }

    void FightTable::styleCombo_changed(int index)
    {
        weightCombo->clear();
        weightCombo->addItem( tr("Select weight"), 0 );

        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

        int style = styleCombo->itemData( index ).toInt();

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")) );
        opts.insert("style",       keeper->prepareParam(And | Equal, style) );
        opts.insert("group",       (QStringList() << "weight") );

        QList<QVariant> weightId = keeper->getListOfFields(OCompetitionWrestler,
                                                         QStringList() << "weight" , opts);

        if ( weightId.size() == 0 )
            return;

        weightCombo->setEnabled(true);

        opts.clear();

        opts.insert("id", keeper->prepareParam(InSet, weightId));
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

    void FightTable::searchButton_clicked()
    {
        while( tableWidget->rowCount() )
            tableWidget->removeRow(0);

         tableWidget->setEnabled(true);

        int style  = styleCombo->itemData(  styleCombo->currentIndex()  ).toInt();
        int weight = weightCombo->itemData( weightCombo->currentIndex() ).toInt();
        int round  = roundCombo->itemData(   roundCombo->currentIndex()   ).toInt();

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        opts.insert("style",       keeper->prepareParam(And|Equal, style));
        opts.insert("weight",    keeper->prepareParam(And|Equal, weight));
        opts.insert("competition_round", keeper->prepareParam(And|Equal, round));

        QStringList fields = QStringList() << "id" << "competition" << "style"
                                           << "weight" << "competition_round"
                                           << "competition_tour" << "num"
                                           << "mat" << "red" << "blue"
                                           << "winner";
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
            tableWidget->setItem(i, 4, item);  // round

            item = new QTableWidgetItem(list.at(5));
            item->setFlags(flags);
            tableWidget->setItem(i, 5, item);  // tour

            item = new QTableWidgetItem(list.at(6));
            item->setFlags(flags);
            tableWidget->setItem(i, 6, item);  // num

            item = new QTableWidgetItem(matHash.value(list.at(7).toInt()).toString());
            item->setFlags(flags|Qt::ItemIsEditable);
            tableWidget->setItem(i, 7, item);  // mat

            item = new QTableWidgetItem(list.at(8));
            item->setFlags(flags);
            tableWidget->setItem(i, 8, item);  // red

            item = new QTableWidgetItem(list.at(9));
            item->setFlags(flags);
            tableWidget->setItem(i, 9, item);  // blue

            item = new QTableWidgetItem(list.at(10));
            item->setFlags(flags);
            tableWidget->setItem(i, 10, item);  // winner

            wrestlerId.append(list.at(8));
            wrestlerId.append(list.at(9));
            wrestlerId.append(list.at(10));

            wrestlerPos.insertMulti(list.at(8).toInt(), QList<int>() << i << 8 );
            wrestlerPos.insertMulti(list.at(9).toInt(), QList<int>() << i << 9 );
            wrestlerPos.insertMulti(list.at(10).toInt(), QList<int>() << i << 10 );
        }

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, wrestlerId));

        QHash<int,QVariant> wrestlers = keeper->getFieldMap(OWrestler,
                                                         QString("title"), opts);

        QHash<int,QVariant>::iterator it = wrestlers.begin();
        for( ; it != wrestlers.end(); it++ )
        {
            QList<QList<int> > posList = wrestlerPos.values(it.key());
            qDebug() << posList;
            for( int i = 0 ; i < posList.size(); i++ )
            {
                QList<int> pos = posList.at(i);
                item = tableWidget->item(pos.at(0), pos.at(1));
                if ( item )
                    item->setText(it.value().toString());
            }
        }
    }
}
