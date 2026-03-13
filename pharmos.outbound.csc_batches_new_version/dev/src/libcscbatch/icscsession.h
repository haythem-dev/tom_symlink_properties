#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_ICSCSESSION_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_ICSCSESSION_H

#include <libbasarcmnutil_bstring.h>

namespace CSC_Batches
{
namespace infrastructure
{
namespace session
{
    class ICSCSession
    {
    public:
        virtual ~ICSCSession() {}

        virtual const basar::I18nString&     getDevice() const = 0;
        virtual const basar::I18nString&     getServer()       = 0;
        virtual basar::Int32                 getPort()         = 0;
    };

} // end namespace session
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_ICSCSESSION_H
