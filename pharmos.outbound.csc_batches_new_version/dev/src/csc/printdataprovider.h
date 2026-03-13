#ifndef GUARD_PRINT_PRINTDATAPROVIDER_H
#define GUARD_PRINT_PRINTDATAPROVIDER_H

#include <libbasarcmnutil_print.h>
#include <libbasarcmnutil_bstring.h>
#include <sstream>

namespace print
{
    class PrintDataProvider : public basar::cmnutil::IPrintDataProvider
    {
    public:
        PrintDataProvider( std::stringstream& );
        ~PrintDataProvider();

        // from IPrintDataProvider interface
        basar::VarString getFormName() const;
        basar::VarString getFormFileName() const;

        void onSendData( basar::cmnutil::CollPrintRawInput &collPrintRawInput );

        void setFormName( const basar::VarString& );
        void setFormFileName( const basar::VarString& );

    private:
        PrintDataProvider ( const PrintDataProvider& );
	    PrintDataProvider operator = ( const PrintDataProvider& );

    private:
        std::stringstream&                                     m_Buffer;
        basar::VarString                                       m_FormName;
        basar::VarString                                       m_FormFileName;
    };
} // end namespace print

#endif
