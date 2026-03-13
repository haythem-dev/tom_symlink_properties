#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERFINDER_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERFINDER_H

#include "icustomergetter.h"

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
        }
    }
}

//----------------------------------------------------------------------------//
// class declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace customer
{
    class ICustomerFinder : public ICustomerGetter
    {
        public:	
            virtual ~ICustomerFinder(){}

            virtual void			                                    findByKey( basar::Int32 pharmacyNo, bool showLocked = false )     = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator  get()                                                             = 0;

            virtual void                                                resetSearchYit()                                                  = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit()                                                    = 0;

            virtual bool                                                isLocked() const                                                  = 0;
    };
}
}
}
#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERFINDER_H
