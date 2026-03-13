//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "transferorderposcollectionpropertystatechecker.h"
#include "libabbauw_properties_definitions.h"
#include "loggerpool/libabbauw_loggerpool.h"

#include <libutil/misc/metaclassmacro_definitions.h>
#include <libutil/misc/log_macro_definitions.h>
#include <libbasarproperty.h>

//---------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace libabbauw
{
    namespace accessor
    {
        namespace transferOrder
        {
            PosCollectionPropertyStateChecker::PosCollectionPropertyStateChecker( basar::db::aspect::AccessorPropertyTable_YIterator yit )
            : m_Logger( LoggerPool::loggerLibAbbaUW ), m_Yit( yit )
            {
            }

            PosCollectionPropertyStateChecker::~PosCollectionPropertyStateChecker()
            {
            }

            bool PosCollectionPropertyStateChecker::isSelectable() const
            {
                METHODNAME_DEF( PosCollectionPropertyStateChecker, isSelectable )

                LOG_YIT( m_Yit, m_Logger, fun );

                return m_Yit.arePropertiesSet( properties::BRANCHNO.toPropertyString() + properties::ORDERNO.toPropertyString() );
            }

            bool PosCollectionPropertyStateChecker::isInsertable() const
            {
                METHODNAME_DEF( PosCollectionPropertyStateChecker, isInsertable )

                static const basar::VarString properties4Insert( properties::BRANCHNO.toPropertyString()                        +
                                                                 properties::ORDERNO.toPropertyString()                         + 
                                                                 properties::POSNO.toPropertyString()                           +
                                                                 properties::ARTICLENO.toPropertyString()                       +
                                                                 properties::ORDEREDQTY.toPropertyString()                      +
                                                                 properties::NONCHARGEDQTY.toPropertyString()                   +
                                                                 properties::DISCOUNTPCT.toPropertyString()                     +
                                                                 properties::ARTICLEPRICE.toPropertyString()                    +
                                                                 properties::TEXTFIELD.toPropertyString() );

                LOG_YIT( m_Yit, getLogger(), fun );
                bool ins = m_Yit.arePropertiesSetAndAllInState( properties4Insert, basar::SS_INSERT );
                return ins;
            }

            bool PosCollectionPropertyStateChecker::isUpdatable() const
            {
                METHODNAME_DEF( PosCollectionPropertyStateChecker, isUpdatable )

                static const basar::VarString propertiesMustSet    ( properties::BRANCHNO.toPropertyString()      +
                                                                     properties::ORDERNO.toPropertyString()       +
                                                                     properties::POSNO.toPropertyString()         +
                                                                     properties::ORDEREDQTY.toPropertyString()    +
                                                                     properties::NONCHARGEDQTY.toPropertyString() +
                                                                     properties::DISCOUNTPCT.toPropertyString()   +
                                                                     properties::TEXTFIELD.toPropertyString()
                                                                   );

                static const basar::VarString propertiesStateUpdate( properties::ORDEREDQTY.toPropertyString()    + 
                                                                     properties::NONCHARGEDQTY.toPropertyString() +
                                                                     properties::DISCOUNTPCT.toPropertyString()   +
                                                                     properties::TEXTFIELD.toPropertyString()
                                                                   );

                bool isUpdateable = m_Yit.arePropertiesSet( propertiesMustSet );
                isUpdateable     &= m_Yit.arePropertiesSetAndOneMustBeInState( propertiesStateUpdate, basar::SS_UPDATE );
                 
                LOG_YIT( m_Yit, getLogger(), fun );
                return isUpdateable;
            }

            bool PosCollectionPropertyStateChecker::isDeletable() const
            {
                METHODNAME_DEF( PosCollectionPropertyStateChecker, isDeletable );
                LOG_YIT( m_Yit, getLogger(), fun );
                return m_Yit.arePropertiesSet( properties::BRANCHNO.toPropertyString() + properties::ORDERNO.toPropertyString() + 
                properties::POSNO.toPropertyString() );
            }

            bool PosCollectionPropertyStateChecker::isSavable() const
            {
                return ( isInsertable() || isUpdatable() );
            }

            bool PosCollectionPropertyStateChecker::isComplete() const
            {
                METHODNAME_DEF( PropertyStateChecker, isComplete )
                
                static const basar::VarString propertyList(   properties::BRANCHNO.toPropertyString()      +
                                                              properties::USER.toPropertyString()          +
                                                              properties::POSNO.toPropertyString()         +
                                                              properties::PZN.toPropertyString()           +
                                                              properties::ORDEREDQTY.toPropertyString()    + 
                                                              properties::NONCHARGEDQTY.toPropertyString() +
                                                              properties::DISCOUNTPCT.toPropertyString()   );

                bool areSet = m_Yit.arePropertiesSet( propertyList );
                bool isOneInStateDelete = m_Yit.are1stPropertiesSetAnd2ndOneInState( propertyList, propertyList, basar::SS_DELETE );

                LOG_YIT( m_Yit, getLogger(), fun );
                return areSet && !isOneInStateDelete ;
            }

            bool PosCollectionPropertyStateChecker::isValid() const
            {
                return isClean() || isSavable();
            }

            bool PosCollectionPropertyStateChecker::isClean() const
            {
                bool isClean = true;
                for( basar::db::aspect::AccessorPropertyTable_XIterator xIt = m_Yit.begin() ; !xIt.isEnd(); ++xIt )
                {
                    isClean &= (xIt.getState() == basar::SS_CLEAN);
                }

                return isClean;
            }

            const log4cplus::Logger& PosCollectionPropertyStateChecker::getLogger() const
            {
                return m_Logger;
            }

        }
    }
}   // end namespace libabbauw
