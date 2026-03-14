//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief Domain module article
 *  \author 
 *  \date 
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "dmbase.h"
#include "cmndm.h"

#include <libutil/accessor/accessorproxy.h>
#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include <libutil/exceptions/accessorexecutionexception.h>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
    //--------------------------------------------------------------------------------------------------//
    // static definition section
    //--------------------------------------------------------------------------------------------------//
	const libutil::misc::ClassInfo& DMBase::getClassInfo()
    {
		static const libutil::misc::ClassInfo info( "DMBase", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
	DMBase::DMBase( const basar::db::aspect::ConnectionRef conn, const basar::VarString& accDefName, log4cplus::Logger& logger )
    : m_Logger( logger ), m_Accessor( new libutil::infrastructure::accessor::AccessorProxy( accDefName, conn ) )
	{
        METHODNAME_DEF( DMBase, DMBase )
        BLOG_TRACE_METHOD( getLogger(), fun );
	}

	DMBase::DMBase( const basar::db::aspect::ConnectionRef conn, const basar::VarString& accDefName, const basar::db::aspect::CachingPolicyEnum eCachePolicy, log4cplus::Logger& logger )
		: m_Logger( logger ), m_Accessor( new libutil::infrastructure::accessor::AccessorProxy( accDefName, conn, eCachePolicy ) )
	{
        METHODNAME_DEF( DMBase, DMBase )
        BLOG_TRACE_METHOD( getLogger(), fun );
	}

	DMBase::~DMBase()
	{
        METHODNAME_DEF( DMBase, ~DMBase )
        BLOG_TRACE_METHOD( getLogger(), fun );
	}
	
	//-------------------------------------------------------------------------------------------------------------------//
	libutil::infrastructure::accessor::AccessorProxyPtr DMBase::getAccessor() 
	{	
		return m_Accessor;
	}

    libutil::infrastructure::accessor::AccessorProxyPtr DMBase::getAccessor() const
	{	
		return m_Accessor;
	}

    basar::db::aspect::AccessorPropertyTableRef DMBase::getPropTab()
    {
        return getAccessor()->getPropertyTable();
    }
    
    basar::db::aspect::AccessorPropertyTableRef DMBase::getPropTab() const
    {
        return getAccessor()->getPropertyTable();
    }

    bool DMBase::isEmpty() const
    {
        // Basar Bug: get().isEmpty() doesn't work here!
        return getPropTab().begin() == getPropTab().end();
    }

    void DMBase::resetSearchYit()
    {
        if( NULL != m_SearchYIterator.get() ){
            m_SearchYIterator->reset();
        }
    }
    
    basar::db::aspect::AccessorPropertyTable_YIterator DMBase::getSearchYit()
    {
        return getSearchYIterator()->get();
    }

    libutil::domMod::SearchYIteratorPtr DMBase::getSearchYIterator()
    {
        //using namespace properties;
		using libutil::domMod::SearchYIteratorPtr;
        using libutil::domMod::SearchYIterator;
        if( NULL == m_SearchYIterator.get() ){
            m_SearchYIterator = SearchYIteratorPtr( new SearchYIterator(getSearchYIteratorAttributeNames()) );
        }

        return m_SearchYIterator;
    }

	void DMBase::evaluatePreconditionFindByPattern()
	{
        METHODNAME_DEF( DMBase, evaluatePreconditionFindByPattern )
	    BLOG_TRACE_METHOD( getLogger(), fun );
	}

    void DMBase::executeAccessor( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch, const AccessorExecuteParameterization& exeParams )
    {
        METHODNAME_DEF( DMBase, executeAccessor )
	    BLOG_TRACE_METHOD( getLogger(), fun );

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
	    //try {
			executeResultInfo = getAccessor()->execute( exeParams.m_AccessMethod, yitSearch, exeParams.m_ClearBeforeSelect, exeParams.m_FlagExcept, exeParams.m_DBHint );
        /*} catch( basar::Exception& e ){
            ::util::ExceptionHandler::processAccessorException( e, m_CSC_Logger->getLogger(), fun, exeParams.m_AccessMethod, __FILE__, __LINE__ );
	    }*/

        if( executeResultInfo.hasError() ) {            
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, exeParams.m_AccessMethod, executeResultInfo );
	    }
    }

    void DMBase::findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( DMBase, findByPattern )
	    BLOG_TRACE_METHOD( getLogger(), fun );
		
        evaluatePreconditionFindByPattern();
        executeAccessor( yitSearch, getFindByPatternExecuteParameters() );
    }

    void DMBase::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( DMBase, findByKey )
	    BLOG_TRACE_METHOD( getLogger(), fun );
		
        evaluatePreconditionFindByPattern();
        executeAccessor( yitSearch, getFindByKeyExecuteParameters() );
    }

    log4cplus::Logger& DMBase::getLogger() const
    {
        return m_Logger;
    }

} // end namespace domMod
} // end namespace libabbauw
