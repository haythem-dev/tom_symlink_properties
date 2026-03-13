#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERCREATIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERCREATIONDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferordercreationdm.h"

#include "ifindarticlepriceptr.h"
#include "iordercreationparameterptr.h"

#include <libbasardbaspect_connectionref.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace libutil
{
    namespace misc
    {
	    class ClassInfo;
    }
}

namespace libabbauw
{
namespace domMod
{
namespace transferOrder  
{

    class TransferOrderCreationDM : public ITransferOrderCreationDM
    {
    public:
	    TransferOrderCreationDM();
	    ~TransferOrderCreationDM();

        void                                                                       injectTransferOrder( transferOrder::ITransferOrderPtr );
        void                                                                       injectArticleConsultant( article::IFindArticlePricePtr );	
        void                                                                       injectOrderCreationParameter( parameter::IOrderCreationParameterPtr );

        // defined in ITransferOrderCreationDM interface
        transferOrder::ITransferOrderPtr                                           create( transferOrder::ITransferOrderInPreparationDMPtr );


    public:
	    static const libutil::misc::ClassInfo&                                     getClassInfo();


    private:
        TransferOrderCreationDM ( const TransferOrderCreationDM& );
	    TransferOrderCreationDM operator = ( const TransferOrderCreationDM& );

        const log4cplus::Logger&                                                   getLogger() const;
        void                                                                       createHead( transferOrder::ITransferOrderInPreparationDMPtr );
        void                                                                       createPos( transferOrder::ITransferOrderInPreparationDMPtr );

    private:
	    const log4cplus::Logger&                                                   m_Logger;
        transferOrder::ITransferOrderPtr                                           m_TransferOrderDM;
        article::IFindArticlePricePtr                                              m_ArticlePriceDM;
        parameter::IOrderCreationParameterPtr                                      m_OrderCreationParameter;
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERCREATIONDM_H
