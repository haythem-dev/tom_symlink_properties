#include "cmndm.h"

#include "parameterdm.h"
#include "parameter_definitions.h"
#include "exceptions/parameterexception.h"

#include "parameteracc.h"

#include <libbasarlogin_manager.h>

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

const libutil::misc::ClassInfo& ParameterDM::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "parameter::ParameterDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

ParameterDM::ParameterDM( basar::db::aspect::ConnectionRef connection )
    : DMBase( connection, domMod::parameter::ACC_PARAMETER, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerParameter )
{
	METHODNAME_DEF( ParameterDM, ParameterDM )
	BLOG_TRACE_METHOD( getLogger(), fun );

	// avoid runtime error due to liker optimizes out the accessor				
	acc_parameter::SelectParameter();
}

ParameterDM::~ParameterDM()
{
	METHODNAME_DEF( ParameterDM, ~ParameterDM )
	BLOG_TRACE_METHOD( getLogger(), fun );

    shutdown();
}

void ParameterDM::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr dbTransactionFactory )
{
    m_DBTransactionFactory = dbTransactionFactory;
}

AccessorExecuteParameterization ParameterDM::getFindByPatternExecuteParameters() const
{
	METHODNAME_DEF( ParameterDM, getFindByPatternExecuteParameters )
	BLOG_TRACE_METHOD( getLogger(), fun );

    return AccessorExecuteParameterization ( SELECT_PARAMETER );
}

basar::VarString ParameterDM::getSearchYIteratorAttributeNames()  const
{
	METHODNAME_DEF( ParameterDM, getSearchYIteratorAttributeNames )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return properties::BRANCHNO.toDescriptionString() + properties::PROGNAME.toDescriptionString();
}

void ParameterDM::doInit( const ParameterInitParams& p )
{
	METHODNAME_DEF( ParameterDM, doInit )
	BLOG_TRACE_METHOD( getLogger(), fun );

	m_InitParams = p;
}

