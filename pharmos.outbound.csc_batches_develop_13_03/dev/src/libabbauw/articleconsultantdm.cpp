//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmndm.h"

#include "articleconsultantdm.h"
#include "articleconsultantdm_definitions.h"

#include "articleconsultantacc.h"

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
    ArticleConsultantDM::ArticleConsultantDM( const basar::db::aspect::ConnectionRef conn )
		: DMBase( conn, ACC_ARTICLECONSULTANT, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerArticle )
	{
        METHODNAME_DEF( ArticleConsultantDM, ArticleConsultantDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

	    acc_articleconsultant::SelectPriceForArticleNo dummy;
	}

	ArticleConsultantDM::~ArticleConsultantDM()
	{
        METHODNAME_DEF( ArticleConsultantDM, ~ArticleConsultantDM )
        BLOG_TRACE_METHOD(getLogger(), fun );

        shutdown();
	}

    void ArticleConsultantDM::doInit(const ArticleConsultantInitParams&  params )
    {
	    METHODNAME_DEF( ArticleConsultantDM, init )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
		m_InitParams = params;
    }

	AccessorExecuteParameterization ArticleConsultantDM::getFindByPatternExecuteParameters() const
	{
        METHODNAME_DEF( ArticleConsultantDM, getFindByPatternExecuteParameters )
	    BLOG_TRACE_METHOD( getLogger(), fun );

		return AccessorExecuteParameterization( SELECT_ARTICLECONSULTANT );
	}

	basar::VarString ArticleConsultantDM::getSearchYIteratorAttributeNames() const
	{
		return properties::ARTICLENO.toDescriptionString();
	}

    void ArticleConsultantDM::doShutdown()
    {
	    METHODNAME_DEF( ArticleConsultantDM, doShutdown )
        BLOG_TRACE_METHOD(getLogger(), fun );
    }
	
	void ArticleConsultantDM::findArticlePrice( basar::Int32 articleno )
	{
		METHODNAME_DEF( ArticleConsultantDM, findArticlePrice )
        BLOG_TRACE_METHOD(getLogger(), fun );

		resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator yitsearch =  getSearchYit();
        yitsearch.setInt32( properties::ARTICLENO, articleno);
		DMBase::findByPattern( yitsearch );
	}

    void ArticleConsultantDM::resetSearchYit()
    {
	    DMBase::resetSearchYit();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator ArticleConsultantDM::getSearchYit()
	{
		return DMBase::getSearchYit();
	}

    basar::db::aspect::AccessorPropertyTable_YIterator ArticleConsultantDM::get()
    {
        return DMBase::getPropTab().begin();
    }

	const libutil::misc::ClassInfo & ArticleConsultantDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "ArticleConsultantDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

} // end namespace article
} // end namespace domMod
} // end namespace libabbauw
