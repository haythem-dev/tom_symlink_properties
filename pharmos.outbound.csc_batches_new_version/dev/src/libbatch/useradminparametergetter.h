//-----------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of database login class
 *  \author     Bischof Bjoern
 *  \date       31.12.2010
 *  \version    00.00.00.01 first version
 */ 
//-----------------------------------------------------------------------------------------------//
#ifndef GUARD_LIBBATCH_USER_ADMIN_PARAMETER_GETTER_H
#define GUARD_LIBBATCH_USER_ADMIN_PARAMETER_GETTER_H

//-----------------------------------------------------------------------------------------------//
// include section
//-----------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>

//-----------------------------------------------------------------------------------------------//
//  forward declaration section
//-----------------------------------------------------------------------------------------------//
namespace log4cplus {
    class Logger;
}

//-----------------------------------------------------------------------------------------------//
//  class declaration section
//-----------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{
    class UserAdminParameterGetter
    {
    public:
	    UserAdminParameterGetter();
	    ~UserAdminParameterGetter();

        bool  getParameterValue( const basar::I18nString& parameterName, basar::I18nString& value ) const;
        bool  getParameterValue( const basar::I18nString& parameter1,    const basar::I18nString& parameterName, basar::I18nString& value ) const;
        bool  getParameterValue( const basar::Int16 parameter1,          const basar::I18nString& parameterName, basar::I18nString& value ) const;
        bool  getParameterValue( const basar::I18nString& parameter1,    const basar::I18nString& parameter2, 
                                       const basar::I18nString& paraName,      basar::I18nString& value ) const;

    private:
        ////////////////////////////////
        // methods
        ////////////////////////////////
	    UserAdminParameterGetter( const UserAdminParameterGetter& r );
	    UserAdminParameterGetter& operator = ( const UserAdminParameterGetter& r );

        ////////////////////////////////
        // variables
        ////////////////////////////////
        const log4cplus::Logger&    m_Logger;
    };

} // end namespace userAdmin
} // end namespace libBatch

#endif // end GUARD_LIBBATCH_USER_ADMIN_PARAMETER_GETTER_H
