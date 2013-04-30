#ifndef MELAMPIG_ADDRESSTYPE_H
#define MELAMPIG_ADDRESSTYPE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT AddressType : public Object
	{
		Q_OBJECT

	public:
		AddressType( Keeper *keeper );
		AddressType( int id, Keeper *keeper );
		AddressType( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_ADDRESSTYPE_H
