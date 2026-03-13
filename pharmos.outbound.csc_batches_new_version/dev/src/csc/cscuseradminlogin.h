#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_LOGIN_CSCUSERADMINLOGIN_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_LOGIN_CSCUSERADMINLOGIN_H

#include <libutil/login.h>

namespace CSC_Batches
{
namespace infrastructure
{
namespace login
{
	class CSCUserAdminLogin : public libutil::infrastructure::login::LoginBase
	{
		public:
			CSCUserAdminLogin( const libutil::infrastructure::login::UserLoginData& );
			~CSCUserAdminLogin();

		private:
			// from LoginBase / ILogin
            bool doLogin();

			CSCUserAdminLogin( const CSCUserAdminLogin& r );
			CSCUserAdminLogin& operator = ( const CSCUserAdminLogin& r );
	};

} // end namespace login
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_LOGIN_CSCUSERADMINLOGIN_H
