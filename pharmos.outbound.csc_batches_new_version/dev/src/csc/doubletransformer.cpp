#include "doubletransformer.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarguitie.h>
#include <sstream>

namespace viewConn
{

    DoubleTransformer::DoubleTransformer( const basar::VarString& GUIPropertyName, const basar::VarString& DBPropertyName, basar::Int32 decimalPlaces )
        : m_ClassName( "DoubleTransformer" ), m_GUIPropertyName(GUIPropertyName), m_DBPropertyName( DBPropertyName ), m_DecimalPlaces( decimalPlaces )
    {
    }

    DoubleTransformer::~DoubleTransformer()
    {
    }

    void DoubleTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator                guiIt,
                                         const basar::db::aspect::AccessorPropertyTable_YIterator   accIt )
    {
        if( accIt.isContainedAndSet( m_DBPropertyName ) )
        {
            std::ostringstream oss;
            oss.setf( std::ios::fixed,std::ios::floatfield );
            oss.precision( m_DecimalPlaces );

            oss << accIt.getFloat64( m_DBPropertyName );
            guiIt.setString( m_GUIPropertyName, oss.str().c_str() );
        }
    }

     void DoubleTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator   guiIt,
                                          basar::db::aspect::AccessorPropertyTable_YIterator  accIt)
     {
         // empty implementation, because nothing to match from the gui to the (data) yit.
         guiIt;
         accIt;
     }

     boost::shared_ptr < basar::gui::tie::Transformer > DoubleTransformer::create()
     {
         return boost::shared_ptr < basar::gui::tie::Transformer > ( new DoubleTransformer(*this) );
     }

} // end namespace viewConn
