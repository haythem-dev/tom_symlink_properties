//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  abstract interface of a base use case controller
 *  \author 
 *  \date   
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_TEMPLATE_USECASECONTROLLER_H
#define GUARD_TEMPLATE_USECASECONTROLLER_H

//problems with Unix: #define NULL 0  //!< \todo remove with original typedef or defincition

#include <libbasarcmnutil_pattern.h> //!< \bug only required to include libbasarappl_systemeventmanager.h
#include <vector> //!< \bug only required to include libbasarcmnutil_bstring.h
#include <libbasar_definitions.h> //!< \bug only required to include libbasarcmnutil_bstring.h
#include <libbasarcmnutil_bstring.h> //!< \bug only required to include libbasarcmnutil_i18nstring.h
#include <libbasarcmnutil_i18nstring.h> //!< \bug only required to include libbasarappl_systemeventmanager.h
#include <libbasarappl_systemeventmanager.h>

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace cscbatchauto {
	
class CscBatchAutoApplBase; // forward declaration

namespace useCase {
//-------------------------------------------------------------------------------------------------//
using basar::appl::EventReturnType;
using basar::appl::IEventSource;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Use Case class to derive all use cases from.

	\n This is a base class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class UseCaseController
{
public:
	UseCaseController();
	virtual ~UseCaseController();

	/*! \brief This method returns a reference to the application controller
		\n no-throw */
	CscBatchAutoApplBase& getManager();
	
	/*! \brief This method starts the use case - each use case must have this method / overwrite it (pure virtual)
		\n no-throw */
	virtual EventReturnType 
		run( IEventSource & rSource,					//!< source where event is thrown
			 AccessorPropertyTable_YIterator yitSearch	//!< search criteria from source
			) = 0;

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	virtual void registerEventHandlers(const bool forSave	//!< register = true / deregister = false
										);

};

//---------------------------------------------------------------------------
} // useCase
} // CscBatches

#endif //GUARD
