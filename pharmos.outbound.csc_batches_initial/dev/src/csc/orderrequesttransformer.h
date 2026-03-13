#ifndef GUARD_CSC_BATCHES_VIEWCONN_ORDERREQUESTTRANSFORMER_H
#define GUARD_CSC_BATCHES_VIEWCONN_ORDERREQUESTTRANSFORMER_H

//------------------------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>
#include <libbasarguitie.h>

//------------------------------------------------------------------------------------------------------------------//
// namespace
//------------------------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
    namespace viewConn
    {

    //------------------------------------------------------------------------------------------------------------------//
    // class declaration
    //------------------------------------------------------------------------------------------------------------------//
    class OrderRequestTransformer : public basar::gui::tie::Transformer
    {
    public:
        /*! destructor                            \n no-throw*/
                        ~OrderRequestTransformer ();

        /*! \n no-throw*/
                        OrderRequestTransformer  ( const basar::VarString& GUIPropertyName, const basar::VarString& DBPropertyName );

        /*! transform values from right to left iterator    \n UnknownPropertyTypeException*/
        virtual void    RightToLeft      ( basar::gui::tie::GuiPropertyTable_YIterator              guiIt, //!< iterator to GuiPropertyTable row
                                           const basar::db::aspect::AccessorPropertyTable_YIterator accIt  //!< iterator to property table/cache controller row
                                         );
        /*! transform values from left to right iterator    \n TypecastFailedException*/
        virtual void    LeftToRight      ( const basar::gui::tie::GuiPropertyTable_YIterator        guiIt, //!< iterator to GuiPropertyTable row
                                           basar::db::aspect::AccessorPropertyTable_YIterator       accIt  //!< iterator to property table/cache controller row
                                         );

        typedef boost::shared_ptr < basar::gui::tie::Transformer > TransformerSharedPtr;
        /*! creates cloned instance                            \n no-throw*/
        virtual TransformerSharedPtr         create    ();

    private:
        basar::VarString                        m_ClassName;            //!< name of class in program
        basar::VarString                        m_GUIPropertyName;      //!< name of control on GUI
        basar::VarString                        m_DBPropertyName;       //!< name of column in database
    };

    //------------------------------------------------------------------------------------------------------------------//
    } // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_ORDERREQUESTTRANSFORMER_H