void ParameterDM::doShutdown()
{
	METHODNAME_DEF( ParameterDM, doShutdown )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void ParameterDM::resetSearchYit()
{
	DMBase::resetSearchYit();
}

basar::db::aspect::AccessorPropertyTableRef ParameterDM::getPropTab()
{
    return DMBase::getPropTab();
}

basar::db::aspect::AccessorPropertyTable_YIterator ParameterDM::getSearchYit()
{
	return DMBase::getSearchYit();
}

void ParameterDM::load()
{
	METHODNAME_DEF( ParameterDM, load )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = getSearchYit();
	yitSearchTmp.setInt16( libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
	yitSearchTmp.setString( libabbauw::properties::PROGNAME, m_InitParams.m_ProgName );
	
	DMBase::findByPattern( yitSearchTmp );
}

bool ParameterDM::isSaveable( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	return yit.getState().contains( basar::SS_INSERT | basar::SS_UPDATE | basar::SS_DELETE );
}

void ParameterDM::save()
{
	METHODNAME_DEF( ParameterDM, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction;

	try 
	{
        transaction = m_DBTransactionFactory->createDBTransaction();
		transaction->begin();

		for( basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().begin(); yit != getPropTab().end(); ++yit )
		{
			LOG_YIT(yit, getLogger(), fun );
			if( isSaveable( yit ) )
			{
				saveParameter( yit );
			}
		}

		transaction->commit();
	} 
	catch( basar::Exception & e )
	{
		transaction->rollback();

		getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		throw;
    }
}

void ParameterDM::saveParameter( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	METHODNAME_DEF( ParameterDM, saveParameter )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitTmp = yit;
	yitTmp.setInt16( libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
	yitTmp.setString( libabbauw::properties::PROGNAME, m_InitParams.m_ProgName );
	
	try {
        const bool clearBeforeSelect = true;
        const bool flagExcept = true;
        const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::HOLD_RECORDSET;
        
		basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute( SAVE_PARAMETER, yitTmp, clearBeforeSelect, flagExcept, dbHint );
    	if( res.hasError() )
		{
			THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_PARAMETER, res );
	    }

	} catch( basar::Exception & e ){
		getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		throw;
    }
}

basar::db::aspect::AccessorPropertyTable_YIterator ParameterDM::addNewParameter( const basar::VarString& paramName )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().insert( basar::FOR_INSERT );
	yit.setString( properties::PURPOSE,  m_InitParams.m_DefaultPurpose );
	yit.setString( properties::PARAMETERNAME, paramName );

	return yit;
}

bool ParameterDM::setInvalidParametersToDefault()
{
	bool parameterSetToDefault = false;

	try
	{
		getOrderSort();
	}
	catch( exceptions::ParameterException& )
	{
		util::SortOrder orderSort;
		orderSort.setOrder( properties::ORDERDATE,  util::SortOrder::ASCENDING  );
		orderSort.setOrder( properties::STATUS,     util::SortOrder::ASCENDING  );
		orderSort.setOrder( properties::ORDERVALUE, util::SortOrder::DESCENDING );
		
		setOrderSort( orderSort );
		parameterSetToDefault = true;
	}

	try
	{
		getDaysToColor();
	}
	catch( exceptions::ParameterException& )
	{
		setDaysToColor( 5 );
		parameterSetToDefault = true;
	}

	try
	{
		getReserveQuantityOrderFactor();
	}
	catch( exceptions::ParameterException& )
	{
		setReserveQuantityOrderFactor( basar::Decimal( 1 ) );
		parameterSetToDefault = true;
	}

    try
	{
		getReserveQuantityReleaseFactor();
	}
	catch( exceptions::ParameterException& )
	{
		setReserveQuantityReleaseFactor( basar::Decimal( 1 ) );
		parameterSetToDefault = true;
	}
 
	try
	{
		getNotSalesEffectiveReserveCheck();
	}
	catch( exceptions::ParameterException& )
	{
		basar::Decimal checkOld = getNotSalesEffectiveReserveCheckOld();

		if(basar::Decimal(0.0) > checkOld)
		{
			setNotSalesEffectiveReserveCheck(false);
		}
		else
		{
			setNotSalesEffectiveReserveCheck(true);
		}

		parameterSetToDefault = true;
	}

	try
	{
		getNotSalesEffectiveReserveQtyReleaseFactor();
	}
	catch( exceptions::ParameterException& )
	{
		basar::Decimal oldReserve = getNotSalesEffectiveReserveCheckOld();
		if(basar::Decimal(0.0) <= oldReserve)
		{
			setNotSalesEffectiveReserveQtyReleaseFactor( oldReserve );
		}
		else
		{
			setNotSalesEffectiveReserveQtyReleaseFactor( basar::Decimal(0.0) );
		}

		parameterSetToDefault = true;
	}

	try
	{
		getNotSalesEffectiveReserveQtyOrderFactor();
	}
	catch( exceptions::ParameterException& )
	{
		setNotSalesEffectiveReserveQtyOrderFactor( getNotSalesEffectiveReserveQtyReleaseFactor() );
		parameterSetToDefault = true;
	}

	try
	{
		getFilterStartDate();
	}
	catch( exceptions::ParameterException& )
	{
		setFilterStartDate( basar::Date( 20000101 ) );
		parameterSetToDefault = true;
	}

	try
	{
		getFilterEndDate();
	}
	catch( exceptions::ParameterException& )
	{
		setFilterEndDate( basar::Date( 20991231 ) );
		parameterSetToDefault = true;
	}

    try
	{
		getLargeScaleOrderLimit();
	}
	catch( exceptions::ParameterException& )
	{
		setLargeScaleOrderLimit( 1000 );
		parameterSetToDefault = true;
	}

	try
	{
		getCSVExportSeparator();
	}
	catch( exceptions::ParameterException& )
	{
		setCSVExportSeparator( ";" );
		parameterSetToDefault = true;
	}

	try
	{
		getDiscountPctTrailer();
	}
	catch( exceptions::ParameterException& )
	{
		setDiscountPctTrailer( "" );
		parameterSetToDefault = true;
	}

	try
	{
		getOrderrouting();
	}
	catch( exceptions::ParameterException& )
	{
		setOrderrouting( "" );
		parameterSetToDefault = true;
	}


	return parameterSetToDefault;
}

basar::db::aspect::AccessorPropertyTable_YIterator ParameterDM::getParameterByName( const basar::VarString& parameterName )
{
	METHODNAME_DEF( ParameterDM, getParameterByName );

	basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( parameterName );
	if( yit.isEnd() )
	{
		basar::VarString msg = "Parameter '" + parameterName + "' not found in database!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), parameterName );
	}

	return yit;
}

basar::db::aspect::AccessorPropertyTable_YIterator ParameterDM::findParameterByName( const basar::VarString& parameterName )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().begin();
	for( ; !yit.isEnd() && yit.getString( properties::PARAMETERNAME ) != parameterName; ++yit );

	return yit;
}

void ParameterDM::setParameter( const basar::VarString& paramName, basar::Int32 value )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( paramName );
	if( yit.isEnd() )
		yit = addNewParameter( paramName );
	
	yit.setInt32( properties::PARAMETERVALUE, value );
}

