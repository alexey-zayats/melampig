#ifndef TEAM_COMPSHEET_H
#define TEAM_COMPSHEET_H

#include <reportwidget.h>

class QCheckBox;

namespace Melampig
{
    class Keeper;
    class Object;
    class TeamCompSheet : public ReportWidget
    {
        Q_OBJECT
        public:
            TeamCompSheet(Object *o, Keeper *keeper, QWidget *parent = 0);

        public slots:
            void styleCombo_changed(int /*index*/);

        protected:
            virtual QString generateData();

        private:
            void genHeader();
            void genCompetition();
            void genClassement();
            void genFooter();

//            QCheckBox *pointsBox;
    };

}

#endif // TEAM_COMPSHEET_H
