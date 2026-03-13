#ifndef GUARD_DOMMOD_LIBABBAUW_ORDERPOSCOLLECTIONPROTOCOLDM_H
#define GUARD_DOMMOD_LIBABBAUW_ORDERPOSCOLLECTIONPROTOCOLDM_H

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasarappl_systemeventmanager.h>
#include "loggerpool.h"
#include <libbasarlogin.h>
#include <libbasardbaspect.h>

using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::cmnutil::ParameterList;


namespace domMod
{
	namespace libAbbauw
	{
		namespace orderProtocol
		{
			class OrderPosCollectionProtocolDM 
			{

                public:
					OrderPosCollectionProtocolDM();									
					~OrderPosCollectionProtocolDM();
            
					// This method inits the module, creates AccessorInstances for the given connection
					void init(const ConnectionRef activeConn);
            
					// call this function to clear all accessor instances and the contents of property tabels
					void shutdown();
            
                    void protocolCreateOrder(basar::db::aspect::AccessorPropertyTable_YIterator);
            
					
                    basar::db::aspect::AccessorPropertyTable_YIterator getPos();

			    private:

				    OrderPosCollectionProtocolDM(const OrderPosCollectionProtocolDM &r);				// Copy-Konstruktor : abschalten
				    OrderPosCollectionProtocolDM &operator = (const OrderPosCollectionProtocolDM &r);	// Assignment operator : abschalten 

				    //---------------------------------------------------------------------------
				    // accessor instances ( created in init() )
				    //---------------------------------------------------------------------------
				    AccessorInstanceRef		m_Accessor;	    //!< instance of accessor , wird als Ref-Klasse automatisch freigegeben
				    basar::Int32			m_Initialized;	//!< counter of init-calls
			};
		
		} // namespace orderProtocol
	} // namespace libAbbauw
} // namespace domMod

#endif // GUARD_DOMMOD_LIBABBAUW_ORDERPOSCOLLECTIONPROTOCOLDM_H
