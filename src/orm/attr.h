// attr.h

#ifndef MELAMPIG_ATTR_H
#define MELAMPIG_ATTR_H

#include <QVariant>
#include "exports.h"
#include "keeper.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Attr
	{
		public:

			enum ViewType
			{

				Bool = 1, // +
				Int = 2, // +
				Double = 3,

				String = 4, // +
				Text = 5,
				Html = 6,

				Date = 7, // +
				Time = 8, // +
				DateTime = 9, // +

				Email = 10, // +/-
				Password = 11, // +

				File = 12,
				Image = 13,
				Svg = 14,

				ObjectRef = 15,

				Checkbox = 16,
				Radio = 17,

				LTree = 18,
				HStore = 19,
				IntArray = 20

			};

			Attr();
			Attr( const QString &name, const QString &title, ViewType type,
				  bool ro, bool visible, ObjectType refType = ONull, bool isArray = false );
			virtual ~Attr();

			Attr& operator=( const Attr &a );

			QString getName() const { return name; }
			QString getTitle() const { return title; }
			ViewType getType() const { return type; }
			ObjectType getRef() const { return ref;}

			bool isReadOnly() const { return ro; }
			bool isVisible() const { return visible; }
			bool isArray() const { return anArray; }

			void setName( const QString &s ) { name = s; }
			void setType( const ViewType tt ) { type = tt; }
			void setReadOnly( bool is ) { ro = is; }
			void setVisible( bool is ) { visible = is; }
			void setTitle( const QString &s ) { title = s; }

			void setRef(const ObjectType o) { ref = o;}
			void setArray(const bool s) { anArray = s; }

		private:
			QString name;
			QString title;

			ViewType type;

			bool ro, visible, anArray;
			ObjectType ref;
	};
}

#endif // MELAMPIG_ATTR_H