void ParameterDM::setParameter( const basar::VarString& paramName, const basar::VarString& value )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( paramName );
	if( yit.isEnd() )
		yit = addNewParameter( paramName );

	yit.setString( properties::MANIFESTATION, value );
}

void ParameterDM::orderSortValid( const util::SortOrder& orderSort )
{
	//METHODNAME_DEF( ParameterDM, orderSortValid );

	(void) orderSort;
	// FIXME
}

basar::VarString ParameterDM::getDiscountPctTrailer()
{
	METHODNAME_DEF( ParameterDM, getDiscountPctTrailer )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::VarString retValue = "";

	retValue = getParameterByName( "DISCOUNT_PCT_TRAILER" ).getString( properties::MANIFESTATION );

	return retValue;
}

basar::VarString ParameterDM::getOrderrouting()
{
	METHODNAME_DEF( ParameterDM, getOrderrouting )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::VarString retValue = "";

	retValue = getParameterByName( "ORDERROUTING" ).getString( properties::MANIFESTATION );

	return retValue;
}

util::SortOrder ParameterDM::getOrderSort()
{
	METHODNAME_DEF( ParameterDM, getOrderSort )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::VarString sortStr = getParameterByName( "ORDERSORT" ).getString( properties::MANIFESTATION );
	util::SortOrder orderSort( sortStr );
	orderSortValid( orderSort );

	return orderSort;
}

void ParameterDM::setOrderSort( const util::SortOrder& orderSort )
{
	METHODNAME_DEF( ParameterDM, setOrderSort )
	BLOG_TRACE_METHOD( getLogger(), fun );

	util::SortOrder sort( orderSort );
	orderSortValid( sort );
	setParameter( "ORDERSORT", orderSort.toString() );
}

void ParameterDM::daysToColorValid( basar::Int32 days )
{
	METHODNAME_DEF( ParameterDM, daysToColorValid );

	if( days < 0 )
	{
		basar::VarString msg = "Parameter '" "DAYS_TO_COLOR" "' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "DAYS_TO_COLOR" );
	}
}

