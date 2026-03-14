#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERCOLLECTIONDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include <libbasardbaspect_connectionref.h>
#include <libbasardbaspect_accessor.h>
#include <libutil/misc/componentinitialization.hpp>
#include "dmbase.h"

#include "icustomercollectionfinder.h"
#include "customercollectioninitparams.h"

namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace customer
{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
    class CustomerCollectionDM : public libutil::misc::ComponentInitialization< CustomerCollectionInitParams, CustomerCollectionDM >,
                                 public ICustomerCollectionFinder,
                                 private DMBase
	{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
	    public:
		    CustomerCollectionDM( const basar::db::aspect::ConnectionRef );
		    ~CustomerCollectionDM();

			// from ICustomerCollectionFinder
			void                                                findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
            basar::db::aspect::AccessorPropertyTableRef			get();
            basar::db::aspect::AccessorPropertyTableRef			get() const;
            bool                                                isEmpty() const;

            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit();

			// from DMBase
			AccessorExecuteParameterization						getFindByPatternExecuteParameters() const;
            basar::VarString									getSearchYIteratorAttributeNames()  const;

        protected:
	            void doInit( const CustomerCollectionInitParams& );
	            void doShutdown();

	    private:
		    CustomerCollectionDM(const CustomerCollectionDM& CustomerCollectionDM);
		    CustomerCollectionDM& operator= (const CustomerCollectionDM& CustomerCollectionDM);
            
        private:
            CustomerCollectionInitParams                        m_InitParams;
	};

} // end namespace customer
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERCOLLECTIONDM_H

