#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMERDM_ICUSTOMERCOLLECTIONFINDER_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMERDM_ICUSTOMERCOLLECTIONFINDER_H

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
    class ICustomerCollectionFinder
    {
        public:	
            virtual ~ICustomerCollectionFinder(){}

            virtual void			                                    findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
            virtual basar::db::aspect::AccessorPropertyTableRef			get()                                                             = 0;
            virtual basar::db::aspect::AccessorPropertyTableRef			get() const                                                       = 0;
            virtual bool                                                isEmpty() const                                                   = 0;
            
            virtual void                                                resetSearchYit()                                                  = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit()                                                    = 0;
    };
} 
} 
}

#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMERDM_ICUSTOMERCOLLECTIONFINDER_H
