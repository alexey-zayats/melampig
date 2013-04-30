#ifndef MELAMPIG_STYLE_H
#define MELAMPIG_STYLE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Style : public Object
	{
		Q_OBJECT

	public:
		Style( Keeper *keeper );
		Style( int id, Keeper *keeper );
		Style( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_STYLE_H
