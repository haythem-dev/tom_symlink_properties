//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       12.06.2015
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H
#define GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libutil/infrastructurecomponent.h>
#include "infrastructure/itenderordercreatorlogingetterptr.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace tenderordercreator
{
    namespace infrastructure
    {
        namespace setup
        {
            class TenderOrderCreatorInfrastructureSetup : public libutil::infrastructure::InfrastructureSetupBase
            {
                public:
                    TenderOrderCreatorInfrastructureSetup();
	                virtual ~TenderOrderCreatorInfrastructureSetup();
        	    
                    void injectLoginGetter( IBatchLoginGetterConstPtr );

                protected:
                    virtual void    doInit();
	                virtual void    doShutdown();
        	    
                private:
                    ////////////////////////////////
                    // methods
                    ////////////////////////////////
                    TenderOrderCreatorInfrastructureSetup( const TenderOrderCreatorInfrastructureSetup& r );
	                TenderOrderCreatorInfrastructureSetup& operator=( const TenderOrderCreatorInfrastructureSetup& r );

                    void login();
                    void prepareLogin();

                    ////////////////////////////////
                    // variables
                    ////////////////////////////////
                    IBatchLoginGetterConstPtr m_BatchLoginGetter;
            };
        }
    }
}

#endif // GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H
