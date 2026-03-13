//--------------------------------------------------------------------------------------------------//
/*! \brief 
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_CMDLINE_APPARGS_H
#define GUARD_ABBAUW_CMDLINE_APPARGS_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>
#include <libutil/misc/util_definitions.h>
#include <sstream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
namespace cmdLine
{
struct AppArgs
{
    AppArgs()
    : m_CountryCode( libutil::STRING_UNSET ),
      m_DB   ( libutil::STRING_UNSET ),
	  m_Server   ( libutil::STRING_UNSET )
    {
    }
    
    bool isInitialized() const
    {
        return isCountryCodeValid() && isDBValid() && isServerValid();
    }
    
    bool isCountryCodeValid() const
    {
        return ( false == m_CountryCode.empty() );
    }
    
    bool isDBValid() const
    {
        return ( false == m_DB.empty() );
    }

	bool isServerValid() const
    {
        return ( false == m_Server.empty() );
    }

    std::ostream& toStream( std::ostream& strm = std::cout ) const
    {
        static basar::ConstString indentation = "\t";
        strm << "APPLICATION ARGUMENTS:" << std::endl;
		strm << indentation << "Country code: <" << m_CountryCode.c_str() << ">" << std::endl;
        strm << indentation << "DB : <"    << m_DB.c_str()    << ">" << std::endl;
		strm << indentation << "DB : <"    << m_Server.c_str()    << ">" << std::endl;
		
        return strm;
    }

    basar::VarString    m_CountryCode;
    basar::VarString    m_DB;
	basar::VarString    m_Server;
};

} // end namespace cmdLine
} // end namespace abbaUW

#include "appargs.inl"

#endif 
