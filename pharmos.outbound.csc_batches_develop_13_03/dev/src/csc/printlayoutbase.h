#ifndef GUARD_PRINT_PRINTLAYOUTBASE_H
#define GUARD_PRINT_PRINTLAYOUTBASE_H

#include "iprintlayout.h"

#include <libbasarcmnutil_bstring.h>
#include <sstream>

namespace print
{
    class PrintLayoutBase : public IPrintLayout
    {
    public:
        PrintLayoutBase()
        {
            m_Buffer.unsetf( std::ios::floatfield );
	        m_Buffer.setf(   std::ios::fixed, std::ios::floatfield );
	        m_Buffer.precision( 2 );
        }

        virtual ~PrintLayoutBase() {}

        std::stringstream& getPrintBuffer()
        {
	        return m_Buffer;
        }

    protected:
        std::stringstream& write()
        {
            return m_Buffer;
        }

        inline void clear()
        {
            m_Buffer.clear();
            m_Buffer.str( "" );
        }

        inline const char* endl() const
        {
	        return "\n";
        }

        inline char sep() const
        {
	        return '|';
        }

        inline void finishPrint()
        {
	        write() << "EODF" << std::flush;
        }

    private:
        std::stringstream                                      m_Buffer;
    };

} // end namespace print

#endif // GUARD_PRINT_PRINTLAYOUTBASE_H
