//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "transferorderheadinpreparationpropertystatechecker.h"
#include "libabbauw_properties_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libutil/misc/log_macro_definitions.h>

#include <libbasarproperty.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace libabbauw
{
    namespace accessor
    {
        namespace transferOrder
        {
            HeadInPreparationPropertyStateChecker::HeadInPreparationPropertyStateChecker( basar::db::aspect::AccessorPropertyTable_YIterator yit )
            : m_Logger( LoggerPool::loggerLibAbbaUW ), m_Yit( yit )
            {
            }

            HeadInPreparationPropertyStateChecker::~HeadInPreparationPropertyStateChecker()
            {
            }

            bool HeadInPreparationPropertyStateChecker::isSelectable() const
            {
                return m_Yit.arePropertiesSet( properties::BRANCHNO.toPropertyString() + properties::USER.toPropertyString() );
            }

            bool HeadInPreparationPropertyStateChecker::isInsertable() const
            {
                METHODNAME_DEF( HeadInPreparationPropertyStateChecker, isInsertable )

                static const basar::VarString propertiesMustSet( properties::BRANCHNO.toPropertyString()            +
                                                                 properties::USER.toPropertyString()
                                                               );

                static const basar::VarString properties4Insert( properties::ORDERTYPE.toPropertyString()           + 
                                                                 properties::MANUFACTURERNO.toPropertyString()      +
                                                                 properties::PHARMACYNO.toPropertyString()
                                                               );

                LOG_YIT( m_Yit, getLogger(), fun );
                bool isInsertable = m_Yit.arePropertiesSet( propertiesMustSet );
                isInsertable     &= m_Yit.arePropertiesSetAndOneMustBeInState( properties4Insert + propertiesMustSet, basar::SS_INSERT );

                return isInsertable;
           }

            bool HeadInPreparationPropertyStateChecker::isUpdatable() const
            {
                METHODNAME_DEF( HeadInPreparationPropertyStateChecker, isUpdatable )

                static const basar::VarString propertiesMustSet    ( properties::BRANCHNO.toPropertyString()            +
                                                                     properties::USER.toPropertyString()                +
                                                                     properties::ORDERTYPE.toPropertyString()           +
                                                                     properties::PHARMACYNO.toPropertyString()          +
                                                                     properties::MANUFACTURERNO.toPropertyString()
                                                                   );

                static const basar::VarString propertiesStateUpdate( properties::ORDERTYPE.toPropertyString()           + 
                                                                     properties::DEFERREDPAYMENTDATE.toPropertyString() +
                                                                     properties::PHARMACYNO.toPropertyString()          + 
                                                                     properties::EDIDELIVERYDATE.toPropertyString()     + 
                                                                     properties::MANUFACTURERNO.toPropertyString()      +
                                                                     properties::HEADERTEXT.toPropertyString() +
                                                                     properties::ORDERNOMF.toPropertyString() +
                                                                     properties::ISMANUALHEADERTEXTFLAG.toPropertyString() 
                                                                   );

                bool isUpdateable = m_Yit.are1stPropertiesSetAnd2ndOneInState( propertiesMustSet, propertiesStateUpdate, basar::SS_UPDATE );
                
                LOG_YIT( m_Yit, getLogger(), fun );
                return isUpdateable;
            }

            bool HeadInPreparationPropertyStateChecker::isDeletable() const
            {
                METHODNAME_DEF( HeadInPreparationPropertyStateChecker, isDeletable );

                static const basar::VarString propertiesMustSet    ( properties::BRANCHNO.toPropertyString()            +
                                                                     properties::USER.toPropertyString()
                                                                   );

                LOG_YIT( m_Yit, getLogger(), fun );
                bool isDeletable = m_Yit.arePropertiesSetAndOneMustBeInState( propertiesMustSet, basar::SS_DELETE );

                return isDeletable;
            }

            bool HeadInPreparationPropertyStateChecker::isSavable() const
            {
                return ( isInsertable() || isUpdatable() || isDeletable() );
            }

            bool HeadInPreparationPropertyStateChecker::isComplete() const
            {
                METHODNAME_DEF( HeadInPreparationPropertyStateChecker, isComplete )
                
                static const basar::VarString propertiesShouldBeSet( properties::BRANCHNO.toPropertyString()            +
                                                                     properties::USER.toPropertyString()                +
                                                                     properties::ORDERTYPE.toPropertyString()           + 
                                                                     properties::PHARMACYNO.toPropertyString()          + 
                                                                     properties::MANUFACTURERNO.toPropertyString()      );
                LOG_YIT( m_Yit, getLogger(), fun );
                return m_Yit.arePropertiesSet( propertiesShouldBeSet );
            }

            bool HeadInPreparationPropertyStateChecker::isValid() const
            {
                return isClean() || isSavable();
            }

            bool HeadInPreparationPropertyStateChecker::isClean() const
            {
                bool isClean = true;
                for( basar::db::aspect::AccessorPropertyTable_XIterator xIt = m_Yit.begin() ; !xIt.isEnd(); ++xIt )
                {
                    isClean &= (xIt.getState() == basar::SS_CLEAN);
                }

                return isClean;
            }

            const log4cplus::Logger& HeadInPreparationPropertyStateChecker::getLogger() const
            {
                return m_Logger;
            }

        }
    }
}   // end namespace libabbauw

