#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QString>
#include <QUrl>

#include <QMainWindow>
#include <orm.h>

class QWebView;
class QAction;
class QPrintPreviewDialog;
class QLabel;
class QComboBox;
class QCheckBox;
class QPushButton;
class QGridLayout;

namespace Melampig
{
    class Keeper;
    class Object;
    class MELAMPIG_UI_EXPORT ReportWidget : public QMainWindow
    {
        Q_OBJECT
        public:
            ReportWidget(Object *object, Keeper *keeper, QWidget *parent = 0);
            ~ReportWidget();

        signals:
            void printReport(QPrinter*);

        public slots:
            void setReportPath(const QString &path) { reportPath = path; }
            void setReportFile(const QString &file) { reportFile = file; }

            void printPreview();
            void printPdf();

            void generateReport(const QString &);
            void init();

            void styleCombo_changed(int index);
            void competitionCombo_changed(int index);
            void weightCombo_changed(int);
            void teamCombo_changed(int /*index*/);

        protected:
            virtual void showEvent ( QShowEvent * event );
            virtual QString generateData() = 0;
            void refereeList();

            QString loadTemplate(const QString &name);
            QString applyTemplateVars( const QString &html, const QMap<QString,QString> &param );

            Keeper *keeper;

            QWebView *webView;
            QPrintPreviewDialog *prevDlg;
            QToolBar *actionBar;

            QMap<QString,QString> vars;
            QGridLayout *filerLayout;

            Object *object;

            QAction *refreshAction;
            QAction *printAction;
            QAction *printPreviewAction;

            QString reportPath;
            QString reportFile;

            QDockWidget *dockWidget;
            QWidget *filterWidget;

            QLabel    *competitionLabel;
            QComboBox *competitionCombo;

            QLabel    *styleLabel;
            QComboBox *styleCombo;

            QLabel    *weightLabel;
            QComboBox *weightCombo;

            QLabel    *teamLabel;
            QComboBox *teamCombo;

            QLabel    *langLabel;
            QCheckBox *ruBox;
            QCheckBox *engBox;

            QAction *buildReport;
            QAction *filterAction;

            bool byteam;
            QString reportLang;
            QString reportPdfName;

            int competitionIndex,
                styleIndex;
    };

}

#endif // REPORTWIDGET_H
