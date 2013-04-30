#ifndef MELAMPIG_REFEREEADDRESS_H
#define MELAMPIG_REFEREEADDRESS_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT RefereeAddress : public Object
	{
		Q_OBJECT

	public:
		RefereeAddress( Keeper *keeper );
		RefereeAddress( int id, Keeper *keeper );
		RefereeAddress( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_REFEREEADDRESS_H
