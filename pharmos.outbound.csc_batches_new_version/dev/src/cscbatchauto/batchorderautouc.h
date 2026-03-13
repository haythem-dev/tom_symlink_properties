//-------------------------------------------------------------------------------------------------//
/*! \file batchorderautouc.h
 *  \brief UseCase controller for batchorderuc
 *  \author Beatrix Trömel
 *  \date 21.05.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_BATCHORDERAUTOUC_H
#define GUARD_BATCHORDERAUTOUC_H

#include <libbasarlogin.h>
#include "usecasecontroller.h"
#include "batchorderdm.h"
#include "cscorderdmptr.h"

namespace cscbatchauto {
  namespace useCase {    
	//-------------------------------------------------------------------------------------------------//
	// using declarations
	//-------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	/*! 
	\brief  UsecaseController for suppliers
	*/
	class BatchOrderAutoUC : public UseCaseController {
	public:
		/*!
		\brief Default constructor.
		*/
		BatchOrderAutoUC();
		BatchOrderAutoUC(basar::db::aspect::ConnectionRef ConnectionCsc);

		/*!
		\brief Destructor.
		*/
		virtual ~BatchOrderAutoUC();		     

		//-------------------------------------------------------------------------------------------------//
		/*! \brief Usecase controller main routine
			\param rSource source where event is thrown
			\param yitMain parameters. branchno, kscserver and kscport have to be in it.
			\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		virtual EventReturnType run(IEventSource & rSource,	AccessorPropertyTable_YIterator yitMain);
	
	private:
		basar::appl::EventReturnType getSettings();
		//-------------------------------------------------------------------------------------------------//
		/*! \brief Searches all bacth orders for given branchno with status '0' and datasourcetype for automatic release
			\param PropTabResult (ref-param) to store the results.
			\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		basar::appl::EventReturnStruct searchOrders(basar::db::aspect::AccessorPropertyTableRef &PropTabResult);
		basar::appl::EventReturnStruct releaseOrders();		

		basar::db::aspect::ConnectionRef m_ConnectionCsc; //!< reference to the existing db connection to Informix
		libcscbatch::domMod::BatchOrderDM m_BatchOrderDM; //!< domain module class
		libcscbatch::domMod::CSCOrderDMPtr m_CscOrderDM; //!< domain module class
		basar::Int16 m_branchNo; //!< branch no from login

	};
  }// namespace useCase
}// namespace
#endif
