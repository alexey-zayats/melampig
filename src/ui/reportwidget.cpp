#include "reportwidget.h"

#include <orm.h>
#include <QtGui>
#include <QtSql>
#include <QtWebKit>

namespace Melampig
{
    ReportWidget::ReportWidget(Object *object, Keeper *keeper, QWidget *parent) :
        QMainWindow(parent), keeper (keeper), object(object)
    {
        byteam = object->get("byteam").compare("true") == 0;

        actionBar = addToolBar("Tools");
        refreshAction = actionBar->addAction( QIcon(":/icons/arrow_refresh.png"), tr("Refresh") );
        refreshAction->setShortcut( Qt::Key_F5 );

        actionBar->addSeparator();

        printAction = actionBar->addAction( QIcon(":/icons/page_white_acrobat.png"), tr("Save to PDF") );
        printPreviewAction = actionBar->addAction( QIcon(":/icons/printer.png"), tr("Print preview") );

        actionBar->addSeparator();

        buildReport = actionBar->addAction( QIcon(":/icons/accept.png"), tr("Build report") );
        buildReport->setEnabled(false);

        connect(refreshAction, SIGNAL(triggered()), this, SLOT(init()));
        connect(printAction, SIGNAL(triggered()), this, SLOT(printPdf()));
        connect(printPreviewAction, SIGNAL(triggered()), this, SLOT(printPreview()));
        connect(buildReport, SIGNAL(triggered()), this, SLOT(init()));

        webView = new QWebView(this);
        setCentralWidget(webView);

        dockWidget = new QDockWidget(tr("Filters"), this);
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                         Qt::RightDockWidgetArea);

        filterWidget = new QWidget(this);

        langLabel = new QLabel( tr("Language"), filterWidget);

        reportLang = "ru";

        ruBox = new QCheckBox(tr("Ru"), filterWidget);
        ruBox->setChecked(true);

        engBox = new QCheckBox(tr("Eng"), filterWidget);

        QButtonGroup *grp = new QButtonGroup();
        grp->addButton(ruBox);
        grp->addButton(engBox);
        grp->setExclusive(true);

        competitionLabel = new QLabel(tr("Competiton"), filterWidget);
        competitionCombo = new QComboBox(filterWidget);
//        competitionCombo->setMaximumWidth(200);
        competitionCombo->setMinimumContentsLength(20);
        competitionCombo->addItem( tr("Select competition"), 0);

        TQueryMap opts;
        opts.insert("byteam", keeper->prepareParam(Equal, QString(byteam ? "true" : "false")));
        QList<QVariant> clist = keeper->getListOfFields(OCompetition, QStringList() << "title" << "id", opts );
        QStringList item;
        for (int i = 0; i < clist.size(); i++ ) {
            item = clist.at(i).toStringList();
            competitionCombo->addItem( item.at(0), item.at(1) );
        }

        styleLabel = new QLabel(tr("Style"), filterWidget );
        styleCombo = new QComboBox(filterWidget);
        styleCombo->addItem( tr("Select style"), 0);

        weightLabel = new QLabel(tr("Weight"), filterWidget);
        weightCombo =  new QComboBox(filterWidget);
        weightCombo->setEnabled(false);

        teamLabel = new QLabel(tr("Team"), filterWidget);
        teamCombo =  new QComboBox(filterWidget);
        teamCombo->setEnabled(false);

        filerLayout = new QGridLayout(filterWidget);

