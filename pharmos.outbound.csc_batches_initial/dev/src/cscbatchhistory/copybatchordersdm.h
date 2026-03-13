#ifndef GUARD_COPYBATCHORDERSDM_H
#define GUARD_COPYBATCHORDERSDM_H

#include <libbasardbaspect_accessor.h>
#include "searchyiteratorptr.h"

namespace domMod 
{
	class CopyBatchOrdersDM 
	{
	public:
		CopyBatchOrdersDM();
		~CopyBatchOrdersDM();

		void init(const basar::db::aspect::ConnectionRef activeConn);
		void shutdown();
		bool copyBatchOrders(const basar::db::aspect::AccessorPropertyTable_YIterator yit);

        basar::db::aspect::AccessorPropertyTableRef getOrderHeadsWhichShouldBeArchived(const basar::db::aspect::AccessorPropertyTable_YIterator yit);
        basar::db::aspect::AccessorPropertyTableRef getOrderPositionsWhichShouldBeArchived(const basar::db::aspect::AccessorPropertyTable_YIterator yit);

		virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit();

	private:
		CopyBatchOrdersDM(const CopyBatchOrdersDM &r);
		CopyBatchOrdersDM &operator = (const CopyBatchOrdersDM &r);
		
		searchyiterator::SearchYIteratorPtr			getSearchYIteratorPtr();
		searchyiterator::SearchYIteratorPtr         m_SearchYIterator;
			
		basar::db::aspect::AccessorInstanceRef	m_CopyBatchOrdersACC;	//!< instance of accessor BatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		basar::Int32 m_Initialized;	//!< counter of init-calls
	};
}

#endif 
