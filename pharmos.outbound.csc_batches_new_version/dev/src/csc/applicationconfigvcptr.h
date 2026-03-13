#ifndef GUARD_BASAR_GUI_QT_APPLICATIONCONFIGVCPTR_H
#define GUARD_BASAR_GUI_QT_APPLICATIONCONFIGVCPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
    namespace gui
    {
        namespace qt
        {
            class About;
            typedef boost::shared_ptr< About > ApplicationConfigVCPtr;
        }
    }
}
#endif