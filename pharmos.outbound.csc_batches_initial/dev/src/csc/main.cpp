#include <libbasarguitie.h>
#include <libbasarcmnutil.h>
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "cscbatchesapplctrl.h"
#include <libutil/exceptions/useradminfailureexception.h>

int main( int argc, char *argv[] )
{
    try
    {
        CSC_Batches::ApplCtrlImpl& applCtr = CSC_Batches::ApplCtrl::getInstance();
        applCtr.init( argc, argv );
        applCtr.run();
        applCtr.shutdown();
        CSC_Batches::ApplCtrl::clear();
    }
    catch( libutil::exceptions::UserAdminFailureException& e )
    {
        const basar::VarString& reason = e.reason();
        // fixme workaround -> this is not an error
        if( reason.compare( 0, 28, "ERROR: No valid user log in:" ) != 0 )
        {
            BLOG_ERROR( libabbauw::LoggerPool::loggerMain, e.what() );
            basar::gui::tie::warningMsgBox(0, reason.c_str() );
        }
    }
    catch( basar::Exception& e )
    {
        BLOG_ERROR( libabbauw::LoggerPool::loggerMain, e.what() );
        basar::gui::tie::warningMsgBox(0, e.what().c_str() );
    }

	return 0;
}
