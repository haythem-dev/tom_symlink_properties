#ifndef GUARD_PRINT_IPRINTPROCESSORPTR_H
#define GUARD_PRINT_IPRINTPROCESSORPTR_H

#include <boost/shared_ptr.hpp>

namespace print
{
	class IPrintProcessor;
    typedef boost::shared_ptr< IPrintProcessor > IPrintProcessorPtr;
}

#endif // GUARD_PRINT_IPRINTPROCESSORPTR_H
