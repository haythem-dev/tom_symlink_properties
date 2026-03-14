//-------------------------------------------------------------------------------------------------//
/*! \file 
  * \brief  
  * \author 
  * \date
*/
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERPOSVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERPOSVC_H


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

class ITransferOrderPosVC
{
    public:	
        virtual ~ITransferOrderPosVC(){};
        
		virtual void                                                init( basar::db::aspect::AccessorPropertyTableRef orderPositions, bool isReserverdQtyDifferent, bool isOrderViewActive ) = 0;
		virtual bool								                shutdown()                                                                  = 0;
		virtual basar::gui::tie::WidgetReturnEnum	                show()                                                                      = 0;
        virtual void                                                waitForEvents()                                                             = 0;
        virtual void                                                info( const basar::I18nString& )                                            = 0;
		virtual void                                                showFileStillOpenedInfo()                                                   = 0;
        virtual void                                                error( const basar::I18nString& )                                           = 0;
        virtual const basar::gui::tie::MessageBoxButtonEnum         question( const basar::I18nString& )                                        = 0;
        
		virtual void                                                confirmOrderSplit( basar::Int32 orderno, basar::Int32 splittedPositions )   = 0;
        virtual void												matchFromTransferOrder( basar::db::aspect::AccessorPropertyTable_YIterator )= 0;
		virtual void												matchFromTransferOrderPositions()                                           = 0;
        virtual void                                                matchFromPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator )     = 0;

		virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSelectedOrderPosition()                                                  = 0;
        
        virtual void                                                setDeletePositionAllowed( bool )                                            = 0;
        virtual void                                                setSplitRedYellowAllowed( bool )                                            = 0;
        virtual void                                                setSplitNarcoticsRefrigeratedAllowed( bool )                                = 0;
};

} // viewConn
} // CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERPOSVC_H
