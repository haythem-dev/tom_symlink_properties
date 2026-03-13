#ifndef GUARD_CSC_BATCHES_VIEWCONN_IORDERRELEASEINFORMATIONVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_IORDERRELEASEINFORMATIONVC_H

#include <libbasarcmnutil_bstring.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace gui
    {
        namespace tie
        {
            enum WidgetReturnEnum;
        }
    }
}

namespace CSC_Batches
{
namespace viewConn
{

class IOrderReleaseInformationVC
{
    public:	
        virtual ~IOrderReleaseInformationVC(){};
        virtual void                                                        setNumberOfOrders( basar::Int32 )                              = 0;
		virtual basar::gui::tie::WidgetReturnEnum                           show()                                                         = 0;
        virtual void                                                        waitForEvents()                                                = 0;
        virtual bool								                        shutdown()                                                     = 0;
        
        virtual void                                                        printError( basar::Int32 orderNo, const basar::VarString& msg )= 0;
        virtual void                                                        printOrderLockedMsg( basar::Int32 orderNo )                    = 0;

        virtual void                                                        update()                                                       = 0;
};

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_IORDERRELEASEINFORMATIONVC_H
