#ifndef GUARD_PRINT_IPRINTPROCESSOR_H
#define GUARD_PRINT_IPRINTPROCESSOR_H

#include "iprintlayoutptr.h"

namespace print
{
	
class IPrintProcessor
{
public:
	virtual ~IPrintProcessor() {}

	virtual void                                                print( ::print::IPrintLayoutPtr )      = 0;
	virtual void                                                preview( ::print::IPrintLayoutPtr )    = 0;
	virtual void                                                previewPDF( ::print::IPrintLayoutPtr ) = 0;
};

} // end namespace print

#endif // GUARD_PRINT_IPRINTPROCESSOR_H
