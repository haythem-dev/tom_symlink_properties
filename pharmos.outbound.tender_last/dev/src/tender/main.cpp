#include "tenderapplctrl.h"

#include <libutil/exception.h>
#include <libbasarguitie.h>

int main( int argc, char *argv[] )
{
    int result = 0; // Think about a class representing all possible return values and the corresponding texts: tender::tenderApplCtrl::OK;
    try
    {
        /* alternative call:
            ==> "tender::tenderApplCtrl::init( argc, argv );" <==
            Here the static method 'getInstance()' is called internally.
            Idea is to offer more readable methods.
        */
        tender::ApplCtrl::getInstance().init( argc, argv );
        tender::ApplCtrl::getInstance().run();
        tender::ApplCtrl::getInstance().shutdown();
        tender::ApplCtrl::clear();

        //result = tender::ApplCtrl::OK_STATUS;
    }
	//TODO find better solution when login window is closed!
	catch( const libutil::exceptions::UserAdminFailureException&  )
	{
	}	
	catch( const basar::Exception & except ) 
    { 
            basar::VarString msg; 
            msg.format( "\n" 
                    "=== basar exception caught ===========================================================\n" 
                    "\n" 
                    "%s\n", 
                    except.what().c_str() ); 
			basar::gui::tie::warningMsgBox( 0, msg.c_str() );
    } 
    catch( const std::exception & ) 
    { 
            basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====" ); 
    } 
    catch( ... )
    {
        //result = tender::ApplCtrl::UNKNOWN_EXCEPTION_CAUGHT;
        result = 99;
    }

    return result;
}
