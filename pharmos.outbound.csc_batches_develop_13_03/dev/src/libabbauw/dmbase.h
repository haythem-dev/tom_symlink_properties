//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief Domain module article
 *  \author 
 *  \date 
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_DMBASE_H
#define GUARD_DOMMOD_DMBASE_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/classinfo.h>
#include <libbasardbaspect_definitions.h>
#include <libutil/accessor/accessorproxy.h>
#include <libutil/accessor/accessorproxyptr.h>
#include <libutil/dbtransaction.h>
#include <libutil/accessor/throw_accessor_execution_exception_macro_definition.h>
#include "accessorexecuteparameterization.h"
#include <libutil/searchyiterator.h>

//-------------------------------------------------------------------------------------------------//
// using declarations section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

namespace libabbauw
{
namespace domMod
{
    struct AccessorExecuteParameterization;
}
}

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
    /*!\brief  */
    class DMBase
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
		    virtual ~DMBase();
            
		    void                                                        resetSearchYit();
            basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

            void                                                        findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );
            void                                                        findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );
            basar::db::aspect::AccessorPropertyTableRef                 getPropTab();
			basar::db::aspect::AccessorPropertyTableRef                 getPropTab() const;

            bool                                                        isEmpty() const;

        protected:
            DMBase( const basar::db::aspect::ConnectionRef conn, const basar::VarString& accDefName, log4cplus::Logger& logger );
			DMBase( const basar::db::aspect::ConnectionRef conn, const basar::VarString& accDefName, const basar::db::aspect::CachingPolicyEnum eCachePolicy, log4cplus::Logger& logger );
            
            
            virtual AccessorExecuteParameterization						getFindByPatternExecuteParameters() const = 0;
            virtual AccessorExecuteParameterization						getFindByKeyExecuteParameters()     const {return AccessorExecuteParameterization("");}
            virtual basar::VarString									getSearchYIteratorAttributeNames()  const = 0;
            virtual void												evaluatePreconditionFindByPattern();
        
			
            void                                                        executeAccessor( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch, 
                                                                                         const AccessorExecuteParameterization& exeParas );
			libutil::infrastructure::accessor::AccessorProxyPtr		    getAccessor();
            libutil::infrastructure::accessor::AccessorProxyPtr		    getAccessor() const;
			log4cplus::Logger&							                getLogger() const;

	    private:
            /////////////////////////////////////////////////////////////////////////
		    // private member methods
		    /////////////////////////////////////////////////////////////////////////
		    DMBase(const DMBase& );
		    DMBase& operator=( const DMBase& );

			libutil::domMod::SearchYIteratorPtr                         getSearchYIterator();

		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////
			log4cplus::Logger&											m_Logger;
            libutil::domMod::SearchYIteratorPtr							m_SearchYIterator;
            libutil::infrastructure::accessor::AccessorProxyPtr         m_Accessor;
	};

} // end namespace domMod
} // end namespace libabbauw

#endif
