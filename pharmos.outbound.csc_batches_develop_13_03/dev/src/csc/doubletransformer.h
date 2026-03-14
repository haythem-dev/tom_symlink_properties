#ifndef GUARD_VIEWCONN_DOUBLETRANSFORMER_H
#define GUARD_VIEWCONN_DOUBLETRANSFORMER_H

#include <libbasarguitie_transformer.h>

namespace viewConn
{
    class DoubleTransformer : public basar::gui::tie::Transformer
    {
    public:
        ~DoubleTransformer();

        DoubleTransformer( const basar::VarString& GUIPropertyName, const basar::VarString& DBPropertyName, basar::Int32 decimalPlaces );

        virtual void    RightToLeft      ( basar::gui::tie::GuiPropertyTable_YIterator              guiIt,
                                           const basar::db::aspect::AccessorPropertyTable_YIterator accIt
                                         );

        virtual void    LeftToRight      ( const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,
                                           basar::db::aspect::AccessorPropertyTable_YIterator       accIt
                                         );

        typedef boost::shared_ptr < basar::gui::tie::Transformer > TransformerSharedPtr;
        virtual TransformerSharedPtr         create    ();

    private:
        basar::VarString                        m_ClassName;            //!< name of class in program
        basar::VarString                        m_GUIPropertyName;        //!< name of control on GUI
        basar::VarString                        m_DBPropertyName;        //!< name of column in database
        basar::Int32                            m_DecimalPlaces;
    };

} // end namespace viewConn

#endif // GUARD_VIEWCONN_DOUBLETRANSFORMER_H
