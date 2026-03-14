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
#include "loggerpool.h"

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
    METHODNAME_DEF( CommandLineParser, CommandLineParser )
	BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW,fun );
}

AbbaUWCmdLineEvaluator::~AbbaUWCmdLineEvaluator()
{
}

bool AbbaUWCmdLineEvaluator::evaluateAppArgs()
{
    METHODNAME_DEF( CommandLineParser, evaluateAppArgs )
	BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW,fun );

    readInCountryCode();
	readInDB();
	readInServer();

    return m_AppArgs.isInitialized();
    //return true;
    /*
	ToDo kes: Cmdline-Args checken
	bool argsValid = m_AppArgs.isCountryCodeValid();		// Auf Gross- und Kleinbuchstaben checken
    argsValid &= m_AppArgs.isInputDirValid();

    return argsValid;*/
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

void AbbaUWCmdLineEvaluator::readInDB()
{
    m_CmdLineParser.getMandatoryVarString( cmdLine::DB, m_AppArgs.m_DB );
}

void AbbaUWCmdLineEvaluator::readInServer()
{
    m_CmdLineParser.getMandatoryVarString( cmdLine::SERVER, m_AppArgs.m_Server );
}



} // end namespace cmdLine
} // end namespace abbaUW
