//--------------------------------------------------------------------------------------------------//
/*! \brief      
 *  \author     Bischof Bjoern
 *  \date       29.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_APP_SESSION_H
#define GUARD_ABBAUW_APP_SESSION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "appargs.h"
#include <libbasarcmnutil.h>
#include <dbconnectionbaseptr.h>
#include <userloginconfiguration.h>
#include <iosfwd>

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{

class AppSession
{  
	public:
        AppSession( libBatch::dbConnection::DBConnectionBasePtr, const cmdLine::AppArgs&, const libBatch::userAdmin::UserLoginConfiguration& );
		~AppSession();

        std::ostream& toStream( std::ostream& strm = std::cout ) const;

        libBatch::dbConnection::DBConnectionBasePtr         m_DBConnection;
        const cmdLine::AppArgs                              m_AppArgs;
        const libBatch::userAdmin::UserLoginConfiguration   m_UserLoginConfiguration;

        void start();
        void end();

	private:
        AppSession( const AppSession& );
        AppSession& operator=( const AppSession& );
};

}

#include "appsession.inl"

#endif 
