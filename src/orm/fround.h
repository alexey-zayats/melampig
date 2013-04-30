#ifndef MELAMPIG_FROUND_H
#define MELAMPIG_FROUND_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT FRound : public Object
	{
		Q_OBJECT

	public:
		FRound( Keeper *keeper );
		FRound( int id, Keeper *keeper );
		FRound( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_FROUND_H
