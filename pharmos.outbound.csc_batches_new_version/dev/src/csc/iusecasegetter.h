#ifndef GUARD_CSCBATCHES_COMPONENTMANAGER_IUSECASEGETTER_H
#define GUARD_CSCBATCHES_COMPONENTMANAGER_IUSECASEGETTER_H

#include <libutil/irunnableptr.h>

#include <vector>
#include <libbasar_definitions.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
		}
	}
}

namespace CSC_Batches
{
namespace componentManager
{

    class IUseCaseGetter
    {
    public:
        virtual ~IUseCaseGetter() {}
        typedef std::vector< basar::db::aspect::AccessorPropertyTable_YIterator > YIterators;

        virtual libutil::useCase::IRunnablePtr                                          getMainUC()                                           = 0;
        
        virtual libutil::useCase::IRunnablePtr				                            getBatchOrderMaintenanceUC()                          = 0;
        virtual libutil::useCase::IRunnablePtr                                          getBatchOrderPosUC( basar::db::aspect::AccessorPropertyTable_YIterator )   = 0;

        virtual libutil::useCase::IRunnablePtr				                            getBVOStatusUC()                                      = 0;
	    virtual libutil::useCase::IRunnablePtr			                                getBVOOrderStatusUC( basar::Int32 orderNo, bool isOrderViewActive )           = 0;
        virtual libutil::useCase::IRunnablePtr                                          getCollectiveOrderWishUC( const YIterators&, bool isOrderViewActive )         = 0;
        virtual libutil::useCase::IRunnablePtr                                          getFilteredCollectiveOrderWishUC( const YIterators&, bool isOrderViewActive ) = 0;

	    virtual libutil::useCase::IRunnablePtr                                          getFindCustomerUC()                                   = 0;
	    virtual libutil::useCase::IRunnablePtr                                          getFindManufacturerUC()                               = 0;
	    virtual libutil::useCase::IRunnablePtr                                          getFindArticleUC()                                    = 0;

	    virtual libutil::useCase::IRunnablePtr                                          getBatchOrderEditionUC()                              = 0;
	    virtual libutil::useCase::IRunnablePtr                                          getTransferOrderPosUC( basar::db::aspect::AccessorPropertyTable_YIterator, bool isOrderViewActive ) = 0;
	    virtual libutil::useCase::IRunnablePtr                                          getUserSettingsUC()                                   = 0;

        virtual libutil::useCase::IRunnablePtr                                          getTransferOrderChangerUC( const YIterators& )        = 0;
        virtual libutil::useCase::IRunnablePtr                                          getTransferOrderCreatorUC()                           = 0;
    };

} // end namespace componentManager
} // end CSC_Batches

#endif // GUARD_CSCBATCHES_COMPONENTMANAGER_IUSECASEGETTER_H
