//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "transferorderposinpreparationcollectionpropertystatechecker.h"
#include "libabbauw_properties_definitions.h"
#include "loggerpool/libabbauw_loggerpool.h"

#include <libutil/misc/metaclassmacro_definitions.h>
#include <libutil/misc/log_macro_definitions.h>
#include <libbasarproperty.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//---------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace libabbauw
{
    namespace accessor
    {
        namespace transferOrder
        {
            PosInPreparationCollectionPropertyStateChecker::PosInPreparationCollectionPropertyStateChecker( basar::db::aspect::AccessorPropertyTable_YIterator yit )
            : m_Logger( LoggerPool::loggerLibAbbaUW ), m_Yit( yit )
            {
            }

            PosInPreparationCollectionPropertyStateChecker::~PosInPreparationCollectionPropertyStateChecker()
            {
            }

            bool PosInPreparationCollectionPropertyStateChecker::isSelectable() const
            {
                return m_Yit.arePropertiesSet( properties::BRANCHNO.toPropertyString() + properties::USER.toPropertyString() );
            }

            bool PosInPreparationCollectionPropertyStateChecker::isInsertable() const
            {
                METHODNAME_DEF( PosInPreparationCollectionPropertyStateChecker, isInsertable )

                static const basar::VarString properties4Insert( properties::BRANCHNO.toPropertyString()            + 
                                                                 properties::USER.toPropertyString()                +
                                                                 properties::POSNO.toPropertyString()               + 
                                                                 properties::PZN.toPropertyString()                 +
                                                                 properties::ORDEREDQTY.toPropertyString()
                                                               );

                LOG_YIT( m_Yit, getLogger(), fun );
                bool ins = m_Yit.arePropertiesSetAndAllInState( properties4Insert, basar::SS_INSERT );
                return ins;
            }

            bool PosInPreparationCollectionPropertyStateChecker::isUpdatable() const
            {
                METHODNAME_DEF( PosInPreparationCollectionPropertyStateChecker, isUpdatable )

                static const basar::VarString propertiesMustSet    ( properties::BRANCHNO.toPropertyString()    +
                                                                     properties::USER.toPropertyString()        +
                                                                     properties::POSNO.toPropertyString()       +
                                                                     properties::POSNO_ORIG.toPropertyString()  +
                                                                     properties::ORDEREDQTY.toPropertyString()
                                                                   );

                static const basar::VarString propertiesStateUpdate( properties::POSNO.toPropertyString()         +
                                                                     properties::PZN.toPropertyString()           +
                                                                     properties::ORDEREDQTY.toPropertyString()    + 
                                                                     properties::NONCHARGEDQTY.toPropertyString() + 
                                                                     properties::DISCOUNTPCT.toPropertyString()   +
                                                                     properties::DISCOUNTAPPLYTO.toPropertyString()+
                                                                     properties::DISCOUNTCALCFROM.toPropertyString()
                                                                   );

                bool isUpdateable = m_Yit.arePropertiesSet( propertiesMustSet );
                isUpdateable     &= m_Yit.arePropertiesSetAndOneMustBeInState( propertiesStateUpdate, basar::SS_UPDATE );
                 
                LOG_YIT( m_Yit, getLogger(), fun );
                return isUpdateable;
            }

            bool PosInPreparationCollectionPropertyStateChecker::isDeletable() const
            {
                METHODNAME_DEF( PosInPreparationCollectionPropertyStateChecker, isDeletable );
                LOG_YIT( m_Yit, getLogger(), fun );
                return m_Yit.arePropertiesSet( properties::BRANCHNO.toPropertyString() + properties::USER.toPropertyString() ) &&
                       m_Yit.arePropertiesSetAndOneMustBeInState( properties::POSNO_ORIG.toPropertyString()    +
                                                                  properties::PZN.toPropertyString()           +
                                                                  properties::ORDEREDQTY.toPropertyString()    + 
                                                                  properties::NONCHARGEDQTY.toPropertyString() + 
                                                                  properties::DISCOUNTPCT.toPropertyString()   +
                                                                  properties::DISCOUNTCALCFROM.toPropertyString()+
                                                                  properties::DISCOUNTAPPLYTO.toPropertyString()+
                                                                  properties::AGP.toPropertyString()+
                                                                  properties::GEP.toPropertyString()+
                                                                  properties::POSITIONSTATUS.toPropertyString(), basar::SS_DELETE );
            }

            bool PosInPreparationCollectionPropertyStateChecker::isSavable() const
            {
                return ( isInsertable() || isUpdatable() || isDeletable() );
            }

            bool PosInPreparationCollectionPropertyStateChecker::isComplete() const
            {
                METHODNAME_DEF( PropertyStateChecker, isComplete )
                
                static const basar::VarString propertyList(   properties::BRANCHNO.toPropertyString()      +
                                                              properties::USER.toPropertyString()          +
                                                              properties::POSNO.toPropertyString()         +
                                                              properties::PZN.toPropertyString()           +
                                                              properties::ORDEREDQTY.toPropertyString()    + 
                                                              properties::NONCHARGEDQTY.toPropertyString() +
                                                              properties::DISCOUNTPCT.toPropertyString()   +
                                                              properties::DISCOUNTCALCFROM.toPropertyString()+ 
                                                              properties::DISCOUNTAPPLYTO.toPropertyString());

                bool areSet = m_Yit.arePropertiesSet( propertyList );
                bool isOneInStateDelete = m_Yit.are1stPropertiesSetAnd2ndOneInState( propertyList, propertyList, basar::SS_DELETE );

                LOG_YIT( m_Yit, getLogger(), fun );
                return areSet && !isOneInStateDelete ;
            }

            bool PosInPreparationCollectionPropertyStateChecker::isValid() const
            {
                return isClean() || isSavable();
            }

            bool PosInPreparationCollectionPropertyStateChecker::isClean() const
            {
                bool isClean = true;
                for( basar::db::aspect::AccessorPropertyTable_XIterator xIt = m_Yit.begin() ; !xIt.isEnd(); ++xIt )
                {
                    isClean &= (xIt.getState() == basar::SS_CLEAN);
                }

                return isClean;
            }

            const log4cplus::Logger& PosInPreparationCollectionPropertyStateChecker::getLogger() const
            {
                return m_Logger;
            }

        }
    }
}   // end namespace libabbauw
