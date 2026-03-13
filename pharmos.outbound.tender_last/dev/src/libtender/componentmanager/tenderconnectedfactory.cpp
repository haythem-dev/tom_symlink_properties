//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include "tenderconnectedfactory.h"

#include <domainmodule/tender/tenderdm.h>

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace libtender
{
    namespace componentManager
    {
        TenderConnectedFactory::TenderConnectedFactory()
        {
        }

        TenderConnectedFactory::~TenderConnectedFactory()
        {
        }
        
        domMod::tender::ITenderPtr TenderConnectedFactory::create( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch ) const
        {
            domMod::tender::ITenderPtr tender = TenderFactoryBase::create();

            try
            {
				if(!yitSearch.isNull() && !yitSearch.isEnd()) //prevent error
				{
					tender->findByKey( yitSearch );
				}
            }
            catch( basar::Exception & /*e*/  )
            {
				// TODO How to create empty Tender?
                //throw;
            }
            
            return tender;
        }
    }
}
