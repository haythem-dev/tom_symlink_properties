#include "cscbatcheshistoryappl.h"
#include "usecasecontroller.h"

namespace cscbatchhistory 
{
	namespace useCase 
	{
		UseCaseController::UseCaseController()
		{ 
		}

		UseCaseController::~UseCaseController()
		{ 
		}

		CscBatchesHistoryApplBase& UseCaseController::getManager()
		{
			return CscBatchesHistoryAppl::getInstance();
		}

		void UseCaseController::registerEventHandlers(const bool)
		{ 
		}
	}
}
