//-------------------------------------------------------------------------------------------------//
/*! \file 
  * \brief  
  * \author
  * \date   2014
*/
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSC_BATCHES_VIEWCONN_IARTICLEFINDERVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_IARTICLEFINDERVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie_definitions.h>
#include <libbasarguitie_msgboxes.h.>

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
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
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{

class IArticleFinderVC
{
    public:	
        virtual ~IArticleFinderVC(){}

		virtual basar::gui::tie::WidgetReturnEnum            show()                                                          = 0;
        virtual bool                                         shutdown()                                                      = 0;
		
        virtual void                                         info( const basar::I18nString& )                                = 0;
        virtual void                                         error( const basar::I18nString& )                               = 0;
        virtual const basar::gui::tie::MessageBoxButtonEnum  question( const basar::I18nString& )                            = 0;
        
		virtual void init( basar::db::aspect::AccessorPropertyTableRef )                                                     = 0;
        virtual void matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator )                             = 0;
		virtual void matchSearchResults()                                                                                    = 0;
        virtual void setMatchCode( const basar::VarString& matchCode )                                                       = 0;
};

} // viewConn
} // CSC_Batches

#endif
