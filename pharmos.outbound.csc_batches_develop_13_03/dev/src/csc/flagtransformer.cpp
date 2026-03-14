//-----------------------------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author
 *  \date
 */
//-----------------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------------------------------------------------//
#include "flagtransformer.h"

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QtWidgets/QApplication>
#include <libbasarqtwidget_tablewidget.h>
#include <QtGui/QPainter>
#pragma warning (pop)

//-----------------------------------------------------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
    namespace viewConn
    {
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        //! \throw  no-throw
        //!
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        FlagTransformer::FlagTransformer( const basar::VarString& GUIPropertyName, const basar::VarString& DBPropertyName, BasarTableWidget* table, bool markRed )
        : m_ClassName( "FlagTransformer" ), m_GUIPropertyName(GUIPropertyName), m_DBPropertyName( DBPropertyName ), m_Table( table ), m_markRed( markRed )
        {
        }
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        /*! \throw  no-throw */
        /*! \Date Transformer for Table NarcTransactionCatalog */
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        FlagTransformer::~FlagTransformer()
        {
        }
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        //! \throw  no-throw
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        void FlagTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator                guiIt,
                                           const basar::db::aspect::AccessorPropertyTable_YIterator   accIt )
        {
            const bool flagIsSet      = (accIt.getInt16( m_DBPropertyName ) == 1);
            const basar::Int32 column = static_cast<basar::Int32>(guiIt.getIndex( m_GUIPropertyName ));
            const basar::Int32 row    = guiIt.getPosition();

            if( flagIsSet )
            {
                guiIt.setString( m_GUIPropertyName, "•" );

                QTableWidgetItem *item = m_Table->item( row, column );

                QFont font = item->font();
                font.setBold( true );
                font.setHintingPreference( QFont::PreferDefaultHinting );
                font.setPointSize( 13 );
                item->setFont( font );
                
                item->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
                if( m_markRed )
                {
                    item->setForeground( QColor(227, 25, 25) ); // dark red
                }
                else
                {
                    item->setForeground( QBrush( QColor(118, 122, 122) ) );
                }
            }
            else
            {
                guiIt.setString( m_GUIPropertyName, "" );
            }
        }
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        //! \throw  no-throw
        //!
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        void FlagTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator   guiIt,
                                           basar::db::aspect::AccessorPropertyTable_YIterator  accIt)
        {
            // empty implementation, because nothing to match from the gui to the (data) yit.
            guiIt;
            accIt;
        }
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        //! \throw  no-throw
        //! \return pointer to created transformer
        //-----------------------------------------------------------------------------------------------------------------------------------------//
        boost::shared_ptr < basar::gui::tie::Transformer > FlagTransformer::create()
        {
            return boost::shared_ptr < basar::gui::tie::Transformer > ( new FlagTransformer(*this) );
        }

    } // viewConn
} // CSC_Batches
