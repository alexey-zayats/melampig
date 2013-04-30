#ifndef MELAMPIG_CROUND_H
#define MELAMPIG_CROUND_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CRound : public Object
	{
		Q_OBJECT

	public:
		CRound( Keeper *keeper );
		CRound( int id, Keeper *keeper );
		CRound( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_CROUND_H
