#ifndef TEAM_MATCH_PROTOCOL_H
#define TEAM_MATCH_PROTOCOL_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;

    class TeamMatchProtocol : public ReportWidget
    {
        Q_OBJECT
        public:
            TeamMatchProtocol(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            virtual QString generateData();

        private:
            QLabel *typeLabel;
            QComboBox *typeCombo;
            QLabel *matLabel;
            QComboBox *matCombo;
            QLabel *roundLabel;
            QComboBox *roundCombo;
    };

}

#endif // TEAM_MATCH_PROTOCOL_H
