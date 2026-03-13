#ifndef GUARD_DOMMOD_SEARCHYITERATOR_SEARCH_DM_H
#define GUARD_DOMMOD_SEARCHYITERATOR_SEARCH_DM_H

#include <libbasardbaspect_connectionref.h>
#include <libbasardbaspect_accessor.h>

namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorInstanceRef;
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace domMod 
{
	namespace searchyiterator 
	{

		class SearchYIterator
		{
		public:
			SearchYIterator(const basar::VarString&, log4cplus::Logger&);
			virtual ~SearchYIterator();

			basar::db::aspect::AccessorPropertyTable_YIterator get() const;
			void reset() const;

		private:
			SearchYIterator(const SearchYIterator& r);
			SearchYIterator operator = (const SearchYIterator& r);

			void init() const;

			basar::db::aspect::AccessorPropertyTableRef getPropTab() const;
			basar::db::aspect::AccessorInstanceRef getAccessor() const;
			void initAccessor() const;
			const basar::VarString createAccessorInstanceName(
				const basar::VarString& accessorName
			) const;

			log4cplus::Logger& m_Logger;
			basar::VarString								m_AccessorName;
			mutable basar::db::aspect::AccessorInstanceRef  m_SearchAcc;
		};

	} // end namespace searchyiterator
} // end namespace domMod

#endif // GUARD_DOMMOD_SEARCHYITERATOR_SEARCH_DM_H
