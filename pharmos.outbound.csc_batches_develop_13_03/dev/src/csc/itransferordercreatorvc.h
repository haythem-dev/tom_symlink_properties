//-------------------------------------------------------------------------------------------------//
/*! \file 
  * \brief  
  * \author 
  * \date
*/
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERCREATORVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERCREATORVC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie_definitions.h>
#include <libbasarguitie_msgboxes.h.>

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
            class AccessorPropertyTableRef;
        }
    }
}

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{

class ITransferOrderCreatorVC
{
    public:	
        virtual ~ITransferOrderCreatorVC(){};
        
        virtual void init( basar::db::aspect::AccessorPropertyTable_YIterator head, basar::db::aspect::AccessorPropertyTableRef pos )                   = 0;

		virtual bool								                        shutdown()                           = 0;
		virtual basar::gui::tie::WidgetReturnEnum	                        show()                               = 0;
        virtual void                                                        waitForEvents()                      = 0;
        virtual void                                                        info( const basar::I18nString& )     = 0;
        virtual void                                                        error( const basar::I18nString& )    = 0;
        virtual const basar::gui::tie::MessageBoxButtonEnum                 question( const basar::I18nString& ) = 0;
        
        virtual void                                                        matchFromManufacturer( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
        virtual void                                                        matchToManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
        virtual void                                                        matchFromPurchaseOrderSupplier( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

        virtual void                                                        matchFromOrderNoMF(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
        virtual void                                                        matchToOrderNoMF(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

        virtual void                                                        matchFromPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator )     = 0;
        virtual void                                                        matchToPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator )       = 0;

        virtual void                                                        matchForHeadertext(basar::db::aspect::AccessorPropertyTable_YIterator)      = 0;

        virtual void                                                        matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator )      = 0;
        virtual void                                                        matchToArticle( basar::db::aspect::AccessorPropertyTable_YIterator )        = 0;

        virtual void                                                        matchFromTransferOrderHeadInPreparation()                                   = 0;
        virtual void                                                        matchToTransferOrderHeadInPreparation()                                     = 0;

        virtual void                                                        matchFromPosition( basar::db::aspect::AccessorPropertyTable_YIterator )     = 0;
        virtual void                                                        matchToPosition( basar::db::aspect::AccessorPropertyTable_YIterator )       = 0;
        
        virtual basar::db::aspect::AccessorPropertyTable_YIterator          getSelectedPosition()                                                       = 0;

        virtual void                                                        switchToEditingMode( bool )                                                 = 0;

        virtual void                                                        matchFromTransferOrderPosInPreparationCollection()                          = 0;

        virtual void                                                        setNextPositionNo( basar::Int32 )                                           = 0;

        virtual void                                                        setSavingAllowed( bool isEnabled )                                          = 0;

        virtual void                                                        setCreatingTransferOrderAllowed( bool isEnabled )                           = 0;

        virtual void                                                        showOrderNoInfoMsg( const basar::Int32 )                                    = 0;
};

} // viewConn
} // CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERCREATORVC_H
