#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERDM_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "icustomerfinder.h"
#include "customerinitparams.h"

#include <libutil/misc/componentinitialization.hpp>
#include "dmbase.h"

namespace basar
{
	namespace db
	{
		namespace aspect
		{
            class ConnectionRef;
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
    class CustomerDM : public libutil::misc::ComponentInitialization< CustomerInitParams, CustomerDM >,
                       public ICustomerFinder,
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
            CustomerDM( const basar::db::aspect::ConnectionRef );
            ~CustomerDM();

            // from ICustomerFinder
            void                                                findByKey( basar::Int32 pharamcyNo, bool showLocked = false );
            basar::db::aspect::AccessorPropertyTable_YIterator  get();
            basar::db::aspect::AccessorPropertyTable_YIterator  get() const;
            bool                                                isEmpty() const;

            bool                                                isLocked() const;

            void                                                resetSearchYit();
            basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit();

            bool                                                isKnown() const;

            // from DMBase
            AccessorExecuteParameterization                     getFindByPatternExecuteParameters() const;
            basar::VarString                                    getSearchYIteratorAttributeNames()  const;

        protected:
            void doInit( const CustomerInitParams& );
            void doShutdown();

        private:
            CustomerDM( const CustomerDM& );
            CustomerDM& operator = ( const CustomerDM& );

        private:
            CustomerInitParams                                  m_InitParams;
	};

} // end namespace customer
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERDM_H

