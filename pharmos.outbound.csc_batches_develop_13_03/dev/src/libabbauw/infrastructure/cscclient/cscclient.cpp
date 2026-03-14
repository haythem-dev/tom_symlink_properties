#include "cscclient.h"

#include "cscclientexception.h"
#include "outofboundsexception.h"

#include <libbasarcmnutil_bstring.h>
#include <cstring>
#include <vector>
#include <sstream>

#include <libbasarcmnutil_logging.h>
#include "loggerpool/libabbauw_loggerpool.h"
#include <log4cplus/logger.h>

#include <kscclient/orderinfo.h>
#include <kscclient/articleinfostruct.h>
#include <kscclient/articleinfostructcode.h>

//#include <callbackinquiry.h>
//#include <callbackdivisioninquiry.h>
//#include <callbackreasoninquiry.h>
//#include <newcallback.h>
//#include <newcallbackcritpos.h>
#include <kscclient/callksc.h>
#include <kscclient/kscsrvdata.h>

/*
    Ugly hack to convert string to buffer which can be used on all calls to kscserver where
    char* is used. A const_cast <char>( str.c_str() ) is unsafe if the buffer is actually written to
    so this is used instead.
*/
namespace
{
    typedef std::vector< char > Buffer;
    Buffer strToNonConst( const basar::VarString& str )
    {
        Buffer buf( str.begin(), str.end() );
        buf.push_back( '\0' );

        return buf;
    }
}

#define CHECK_LEN(str, len) \
    if( str.length() > len ) \
    { \
        basar::VarString reason = "Value out of bounds!"; \
        throw libabbauw::infrastructure::cscClient::OutOfBoundsException( basar::ExceptInfo( __FUNCTION__, reason, __FILE__, __LINE__ ) ); \
    }

namespace libabbauw
{
namespace infrastructure
{
namespace cscClient
{

    CSCClient::CSCClient() : m_Logger( LoggerPool::loggerLibAbbaUW )
    {
        clearBuffer();
    }

    void CSCClient::clearBuffer()
    {
        std::memset( m_returnData, 0, sizeof( orderinfoS ) );
    }

    void CSCClient::connect( const basar::VarString& device, const basar::VarString& server, basar::Int32 port )
    {
        CHECK_LEN( device, L_KSCSRVDATA_DEVICENAME );
        CHECK_LEN( server, 80 );           // FIXME there is no define for buffer length in kscclient
        const basar::Int32 defaultUID = 0;

        clearBuffer();
        basar::Int32 result = ConnectServerNoIni( strToNonConst( device ).data(), defaultUID, port, strToNonConst( server ).data(),
                                                  m_returnData );
        if( result != 0 )
        {
            std::ostringstream reason;
            reason << "ConnectServerNoIni failed! result = '" << result << "', device = '" << device << "', uid = '"
                   << defaultUID << "', server:port = '" << server << ":" << port << "'";
            BLOG_ERROR( m_Logger, reason.str().c_str() );
            
            throw CSCClientException( basar::ExceptInfo( __FUNCTION__, reason.str(), __FILE__, __LINE__ ), "Connection" );
        }
    }

    void CSCClient::disconnect()
    {
        clearBuffer();
        
        basar::Int32 result = DisconnectServer( m_returnData );
        if( result != 0 )
        {
            basar::VarString reason = "DisconnectServer failed!";
            throw CSCClientException( basar::ExceptInfo( __FUNCTION__, reason, __FILE__, __LINE__ ), "Connection" );
        }
    }
} // end namespace cscClient
} // end namespace infrastructure
} // end namespace libabbauw
