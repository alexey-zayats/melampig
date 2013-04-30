#ifndef MELAMPIG_WRESTLER_H
#define MELAMPIG_WRESTLER_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Wrestler : public Object
	{
		Q_OBJECT

	public:
		Wrestler( Keeper *keeper );
		Wrestler( int id, Keeper *keeper );
		Wrestler( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_WRESTLER_H
