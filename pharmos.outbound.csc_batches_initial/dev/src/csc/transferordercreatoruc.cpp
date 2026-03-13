#include "transferordercreatoruc.h"
#include "iusecasegetter.h"

#include "itransferordercreatorvc.h"

// dm
#include "libabbauw/itransferorderinpreparationdm.h"
#include "libabbauw/itransferorderheadinpreparation.h"
#include "libabbauw/itransferorderposinpreparationcollection.h"

#include "libabbauw/itransferorderptr.h"
#include "libabbauw/itransferorder.h"
#include "libabbauw/itransferorderhead.h"
#include "libabbauw/itransferorderposcollection.h"
#include "libabbauw/itransferordercreationdm.h"

#include "libabbauw/ifindarticle.h"
#include "libabbauw/icustomerfinder.h"
#include "libabbauw/ifindmanufacturer.h"
#include "libabbauw/ifindpurchaseordersupplier.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocol.h"

#include "libabbauw/iordercheckparameter.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#pragma warning (push)
#pragma warning(disable: 4231 4800 )
#include <libutil/usecase.h>
#pragma warning (pop)

#include <libutil/util.h>

namespace CSC_Batches
{
namespace useCase
{
    BEGIN_HANDLERS_REGISTRATION( TransferOrderCreatorUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, FindArticleRequested                      )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, UpdateArticleRequested                    )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckArticleRequested                     )

        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckOrderNoMFRequested                   )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckManualHeadertextRequested            )

        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, FindPharmacyRequested                     )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, UpdatePharmacyRequested                   )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckPharmacyNoRequested                  )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckHeadertextRequested                  )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, AddPositionRequested                      )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, ChangePositionRequested                   )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, RemovePositionRequested                   )

        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, SwitchToPosEditModeRequested              )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, SwitchToPosInputModeRequested             )

        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, ResetTransferOrderInPreparationRequested  )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckSaveStateRequested                   )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CheckCreateTransferOrderStateRequested    )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, SaveTransferOrderInPreparationRequested   )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, CreateTransferOrderRequested              )
        SYSTEM_EVENT_HANDLER_REGISTRATION( TransferOrderCreatorUC, DialogCloseRequested                      )

    END_HANDLERS_REGISTRATION()
	
	TransferOrderCreatorUC::TransferOrderCreatorUC()
	{
	    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, "TransferOrderCreatorUC::TransferOrderCreatorUC().");
	}

	TransferOrderCreatorUC::~TransferOrderCreatorUC()
	{
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, "TransferOrderCreatorUC::~TransferOrderCreatorUC().");
	}

    void TransferOrderCreatorUC::injectVC( viewConn::ITransferOrderCreatorVCPtr vc )
    {
        m_TransferOrderCreatorVC = vc;
    }
    
    void TransferOrderCreatorUC::injectUseCaseGetter( componentManager::IUseCaseGetterPtr useCaseGetter )
    {
        m_UseCaseGetter = useCaseGetter;
    }

    void TransferOrderCreatorUC::injectDM( libabbauw::domMod::transferOrder::ITransferOrderInPreparationDMPtr dm )
    {
        m_TransferOrderInPreparation = dm;
    }   

    void TransferOrderCreatorUC::injectDM( libabbauw::domMod::customer::ICustomerFinderPtr dm )
    {
        m_CustomerFinder = dm;
    }
    
    void TransferOrderCreatorUC::injectDM( libabbauw::domMod::article::IFindArticlePtr dm )
    {
        m_FindArticle = dm;
    }

    void TransferOrderCreatorUC::injectDM( libabbauw::domMod::manufacturerdm::IFindManufacturerPtr dm )
    {
        m_FindManufacturer = dm;
    }

    void TransferOrderCreatorUC::injectDM( libabbauw::domMod::transferOrder::ITransferOrderCreationDMPtr dm )
    {
        m_TransferOrderCreation = dm;
    }

    void TransferOrderCreatorUC::injectDM( libabbauw::domMod::purchaseordersupplierdm::IFindPurchaseOrderSupplierPtr dm )
    {
        m_PurchaseOrderSupplier = dm;
    }

	void TransferOrderCreatorUC::injectDM( libabbauw::domMod::orderProtocol::IOrderProtocolPtr dm )
	{
		m_OrderProtocol = dm;
	}

    void TransferOrderCreatorUC::injectParameter( libabbauw::domMod::parameter::IOrderCheckParameterPtr orderCheckParameter )
    {
        m_OrderCheckParameter = orderCheckParameter;
    }
    
    void TransferOrderCreatorUC::resetTransferOrderInPreparation()
    {
        m_TransferOrderInPreparation->markAsDeleted();
        m_TransferOrderCreatorVC->matchFromTransferOrderHeadInPreparation();
        m_TransferOrderCreatorVC->matchFromTransferOrderPosInPreparationCollection();
        m_TransferOrderCreatorVC->setNextPositionNo( m_TransferOrderInPreparation->getNextFreePosNo() );
        m_TransferOrderCreatorVC->switchToEditingMode( false );

		m_TransferOrderCreatorVC->setSavingAllowed( m_TransferOrderInPreparation->isChanged() );
        m_TransferOrderCreatorVC->setCreatingTransferOrderAllowed( m_TransferOrderInPreparation->isComplete() );
        
        m_PurchaseOrderSupplier->clear();
        m_TransferOrderCreatorVC->matchFromPurchaseOrderSupplier( m_PurchaseOrderSupplier->get() );
    }

    void TransferOrderCreatorUC::saveTransferOrderInPreparation()
    {
        // read head yit from VC
        m_TransferOrderCreatorVC->matchToTransferOrderHeadInPreparation();

        // store head + updated position property table
        m_TransferOrderInPreparation->save();

        m_TransferOrderCreatorVC->setSavingAllowed( m_TransferOrderInPreparation->isChanged() );
    }

    void TransferOrderCreatorUC::matchToArticle()
    {
        m_FindArticle->resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator yit = m_FindArticle->getSearchYit();
        m_TransferOrderCreatorVC->matchToArticle( yit );
        if( ! m_PurchaseOrderSupplier->isEmpty() )
        {
            basar::Int32 purchaseOrderSupplierNo = m_PurchaseOrderSupplier->get().getInt32( libabbauw::properties::PURCHASEORDERSUPPLIERNO );
            yit.setInt32( libabbauw::properties::PURCHASEORDERSUPPLIERNO, purchaseOrderSupplierNo );
        }
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, FindArticleRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, FindArticleRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );

        try
		{
            // uc starts search and shows results in own window
            matchToArticle();
            m_UseCaseGetter->getFindArticleUC()->run();
			result = basar::appl::HANDLER_OK;
		}
		catch( basar::Exception& e ) {
			m_TransferOrderCreatorVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, UpdateArticleRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, UpdateArticleRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );

        try
		{
            m_TransferOrderCreatorVC->matchFromArticle( yitFromEvSrc );
			result = basar::appl::HANDLER_OK;
		}
		catch( basar::Exception& e ) {
			m_TransferOrderCreatorVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, CheckArticleRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, CheckArticleRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );
        
        try 
        {
            matchToArticle();
            m_FindArticle->findArticle( m_FindArticle->getSearchYit() );
            if( m_FindArticle->get().size() > 1 )
            {
                // open article search window if several items are found
                m_UseCaseGetter->getFindArticleUC()->run();
            }
            else
            {
                m_TransferOrderCreatorVC->matchFromArticle( m_FindArticle->get().begin() );
            }

            result = HANDLER_OK;
        } 
        catch( basar::Exception& e ) 
        {
            m_TransferOrderCreatorVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, FindPharmacyRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, FindPharmacyRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );

        try
		{
            m_UseCaseGetter->getFindCustomerUC()->run();
            result = basar::appl::HANDLER_OK;
		}
		catch( basar::Exception& e ) {
			m_TransferOrderCreatorVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, UpdatePharmacyRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, UpdatePharmacyRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );

        try
		{
            m_TransferOrderCreatorVC->matchFromPharmacy( yitFromEvSrc );
            m_TransferOrderCreatorVC->matchToTransferOrderHeadInPreparation();
            m_TransferOrderCreatorVC->setSavingAllowed( m_TransferOrderInPreparation->isChanged() );
            m_TransferOrderCreatorVC->setCreatingTransferOrderAllowed( m_TransferOrderInPreparation->isComplete() );
			result = basar::appl::HANDLER_OK;
		}
		catch( basar::Exception& e ) {
			m_TransferOrderCreatorVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, CheckPharmacyNoRequested )
	{
		METHODNAME_DEF( TransferOrderCreatorUC, CheckPharmacyNoRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );
        
		try {
            m_CustomerFinder->resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator yit = m_CustomerFinder->getSearchYit();
			m_TransferOrderCreatorVC->matchToPharmacy( yit );
            m_CustomerFinder->findByKey( yit.getInt32( libabbauw::properties::PHARMACYNO ) );
			m_TransferOrderCreatorVC->matchFromPharmacy( m_CustomerFinder->get() );

			result = HANDLER_OK;
		} catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
            result.message = e.what();
        }

		return result;
	}

    SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderCreatorUC, CheckOrderNoMFRequested)
    {
        METHODNAME_DEF(TransferOrderCreatorUC, CheckOrderNoMFRequested)
            BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result(HANDLER_ERROR);

        try {

            m_FindManufacturer->resetSearchYit();
            basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_FindManufacturer->getSearchYit();
            m_TransferOrderCreatorVC->matchToManufacturer(searchYit);
            //m_FindManufacturer->findByKey(searchYit.getInt32(libabbauw::properties::MANUFACTURERNO));
            m_FindManufacturer->findManufacturer(searchYit);

            m_TransferOrderCreatorVC->matchForHeadertext(m_FindManufacturer->getPropTab().begin());

            result = HANDLER_OK;
        }
        catch (basar::Exception& e) {
            m_TransferOrderCreatorVC->error(e.what());
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderCreatorUC, CheckManualHeadertextRequested)
    {
        METHODNAME_DEF(TransferOrderCreatorUC, CheckManualHeadertextRequested)
            BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result(HANDLER_ERROR);

        try {

            m_FindManufacturer->resetSearchYit();
            basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_FindManufacturer->getSearchYit();
            m_TransferOrderCreatorVC->matchToManufacturer(searchYit);
            m_FindManufacturer->findManufacturer(searchYit);

            m_TransferOrderCreatorVC->matchForHeadertext(m_FindManufacturer->getPropTab().begin());

            result = HANDLER_OK;
        }
        catch (basar::Exception& e) {
            m_TransferOrderCreatorVC->error(e.what());
            result.message = e.what();
        }

        return result;
    }


    SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderCreatorUC, CheckHeadertextRequested)
    {
        METHODNAME_DEF(TransferOrderCreatorUC, CheckHeadertextRequested)
            BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result(HANDLER_ERROR);

        try {
            result = HANDLER_OK;
        }
        catch (basar::Exception& e) {
            m_TransferOrderCreatorVC->error(e.what());
            result.message = e.what();
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, SwitchToPosEditModeRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, SwitchToPosEditModeRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );

        try {
            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderCreatorVC->getSelectedPosition();
            m_TransferOrderCreatorVC->matchFromPosition( yitPos );
            m_TransferOrderCreatorVC->switchToEditingMode( true );

            result = basar::appl::HANDLER_OK;
		}
        catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, SwitchToPosInputModeRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, SwitchToPosInputModeRequested )
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );

        try {
            m_TransferOrderCreatorVC->setNextPositionNo( m_TransferOrderInPreparation->getNextFreePosNo() );
            m_TransferOrderCreatorVC->switchToEditingMode( false );

            result = basar::appl::HANDLER_OK;
		}
        catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, AddPositionRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, AddPositionRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );
        basar::Decimal agp;
        basar::Decimal gep;

        try {
            const bool isFirstPosition = m_TransferOrderInPreparation->getPosCollection()->isEmpty();

            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderInPreparation->addEmptyPosition();
            m_TransferOrderCreatorVC->matchToPosition( yitPos );
            // search article again to get internal articleno
            m_FindArticle->resetSearchYit();
            m_TransferOrderCreatorVC->matchToPosition( m_FindArticle->getSearchYit() );
            m_FindArticle->findArticle( m_FindArticle->getSearchYit() );
            yitPos.setInt32( libabbauw::properties::ARTICLENO, m_FindArticle->get().begin().getInt32( libabbauw::properties::ARTICLENO ) );

            gep = m_FindArticle->get().begin().getDecimal(libabbauw::properties::GEP);
            yitPos.setDecimal(libabbauw::properties::GEP, gep);
            agp = m_FindArticle->get().begin().getDecimal(libabbauw::properties::AGP);
            yitPos.setDecimal(libabbauw::properties::AGP, agp);

            if( isFirstPosition )
            {
                m_FindManufacturer->resetSearchYit();
                basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_FindManufacturer->getSearchYit();
                searchYit.setInt32( libabbauw::properties::MANUFACTURERNO, m_FindArticle->get().begin().getInt32( libabbauw::properties::MANUFACTURERNO ) );
			    m_FindManufacturer->findManufacturer( searchYit );
			    m_TransferOrderCreatorVC->matchFromManufacturer( m_FindManufacturer->getPropTab().begin() );

                m_CustomerFinder->resetSearchYit();
                basar::db::aspect::AccessorPropertyTable_YIterator yit = m_CustomerFinder->getSearchYit();
                m_TransferOrderCreatorVC->matchToPharmacy(yit);
                m_CustomerFinder->findByKey(yit.getInt32(libabbauw::properties::PHARMACYNO));

                m_TransferOrderCreatorVC->matchForHeadertext(m_FindManufacturer->getPropTab().begin());

                basar::Int32 purchaseOrderSupplierNo = m_FindArticle->get().begin().getInt32( libabbauw::properties::PURCHASEORDERSUPPLIERNO );

                m_PurchaseOrderSupplier->findPurchaseOrderSupplierById( purchaseOrderSupplierNo );
                m_TransferOrderCreatorVC->matchFromPurchaseOrderSupplier( m_PurchaseOrderSupplier->get() );
            }
            m_FindArticle->clear();

            m_TransferOrderCreatorVC->matchToTransferOrderHeadInPreparation();
            m_TransferOrderCreatorVC->matchFromTransferOrderPosInPreparationCollection();
            // set next position no
            m_TransferOrderCreatorVC->setNextPositionNo( m_TransferOrderInPreparation->getNextFreePosNo() );

            m_TransferOrderCreatorVC->setSavingAllowed( m_TransferOrderInPreparation->isChanged() );
            m_TransferOrderCreatorVC->setCreatingTransferOrderAllowed( m_TransferOrderInPreparation->isComplete() );

            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e )
        {
            m_TransferOrderCreatorVC->error( e.what() );
        }
        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, ChangePositionRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, ChangePositionRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );
        basar::Decimal  agp;
        basar::Decimal  gep;

        try {
            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderCreatorVC->getSelectedPosition();
            m_TransferOrderCreatorVC->matchToPosition( yitPos ); // write changed values back to yit
            m_FindArticle->resetSearchYit();
            m_TransferOrderCreatorVC->matchToPosition(m_FindArticle->getSearchYit());
            m_FindArticle->findArticle(m_FindArticle->getSearchYit());
            gep = m_FindArticle->get().begin().getDecimal(libabbauw::properties::GEP);
            yitPos.setDecimal(libabbauw::properties::GEP, gep);
            agp = m_FindArticle->get().begin().getDecimal(libabbauw::properties::AGP);
            yitPos.setDecimal(libabbauw::properties::AGP, agp);
            m_TransferOrderCreatorVC->matchFromTransferOrderPosInPreparationCollection();
            m_TransferOrderCreatorVC->switchToEditingMode( false ); // leave editing mode
            m_TransferOrderCreatorVC->setNextPositionNo( m_TransferOrderInPreparation->getNextFreePosNo() );
            m_TransferOrderCreatorVC->setSavingAllowed( m_TransferOrderInPreparation->isChanged() );

            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, RemovePositionRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, RemovePositionRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );

        try {
            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderCreatorVC->getSelectedPosition();
            m_TransferOrderInPreparation->markPosAsDeleted( yitPos );
            m_TransferOrderCreatorVC->matchFromTransferOrderPosInPreparationCollection();
            m_TransferOrderCreatorVC->matchToPosition( yitPos );
            m_TransferOrderCreatorVC->switchToEditingMode( false );
            m_TransferOrderCreatorVC->setNextPositionNo( m_TransferOrderInPreparation->getNextFreePosNo() );

            m_TransferOrderCreatorVC->setSavingAllowed(  m_TransferOrderInPreparation->isChanged() );
            m_TransferOrderCreatorVC->setCreatingTransferOrderAllowed( m_TransferOrderInPreparation->isComplete() );

            if( m_TransferOrderInPreparation->getPosCollection()->isEmpty() )
            {
                m_FindManufacturer->clear();
                m_PurchaseOrderSupplier->clear();
                m_TransferOrderCreatorVC->matchFromManufacturer( m_FindManufacturer->getPropTab().begin() );
                m_TransferOrderCreatorVC->matchFromPurchaseOrderSupplier( m_PurchaseOrderSupplier->get() );
            }

            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, CheckSaveStateRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, CheckSaveStateRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        m_TransferOrderCreatorVC->matchToTransferOrderHeadInPreparation();
        m_TransferOrderCreatorVC->setSavingAllowed( m_TransferOrderInPreparation->isChanged() );
            
        return basar::appl::EventReturnType();
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, CheckCreateTransferOrderStateRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, CheckSaveStateRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        m_TransferOrderCreatorVC->setCreatingTransferOrderAllowed( m_TransferOrderInPreparation->isComplete() );
            
        return basar::appl::EventReturnType();
    }
    

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, SaveTransferOrderInPreparationRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, SaveTransferOrderInPreparationRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );
        
        try 
		{
            saveTransferOrderInPreparation();

            result = basar::appl::HANDLER_OK;
		}
        catch( basar::Exception& e ) 
		{
            m_TransferOrderCreatorVC->error( e.what() );
        }

		return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, ResetTransferOrderInPreparationRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, ResetTransferOrderInPreparationRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );

        try {
            resetTransferOrderInPreparation();

            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, CreateTransferOrderRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, CreateTransferOrderRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );

	    try 
		{
            if( m_OrderCheckParameter->emitWarningOnNonChargedQty() && m_TransferOrderInPreparation->containsNonChargedQtyPosition() )
            {
                // Check if order contains positions containing ND and emit warning
                QString msg = TransferOrderCreatorUC::tr( "Non charged quantity (ND) was entered. Apply?" );
                if( basar::gui::tie::ButtonYes != m_TransferOrderCreatorVC->question( msg.toLocal8Bit().constData() ) )
                {
                    return basar::appl::HANDLER_OK;
                }
            }

            libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder = m_TransferOrderCreation->create( m_TransferOrderInPreparation );

            transferOrder->save();
            m_TransferOrderCreatorVC->showOrderNoInfoMsg( transferOrder->getOrderNo() );

			//Protocol it!
			m_OrderProtocol->protocolCreateOrder( transferOrder->getHead()->get(), transferOrder->getPosCollection()->get() );

            // clear transferOrder in preparation if everything went well
            resetTransferOrderInPreparation();
            saveTransferOrderInPreparation();

            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e ) 
        {
            m_TransferOrderCreatorVC->error( e.what() );
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( TransferOrderCreatorUC, DialogCloseRequested )
    {
        METHODNAME_DEF( TransferOrderCreatorUC, DialogCloseRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

	    m_TransferOrderCreatorVC->shutdown();
		return basar::appl::EventReturnType();
    }

    void TransferOrderCreatorUC::run()
	{
        METHODNAME_DEF( TransferOrderCreatorUC, run )
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, fun );
        registerEventHandlers( true );

        try {
            m_TransferOrderInPreparation->load();

			m_TransferOrderCreatorVC->init( m_TransferOrderInPreparation->getHead()->get(), m_TransferOrderInPreparation->getPosCollection()->get() );
			m_TransferOrderCreatorVC->show();

            m_TransferOrderCreatorVC->matchFromTransferOrderHeadInPreparation();
            m_TransferOrderCreatorVC->matchFromTransferOrderPosInPreparationCollection();
            m_TransferOrderCreatorVC->setSavingAllowed( false );
            m_TransferOrderCreatorVC->setCreatingTransferOrderAllowed( m_TransferOrderInPreparation->isComplete() );
            m_TransferOrderCreatorVC->setNextPositionNo( m_TransferOrderInPreparation->getNextFreePosNo() );

            // show purchase order supplier number if order has positions
            if( ! m_TransferOrderInPreparation->getPosCollection()->isEmpty() )
            {
                basar::VarString pzn = m_TransferOrderInPreparation->getPosCollection()->get().begin().getString( libabbauw::properties::PZN );
                
                m_FindArticle->resetSearchYit();
                m_FindArticle->getSearchYit().setString( libabbauw::properties::PZN, pzn );
                m_FindArticle->findArticle( m_FindArticle->getSearchYit() );
                basar::Int32 purchaseOrderSupplierNo = m_FindArticle->get().begin().getInt32( libabbauw::properties::PURCHASEORDERSUPPLIERNO );

                m_PurchaseOrderSupplier->findPurchaseOrderSupplierById( purchaseOrderSupplierNo );
                m_TransferOrderCreatorVC->matchFromPurchaseOrderSupplier( m_PurchaseOrderSupplier->get() );
            }

		} catch( basar::Exception& e ) {
            m_TransferOrderCreatorVC->error( e.what() );
        }
        
        m_TransferOrderCreatorVC->waitForEvents();
        
        registerEventHandlers( false );
	}
}
}
