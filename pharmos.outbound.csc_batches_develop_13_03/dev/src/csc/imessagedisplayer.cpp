#include <libutil/viewconn/imessagedisplayer.h>

// FIXME: Hack - avoid linking error due to missing destructor definition
// inside libutil. When fixed there this file should be removed!

namespace libutil
{
namespace viewConn
{
    IMessageDisplayer::~IMessageDisplayer()
    {
    }
}
}