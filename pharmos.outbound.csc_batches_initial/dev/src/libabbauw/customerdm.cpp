//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmndm.h"

#include "customerdm.h"
#include "customerdm_definitions.h"

#include "customeracc.h"

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
    CustomerDM::CustomerDM( const basar::db::aspect::ConnectionRef conn )
        : DMBase( conn, ACC_CUSTOMER, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerCustomer )
	{
        METHODNAME_DEF( CustomerDM, CustomerDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		acc_customer::SelectCustomer dummy;
	}

	CustomerDM::~CustomerDM()
	{
        METHODNAME_DEF( CustomerDM, ~CustomerDM )
        BLOG_TRACE_METHOD(getLogger(), fun );

        shutdown();
	}

    void CustomerDM::doInit(const CustomerInitParams&  params )
    {
	    METHODNAME_DEF( CustomerDM, init )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		m_InitParams = params;
    }

    void CustomerDM::doShutdown()
    {
	    METHODNAME_DEF( CustomerDM, doShutdown )
        BLOG_TRACE_METHOD(getLogger(), fun );
    }

	AccessorExecuteParameterization CustomerDM::getFindByPatternExecuteParameters() const
	{
        METHODNAME_DEF( CustomerDM, getFindByPatternExecuteParameters )
	    BLOG_TRACE_METHOD( getLogger(), fun );

        return AccessorExecuteParameterization( SELECT_CUSTOMER );
	}

	basar::VarString CustomerDM::getSearchYIteratorAttributeNames() const
	{
		return libabbauw::properties::PHARMACYNO.toDescriptionString()    +
		       libabbauw::properties::BRANCHNO.toDescriptionString()      +
               libabbauw::properties::CUSTOMERLOCKED.toDescriptionString();
	}
	
	void CustomerDM::findByKey( basar::Int32 pharmacyNo, bool showLocked )
	{
		METHODNAME_DEF( CustomerDM, findByPattern )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = getSearchYit();
		yitSearchTmp.setInt16( properties::BRANCHNO,   m_InitParams.m_BranchNo );
        yitSearchTmp.setInt32( properties::PHARMACYNO, pharmacyNo              );

        if( ! showLocked )
        {
            yitSearchTmp.setInt16( properties::CUSTOMERLOCKED, 1 ); // true
        }

		DMBase::findByPattern( yitSearchTmp );
	}

    basar::db::aspect::AccessorPropertyTable_YIterator CustomerDM::get()
	{
        return DMBase::getPropTab().begin();
	}

    basar::db::aspect::AccessorPropertyTable_YIterator CustomerDM::get() const
    {
        return DMBase::getPropTab().begin();
    }

    bool CustomerDM::isEmpty() const
    {
        return DMBase::isEmpty();
    }

    void CustomerDM::resetSearchYit()
    {
	    DMBase::resetSearchYit();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator CustomerDM::getSearchYit()
	{
		return DMBase::getSearchYit();
	}
    
    bool CustomerDM::isKnown() const
	{
        bool known = ! isEmpty();
        if( known )
        {
            // compare pharmacyno with pseudo pharmacyno which is used if no customer has been found
            known = get().getInt32( properties::PHARMACYNO ) != lit::PSEUDO_PHARMACYNO;
        }

        return known;
	}

    bool CustomerDM::isLocked() const
    {
        return (! isEmpty() ) && get().getInt16( properties::CUSTOMERLOCKED ) > 0;
    }

	const libutil::misc::ClassInfo & CustomerDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "CustomerDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

} // end namespace customer
} // end namespace domMod
} // end namespace libabbauw
