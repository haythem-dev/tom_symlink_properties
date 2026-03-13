//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       12.06.2015
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libutil/infrastructurecomponent.h>
#include "infrastructure/itenderbatchprocessinglogingetterptr.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace setup
        {
            class TenderBatchProcessingInfrastructureSetup : public libutil::infrastructure::InfrastructureSetupBase
            {
                public:
                    TenderBatchProcessingInfrastructureSetup();
	                virtual ~TenderBatchProcessingInfrastructureSetup();
        	    
                    void injectLoginGetter( IBatchLoginGetterConstPtr );

                protected:
                    virtual void    doInit();
	                virtual void    doShutdown();
        	    
                private:
                    ////////////////////////////////
                    // methods
                    ////////////////////////////////
                    TenderBatchProcessingInfrastructureSetup( const TenderBatchProcessingInfrastructureSetup& r );
	                TenderBatchProcessingInfrastructureSetup& operator=( const TenderBatchProcessingInfrastructureSetup& r );

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

#endif // GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H
