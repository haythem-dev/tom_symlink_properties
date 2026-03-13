    //-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief 
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmndm.h"

#include "manufacturerdm.h"
#include "manufacturerdm_definitions.h"
#include "manufactureracc.h"

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace manufacturerdm
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    ManufacturerDM::ManufacturerDM( const basar::db::aspect::ConnectionRef conn )
		: DMBase( conn, lit::ACC_MANUFACTURER, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerManufacturer )
	{
        METHODNAME_DEF( ManufacturerDM, ManufacturerDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		acc_manufacturer::SelectManufacturer dummy;
	}

	ManufacturerDM::~ManufacturerDM()
	{
        METHODNAME_DEF( ManufacturerDM, ~ManufacturerDM )
        BLOG_TRACE_METHOD(getLogger(), fun );

        shutdown();
	}

    void ManufacturerDM::doInit(const ManufacturerInitParams&  params )
    {
	    METHODNAME_DEF( ManufacturerDM, init )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		m_InitParams = params;       
    }

	AccessorExecuteParameterization ManufacturerDM::getFindByPatternExecuteParameters() const
	{
		return AccessorExecuteParameterization( lit::SELECT_MANUFACTURER );
	}

	basar::VarString ManufacturerDM::getSearchYIteratorAttributeNames() const
    {
		return libabbauw::properties::MANUFACTURERNO.toDescriptionString() +
               libabbauw::properties::MATCHCODE.toDescriptionString()      +
               libabbauw::properties::BRANCHNO.toDescriptionString();
	}

    void ManufacturerDM::doShutdown()
    {
	    METHODNAME_DEF( ManufacturerDM, doShutdown )
        BLOG_TRACE_METHOD(getLogger(), fun );
    }
	
	void ManufacturerDM::findManufacturer( basar::db::aspect::AccessorPropertyTable_YIterator yIt )
	{
		METHODNAME_DEF( ManufacturerDM, findManufacturer )
        BLOG_TRACE_METHOD(getLogger(), fun );

		basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
		yitSearchTmp.setInt16( libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
		DMBase::findByPattern( yitSearchTmp );
	}

	basar::db::aspect::AccessorPropertyTable_YIterator ManufacturerDM::getSearchYit()
	{
		return DMBase::getSearchYit();
	}

    void ManufacturerDM::resetSearchYit()
    {
	    DMBase::resetSearchYit();
    }

	basar::db::aspect::AccessorPropertyTableRef	ManufacturerDM::getPropTab()
	{
		return DMBase::getPropTab();
	}

    void ManufacturerDM::clear()
    {
        DMBase::getPropTab().clear();
    }

	const libutil::misc::ClassInfo & ManufacturerDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "ManufacturerDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

} // end namespace manufacturerdm
} // end namespace domMod
} // end namespace libabbauw
