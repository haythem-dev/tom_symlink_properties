#ifndef GUARD_CSCBATCHESHISTORY_USECASECONTROLLER_H
#define GUARD_CSCBATCHESHISTORY_USECASECONTROLLER_H

#ifdef WIN32
	#pragma warning (push)
	#pragma warning(disable: 4512)
	#include <libbasarappl_systemeventmanager.h>
	#pragma warning (pop)
#else
	#include <libbasarappl_systemeventmanager.h>
#endif

namespace cscbatchhistory 
{
	class CscBatchesHistoryApplBase; // forward declaration

	namespace useCase 
	{
		class UseCaseController
		{
		public:
			UseCaseController();
			virtual ~UseCaseController();

			/*! \brief This method returns a reference to the application controller
				\n no-throw */
			CscBatchesHistoryApplBase& getManager();
			
			/*! \brief This method starts the use case - each use case must have this method / overwrite it (pure virtual)
				\n no-throw */
			virtual basar::appl::EventReturnType run(basar::db::aspect::AccessorPropertyTable_YIterator yitSearch) = 0;

			/*! \brief std method; has to be the first call in run() if events should be caught;
				\n no-throw */
			virtual void registerEventHandlers(const bool forSave); 
		};
	}
}
#endif //GUARD
