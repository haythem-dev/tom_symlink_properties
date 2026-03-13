//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderproposalcollectiondm.h"

#include "infrastructure/accessor/orderproposal/orderproposalcollectionacc.h"
#include "infrastructure/accessor/orderproposal/orderproposalcollectionacc_definitions.h"

#include "componentmanager/iorderproposalfactory.h"
#include "domainmodule/orderproposal/iorderproposal.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include "bitfieldresolver.h"

#include <libutil/exception.h>
#include <libutil/util.h>

#include <boost/make_shared.hpp>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    const libutil::misc::ClassInfo & OrderProposalCollectionDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "OrderProposalCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    OrderProposalCollectionDM::OrderProposalCollectionDM()
		: m_Logger( libabbauw::LoggerPool::loggerOrderProposal )
	{
        METHODNAME_DEF( OrderProposalCollectionDM, OrderProposalCollectionDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

	OrderProposalCollectionDM::~OrderProposalCollectionDM()
	{
        METHODNAME_DEF( OrderProposalCollectionDM, ~OrderProposalCollectionDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

    const log4cplus::Logger& OrderProposalCollectionDM::getLogger() const
    {
        return m_Logger;
    }

    void OrderProposalCollectionDM::injectOrderProposalCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    libutil::infrastructure::accessor::IAccessorPtr OrderProposalCollectionDM::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        return m_Accessor;
    }

    void OrderProposalCollectionDM::injectOrderProposalFactory( componentManager::IOrderProposalFactoryPtr orderProposalFactory )
    {
        m_OrderProposalFactory = orderProposalFactory;
    }

    componentManager::IOrderProposalFactoryPtr OrderProposalCollectionDM::getOrderProposalFactory() const
    {
        CHECK_INSTANCE_EXCEPTION( m_OrderProposalFactory.get() );
        return m_OrderProposalFactory;
    }

    basar::db::aspect::AccessorPropertyTableRef OrderProposalCollectionDM::get()
	{
		return getAccessor()->getPropertyTable();
	}
    
    basar::db::aspect::AccessorPropertyTableRef OrderProposalCollectionDM::get() const
	{
		return getAccessor()->getPropertyTable();
	}

	basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalCollectionDM::getSearchYit()
	{
		return getSearchYIterator()->get();
	}

	void OrderProposalCollectionDM::resetSearchYit()
	{
		getSearchYIterator()->reset();
	}

    void OrderProposalCollectionDM::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
	{
        METHODNAME_DEF( OrderProposalCollectionDM, findByKey )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using libabbauw::infrastructure::accessor::orderProposal::lit::SELECT_ORDER_PROPOSALS_BY_KEY;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( SELECT_ORDER_PROPOSALS_BY_KEY, yitSearch );

			setDeliveryNote(m_Accessor->getPropertyTable());
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SELECT_ORDER_PROPOSALS_BY_KEY, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_ORDER_PROPOSALS_BY_KEY, executeResultInfo )
        }
	}

    basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalCollectionDM::addNewItem()
    {
        METHODNAME_DEF( OrderProposalCollectionDM, addNewItem )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return get().insert( basar::FOR_UPDATE );
    }

    void OrderProposalCollectionDM::removeItem( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        METHODNAME_DEF( OrderProposalCollectionDM, removeItem )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( ! yit.isEnd() )
        {
            get().erase( yit );
        }
    }

    void OrderProposalCollectionDM::save()
    {
        METHODNAME_DEF( OrderProposalCollectionDM, save )
        BLOG_TRACE_METHOD( getLogger(), fun );

        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); ! yit.isEnd(); ++yit )
        {
            IOrderProposalPtr orderProposal = getOrderProposalFactory()->create( yit  );
            orderProposal->save();
        }
    }

    libutil::domMod::SearchYIteratorPtr OrderProposalCollectionDM::getSearchYIterator()
    {
        METHODNAME_DEF( OrderProposalCollectionDM, getSearchYIterator )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( NULL == m_SearchYIterator.get() )
        {
            m_SearchYIterator = boost::make_shared< libutil::domMod::SearchYIterator >( properties::BRANCHNO.toDescriptionString() +
                                                                                        properties::ORDERNO.toDescriptionString()  +
                                                                                        properties::SORT.toDescriptionString()
                                                                                      );
        }

        return m_SearchYIterator;
    }

	void OrderProposalCollectionDM::setDeliveryNote(basar::db::aspect::AccessorPropertyTableRef propTab)
	{
		basar::db::aspect::AccessorPropertyTable_YIterator proposal = propTab.begin();

		//Iterate through all positions
		while( !proposal.isEnd() )
		{
			basar::VarString deliveryNote = "";
			basar::Int32	 bitField1	  = 0;
			basar::Int32	 bitField2	  = 0;

			//Add artikellokal.etartschalter1 + artikelzentral.etartschalter1 to one bitfield,
			//because the information for the delivery note could be in both!
			bitField1 = proposal.getInt32(properties::LOCALARTICLEBITFIELD)  + proposal.getInt32(properties::CENTRALARTICLEBITFIELD);
			bitField2 = proposal.getInt32(properties::LOCALARTICLEBITFIELD2) + proposal.getInt32(properties::CENTRALARTICLEBITFIELD2);

			deliveryNote = util::BitFieldResolver().resolveDeliveryNote( bitField1, bitField2, proposal.getInt32(properties::STOCK) );

			proposal.setString( properties::DELIVERYNOTE, deliveryNote );

			++proposal;
		}
	}

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw
