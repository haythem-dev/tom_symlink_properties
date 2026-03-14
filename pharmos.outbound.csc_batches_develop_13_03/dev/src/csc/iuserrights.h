#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_IUSERRIGHTS_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_IUSERRIGHTS_H

namespace CSC_Batches
{
namespace infrastructure
{
namespace rights
{
    class IUserRights
    {
    public:
        virtual ~IUserRights() {}

        virtual void                                                     checkUserAllowedToLaunchAppl()                  = 0;
        virtual void                                                     checkUserAllowedToAccessBatchOrderMaintenance() = 0;

        virtual void                                                     checkUserAllowedToAccessTransferOrders()     = 0;
        virtual void                                                     checkUserAllowedToCreateTransferOrders()     = 0;
        virtual bool                                                     isUserAllowedToChangeTransferOrders()        = 0;

        virtual bool                                                     isUserAllowedToPlaceOrderWishes()            = 0;
        
        virtual void                                                     checkUserAllowedToAccessSettings()           = 0;
        virtual bool                                                     isUserAllowedToChangePrivilegedSettings()    = 0;
        virtual bool                                                     isUserAllowedToAccessDeveloperInfo()         = 0;

    };

} // end namespace rights
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_IUSERRIGHTS_H
