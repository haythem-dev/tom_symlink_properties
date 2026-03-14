#ifndef GUARD_ORDERPOSPROTOCOLDM_H
#define GUARD_ORDERPOSPROTOCOLDM_H

#include <libbasar_definitions.h>
#include <libbasardbaspect_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasardbaspect_connectionref.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_bstring.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarcmnutil_parameterlist.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include "loggerpool.h"
//#include <libbasarlogin.h>
#include <libbasardbaspect.h>
#include "searchyiteratorptr.h"
#include "searchyiterator.h"

namespace domMod 
{
	class OrderPosProtocolDM 
    {
	    public:
	    	OrderPosProtocolDM();
	    	~OrderPosProtocolDM();

	    	void init(const basar::db::aspect::ConnectionRef activeConn);
	    	void shutdown();

            bool saveOrderPosProtocol();
            bool deleteOrderPosProtocol(basar::db::aspect::AccessorPropertyTable_YIterator yit);
            void setBackup(basar::db::aspect::AccessorPropertyTableRef);

            virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit();

	    private:
	    	OrderPosProtocolDM(const OrderPosProtocolDM &r);
	    	OrderPosProtocolDM &operator = (const OrderPosProtocolDM &r);
	    	
	    	basar::db::aspect::AccessorInstanceRef	    m_Accessor;	//!< instance of accessor BatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
	    	basar::Int32                                m_Initialized;	//!< counter of init-calls

            basar::db::aspect::AccessorPropertyTableRef m_BackUp;

            searchyiterator::SearchYIteratorPtr			getSearchYIteratorPtr();
		    searchyiterator::SearchYIteratorPtr         m_SearchYIterator;
	};
} 

#endif
