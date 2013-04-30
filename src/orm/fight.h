#ifndef MELAMPIG_FIGHT_H
#define MELAMPIG_FIGHT_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Fight : public Object
	{
		Q_OBJECT

	public:
		Fight( Keeper *keeper );
		Fight( int id, Keeper *keeper );
		Fight( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_FIGHT_H
