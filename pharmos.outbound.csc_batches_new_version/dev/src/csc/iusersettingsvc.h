//-------------------------------------------------------------------------------------------------//
/*! \file 
  * \brief  
  * \author 
  * \date  
*/
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSC_BATCHES_VIEWCONN_IUSERSETTINGSVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_IUSERSETTINGSVC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie_msgboxes.h>
#include <libbasarguitie_definitions.h>

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

class IUserSettingsVC
{
    public:	
        virtual ~IUserSettingsVC(){};
        
		virtual basar::gui::tie::WidgetReturnEnum                           show()                                                    = 0;
        virtual void                                                        waitForEvents()                                           = 0;
		virtual bool								                        shutdown()                                                = 0;

		virtual void                                                        matchFrom()                                               = 0;
		virtual void                                                        matchTo()                                                 = 0;

        virtual void                                                        info( const basar::I18nString& )                          = 0;
        virtual void                                                        error( const basar::I18nString& )                         = 0;
        virtual const basar::gui::tie::MessageBoxButtonEnum                 question( const basar::I18nString& )                      = 0;
};

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_IUSERSETTINGSVC_H
