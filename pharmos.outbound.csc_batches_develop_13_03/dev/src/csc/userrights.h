#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_USERRIGHTS_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_USERRIGHTS_H

#include "iuserrights.h"

#include <libutil/parameter.h>
#include <libutil/session.h>

namespace CSC_Batches
{
namespace infrastructure
{
namespace rights
{
    class UserRights : public IUserRights
    {
    public:
        UserRights();
        ~UserRights();

        void                                                     injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr );
        void                                                     injectSession( libutil::infrastructure::session::ISessionPtr );

        // from IUserRights interface
        void                                                     checkUserAllowedToLaunchAppl();
        void                                                     checkUserAllowedToAccessBatchOrderMaintenance();

        void                                                     checkUserAllowedToAccessTransferOrders();
        void                                                     checkUserAllowedToCreateTransferOrders();
        bool                                                     isUserAllowedToChangeTransferOrders();

        bool                                                     isUserAllowedToPlaceOrderWishes();
        
        void                                                     checkUserAllowedToAccessSettings();
        bool                                                     isUserAllowedToChangePrivilegedSettings();

        bool                                                     isUserAllowedToAccessDeveloperInfo();

    private:
        // forbidden
        UserRights( const UserRights& );
        UserRights& operator = ( const UserRights& );

        libutil::infrastructure::parameter::IParameterGatewayPtr getParameterGateway() const;
        libutil::infrastructure::session::ISessionPtr            getSession() const;

        bool                                                     isUserGranted(    const basar::I18nString& right, basar::I18nString& msg );
        bool                                                     isLegitimated(    const basar::I18nString& right );
        void                                                     checkLegitimated( const basar::I18nString& right );

    private:
        const log4cplus::Logger&                                 m_Logger;

        libutil::infrastructure::parameter::IParameterGatewayPtr m_ParameterGateway;
        libutil::infrastructure::session::ISessionPtr            m_Session;
    };

} // end namespace rights
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_USERRIGHTS_H
