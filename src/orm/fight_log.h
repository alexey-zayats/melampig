#ifndef MELAMPIG_FIGHTLOG_H
#define MELAMPIG_FIGHTLOG_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT FightLog : public Object
	{
		Q_OBJECT

	public:
		FightLog( Keeper *keeper );
		FightLog( int id, Keeper *keeper );
		FightLog( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_FIGHTLOG_H
