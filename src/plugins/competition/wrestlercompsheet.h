#ifndef WRESTLER_COMPSHEET_H
#define WRESTLER_COMPSHEET_H

#include <reportwidget.h>

class QCheckBox;

namespace Melampig
{
    class Keeper;
    class Object;
	class WrestlerCompSheet : public ReportWidget
    {
        Q_OBJECT
        public:
			WrestlerCompSheet(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            virtual QString generateData();

        private:
            void genCompetition();
            void genRepechage();
            void genClassement();

            QCheckBox *pointsBox;

            QMap<QString,QString> pVars;
            QString page_tmpl;
    };

}

#endif // WRESTLER_COMPSHEET
