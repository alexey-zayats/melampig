#ifndef MELAMPIG_COMPETITION_H
#define MELAMPIG_COMPETITION_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Competition : public Object
	{
		Q_OBJECT

	public:
		Competition( Keeper *keeper );
		Competition( int id, Keeper *keeper );
		Competition( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_COMPETITION_H
