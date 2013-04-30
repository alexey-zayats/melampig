#ifndef MELAMPIG_CTOUR_H
#define MELAMPIG_CTOUR_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CTour : public Object
	{
		Q_OBJECT

	public:
		CTour( Keeper *keeper );
		CTour( int id, Keeper *keeper );
		CTour( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_CROUND_H
