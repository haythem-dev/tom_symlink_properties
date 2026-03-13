//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief 
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_DM_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_DM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "pubatchorderposinitparams.h"
#include "igetpubatchorderpos.h"
#include "isetpubatchorderpos.h"
#include <libutil/classinfo.h>
#include <libutil/misc/componentinitialization.hpp>

using basar::db::aspect::AccessorInstanceRef;
//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
namespace OrderProposalTransfer
{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
    class PuBatchOrderPosDM :  public IPuBatchOrderPositionWriter, public IPuBatchOrderPositionFinder, 
                               public libutil::misc::ComponentInitialization< PuBatchOrderPosInitParams, PuBatchOrderPosDM >
	{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
	    public:
		    PuBatchOrderPosDM( const basar::db::aspect::ConnectionRef );
		    ~PuBatchOrderPosDM();

			virtual basar::db::aspect::AccessorPropertyTableRef getTable( ) const;
			virtual void savePuBatchOrderPositions( basar::db::aspect::AccessorPropertyTable_YIterator it );
			virtual const basar::db::aspect::AccessorPropertyTableRef findPuBatchOrderPositions();

			virtual void commitTransaction();	
			virtual void beginTransaction();
			virtual void rollbackTransaction();
			virtual bool isInTransaction() const;

        protected:
	            void doInit( const PuBatchOrderPosInitParams& );
	            void doShutdown();

	    private:

			
            /////////////////////////////////////////////////////////////////////////
		    // private member methods
		    /////////////////////////////////////////////////////////////////////////
		    PuBatchOrderPosDM(const PuBatchOrderPosDM& PuBatchOrderPosDM);
		    PuBatchOrderPosDM& operator= (const PuBatchOrderPosDM& PuBatchOrderPosDM);

                   
		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////

            basar::db::aspect::ConnectionRef m_Connection;
			basar::db::aspect::AccessorInstanceRef m_Accessor;
			PuBatchOrderPosInitParams              m_InitParams;  

	};
} // namespace 
} // end namespace 

#endif 