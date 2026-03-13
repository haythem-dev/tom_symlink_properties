#ifndef GUARD_CSCBATCHESHISTORY_APPL_H
#define GUARD_CSCBATCHESHISTORY_APPL_H

#include "connection.h"

// use cases
#include "cscbatcheshistoryuc.h"
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasarappl_systemeventmanager.h>
#include <libbasardbaspect_connectionref.h>
#include <libbasarcmnutil_i18nstring.h>


namespace cscbatchhistory
{
	//**************************************************************************************************
	// forward declaration
	//**************************************************************************************************
	class CscBatchesHistoryAppl;

	class CscBatchesHistoryApplBase
	{
		friend class basar::cmnutil::Singleton<CscBatchesHistoryApplBase>; //!< singleton as friend for construction!

	public:
		virtual ~CscBatchesHistoryApplBase();

		//**************************************************************************************************
		/*! \brief Main entrance, called from main()
			\param rSource source where event is thrown
			\param yitSearch search criteria from source
			\return basar::appl::HANDLER_OK, if everything worked fine \n
					basar::appl::HANDLER_ERROR otherwise
			\throw no-throw
		*/
		//**************************************************************************************************
		basar::appl::EventReturnType run(basar::db::aspect::AccessorPropertyTable_YIterator yitSearch);

		basar::db::aspect::ConnectionRef getConnectionCsc();

	private:
		CscBatchesHistoryApplBase();

		CscBatchesHistoryApplBase(const CscBatchesHistoryApplBase& rCopy);
		CscBatchesHistoryApplBase& operator =	(const CscBatchesHistoryApplBase& rCopy);

		basar::appl::EventReturnType init();

		void shutdown();

		bool getUadmParameter();

		Connection m_ConnectionCsc;	//!< connection for User-DB

		useCase::CscBatchesHistoryUC m_CscBatchesHistoryUC; //!< Instance of the use case controller for orders.

		basar::Int16 m_BranchNo;
		basar::I18nString m_Host;
		basar::I18nString m_Db;
	};

	//**************************************************************************************************
	/*! \brief Derived class from CscBatchesHistoryApplBase. \n
		This is a final class. Singleton \n
		\note  class is necessary for dll exporting Singleton\<CscBatchesHistoryApplBase\>
	*/
	//**************************************************************************************************
	class CscBatchesHistoryAppl : public basar::cmnutil::Singleton<CscBatchesHistoryApplBase>
	{
		friend class dummy;	//!< prevents pedantic compiler warning

	private:
		CscBatchesHistoryAppl();
		CscBatchesHistoryAppl(const CscBatchesHistoryAppl& r);
		CscBatchesHistoryAppl& operator = (const CscBatchesHistoryAppl& r);
	};
}

/*! \ingroup DIFF_WIN_UNIX */

#endif	// GUARD
