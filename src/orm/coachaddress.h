#ifndef MELAMPIG_COACHADDRESS_H
#define MELAMPIG_COACHADDRESS_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CoachAddress : public Object
	{
		Q_OBJECT

	public:
		CoachAddress( Keeper *keeper );
		CoachAddress( int id, Keeper *keeper );
		CoachAddress( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_COACHADDRESS_H
