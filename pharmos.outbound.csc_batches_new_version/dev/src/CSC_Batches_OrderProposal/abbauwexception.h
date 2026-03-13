#ifndef GUARD_ABBAUW_EXCEPTION_H
#define GUARD_ABBAUW_EXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
	class AbbauwException : public basar::Exception
	{
		public:
			/*! \brief std constructor */
			AbbauwException( const basar::ExceptInfo & sInfo, const basar::VarString & exceptionType );
			/*! \brief std-destructor */
			virtual ~AbbauwException();

		private:
			//!< forbidden
			AbbauwException();					
	};

} // end namespace libabbauw



#endif // GUARD_ABBAUW_EXCEPTION_H