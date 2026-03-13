#include "transferorderpositionconnectedfactory.h"
#include "transferorderpositiondm.h"

#include "infrastructure/accessor/transferorder/transferorderpositionacc.h"
#include "infrastructure/accessor/transferorder/transferorderpositionacc_definitions.h"

#include <boost/make_shared.hpp>

#include <libbasardbaspect_accessorpropertytable_yiterator.h>

namespace libabbauw
{
namespace componentManager
{

    TransferOrderPositionConnectedFactory::TransferOrderPositionConnectedFactory()
    {
    }

    TransferOrderPositionConnectedFactory::~TransferOrderPositionConnectedFactory()
    {
    }

    libutil::infrastructure::accessor::IAccessorPtr TransferOrderPositionConnectedFactory::getTransferOrderPositionAccessor() const
    {
        infrastructure::accessor::transferOrder::SelectTransferOrderPositionByKey();

        using libutil::infrastructure::accessor::AccessorProxy;
        return boost::make_shared< AccessorProxy >( infrastructure::accessor::transferOrder::lit::TRANSFER_ORDER_POSITION_ACC, getDBConnection() );
    }

    domMod::transferOrder::TransferOrderPositionDMPtr TransferOrderPositionConnectedFactory::getTransferOrderPositionDM() const
    {
        using libutil::infrastructure::accessor::AccessorProxy;
        domMod::transferOrder::TransferOrderPositionDMPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderPositionDM >();
        //dm->injectTransferOrderPositionAccessor( boost::make_shared< AccessorProxy >( infrastructure::accessor::transferOrder::lit::TRANSFER_ORDER_POSITION_ACC, getDBConnection() ) );

        return dm;
    }

    void TransferOrderPositionConnectedFactory::injectDBConnection( libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr dbConnDataGetter )
    {
        m_DBConnectionDataGetter = dbConnDataGetter;
    }

    basar::db::aspect::ConnectionRef TransferOrderPositionConnectedFactory::getDBConnection() const
    {
        CHECK_INSTANCE_EXCEPTION( m_DBConnectionDataGetter.get() );
        return m_DBConnectionDataGetter->getCurrentConnection();
    }

    domMod::transferOrder::ITransferOrderPositionPtr TransferOrderPositionConnectedFactory::create( basar::db::aspect::AccessorPropertyTable_YIterator yit ) const
    {
        domMod::transferOrder::TransferOrderPositionDMPtr dm = getTransferOrderPositionDM();
        dm->set( yit );

        return dm;
    }

} // end namespace componentManager
} // end namespace libabbauw
