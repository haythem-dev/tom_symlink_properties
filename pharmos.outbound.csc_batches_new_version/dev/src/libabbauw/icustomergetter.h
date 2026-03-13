#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERGETTER_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERGETTER_H

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
    class ICustomerGetter
    {
        public:	
            virtual ~ICustomerGetter(){}

            virtual basar::db::aspect::AccessorPropertyTable_YIterator  get() const                                                       = 0;
            virtual bool                                                isEmpty() const                                                   = 0;

            virtual bool                                                isKnown() const                                                   = 0;
    };
}
}
}
#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERGETTER_H
