#ifndef MELAMPIG_GENDER_H
#define MELAMPIG_GENDER_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Gender : public Object
	{
		Q_OBJECT

	public:
		Gender( Keeper *keeper );
		Gender( int id, Keeper *keeper );
		Gender( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_GENDER_H
