#ifndef MELAMPIG_MAT_H
#define MELAMPIG_MAT_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Mat : public Object
	{
		Q_OBJECT

	public:
		Mat( Keeper *keeper );
		Mat( int id, Keeper *keeper );
		Mat( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_MAT_H
