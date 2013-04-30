#ifndef MELAMPIG_COMPTYPE_H
#define MELAMPIG_COMPTYPE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompType : public Object
	{
		Q_OBJECT

	public:
		CompType( Keeper *keeper );
		CompType( int id, Keeper *keeper );
		CompType( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_CTYPE_H