basar::Int32 ParameterDM::getDaysToColor()
{
	METHODNAME_DEF( ParameterDM, getDaysToColor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32 days = getParameterByName( "DAYS_TO_COLOR" ).getInt32( properties::PARAMETERVALUE );
	daysToColorValid( days );

	return days;
}

void ParameterDM::setDaysToColor( basar::Int32 daysToColor )
{
	METHODNAME_DEF( ParameterDM, setDaysToColor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	daysToColorValid( daysToColor );
	setParameter( "DAYS_TO_COLOR", daysToColor );
}

bool ParameterDM::getFormatHeaderTextEnabled()
{
	METHODNAME_DEF( ParameterDM, getDaysToColor )
	BLOG_TRACE_METHOD( getLogger(), fun );

    basar::I18nString value;
    basar::I18nString branchNo;
    branchNo.itos( m_InitParams.m_BranchNo );

	basar::login::Manager::getInstance().getParaValue( branchNo, "FORMAT_HEADERTEXT", value);
    
    return (value == "1" || value == "true") ;
}

void ParameterDM::reserveQuantityFactorValid( basar::Decimal reserveQtyFactor )
{
	METHODNAME_DEF( ParameterDM, reserveQuantityFactorValid );

	if( reserveQtyFactor < basar::Decimal( 0 ) )
	{
		basar::VarString msg = "Parameter 'RES_QTY_REL_FACTOR' or 'RES_QTY_ORDER_FACTOR' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "Reserve Factor" );
	}
}

void ParameterDM::notSalesEffectiveReserveCheckValid( basar::VarString check )
{
	METHODNAME_DEF( ParameterDM, notSalesEffectiveReserveCheckValid );

	if( "YES" != check && "NO" != check )
	{
		basar::VarString msg = "Parameter 'RES_NOTSALEFF_CHECK' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "Not Sales Effective Reserve Check" );
	}
}

void ParameterDM::notSalesEffectiveReserveQuantityFactorValid( basar::Decimal reserveQtyFactor )
{
	METHODNAME_DEF( ParameterDM, reserveQnotSalesEffectiveReserveQuantityFactorValiduantityFactorValid );

	if( reserveQtyFactor < basar::Decimal( 0 ) )
	{
		basar::VarString msg = "Parameter 'RES_NOTSALEFF_ORDER' or 'RES_NOTSELAFF_REL' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "Not Sales Effective Reserve Factor" );
	}
}

basar::Decimal ParameterDM::getReserveQuantityReleaseFactor()
{
	METHODNAME_DEF( ParameterDM, getReserveQuantityReleaseFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Decimal reserveQtyFactor;

	reserveQtyFactor = basar::Decimal( getParameterByName( "RES_QTY_REL_FACTOR" ).getString( properties::MANIFESTATION ) );
	reserveQuantityFactorValid( reserveQtyFactor );

	return reserveQtyFactor;
}

void ParameterDM::setReserveQuantityReleaseFactor( basar::Decimal reserveQtyFactor )
{
	METHODNAME_DEF( ParameterDM, setReserveQuantityReleaseFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	reserveQuantityFactorValid( reserveQtyFactor );
	setParameter( "RES_QTY_REL_FACTOR", reserveQtyFactor.toString() );
}

basar::Decimal ParameterDM::getReserveQuantityOrderFactor()
{
	METHODNAME_DEF( ParameterDM, getReserveQuantityOrderFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Decimal reserveQtyFactor;

	reserveQtyFactor = basar::Decimal( getParameterByName( "RES_QTY_ORDER_FACTOR" ).getString( properties::MANIFESTATION ) );
	reserveQuantityFactorValid( reserveQtyFactor );

	return reserveQtyFactor;
}

void ParameterDM::setReserveQuantityOrderFactor( basar::Decimal reserveQtyFactor )
{
	METHODNAME_DEF( ParameterDM, setReserveQuantityOrderFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	reserveQuantityFactorValid( reserveQtyFactor );
	setParameter( "RES_QTY_ORDER_FACTOR", reserveQtyFactor.toString() );
}

void ParameterDM::setNotSalesEffectiveReserveCheck( bool isChecked )
{
	METHODNAME_DEF( ParameterDM, setNotSalesEffectiveReserveCheck )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if(isChecked)
	{
		setParameter( "RES_NOTSALEFF_CHECK", "YES" );
	}
	else
	{
		setParameter( "RES_NOTSALEFF_CHECK", "NO" );
	}
}

bool ParameterDM::isNotSalesEffectiveReserveCheckSet()
{
	METHODNAME_DEF( ParameterDM, isNotSalesEffectiveReserveCheckSet )
	BLOG_TRACE_METHOD( getLogger(), fun );

	bool isChecked = false;

    basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( "RES_NOTSALEFF_CHECK" );
    if( ! yit.isEnd() && ! yit.getString( properties::MANIFESTATION ).empty() )
    {
		if( "YES" == yit.getString( properties::MANIFESTATION ) )
		{
			isChecked = true;
		}
    }

	return isChecked;
}

basar::VarString ParameterDM::getNotSalesEffectiveReserveCheck()
{
	METHODNAME_DEF( ParameterDM, getNotSalesEffectiveReserveCheck )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::VarString checked = "";

    basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( "RES_NOTSALEFF_CHECK" );
    if( ! yit.isEnd() && ! yit.getString( properties::MANIFESTATION ).empty() )
    {
        checked =  yit.getString( properties::MANIFESTATION );
    }

	notSalesEffectiveReserveCheckValid(checked);

	return checked;
}

basar::Decimal ParameterDM::getNotSalesEffectiveReserveCheckOld()
{
	METHODNAME_DEF( ParameterDM, getNotSalesEffectiveReserveCheckOld )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Decimal checked;

    basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( "RESERVE_NOTSALESEFF" );
    if( ! yit.isEnd() && ! yit.getString( properties::MANIFESTATION ).empty() )
    {
        checked =  basar::Decimal( yit.getString( properties::MANIFESTATION ) );
    }

	return checked;
}

basar::Decimal ParameterDM::getNotSalesEffectiveReserveQtyReleaseFactor()
{
    METHODNAME_DEF( ParameterDM, getNotSalesEffectiveReserveQtyReleaseFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Decimal reserveQtyFactor;

    basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( "RES_NOTSALEFF_REL" );
    if( ! yit.isEnd() && ! yit.getString( properties::MANIFESTATION ).empty() )
    {
        reserveQtyFactor = basar::Decimal( yit.getString( properties::MANIFESTATION ) );
    }
    else
    {
        reserveQtyFactor = basar::Decimal( -1.0 ); // Factor is unset
    }

	notSalesEffectiveReserveQuantityFactorValid( reserveQtyFactor );

	return reserveQtyFactor;
}

void ParameterDM::setNotSalesEffectiveReserveQtyReleaseFactor( basar::Decimal reserveQtyFactor )
{
    METHODNAME_DEF( ParameterDM, setNotSalesEffectiveReserveQtyReleaseFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	setParameter( "RES_NOTSALEFF_REL", reserveQtyFactor.toString() );
}

bool ParameterDM::isNotSalesEffectiveReserveQtyReleaseFactorSet()
{
    return getNotSalesEffectiveReserveQtyReleaseFactor() >= basar::Decimal( 0.0 );
}

basar::Decimal ParameterDM::getNotSalesEffectiveReserveQtyOrderFactor()
{
    METHODNAME_DEF( ParameterDM, getNotSalesEffectiveReserveQtyOrderFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Decimal reserveQtyFactor;

    basar::db::aspect::AccessorPropertyTable_YIterator yit = findParameterByName( "RES_NOTSALEFF_ORDER" );
    if( ! yit.isEnd() && ! yit.getString( properties::MANIFESTATION ).empty() )
    {
        reserveQtyFactor = basar::Decimal( yit.getString( properties::MANIFESTATION ) );
    }
    else
    {
        reserveQtyFactor = basar::Decimal( -1.0 ); // Factor is unset
    }

	notSalesEffectiveReserveQuantityFactorValid( reserveQtyFactor );

	return reserveQtyFactor;
}

void ParameterDM::setNotSalesEffectiveReserveQtyOrderFactor( basar::Decimal reserveQtyFactor )
{
    METHODNAME_DEF( ParameterDM, setNotSalesEffectiveReserveQtyReleaseFactor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	setParameter( "RES_NOTSALEFF_ORDER", reserveQtyFactor.toString() );
}

bool ParameterDM::isNotSalesEffectiveReserveQtyOrderFactorSet()
{
    return getNotSalesEffectiveReserveQtyOrderFactor() >= basar::Decimal( 0.0 );
}

void ParameterDM::transitStorageLocationSlotValid( const basar::VarString& transitStorageLocationSlot )
{
	METHODNAME_DEF( ParameterDM, transitStorageLocationSlotValid );

	if( transitStorageLocationSlot.empty() )
	{
		basar::VarString msg = "Parameter '" "STORAGESLOT_TRANSIT" "' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "STORAGESLOT_TRANSIT" );
	}
}

basar::VarString ParameterDM::getTransitStorageLocationSlot()
{
    METHODNAME_DEF( ParameterDM, getTransitStorageLocationSlot )
	BLOG_TRACE_METHOD( getLogger(), fun );

    basar::VarString transitStorageLocationSlot;

    basar::db::aspect::AccessorPropertyTable_YIterator param = findParameterByName( "STORAGESLOT_TRANSIT" );
    if( ! param.isEnd() )
    {
        transitStorageLocationSlot = param.getString( properties::MANIFESTATION );
    }
    //transitStorageLocationSlotValid( transitStorageLocationSlot );
    if( transitStorageLocationSlot.empty() )
    {
	    // not mandatory
        BLOG_WARN( getLogger(), "Parameter <STORAGESLOT_TRANSIT> not set!" );
        transitStorageLocationSlot = "-1";
    }

    return transitStorageLocationSlot;
}

bool ParameterDM::emitWarningOnNonChargedQty()
{
    basar::VarString emitWarnOnNonChargedQty;

    basar::db::aspect::AccessorPropertyTable_YIterator param = findParameterByName( "EMIT_WARN_ON_ND" );
    if( ! param.isEnd() )
    {
        emitWarnOnNonChargedQty = param.getString( properties::MANIFESTATION );
    }

    return emitWarnOnNonChargedQty == "TRUE" || emitWarnOnNonChargedQty == "YES" || emitWarnOnNonChargedQty == "1";
}

bool ParameterDM::releaseRedYellowOrdersPostponed()
{
    basar::VarString releasePostponed;

    basar::db::aspect::AccessorPropertyTable_YIterator param = findParameterByName( "POSTPONE_RED_YELLOW" );
    if( ! param.isEnd() )
    {
        releasePostponed = param.getString( properties::MANIFESTATION );
    }

    return releasePostponed == "TRUE" || releasePostponed == "YES" || releasePostponed == "1";
}

void ParameterDM::filterStartDateValid( const basar::Date& startDate  )
{
	METHODNAME_DEF( ParameterDM, filterStartDateValid );

	if( ! startDate.isValid() )
	{
		basar::VarString msg = "Parameter '" "FILTER_START_DATE" "' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "FILTER_START_DATE" );
	}
}

basar::Date ParameterDM::getFilterStartDate()
{
	METHODNAME_DEF( ParameterDM, getFilterStartDate )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Date startDate( getParameterByName( "FILTER_START_DATE" ).getInt32( properties::PARAMETERVALUE ) );
	filterStartDateValid( startDate );

	return startDate;
}

void ParameterDM::setFilterStartDate( const basar::Date& startDate )
{
	METHODNAME_DEF( ParameterDM, setFilterStartDate )
	BLOG_TRACE_METHOD( getLogger(), fun );

	filterStartDateValid( startDate  );
	setParameter( "FILTER_START_DATE", startDate.getDate() );
}

void ParameterDM::filterEndDateValid( const basar::Date& endDate )
{
	METHODNAME_DEF( ParameterDM, filterEndDateValid );

	if( ! endDate.isValid() )
	{
		basar::VarString msg = "Parameter '" "RESERVE_PERCENT" "' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "RESERVE_PERCENT" );
	}
}

void ParameterDM::csvExportSeparatorValid( const basar::VarString separator )
{
	METHODNAME_DEF( ParameterDM, csvExportSeparatorValid );

	if( separator.empty() )
	{
		basar::VarString msg = "Parameter '" "CSV_DELIMITER_COL" "' not valid!";
		throw exceptions::ParameterException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), "CSV_DELIMITER_COL" );
	}
}

basar::Date ParameterDM::getFilterEndDate()
{
	METHODNAME_DEF( ParameterDM, getFilterEndDate )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Date endDate( getParameterByName( "FILTER_END_DATE" ).getInt32( properties::PARAMETERVALUE ) );
	filterEndDateValid( endDate );

	return endDate;
}

void ParameterDM::setFilterEndDate( const basar::Date& endDate )
{
	METHODNAME_DEF( ParameterDM, setFilterEndDate )
	BLOG_TRACE_METHOD( getLogger(), fun );

	filterEndDateValid( endDate );
	setParameter( "FILTER_END_DATE", endDate.getDate() );
}

ParameterDM::OrderTypeCollection ParameterDM::getOrderTypes()
{
    METHODNAME_DEF( ParameterDM, getOrderTypes )
	BLOG_TRACE_METHOD( getLogger(), fun );

    OrderTypeCollection orderTypeCollection;
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().begin(); yit != getPropTab().end(); ++yit )
    {
        if( yit.getString( properties::PARAMETERNAME ) == "OPB_COMBINATION" )
        {
			//DZ: transfer orders with order type DG should not be able to be created manually
			//CMI transfer orders should only be processed via edifact messages
			if (yit.getString(properties::MANIFESTATION) == libabbauw::lit::DG_ORDERTYPECOMBINATION)
			{
				continue;
			}

            orderTypeCollection.push_back( yit.getString( properties::MANIFESTATION ) );
        }
    }

    return orderTypeCollection;
}

ParameterDM::OrderTypeCollection ParameterDM::getOrderTypesForFiltering()
{
	METHODNAME_DEF(ParameterDM, getOrderTypesForFiltering)
		BLOG_TRACE_METHOD(getLogger(), fun);

	OrderTypeCollection orderTypeCollection;
	for (basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().begin(); yit != getPropTab().end(); ++yit)
	{
		if (yit.getString(properties::PARAMETERNAME) == "OPB_COMBINATION")
		{
			orderTypeCollection.push_back(yit.getString(properties::MANIFESTATION));
		}
	}

	return orderTypeCollection;
}

basar::Int32 ParameterDM::getLargeScaleOrderLimit()
{
    METHODNAME_DEF( ParameterDM, getLargeScaleOrderLimit )
	BLOG_TRACE_METHOD( getLogger(), fun );

    basar::Int32 qty = getParameterByName( "LARGE_QTY" ).getInt32( properties::PARAMETERVALUE );

    return qty;
}

void ParameterDM::setLargeScaleOrderLimit( basar::Int32 qty )
{
    METHODNAME_DEF( ParameterDM, setLargeScaleOrderLimit )
	BLOG_TRACE_METHOD( getLogger(), fun );

    setParameter( "LARGE_QTY", qty );
}

basar::VarString ParameterDM::getCSVExportSeparator()
{
	METHODNAME_DEF( ParameterDM, getCSVExportSeparator )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::VarString separator = getParameterByName( "CSV_DELIMITER_COL" ).getString( properties::MANIFESTATION );

	csvExportSeparatorValid( separator );

	return separator;
}

void ParameterDM::setCSVExportSeparator( basar::VarString separator )
{
    METHODNAME_DEF( ParameterDM, setCSVExportSeparator )
	BLOG_TRACE_METHOD( getLogger(), fun );

    setParameter( "CSV_DELIMITER_COL", separator );
}

void ParameterDM::setDiscountPctTrailer(basar::VarString trailer)
{
	METHODNAME_DEF( ParameterDM, setDiscountPctTrailer )
	BLOG_TRACE_METHOD( getLogger(), fun );

	setParameter("DISCOUNT_PCT_TRAILER", trailer);
}

void ParameterDM::setOrderrouting(basar::VarString orderrouting)
{
	METHODNAME_DEF( ParameterDM, setOrderrouting )
	BLOG_TRACE_METHOD( getLogger(), fun );

	setParameter("ORDERROUTING", orderrouting);
}

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw
