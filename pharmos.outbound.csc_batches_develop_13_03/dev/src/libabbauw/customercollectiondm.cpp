//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmndm.h"

#include "customercollectiondm.h"
#include "customercollectiondm_definitions.h"

#include "customercollectionacc.h"

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace customer
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    CustomerCollectionDM::CustomerCollectionDM( const basar::db::aspect::ConnectionRef conn )
        : DMBase( conn, ACC_CUSTOMER_COLLECTION, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerCustomer )
	{
        METHODNAME_DEF( CustomerCollectionDM, CustomerCollectionDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		acc_customercollection::SelectCustomerCollection dummy;
	}

	CustomerCollectionDM::~CustomerCollectionDM()
	{
        METHODNAME_DEF( CustomerCollectionDM, ~CustomerCollectionDM )
        BLOG_TRACE_METHOD(getLogger(), fun );

        shutdown();
	}

    void CustomerCollectionDM::doInit(const CustomerCollectionInitParams&  params )
    {
	    METHODNAME_DEF( CustomerCollectionDM, init )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		m_InitParams = params;
    }

    void CustomerCollectionDM::doShutdown()
    {
	    METHODNAME_DEF( CustomerCollectionDM, doShutdown )
        BLOG_TRACE_METHOD(getLogger(), fun );
    }

	AccessorExecuteParameterization CustomerCollectionDM::getFindByPatternExecuteParameters() const
	{
        METHODNAME_DEF( CustomerCollectionDM, getFindByPatternExecuteParameters )
	    BLOG_TRACE_METHOD( getLogger(), fun );

        return AccessorExecuteParameterization( SELECT_CUSTOMER_COLLECTION );
	}

	basar::VarString CustomerCollectionDM::getSearchYIteratorAttributeNames() const
	{
		return libabbauw::properties::PHARMACYNO.toDescriptionString() + 
               libabbauw::properties::MATCHCODE.toDescriptionString()  + 
		       libabbauw::properties::BRANCHNO.toDescriptionString();
	}
	
	void CustomerCollectionDM::findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yIt )
	{
		METHODNAME_DEF( CustomerCollectionDM, findByPattern )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
		yitSearchTmp.setInt16( libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
		DMBase::findByPattern( yitSearchTmp );
	}

	basar::db::aspect::AccessorPropertyTableRef	CustomerCollectionDM::get()
	{
		return DMBase::getPropTab();
	}

	basar::db::aspect::AccessorPropertyTableRef	CustomerCollectionDM::get() const
	{
		return DMBase::getPropTab();
	}

    bool CustomerCollectionDM::isEmpty() const
    {
        return DMBase::isEmpty();
    }

    void CustomerCollectionDM::resetSearchYit()
    {
	    DMBase::resetSearchYit();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator CustomerCollectionDM::getSearchYit()
	{
		return DMBase::getSearchYit();
	}

	const libutil::misc::ClassInfo & CustomerCollectionDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "CustomerCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

} // end namespace customer
} // end namespace domMod
} // end namespace libabbauw
