#ifndef GUARD_BATCHORDERARCHIVEDM_H
#define GUARD_BATCHORDERARCHIVEDM_H

#include <libbasardbaspect_accessor.h>
#include "searchyiteratorptr.h"

namespace domMod 
{
	class ArchiveDM 
	{
	public:
		ArchiveDM();
		~ArchiveDM();

		void init(const basar::db::aspect::ConnectionRef activeConn);
		void shutdown();

		bool deleteOrderHeadArchive(const basar::db::aspect::AccessorPropertyTable_YIterator yit);
		bool deleteOrderPosArchive(const basar::db::aspect::AccessorPropertyTable_YIterator yit);
		bool deleteOrderProposalArchive(const basar::db::aspect::AccessorPropertyTable_YIterator yit);

		virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit();

	private:
		ArchiveDM(const ArchiveDM &r);
		ArchiveDM &operator = (const ArchiveDM &r);
		
		searchyiterator::SearchYIteratorPtr	getSearchYiterator();

		searchyiterator::SearchYIteratorPtr		m_SearchYit;
		basar::db::aspect::AccessorInstanceRef	m_ArchiveAccessor;	
		basar::Int32							m_Initialized;
	};
}

#endif //GUARD_BATCHORDERARCHIVEDM_H
