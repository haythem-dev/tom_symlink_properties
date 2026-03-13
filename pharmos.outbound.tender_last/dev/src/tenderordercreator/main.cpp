//----------------------------------------------------------------------------
/*! \file
 *  \brief  Should have the same, short and consistent structure / layout.
 *  \author
 *  \date
 */
//----------------------------------------------------------------------------
#include "infrastructure/tenderordercreatorapplctrl.h"
/*
#ifndef WIN32
#include "gitversioninfo.h"
#endif
*/
//----------------------------------------------------------------------------

extern "C" {
	extern const char versioninfo[];
}

//----------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	basar::cmnutil::CmdLineTokenizer args;
	args.setCmdLine(argc, argv);

	if (args.isParam("-version"))
	{
		std::cout << versioninfo << std::endl;
        /*
#ifndef WIN32
		std::cout << GitVersionInfo::instance() << std::endl;
#endif
        */
		exit( 0 );
	}

    int result = 0;
    try
    {
        tenderordercreator::ApplCtrl::getInstance().init( argc, argv );
        tenderordercreator::ApplCtrl::getInstance().run();
        tenderordercreator::ApplCtrl::getInstance().shutdown();
        tenderordercreator::ApplCtrl::clear();
    }
    catch( ... )
    {
        result = 99;
    }

    return result;
}
