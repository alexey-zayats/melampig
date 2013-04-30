#ifndef WRESTLER_START_PROTOCOL_H
#define WRESTLER_START_PROTOCOL_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;

    class WrestlerStartProtocol : public ReportWidget
    {
        Q_OBJECT
        public:
            WrestlerStartProtocol(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            virtual QString generateData();

        private:
            QLabel *typeLabel;
            QComboBox *typeCombo;
            QLabel *matLabel;
            QComboBox *matCombo;
            QLabel *roundLabel;
            QComboBox *roundCombo;
            QLabel *tourLabel;
            QComboBox *tourCombo;
    };

}

#endif // WRESTLER_START_PROTOCOL_H
