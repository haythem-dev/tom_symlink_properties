//-------------------------------------------------------------------------------------------------//
/*! \file	cscbatchesappl.h
 *  \brief  application class (main) for CscBatches component (singleton)
 *  \author	Beatrix Trömel
 *  \date	01.09.2008 
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSCBATCHAUTOAPPL_H
#define GUARD_CSCBATCHAUTOAPPL_H

//-------------------------------------------------------------------------------------------------//
// includes 
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include "connection.h"

namespace cscbatchauto {
	namespace useCase {
		class BatchOrderAutoUC;
	}
}

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace cscbatchauto {

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class CscBatchAutoAppl;

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  One and only instance for controlling CscBatches application.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class CscBatchAutoApplBase
{

	//---------------------------------------------------------//
	// friends
	//---------------------------------------------------------//	
	friend class basar::cmnutil::Singleton<CscBatchAutoApplBase>; //!< singleton as friend for construction!

public:
	/*! \brief destructor \n no-throw */
	virtual ~CscBatchAutoApplBase();

	/*! \brief main entrance, called from main()
		\n no-throw */
	basar::appl::EventReturnType	run(basar::appl::IEventSource & rSource,					//!< source where event is thrown
										basar::db::aspect::AccessorPropertyTable_YIterator yitSearch	//!< search criteria from source
										);
	//-------------------------------------------------------------------------------------------------//
	/*!	\throw  no-throw */
	//-------------------------------------------------------------------------------------------------//
	ConnectionRef getConnection();

private:
	CscBatchAutoApplBase(); //!< std constructor, private because of singleton
	CscBatchAutoApplBase(const CscBatchAutoApplBase & rCopy);	//!< forbidden copy constructor
	CscBatchAutoApplBase & operator =	(const CscBatchAutoApplBase & rCopy);	//!< forbidden assignment operator

	/*! \brief initialize members
		\n no-throw */
	basar::appl::EventReturnType init();
	bool getUadmParameter();

	//**************************************************************************************************
	/*! \brief	de-initialize members, so that not all value members have to be held the whole time!\n
				disconnect from database
		\throw	no-throw 
	*/
	//**************************************************************************************************
	void shutdown();

	//---------------------------------------------------------//
	// UseCases
	//---------------------------------------------------------//	
	useCase::BatchOrderAutoUC *m_BatchOrderAutoUC; //!< Instance of the use case controller for orders.

	//---------------------------------------------------------//
	// Connection
	//---------------------------------------------------------//
	Connection m_ConnectionCsc;	//!< connection for cscbatchauto
	
	//---------------------------------------------------------//
	// Other
	//---------------------------------------------------------//
	basar::Int16		m_BranchNo;
	basar::I18nString	m_Host;
	basar::I18nString	m_Db;
	basar::I18nString	m_KscServer;
	basar::I18nString	m_KscPort;
};
	
//---------------------------------------------------------------------------------------//
/*! \brief  derived class from CscBatchesApplBase.
	This is a final class. Singleton 
 *  \note  class is necessary for dll exporting Singleton\<CscBatchesApplBase\>
 */
class CscBatchAutoAppl : public basar::cmnutil::Singleton<CscBatchAutoApplBase>
{
	friend class dummy;	//!< prevents pedantic compiler warning

private:
	/*! \brief standard constructor 
	\n     no-throw */
	CscBatchAutoAppl();
	/*! \brief copy constructor 
	\n     no-throw */
	CscBatchAutoAppl(const CscBatchAutoAppl & r);
	/*! \brief assign operator 
	\n     no-throw */
	CscBatchAutoAppl & operator = (const CscBatchAutoAppl & r);
};

//---------------------------------------------------------------------------------------//

} // namespace cscbatchauto

//---------------------------------------------------------------------------------------//

#endif	// GUARD_CSCBATCHAUTOAPPL_H
