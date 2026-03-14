#ifndef GUARD_PRINT_IPRINTLAYOUT_H
#define GUARD_PRINT_IPRINTLAYOUT_H

#include <libbasarcmnutil_bstring.h>
#include <sstream>

namespace print
{
    class IPrintLayout
    {
    public:
        virtual ~IPrintLayout() {}

        virtual void preparePrintData()                        = 0;
        virtual std::stringstream& getPrintBuffer()            = 0;

        virtual basar::VarString getFormName()     const       = 0;
        virtual basar::VarString getFormFileName() const       = 0;
    };

} // end namespace print

#endif // GUARD_PRINT_IPRINTLAYOUT_H
