#ifndef GUARD_PRINT_PRINTPROCESSOR_H
#define GUARD_PRINT_PRINTPROCESSOR_H

#include "iprintprocessor.h"
#include <libbasar_definitions.h>

namespace print
{

class PrintProcessor : public IPrintProcessor
{
public:
	PrintProcessor();
	virtual ~PrintProcessor() {}

	// from IPrintProcessor
	virtual void                                                print( ::print::IPrintLayoutPtr );
	virtual void                                                preview( ::print::IPrintLayoutPtr );
	virtual void                                                previewPDF( ::print::IPrintLayoutPtr );

private:
	PrintProcessor( const PrintProcessor& );
	PrintProcessor& operator = ( const PrintProcessor& );

	bool                                                        fileIsOpen( const basar::VarString& file );

private:
    IPrintLayoutPtr                                             m_PrintLayout;
};

} // end namespace print

#endif // end GUARD_PRINT_PRINTPROCESSOR_H
