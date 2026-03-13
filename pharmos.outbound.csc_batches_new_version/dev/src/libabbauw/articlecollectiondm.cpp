//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmndm.h"

#include "articlecollectiondm.h"
#include "articlecollectiondm_definitions.h"

#include "articlecollectionacc.h"

#include <boost/algorithm/string.hpp>
//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace article
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    const libutil::misc::ClassInfo & ArticleCollectionDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "ArticleCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    ArticleCollectionDM::ArticleCollectionDM( const basar::db::aspect::ConnectionRef conn )
        : DMBase( conn, lit::ACC_ARTICLE, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerArticle )
	{
        METHODNAME_DEF( ArticleCollectionDM, ArticleCollectionDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		acc_article::SelectArticle dummy;
	}

	ArticleCollectionDM::~ArticleCollectionDM()
	{
        METHODNAME_DEF( ArticleCollectionDM, ~ArticleCollectionDM )
        BLOG_TRACE_METHOD(getLogger(), fun );

        shutdown();
	}

    void ArticleCollectionDM::doInit(const ArticleInitParams&  params )
    {
	    METHODNAME_DEF( ArticleCollectionDM, init )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		m_InitParams = params;       
    }

	AccessorExecuteParameterization ArticleCollectionDM::getFindByPatternExecuteParameters() const
	{
		return AccessorExecuteParameterization( lit::SELECT_ARTICLE );
	}

	basar::VarString ArticleCollectionDM::getSearchYIteratorAttributeNames() const
    {
        return properties::BRANCHNO.toDescriptionString()                +
               properties::PZN.toDescriptionString()                     +
               properties::MATCHCODE.toDescriptionString()               +
			   properties::EAN.toDescriptionString()                     +
               properties::MANUFACTURERNO.toDescriptionString()          +
               properties::AGP.toDescriptionString()                     +
               properties::GEP.toDescriptionString()                     +
               properties::PURCHASEORDERSUPPLIERNO.toDescriptionString() ;
	}

    void ArticleCollectionDM::doShutdown()
    {
	    METHODNAME_DEF( ArticleCollectionDM, doShutdown )
        BLOG_TRACE_METHOD(getLogger(), fun );
    }
	
	void ArticleCollectionDM::findArticle( basar::db::aspect::AccessorPropertyTable_YIterator yIt )
	{
		METHODNAME_DEF( ArticleCollectionDM, findArticle )
        BLOG_TRACE_METHOD(getLogger(), fun );

		basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
		yitSearchTmp.setInt16( libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );

     if( yitSearchTmp.isContainedAndSet( properties::MATCHCODE ) )
        {
            //percent - sign is valid in the Matchcode-db field
            basar::VarString matchcode( yitSearchTmp.getString( properties::MATCHCODE ) );
            boost::replace_all( matchcode, "%", "\\%" );
            // replace % with \% to avoid getting a wildcard-sign in SQL
            yitSearchTmp.setString( properties::MATCHCODE, matchcode);
        }


     DMBase::findByPattern( yitSearchTmp );
	}

	basar::db::aspect::AccessorPropertyTable_YIterator ArticleCollectionDM::getSearchYit()
	{
		return DMBase::getSearchYit();
	}

    void ArticleCollectionDM::resetSearchYit()
    {
	    DMBase::resetSearchYit();
    }

	basar::db::aspect::AccessorPropertyTableRef	ArticleCollectionDM::get()
	{
		return DMBase::getPropTab();
	}

    void ArticleCollectionDM::clear()
    {
        DMBase::getPropTab().clear();
    }

} // end namespace article
} // end namespace domMod
} // end namespace libabbauw
