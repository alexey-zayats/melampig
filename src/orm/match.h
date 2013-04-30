#ifndef MELAMPIG_MATCH_H
#define MELAMPIG_MATCH_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Match : public Object
	{
		Q_OBJECT

	public:
		Match( Keeper *keeper );
		Match( int id, Keeper *keeper );
		Match( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_MATCH_H
