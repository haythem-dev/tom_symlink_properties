//--------------------------------------------------------------------------------------------------//
/*! \brief 
 *  \author     Bjoern Bischof
 *  \date       24.12.2010
 *  \version    00.00.00.01
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
    : m_CountryCode	( libutil::STRING_UNSET ),
      m_InputDir	( libutil::STRING_UNSET ),
	  m_TransferDir ( libutil::STRING_UNSET ),
	  m_ForBranchnos( libutil::STRING_UNSET )
    {
    }
    
    bool isInitialized() const
    {
        return isCountryCodeValid() && isTransferDirValid() && isInputDirValid() && isForBranchnosValid();
    }
    
    bool isCountryCodeValid() const
    {
		if (m_CountryCode.empty()) {
			return false;
		}

		// For now only Switzerland ( CH) and both Germanies (North DE1 and South DE2) can work with ABBA-UW
		if (   "CH" != m_CountryCode && "ch" != m_CountryCode
			&& "DE1" != m_CountryCode && "de1" != m_CountryCode
			&& "DE2" != m_CountryCode && "de2" != m_CountryCode)
		{
			return false;
		}

		return true;

        //return ( false == m_CountryCode.empty() );
    }

    bool isTransferDirValid() const
    {
        return ( false == m_TransferDir.empty() );
    }
    
    bool isInputDirValid() const
    {
        return ( false == m_InputDir.empty() );
    }

	bool isForBranchnosValid() const
    {
        return ( false == m_InputDir.empty() );
    }


    std::ostream& toStream( std::ostream& strm = std::cout ) const
    {
        static basar::ConstString indentation = "\t";
        strm << "APPLICATION ARGUMENTS:" << std::endl;
		strm << indentation << "Country code: <" << m_CountryCode.c_str()		<< ">" << std::endl;
        strm << indentation << "Transfer Dir: <" << m_TransferDir.c_str()		<< ">" << std::endl;
        strm << indentation << "Input Dir: <"    << m_InputDir.c_str()			<< ">" << std::endl;
        strm << indentation << "For Branchnos: <"    << m_ForBranchnos.c_str()  << ">" << std::endl;
		
        return strm;
    }

    basar::VarString    m_CountryCode;
    basar::VarString    m_InputDir;
    basar::VarString    m_TransferDir;
	basar::VarString    m_ForBranchnos;
};

} // end namespace cmdLine
} // end namespace abbaUW

#include "appargs.inl"

#endif 
