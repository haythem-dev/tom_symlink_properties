#ifndef GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_MANUFACTURERDM_H
#define GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_MANUFACTURERDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ifindmanufacturer.h"
#include "manufacturerinitparams.h"

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
namespace manufacturerdm
{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
    class ManufacturerDM : public libutil::misc::ComponentInitialization< ManufacturerInitParams, ManufacturerDM >,
		                   public IFindManufacturer,
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
		    ManufacturerDM( const basar::db::aspect::ConnectionRef );
		    ~ManufacturerDM();

            // defined in ifindmanufacturer interface
			void                                                        findManufacturer( basar::db::aspect::AccessorPropertyTable_YIterator );
	        void                                                        resetSearchYit();
	        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();
	        basar::db::aspect::AccessorPropertyTableRef                 getPropTab();
            void                                                        clear();

        protected:
            // from DMBase
            AccessorExecuteParameterization						        getFindByPatternExecuteParameters() const;
            basar::VarString									        getSearchYIteratorAttributeNames()  const;
	        
            // from ComponentInitialization
            void doInit( const ManufacturerInitParams& );
	        void doShutdown();

	    private:
		
            /////////////////////////////////////////////////////////////////////////
		    // private member methods
		    /////////////////////////////////////////////////////////////////////////
		    ManufacturerDM(const ManufacturerDM& ManufacturerDM);
		    ManufacturerDM& operator= (const ManufacturerDM& ManufacturerDM);
            
		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////
            ManufacturerInitParams						m_InitParams;
	};
} // end namespace manufacturerdm
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_MANUFACTURERDM_H