        filerLayout->addWidget(langLabel, 0, 0);
        filerLayout->addWidget(ruBox, 0, 1);
        filerLayout->addWidget(engBox, 0, 2);
        filerLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 3);

        filerLayout->addWidget(competitionLabel, 1, 0);
        filerLayout->addWidget(competitionCombo, 1, 1, 1, 2);
        filerLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 3);

        filerLayout->addWidget(styleLabel, 2, 0);
        filerLayout->addWidget(styleCombo, 2, 1, 1, 2);
        filerLayout->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Minimum), 2, 3);

        filerLayout->addWidget(weightLabel, 3, 0);
        filerLayout->addWidget(weightCombo, 3, 1, 1, 2);

        filerLayout->addWidget(teamLabel, 3, 0);
        filerLayout->addWidget(teamCombo, 3, 1, 1, 2);

        filerLayout->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Minimum), 3, 3);

        filerLayout->addItem(new QSpacerItem(1,200, QSizePolicy::Minimum, QSizePolicy::Expanding), 100, 0, 1, 3);


        connect(styleCombo, SIGNAL(activated(int)), this, SLOT(styleCombo_changed(int)));
        connect(weightCombo, SIGNAL(activated(int)), this, SLOT(weightCombo_changed(int)));
        connect(teamCombo, SIGNAL(activated(int)), this, SLOT(teamCombo_changed(int)));


        dockWidget->setWidget(filterWidget);
        addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

        actionBar->addSeparator();

        filterAction = dockWidget->toggleViewAction();
        actionBar->addAction(filterAction);

        if ( byteam ) {
            weightCombo->setVisible(false);
            weightLabel->setVisible(false);
            teamLabel->setVisible(true);
            teamCombo->setVisible(true);
        } else {
            teamLabel->setVisible(false);
            teamCombo->setVisible(false);
            weightCombo->setVisible(true);
            weightLabel->setVisible(true);
        }

        connect(competitionCombo, SIGNAL(activated(int)), this, SLOT(competitionCombo_changed(int)));

        competitionIndex = -1;
        styleIndex = -1;
    }

    ReportWidget::~ReportWidget()
    {
    }

    void ReportWidget::showEvent ( QShowEvent * event )
    {
        int idx = competitionCombo->findText(object->get("title"));
        if ( idx >= 0 ) {
            competitionCombo->setCurrentIndex( idx );
            this->competitionCombo_changed(idx);
        }
        QMainWindow::showEvent(event);
    }

    void ReportWidget::competitionCombo_changed(int index)
    {
        if ( competitionIndex == index ) return;

        competitionIndex = index;
        styleIndex = -1;

        buildReport->setEnabled(false);

        styleCombo->setEnabled(false);
        weightCombo->setEnabled(false);
        teamCombo->setEnabled(false);

        styleCombo->clear();
        styleCombo->addItem( tr("Select style"), 0);

        weightCombo->clear();
        weightCombo->addItem( tr("Select weight"), 0 );

        teamCombo->clear();
        teamCombo->addItem( tr("Select team"), 0 );


        int cid = competitionCombo->itemData(competitionCombo->currentIndex()).toInt();
        if ( cid == 0 ) return;

        object->restore(cid);
        byteam = object->get("byteam").compare("true") == 0;

        if ( byteam ) {
            weightCombo->setVisible(false);
            weightLabel->setVisible(false);
            teamLabel->setVisible(true);
            teamCombo->setVisible(true);
        } else {
            teamLabel->setVisible(false);
            teamCombo->setVisible(false);
            weightCombo->setVisible(true);
            weightLabel->setVisible(true);
        }



        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, cid));
        QList<QVariant> idlist = keeper->getFieldList(OCompetitionStyle, "style", opts );

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, idlist));

        QList<QVariant> list = keeper->getListOfFields(OStyle, QStringList() << "title" << "id", opts );

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            styleCombo->addItem( item.at(0), item.at(1) );
        }

        styleCombo->setEnabled(true);
        weightCombo->setEnabled(true);
        teamCombo->setEnabled(true);
    }

    void ReportWidget::styleCombo_changed(int index)
    {
        if ( styleIndex == index ) return;

        styleIndex = index;

        buildReport->setEnabled(false);

        weightCombo->setEnabled(false);
        teamCombo->setEnabled(false);

        weightCombo->clear();
        weightCombo->addItem( tr("Select weight"), 0 );

        teamCombo->clear();
        teamCombo->addItem( tr("Select team"), 0 );


        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")) );

        QList<QVariant> weightId = keeper->getListOfFields(OCompetitionWeight,
                                                         QStringList() << "weight" , opts);

        if ( weightId.size() == 0 )
            return;

        weightCombo->setEnabled(true);

        opts.clear();

        opts.insert("id", keeper->prepareParam(InSet, weightId));
        opts.insert("order", QStringList() << "title");
        QList<QVariant> list = keeper->getListOfFields(OWeight,
                                                       QStringList() << "title" << "id", opts );

        if ( list.size() == 0 )
            return;

        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            weightCombo->addItem( item.at(0), item.at(1) );
        }

        // ----------------

        int cid = competitionCombo->itemData( competitionCombo->currentIndex() ).toInt();
        int sid = styleCombo->itemData( styleCombo->currentIndex() ).toInt();

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, cid) );
        opts.insert("style", keeper->prepareParam(And|Equal, sid) );

        QList<QVariant> teamId = keeper->getListOfFields(OCompetitionTeam,
                                                         QStringList() << "team" , opts);

        if ( teamId.size() == 0 )
            return;

        teamCombo->setEnabled(true);

        opts.clear();

        opts.insert("id", keeper->prepareParam(InSet, teamId));
        opts.insert("order", QStringList() << "title");
        list = keeper->getListOfFields(OTeam, QStringList() << "title" << "id", opts );

        if ( list.size() == 0 )
            return;

        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            teamCombo->addItem( item.at(0), item.at(1) );
        }

        weightCombo->setEnabled(true);
        teamCombo->setEnabled(true);
    }

    void ReportWidget::weightCombo_changed(int /*index*/)
    {
        int cid = competitionCombo->itemData( competitionCombo->currentIndex() ).toInt();
        int sid = styleCombo->itemData( styleCombo->currentIndex() ).toInt();
        int wid = weightCombo->itemData( weightCombo->currentIndex() ).toInt();

        if ( cid > 0 && sid > 0 && wid > 0 )
            buildReport->setEnabled(true);
    }

    void ReportWidget::teamCombo_changed(int /*index*/)
    {
        int cid = competitionCombo->itemData( competitionCombo->currentIndex() ).toInt();
        int sid = styleCombo->itemData( styleCombo->currentIndex() ).toInt();
        int tid = teamCombo->itemData( teamCombo->currentIndex() ).toInt();

        if ( cid > 0 && sid > 0 && tid > 0 )
            buildReport->setEnabled(true);
    }

    void ReportWidget::init()
    {
        reportLang = ruBox->isChecked() ? "ru" : "eng";
        generateReport( generateData() );
    }

    QString ReportWidget::loadTemplate(const QString &name)
    {
        QString filePath = QDir::cleanPath(QApplication::applicationDirPath() + reportPath) + QString("/") + name;

        QFile file( filePath );

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return QString("Template \"%1\" is not found").arg(name);

        QString html;
        while (!file.atEnd()) {
            html += file.readLine();
        }

        file.close();
        return html;
    }

    QString ReportWidget::applyTemplateVars( const QString &html, const QMap<QString,QString> &param )
    {
        QString data = html;
        QMap<QString,QString>::const_iterator it =  param.begin();
        for( ; it != param.end(); it++ ) {
            data = data.replace(it.key(), it.value());
        }
        return data;
    }

    void ReportWidget::generateReport(const QString &html)
    {
        QUrl baseUrl =  QUrl::fromLocalFile( QDir::cleanPath(QApplication::applicationDirPath() + "/../share"  ) + "/");
        webView->setHtml(html, baseUrl);

#if 1
       QString fpath = QString("%1/report.html").arg( QDir::tempPath() );

       QFile file(fpath);
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           return;

       QTextStream out(&file);
       out << html;

       file.close();
#endif
    }

    void ReportWidget::printPreview()
    {
        QPrinter p(QPrinter::HighResolution);
        p.setPageSize(QPrinter::A4);
        p.setFullPage(true);

        QPrintPreviewDialog dialog(&p, this);

        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), webView, SLOT(print(QPrinter*)));

        dialog.exec();
    }

    void ReportWidget::printPdf()
    {
        QSettings settings;
        settings.beginGroup("reports/settings");

        bool is_open = settings.value("is_open", true).toBool();
        bool is_save = settings.value("is_save", true).toBool();

        QString openProgram;

    #ifdef WIN32
        QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\AcroExch.Document\\Shell\\Open\\Command",QSettings::NativeFormat);
        QString acroRdPath = reg.value("Default").toString();
        acroRdPath = acroRdPath.replace("%1", "");
        acroRdPath = acroRdPath.replace("\"", "");
        acroRdPath = acroRdPath.trimmed();

        openProgram = settings.value("open_in", acroRdPath).toString();
    #else
        openProgram = settings.value("open_in", "/usr/bin/evince").toString();
    #endif

        bool ctafn = settings.value("comp_title_as_file_name", true).toBool();
        QString fileName = ctafn ? QString("%1.pdf").arg(object->get("title")) : settings.value("save_filename", "report.pdf").toString();
        QString dirName = is_save ? settings.value("save_folder", QDir::tempPath()).toString() : QDir::tempPath();

        QString reportPdf = QString("%1/%2").arg( dirName ).arg(fileName);

        settings.endGroup();


        QPrinter p(QPrinter::HighResolution);
        p.setPageSize(QPrinter::A4);
        p.setFullPage(true);
        p.setOutputFileName( reportPdf );
        p.setOutputFormat(QPrinter::PdfFormat);

        webView->print(&p);

        if ( is_open ) {
            QStringList arguments;
            arguments << reportPdf;

            QProcess *myProcess = new QProcess(this);
            myProcess->start(openProgram, arguments);
        }
    }

    void ReportWidget::refereeList()
    {
        QString sql("\
                 select \
                       r.title, \
                       p.name, \
                       rc.title \
                 from \
                       competition_referee as c, \
                       referee as r, \
                       position as p, \
                       rang as rc \
                 where \
                       c.competition = ? and \
                       r.id = c.referee and \
                       p.id = c.position and \
                       rc.id = r.rang \
                 order by c.id");

        QSqlQuery query(keeper->database());
        query.prepare(sql);
        query.addBindValue(object->get("id"));

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
            return;
        }

        if ( query.size() == 0 ) {
            vars.insert("{referee.chief.category}", "" );
            vars.insert("{referee.chief.title}", "" );
            vars.insert("{secretary.chief.category}", "" );
            vars.insert("{secretary.chief.title}", "" );
        } else {
            int chief0 = 0, chief1 = 0;
            while ( query.next() ) {
                QString rtitle = query.value(0).toString();
                QString cname = query.value(2).toString();
                QString pname = query.value(1).toString();
                if ( pname.compare("referee.chief") == 0 ) {
                    vars.insert("{referee.chief.category}", cname );
                    vars.insert("{referee.chief.title}", rtitle );
                    chief0 = 1;
                } else if ( pname.compare("secretary.chief") == 0 ) {
                    vars.insert("{secretary.chief.category}", cname );
                    vars.insert("{secretary.chief.title}", rtitle );
                    chief1 = 1;
                }
            }

            if ( chief0 == 0 ) {
                vars.insert("{referee.chief.category}", "" );
                vars.insert("{referee.chief.title}", "" );
            }
            if ( chief1 == 0 ) {
                vars.insert("{secretary.chief.category}", "" );
                vars.insert("{secretary.chief.title}", "" );
            }
        }
    }
}
