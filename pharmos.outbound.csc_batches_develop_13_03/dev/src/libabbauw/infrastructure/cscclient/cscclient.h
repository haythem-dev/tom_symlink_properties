#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_CSCCLIENT_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_CSCCLIENT_H

#include <libbasar_definitions.h>

// forward declaration
struct N_ORDERINFO;
typedef struct N_ORDERINFO orderinfoS;

namespace log4cplus
{
    class Logger;
}

namespace libabbauw
{
namespace infrastructure
{
namespace cscClient
{
    class CSCClient
    {
    public:
        CSCClient();

        void connect( const basar::VarString& device, const basar::VarString& server, basar::Int32 port );
        void disconnect();

    private:
        // forbidden
        CSCClient( const CSCClient& );
        CSCClient& operator = ( const CSCClient& );

        void clearBuffer();

    private:
        const log4cplus::Logger&   m_Logger;
        orderinfoS*			       m_returnData;
    };

} // end namespace cscClient
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_CSCCLIENT_H
