// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/pharmacygroup/pharmacygroupcollectionacc.h>
#include <infrastructure/accessor/pharmacygroup/pharmacygroupcollectionacc_definitions.h>

namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace pharmacygroup
{
    BEGIN_ACCESSOR_DEFINITION( lit::PHARMACY_GROUP_COLLECTION_ACC )

        BEGIN_PROPERTY_DESCRIPTION_LIST
        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_GROUPID )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_RANKING )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::UPD_FLAG  )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO  )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::RECLAIM_OMG_DISC )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_GROUP_NAME )
        END_PROPERTY_DESCRIPTION_LIST
  
        ACCESS_METHOD( lit::SELECT_PHARMACY_GROUP_BY_PATTERN );
        SQL_BUILDER_CREATE( SelectPharmacyGroupByPattern )
        SQL_BUILDER_PUSH_BACK( SelectPharmacyGroupByPattern )

        ACCESS_METHOD( lit::SELECT_PHARMACY_GROUP_CUSTOMERS_ASSIGNMENT_BY_PATTERN );
        SQL_BUILDER_CREATE( SelectPharmacyGroupCustomerAssignmentByPattern )
        SQL_BUILDER_PUSH_BACK( SelectPharmacyGroupCustomerAssignmentByPattern )

        ACCESS_METHOD( lit::SELECT_PHARMACY_GROUP_BY_GROUP_ID );
        SQL_BUILDER_CREATE( SelectPharmacyGroupByGroupID )
        SQL_BUILDER_PUSH_BACK( SelectPharmacyGroupByGroupID )
  
        ACCESS_METHOD( lit::SELECT_PHARMACY_GROUP_BY_PSEUDOCUSTOMER_NO );
        SQL_BUILDER_CREATE( SelectPharmacyGroupByPseudoCustomerNo )
        SQL_BUILDER_PUSH_BACK( SelectPharmacyGroupByPseudoCustomerNo )

    END_ACCESSOR_DEFINITION
  
    BUILDER_DEFINITION( SelectPharmacyGroupByPattern )
  
    bool SelectPharmacyGroupByPattern::isExecutable() const
    {
        METHODNAME_DEF( SelectPharmacyGroupByPattern, isExecutable );
        static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );
        
        bool executable = false;
        
        if( arePropertiesSet( properties::PHARMACY_GROUPID.toPropertyString()) ||  arePropertiesSet( properties::PHARMACY_GROUP_NAME.toPropertyString() ) )
        {
            executable = true;
        }
    
        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    
        return executable;
    }
  
    void SelectPharmacyGroupByPattern::buildSQLString()
    {
		// TODO SK: on searching/verifying a pharmacy group from tender application
		// (tender maintenance) we have the pseudo customer number of the pharmacy group only,
		// so we have to select from table pharmacygroups
		// on creating csc orders from tender application we have a real customer number.
		// in this case we have to select from customerpharmacygr

		// I would suggest to create 2 selects:

		/*	SelectPharmacyGroupByPattern
		
			verify pharmacygroup and pseudo customer no:
			select pharmacygroupid,customerno as pseudo_customerno from pharmacygroups where customerno>0
			only pharmacygroups with a pseudo customer > 0 no are valid.




			SelectPharmacyGroupCustomerAssignmentByPattern
			
			verify if a customer belongs to a pharmacy group:
			select pharmacygroupid,customerno from customerpharmacygr 
			where customerno>0

			we do need 2 fínd methods in the DM also.
		*/

        static const basar::VarString constSQL(
                  "SELECT DISTINCT "
                  "pg.pharmacygroupid AS "                  + properties::PHARMACY_GROUPID.getName() + ", "
                  "pg.customerno AS "                       + properties::CUSTOMER_NO.getName()	 + ", "
                  "pg.group_name AS "                       + properties::PHARMACY_GROUP_NAME.getName() + " "
                  "FROM pharmacygroups AS pg "
                  "WHERE pg.customerno > 0 " 
                 );
                  
        basar::VarString variableSQL;
        
        if( arePropertiesSetAndOneMustBeInState( properties::PHARMACY_GROUPID , basar::SS_CLEAN))
        {
            variableSQL.append( "AND pg.pharmacygroupid = '" + properties::PHARMACY_GROUPID.toSQLReplacementString() + "' " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::PHARMACY_GROUP_NAME , basar::SS_CLEAN))
        {
            variableSQL.append( "AND UPPER(pg.group_name) LIKE UPPER('%"  + properties::PHARMACY_GROUP_NAME.toSQLReplacementString() + "%') " );
        }
        
        basar::VarString organizeResult( "ORDER BY pg.pharmacygroupid ASC " );
        
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
   }

      BUILDER_DEFINITION( SelectPharmacyGroupCustomerAssignmentByPattern )
  
    bool SelectPharmacyGroupCustomerAssignmentByPattern::isExecutable() const
    {
        METHODNAME_DEF( SelectPharmacyGroupCustomerAssignmentByPattern, isExecutable );
        static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );
        
        bool executable = false;
        
        if( arePropertiesSet( properties::PHARMACY_GROUPID.toPropertyString() ) ||  
            arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) || 
            arePropertiesSet( properties::BRANCH_NO.toPropertyString() ) )
        {
            executable = true;
        }
    
        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    
        return executable;
    }
  
    void SelectPharmacyGroupCustomerAssignmentByPattern::buildSQLString()
    {
        static const basar::VarString constSQL(
                  "SELECT DISTINCT "
                  "cpg.pharmacygroupid AS "                  + properties::PHARMACY_GROUPID.getName() + ", "
                  "cpg.customerno AS "                       + properties::CUSTOMER_NO.getName() + ", "
                  "cpg.branchno AS "                         + properties::BRANCH_NO.getName() + " "
                  "FROM customerpharmacygr AS cpg "
                  "INNER JOIN kunde AS k ON k.idfnr = cpg.customerno AND k.filialnr = cpg.branchno "
                  "INNER JOIN kundeschalter AS ks ON ks.idfnr = cpg.customerno AND ks.vertriebszentrumnr = cpg.branchno "
                  "INNER JOIN filiale AS f ON f.filialnr = cpg.branchno "
                  "WHERE " 
                  "k.originalfilialnr = 0 AND ks.skdkundeaegesperrt = '0' "
                 );
                  
        basar::VarString variableSQL;
        
        if( arePropertiesSetAndOneMustBeInState( properties::PHARMACY_GROUPID , basar::SS_CLEAN))
        {
            variableSQL.append( "AND cpg.pharmacygroupid = '" + properties::PHARMACY_GROUPID.toSQLReplacementString() + "' " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::CUSTOMER_NO , basar::SS_CLEAN))
        {
            variableSQL.append( "AND cpg.customerno = "  + properties::CUSTOMER_NO.toSQLReplacementString() + " " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::BRANCH_NO , basar::SS_CLEAN))
        {
            variableSQL.append( "AND cpg.branchno = "  + properties::BRANCH_NO.toSQLReplacementString() + " " );
        }
        
        basar::VarString organizeResult( "ORDER BY cpg.pharmacygroupid ASC " );
        
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
   }

    BUILDER_DEFINITION( SelectPharmacyGroupByGroupID )
  
    bool SelectPharmacyGroupByGroupID::isExecutable() const
    {
        METHODNAME_DEF( SelectPharmacyGroupByGroupID, isExecutable );
        static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );

        bool executable = false;
        
        if( arePropertiesSet( properties::PHARMACY_GROUPID.toPropertyString() ) )
        {
            executable = true;
        }

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

        return executable;
    }
  
    void SelectPharmacyGroupByGroupID::buildSQLString()
    {
        static const basar::VarString constSQL(
                  "SELECT DISTINCT "
                  "cpg.pharmacygroupid AS "                 + properties::PHARMACY_GROUPID.getName() + ", "
                  "cpg.customerno AS "                      + properties::CUSTOMER_NO.getName()	 + ", "
                  "cpg.branchno AS "                        + properties::BRANCH_NO.getName() + ", "
                  "cpg.ranking AS "                         + properties::PHARMACY_RANKING.getName() + ", "
                  "cpg.upd_flag AS "                        + properties::UPD_FLAG.getName() + " "
                  "FROM customerpharmacygr AS cpg "
                  "WHERE 1=1 "
                  );
                  
        basar::VarString variableSQL;
        
        if( isContainedAndSet( properties::PHARMACY_GROUPID ) )
        {
            variableSQL.append( "AND cpg.pharmacygroupid = '" + properties::PHARMACY_GROUPID.toSQLReplacementString() + "' " );
        }
        
        basar::VarString organizeResult( "ORDER BY cpg.customerno ASC" );
        
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
    }

    BUILDER_DEFINITION( SelectPharmacyGroupByPseudoCustomerNo )

    bool SelectPharmacyGroupByPseudoCustomerNo::isExecutable() const
    {
        METHODNAME_DEF( SelectPharmacyGroupByPseudoCustomerNo, isExecutable );
        static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );

        bool executable = false;
        
        if( arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) )
        {
            executable = true;
        }

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

        return executable;
    }

    void SelectPharmacyGroupByPseudoCustomerNo::buildSQLString()
    {
        static const basar::VarString constSQL(
                  "SELECT "
                  "pharmacygroupid AS "   + properties::PHARMACY_GROUPID.getName() + ", "
                  "reclaim_omg_disc AS "  + properties::RECLAIM_OMG_DISC.getName() + " "
                  "FROM pharmacygroups where customerno = " + properties::CUSTOMER_NO.toSQLReplacementString()
                  );

        resolve( constSQL );
    }

} // end namespace article
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender
