//-------------------------------------------------------------------------------------------------//
/*! \file commandlineparser.cpp
 *  \brief definition of class CommandlineParser
 *  \author Dirk Kapusta
 *  \date 21.10.2008
 *  \version 00.00.00.01 first version
 */ 
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>
#include "cmdlineparser.h"
#include "cmdlineexception.h"
#include "loggerpool.h"
#include "classinfo_macros.h"
#include <algorithm>

//-------------------------------------------------------------------------------------------------//
// using declaration section
//-------------------------------------------------------------------------------------------------//
using basar::VarString;

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace cmdLine
{

/*! \throw no throw */
CmdLineParser::CmdLineParser( basar::Int32 argc, char **argv )
: m_Logger( LoggerPool::LoggerCmdLine )
{
	METHODNAME_DEF( CmdLineParser, CmdLineParser )
	BLOG_TRACE_METHOD( m_Logger, fun );

	for (int i = 0; i < argc; i++)
		m_ArgumentList.push_back(argv[i]);
}

/*! \throw no throw */
CmdLineParser::~CmdLineParser()
{
	METHODNAME_DEF( CmdLineParser, ~CmdLineParser )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_ArgumentList.clear();
}

/*! \throw CommandlineException */
bool CmdLineParser::getMandatoryVarString( basar::ConstString paramName, basar::VarString &value ) const
{
	METHODNAME_DEF( CmdLineParser, getMandatoryVarString )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getVarString( paramName, value );

	if( false == ret )
	{
        std::stringstream ss; ss << "ERROR: Parameter <" << paramName << "> is not set!";
        throw CmdLineException( basar::ExceptInfo( fun, ss.str().c_str(), __FILE__, __LINE__ ) );
	}

	return ret;
}

/*! \throw CommandlineException */
bool CmdLineParser::getMandatoryInt16( basar::ConstString paramName, basar::Int16 &value ) const
{
	METHODNAME_DEF( CmdLineParser, getMandatoryInt16 )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getInt16( paramName, value );

	if( false == ret )
	{
		std::stringstream ss; ss << "ERROR: Parameter <" << paramName << "> is not set or value cannot be converted to basar::Int16!";
		throw CmdLineException( basar::ExceptInfo( fun, ss.str().c_str(), __FILE__, __LINE__ ) );
	}

	return ret;
}

/*! \throw CommandlineException */
bool CmdLineParser::getMandatoryInt32( basar::ConstString paramName, basar::Int32 &value ) const
{
	METHODNAME_DEF( CmdLineParser, getMandatoryInt32 )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getInt32( paramName, value );

	if( false == ret )
	{
        std::stringstream ss; ss << "ERROR: Parameter <" << paramName << "> is not set or value cannot be converted to basar::Int32!";
        throw CmdLineException( basar::ExceptInfo( fun, ss.str().c_str(), __FILE__, __LINE__ ) );
	}

	return ret;
}

/*! \throw CommandlineException */
bool CmdLineParser::getMandatoryDate( basar::ConstString paramName, basar::Date &value ) const
{
	METHODNAME_DEF( CmdLineParser, getMandatoryDate )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getDate( paramName, value );

	if( false == ret )
	{
		std::stringstream ss; ss << "ERROR: Parameter <" << paramName << "> is not set or value cannot be converted to basar::Date, use YYYYMMDD!";
		throw CmdLineException( basar::ExceptInfo( fun, ss.str().c_str(), __FILE__, __LINE__ ) );
	}

	return ret;
}

/*! \throw CommandlineException */
bool CmdLineParser::getMandatoryDateTime( basar::ConstString paramName, basar::DateTime &value ) const
{
	METHODNAME_DEF( CmdLineParser, getMandatoryDateTime )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getDateTime( paramName, value );

	if( false == ret )
	{
		std::stringstream ss; ss << "ERROR: Parameter <" << paramName << "> is not set or value cannot be converted to basar::DateTime, use YYYYMMDDHHMM or YYYYMMDD!";
		throw CmdLineException( basar::ExceptInfo( fun, ss.str().c_str(), __FILE__, __LINE__ ) );
	}

	return ret;
}

/*! \throw CommandlineException */
bool CmdLineParser::getMandatoryVarStrings( basar::ConstString paramName, std::vector<basar::VarString> &value ) const
{
	METHODNAME_DEF( CmdLineParser, getMandatoryVarStrings )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getVarStrings( paramName, value );

	if( false == ret )
	{
		std::stringstream ss; ss << "ERROR: Parameter <" << paramName << "> is not set!";
		throw CmdLineException( basar::ExceptInfo( fun, ss.str().c_str(), __FILE__, __LINE__ ) );
	}

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::getVarString( basar::ConstString paramName, basar::VarString &value ) const
{
	METHODNAME_DEF( CmdLineParser, getVarString )
	BLOG_TRACE_METHOD( m_Logger, fun );

	bool ret = this->getValOfParam( paramName, value );

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::getInt16( basar::ConstString paramName, basar::Int16 &value ) const
{
	METHODNAME_DEF( CmdLineParser, getInt16 )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::VarString temp;	
	bool ret = this->getValOfParam( paramName, temp );
	ret &= temp.isDigit();

	if( true == ret )
	{
		basar::Int16 iTemp;

		try
		{
			iTemp = static_cast<basar::Int16>( temp.stoi() );
		}
		catch( ... )
		{
			return false;
		}

		value = iTemp;
	}

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::getInt32( basar::ConstString paramName, basar::Int32 &value ) const
{
	METHODNAME_DEF( CmdLineParser, getInt32 )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::VarString temp;	
	bool ret = this->getValOfParam( paramName, temp );
	ret &= temp.isDigit();

	if( true == ret )
	{
		basar::Int32 iTemp;

		try
		{
			iTemp = static_cast<basar::Int32>( temp.stoi() );
		}
		catch( ... )
		{
			return false;
		}

		value = iTemp;
	}

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::getDate( basar::ConstString paramName, basar::Date &value ) const
{
	METHODNAME_DEF( CmdLineParser, getDate )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::VarString temp;	
	bool ret = this->getValOfParam( paramName, temp );
	ret &= temp.isDigit();

	if( true == ret )
	{
		basar::Date dTemp;

		try
		{
			basar::Int32 iTemp;

			if( 8 == temp.length() )
			{
				iTemp = temp.stoi();
				dTemp = basar::Date( iTemp );
			}
			else
			{
				return false;
			}
		}
		catch( ... )
		{
			return false;
		}

		value = dTemp;
	}

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::getDateTime( basar::ConstString paramName, basar::DateTime &value ) const
{
	METHODNAME_DEF( CmdLineParser, getDateTime )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::VarString temp;
	bool ret = this->getValOfParam( paramName, temp );
	ret &= temp.isDigit();

	if( true == ret )
	{
		basar::DateTime dtTemp;
		
		try
		{
			basar::Int32 iDate = 0;
			basar::Int32 iTime = 0;

			if( 12 == temp.length() )
			{
				basar::VarString strDate = temp.substr( 0, 8 );
				basar::VarString strTime = temp.substr( 8, 4 );
				iDate = strDate.stoi();
				iTime = strTime.stoi();
				iTime *= 100000;
				dtTemp = basar::DateTime( iDate, iTime );
			}
			else if( 8 == temp.length() )
			{
				iDate = temp.stoi();
				iTime = 0;
				dtTemp = basar::DateTime( iDate, iTime );
			}	
			else
			{
				return false;
			}
		}
		catch( ... )
		{
			return false;
		}

		value = dtTemp;
	}

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::getVarStrings( basar::ConstString paramName, std::vector<basar::VarString> &value ) const
{
	METHODNAME_DEF( CmdLineParser, getVarStrings )
	BLOG_TRACE_METHOD( m_Logger, fun );

	unsigned index = 0; 
	bool ret = false;
	for(ArgumentListType::const_iterator it = m_ArgumentList.begin(); it != m_ArgumentList.end(); it++, index++) {
		if (*it == paramName) {
			ret = true;
			break;
		}
	}

	if( true == ret )
	{
		index++;
		if( index < m_ArgumentList.size() )
		{
			do
			{
				value.push_back( m_ArgumentList[ index ] );
				index++;
			}
			while( index < m_ArgumentList.size() );
		}
		else
		{
			ret = false;
		}
	}

	return ret;
}

/*! \throw no throw */
bool CmdLineParser::isParam( basar::ConstString paramName ) const
{
	METHODNAME_DEF( CmdLineParser, isParam )
	BLOG_TRACE_METHOD( m_Logger, fun );
	ArgumentListType::const_iterator it = std::find(m_ArgumentList.begin(), m_ArgumentList.end(), paramName);
	return it != m_ArgumentList.end();
}

/*! \throw no throw */
basar::VarString CmdLineParser::applicationFilePath() const
{
	METHODNAME_DEF( CmdLineParser, applicationFilePath )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return basar::cmnutil::getExecPath();
}

/*! \throw no throw */
bool CmdLineParser::getValOfParam( basar::ConstString paramName, basar::VarString &value ) const
{
	METHODNAME_DEF( CmdLineParser, getValOfParam )
	BLOG_TRACE_METHOD( m_Logger, fun );

	unsigned index = 0; 
	bool ret = false;
	for(ArgumentListType::const_iterator it = m_ArgumentList.begin(); it != m_ArgumentList.end(); ++it, ++index) {
		if (*it == paramName) {
			ret = true;
			break;
		}
	}

	if( true == ret )
	{
		++index;
		if( index < m_ArgumentList.size() )
		{
			value = m_ArgumentList[ index ];
		}
		else
		{
			ret = false;
		}
	}

	return ret;
}

/*! \throw no throw */
CmdLineParser::operator const basar::VarString() const
{
	METHODNAME_DEF( CmdLineParser, (operator const basar::VarString()) )
	BLOG_TRACE_METHOD( m_Logger, fun );

	std::stringstream ss; 
	for (ArgumentListType::const_iterator it = m_ArgumentList.begin(); it != m_ArgumentList.end(); it++) {
		ss << "\"" << *it << "\"";
	}
	return ss.str().c_str();
}
/*! \throw no throw */
bool CmdLineParser::existArguments() const
{
    METHODNAME_DEF( CmdLineParser, existArguments )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return 1 < m_ArgumentList.size();
}

} // end namespace cmdLine
} // end namespace libBatch

