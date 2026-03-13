#include "cmndm.h"
#include "transferordercreationdm.h"

#include "itransferorder.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"

#include "itransferorderinpreparationdm.h"
#include "itransferorderheadinpreparation.h"
#include "itransferorderposinpreparationcollection.h"

#include "ifindarticleprice.h"
#include "iordercreationparameter.h"

#include "orderstatus.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    const libutil::misc::ClassInfo& TransferOrderCreationDM::getClassInfo()
    {
	    static const libutil::misc::ClassInfo info( "transferOrder::TransferOrderCreationDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

    TransferOrderCreationDM::TransferOrderCreationDM()
        : m_Logger( libabbauw::LoggerPool::loggerTransferOrderCreation )
    {
        METHODNAME_DEF( TransferOrderCreationDM, TransferOrderCreationDM )
	    BLOG_TRACE_METHOD( getLogger(), fun );
    }

    TransferOrderCreationDM::~TransferOrderCreationDM()
    {
	    METHODNAME_DEF( TransferOrderCreationDM, ~TransferOrderCreationDM )
	    BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void TransferOrderCreationDM::injectTransferOrder( transferOrder::ITransferOrderPtr transferOrder )
    {
        m_TransferOrderDM = transferOrder;
    }

    void TransferOrderCreationDM::injectArticleConsultant( article::IFindArticlePricePtr articlePrice )
    {
        m_ArticlePriceDM = articlePrice;
    }

    void TransferOrderCreationDM::injectOrderCreationParameter( parameter::IOrderCreationParameterPtr orderCreationParam )
    {
        m_OrderCreationParameter = orderCreationParam;
    }

    transferOrder::ITransferOrderPtr TransferOrderCreationDM::create( transferOrder::ITransferOrderInPreparationDMPtr transferOrderInPrep )
    {
        m_TransferOrderDM->clear();

        createHead( transferOrderInPrep );
        createPos(  transferOrderInPrep );

        return m_TransferOrderDM;
    }

    void TransferOrderCreationDM::createHead( transferOrder::ITransferOrderInPreparationDMPtr transferOrderInPrep )
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yitHead = m_TransferOrderDM->getHead()->get();
        yitHead.match( transferOrderInPrep->getHead()->get() );

        yitHead.setDate(   properties::ORDERDATE, basar::Date().getCurrent() );
        yitHead.setString( properties::DATASOURCETYPE, lit::UW_ORDERTYPE );
        yitHead.setString( properties::STATUS, OrderStatus( OrderStatus::ZU ).toString() );

        basar::I18nString currentHeaderText = yitHead.getString(properties::HEADERTEXT);

        if (currentHeaderText.empty())
        {
            if( m_OrderCreationParameter->getFormatHeaderTextEnabled() )
            {
                yitHead.setString( properties::HEADERTEXT, lit::UW_HEADERTEXT_PREFIX + yitHead.getString( properties::MANUFACTURERNAME ) );
            }
        }
    }

    void TransferOrderCreationDM::createPos( transferOrder::ITransferOrderInPreparationDMPtr transferOrderInPrep )
    {
        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = transferOrderInPrep->getPosCollection()->get().begin(); !yit.isEnd(); ++yit )
        {
            if( ! yit.getState().contains( basar::SS_DELETE ) )
            {
                basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderDM->addEmptyPosition();
                yitPos.match( yit );
                
                m_ArticlePriceDM->findArticlePrice( yit.getInt32( properties::ARTICLENO ) );

                // orderno is set automatically after head insert
                yitPos.setInt32(    properties::ORDEREDQTY,                 yit.getInt32( properties::ORDEREDQTY ) );
                yitPos.setDecimal(  properties::ARTICLEPRICE,               m_ArticlePriceDM->get().getDecimal( properties::ARTICLEPRICE ) );
                yitPos.setDecimal(  properties::WHOLESALERPURCHASEPRICE,    yitPos.getDecimal( properties::ARTICLEPRICE ) * yitPos.getInt32( properties::ORDEREDQTY ) );
                yitPos.setString(   properties::TEXTFIELD,                  "" );
                yitPos.setInt32(    properties::CUSTOMERPOSNO,              0  );
				yitPos.setString(	properties::ENTRYTYPE,					"M" ); //M for manually created order
                yitPos.setInt16(    properties::DISCOUNTAPPLYTO,            yit.getInt16(properties::DISCOUNTAPPLYTO) );
                yitPos.setInt16(    properties::DISCOUNTCALCFROM,           yit.getInt16(properties::DISCOUNTCALCFROM) );
			}
        }
    }

    const log4cplus::Logger& TransferOrderCreationDM::getLogger() const
    {
        METHODNAME_DEF( TransferOrderCreationDM, getLogger )
	    BLOG_TRACE_METHOD( m_Logger, fun );

        return m_Logger;
    }

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
