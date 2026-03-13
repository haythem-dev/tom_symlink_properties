#include "printdataprovider.h"

namespace print
{
    PrintDataProvider::PrintDataProvider( std::stringstream& buffer ) : m_Buffer( buffer )
    {
    }

    PrintDataProvider::~PrintDataProvider()
    {
    }

    basar::VarString PrintDataProvider::getFormName() const
    {
        return m_FormName;
    }

    basar::VarString PrintDataProvider::getFormFileName() const
    {
        return m_FormFileName;
    }

    void PrintDataProvider::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
    {
        for( char a; m_Buffer.get( a ); )
        {
	        collPrintRawInput.push( a );
        }
    }

    void PrintDataProvider::setFormName( const basar::VarString& formName )
    {
        m_FormName = formName;
    }

    void PrintDataProvider::setFormFileName( const basar::VarString& formFileName )
    {
        m_FormFileName = formFileName;
    }
}
