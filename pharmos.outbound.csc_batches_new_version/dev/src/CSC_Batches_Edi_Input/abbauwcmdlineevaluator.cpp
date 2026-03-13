//--------------------------------------------------------------------------------------------------//
/*! \brief 
 *  \author     
 *  \date       
 *  \version    
 */ //--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "abbauwcmdlineevaluator.h"
#include "classinfo_macros.h"
#include "abbauwcmdlineoption_definitions.h"
#include <libbasarcmnutil.h>
#include <sstream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
namespace cmdLine
{

AbbaUWCmdLineEvaluator::AbbaUWCmdLineEvaluator( const libBatch::cmdLine::CmdLineConfiguration& cmdLineConfiguration )
: libBatch::cmdLine::CmdLineEvaluator( cmdLineConfiguration )
{
    METHODNAME_DEF( CommandLineParser, CommandLineParser ); (void) fun;
}

AbbaUWCmdLineEvaluator::~AbbaUWCmdLineEvaluator()
{
}

bool AbbaUWCmdLineEvaluator::evaluateAppArgs()
{
    METHODNAME_DEF( CommandLineParser, evaluateAppArgs ); (void) fun;

	// ToDo perhaps kes: In these methods we could check for the validity of the arguments, then we could omit these checks during processing later on.
    readInCountryCode();
	readTransferDir();
	readInputDir();
	readForBranchnos();

    return m_AppArgs.isInitialized();
}

const AppArgs& AbbaUWCmdLineEvaluator::getAppArgs() const
{
    assert( m_AppArgs.isInitialized() );
    return m_AppArgs;
}

void AbbaUWCmdLineEvaluator::readInCountryCode()
{
    m_CmdLineParser.getMandatoryVarString( cmdLine::COUNTRY_CODE, m_AppArgs.m_CountryCode );
}

void AbbaUWCmdLineEvaluator::readTransferDir()
{
	m_CmdLineParser.getMandatoryVarString( cmdLine::TRANSFER_DIR, m_AppArgs.m_TransferDir);
}

void AbbaUWCmdLineEvaluator::readInputDir()
{
	m_CmdLineParser.getMandatoryVarString( cmdLine::INPUT_DIR, m_AppArgs.m_InputDir);
}

void AbbaUWCmdLineEvaluator::readForBranchnos()
{
	// Parameter is optional
	if (   m_CmdLineParser.isParam(cmdLine::FOR_BRANCHNOS))
	{
		m_CmdLineParser.getMandatoryVarString( cmdLine::FOR_BRANCHNOS, m_AppArgs.m_ForBranchnos);
	}
	else
	{
		m_AppArgs.m_ForBranchnos = "";
	}
}

} // end namespace cmdLine
} // end namespace abbaUW
