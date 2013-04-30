#ifndef MELAMPIG_COACH_H
#define MELAMPIG_COACH_H

#include "object.h"

namespace Melampig
{
    class MELAMPIG_ORM_EXPORT Coach : public Object
    {
        Q_OBJECT

    public:
        Coach( Keeper *keeper );
        Coach( int id, Keeper *keeper );
        Coach( const TQueryMap &opts, Keeper *keeper );

    protected:
        void init();

    };
}

#endif // MELAMPIG_WRESTLER_H
