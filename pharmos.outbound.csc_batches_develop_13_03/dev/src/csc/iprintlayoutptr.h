#ifndef GUARD_PRINT_IPRINTLAYOUTPTR_H
#define GUARD_PRINT_IPRINTLAYOUTPTR_H

#include <boost/shared_ptr.hpp>

namespace print
{
	class IPrintLayout;
    typedef boost::shared_ptr< IPrintLayout > IPrintLayoutPtr;
}

#endif // GUARD_PRINT_IPRINTLAYOUTPTR_H
