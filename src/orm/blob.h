// blob.h

#ifndef CMSBLOB_H
#define CMSBLOB_H

#include "object.h"
#include <QByteArray>

namespace CMS
{
	class CMSORM_EXPORT Blob : public Object
	{
		Q_OBJECT
		public:
			Blob ( Keeper *keeper );
			Blob ( Keeper *keeper, int id );
			Blob ( Keeper *keeper, const TQueryMap &opts );

			bool writeLO( const QByteArray *ba );
			QByteArray readLO();

		protected:
			void init();

	};
}

#endif // CMSBLOB_H
