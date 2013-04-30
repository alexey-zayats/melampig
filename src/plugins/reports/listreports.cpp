#include "listreports.h"

#include "frontpagereport.h"
#include "refereelistreport.h"
#include "entriesnumreport.h"
#include "entrieslistreport.h"
#include "teamcountryreport.h"
#include "teamscorereport.h"
#include "personalcountryreport.h"
#include "personalscorereport.h"

#include <QtGui>
#include <QtWebKit>

#include <QProcess>
#include <QApplication>

#include <orm.h>
#include <ui.h>

namespace Melampig
{
    ListReports::ListReports(Keeper *keeper, QWidget *parent) :
        QMainWindow(parent), keeper(keeper)
    {
        ui.setupUi(this);

        isRuLang = true;
        comp = 0;

        ui.actionPdf->setEnabled(false);
        ui.actionPreview->setEnabled(false);
        ui.actionGenerate->setEnabled(false);

        attr = new Attr("competition", tr("competition"), Attr::ObjectRef, false, true, OCompetition);

        os = new ObjectSelector(attr, keeper, this);
        os->setParentWidget(parent);

        QLabel *l = new QLabel(tr("Competition"), this);
        ui.toolBar->addWidget(l);
        ui.toolBar->addWidget(os);

        connect(os, SIGNAL(objectSelected(Object*)), this, SLOT(competitionSelected(Object*)));

        connect(ui.actionPdf,     SIGNAL(triggered()), this, SLOT(printPdf()));
        connect(ui.actionPreview, SIGNAL(triggered()), this, SLOT(printPreview()));
        connect(ui.actionGenerate, SIGNAL(triggered()), this, SLOT(generateReport()));

        connect(ui.ruRadio, SIGNAL(clicked()), this, SLOT(ruRadio_clicked()));
        connect(ui.engRadio, SIGNAL(clicked()), this, SLOT(engRadio_clicked()));

        ui.teamCountry->setEnabled(false);
        ui.teamScore->setEnabled(false);
        ui.personalCountry->setEnabled(false);
        ui.personalScore->setEnabled(false);

        ui.ruRadio->setChecked(true);
    }

    ListReports::~ListReports()
    {
        delete attr;
    }

    void ListReports::competitionSelected(Object *com)
    {
        isByTeam = com->get("byteam").compare("true") == 0 ? true : false;
        ui.teamCountry->setEnabled(isByTeam);
        ui.teamScore->setEnabled(isByTeam);
        ui.personalCountry->setEnabled(!isByTeam);
        ui.personalScore->setEnabled(!isByTeam);

        comp = com;

        ui.actionGenerate->setEnabled(true);
    }

//        QString reportHtml = readHtml(reportFile);
//        ui.webView->setHtml(reportHtml, urlPrefix);

    void ListReports::ruRadio_clicked()
    {
        isRuLang = ui.ruRadio->isChecked();
    }

    void ListReports::engRadio_clicked()
    {
        isRuLang = !(ui.engRadio->isChecked());
    }

    void ListReports::generateReport()
    {
        QList<QObject*> reports;

        if ( ui.frontBox->isChecked() ) {
            // generate fronpage
            reports << new FrontPageReport(comp, keeper);
        }

        if ( ui.refereeBox->isChecked() ) {
            // generate referee list
            reports << new RefereeListReport(comp, keeper);
        }

        if ( isByTeam ) {
            if ( ui.teamCountry->isChecked() ) {
                // generate list of country by team
                reports << new TeamCountryReport(comp, keeper);
            }
        } else {
            if ( ui.personalCountry->isChecked() ) {
                // generate list of country fo personal
                reports << new PersonalCountryReport(comp, keeper);
            }
        }

        if ( ui.numEntrBox->isChecked() ) {
            // generate table with num of entries by country
            reports << new EntriesNumReport(comp, keeper);
        }

        if ( ui.listEntrBox->isChecked() ) {
            // generate list of entries by team
            reports << new EntriesListReport(comp, keeper);
        }

        if ( isByTeam ) {
            if ( ui.teamScore->isChecked() ) {
                // generate team competition scoresheet
                reports << new TeamScoreReport(comp, keeper);
            }
        } else {
            if ( ui.personalScore->isChecked() ) {
                // generate personal competition scoresheet
                reports << new PersonalScoreReport(comp, keeper);
            }
        }

        QString html;
        for( int i = 0; i < reports.size(); i++ ) {
            ReportBase *rp = qobject_cast<ReportBase*>(reports.at(i));
            html += rp->generate(isRuLang);
        }


#if 1
       QString fpath = QString("%1/report.html").arg( QDir::tempPath() );

       QFile file(fpath);
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           return;

       QTextStream out(&file);
       out << html;

       file.close();
#endif

        QString path("/../share");
        QUrl baseUrl =  QUrl::fromLocalFile( QDir::cleanPath(QApplication::applicationDirPath() + path  ) + "/");
        ui.webView->setHtml(html, baseUrl);

        ui.actionPdf->setEnabled(true);
        ui.actionPreview->setEnabled(true);
    }

    void ListReports::keyPressEvent ( QKeyEvent *event )
    {
        if ( event->key() == Qt::Key_F5 ) {
        }
    }

    void ListReports::printPdf()
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
        QString fileName = ctafn ? QString("%1.pdf").arg(comp->get("title"))
                                 : settings.value("save_filename", "report.pdf").toString();
        QString dirName = is_save ? settings.value("save_folder", QDir::tempPath()).toString()
                                  : QDir::tempPath();

        QString reportPdf = QString("%1/%2").arg( dirName ).arg(fileName);

        settings.endGroup();


        QPrinter p(QPrinter::HighResolution);
        p.setPageSize(QPrinter::A4);
        p.setFullPage(true);
        p.setOutputFileName( reportPdf );
        p.setOutputFormat(QPrinter::PdfFormat);

        ui.webView->print(&p);

        if ( is_open ) {
            QStringList arguments;
            arguments << reportPdf;

            QProcess *myProcess = new QProcess(this);
            myProcess->start(openProgram, arguments);
        }
    }

    void ListReports::printPreview ()
    {
        QPrinter p(QPrinter::HighResolution);
        p.setPageSize(QPrinter::A4);
        p.setFullPage(true);

        QPrintPreviewDialog dialog(&p, this);

        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), ui.webView, SLOT(print(QPrinter*)));

        dialog.exec();
    }
}
