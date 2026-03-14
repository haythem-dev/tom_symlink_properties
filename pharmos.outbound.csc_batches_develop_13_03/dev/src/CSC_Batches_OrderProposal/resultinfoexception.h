#ifndef GUARD_RESULT_INFO_EXCEPTION_H
#define GUARD_RESULT_INFO_EXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

#include "abbauwexception.h"

namespace libabbauw
{
	class ResultInfoException : public AbbauwException
	{
		public:
			/*! \brief std constructor */
			ResultInfoException( const basar::ExceptInfo & sInfo );
			/*! \brief std-destructor */
			virtual ~ResultInfoException();

		private:
			//!< forbidden
			ResultInfoException();					
	};

} // end namespace libabbauw



#endif // GUARD_ABBAUW_EXCEPTION_H
