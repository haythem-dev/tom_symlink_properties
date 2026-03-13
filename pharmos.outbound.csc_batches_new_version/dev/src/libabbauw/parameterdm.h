#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_PARAMETERDM_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_PARAMETERDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iparameterdm.h"
#include "parameterinitparams.h"

#include <libutil/misc/componentinitialization.hpp>
#include "dmbase.h"

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
            class ConnectionRef;
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libutil
{
	class ClassInfo;
}

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class ParameterDM : public libutil::misc::ComponentInitialization< ParameterInitParams, ParameterDM >,
                    public IParameterDM,
	                private DMBase
{
public:
	ParameterDM ( basar::db::aspect::ConnectionRef );
	~ParameterDM ();

    void                                                   injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );

	// defined in parameter interface
	void                                                   save();

	// defined in parameter interface
	void                                                   load();
	bool                                                   setInvalidParametersToDefault();

	basar::VarString									   getDiscountPctTrailer();
	basar::VarString									   getOrderrouting();


	// defined in userfilterparameter interface
	util::SortOrder                                        getOrderSort();
	basar::Date                                            getFilterStartDate();
	basar::Date                                            getFilterEndDate();

	// defined in orderparameter interface
	basar::Int32                                           getDaysToColor();
    basar::Int32                                           getLargeScaleOrderLimit();

    basar::Decimal                                         getReserveQuantityReleaseFactor();
    basar::Decimal                                         getReserveQuantityOrderFactor();

	basar::VarString									   getNotSalesEffectiveReserveCheck();
	bool	                                               isNotSalesEffectiveReserveCheckSet();
	basar::Decimal										   getNotSalesEffectiveReserveCheckOld();

	basar::Decimal                                         getNotSalesEffectiveReserveQtyReleaseFactor();
	basar::Decimal										   getNotSalesEffectiveReserveQtyOrderFactor();
    bool                                                   isNotSalesEffectiveReserveQtyReleaseFactorSet();
	bool                                                   isNotSalesEffectiveReserveQtyOrderFactorSet();

	basar::VarString									   getCSVExportSeparator();


    // defined in ordercreationparameter interface
    bool                                                   getFormatHeaderTextEnabled();

	// defined in stock parameter interface
    basar::VarString                                       getTransitStorageLocationSlot();

    // defined in ordertypecollection parameter interface
    OrderTypeCollection                                    getOrderTypes();
	OrderTypeCollection                                    getOrderTypesForFiltering();

    // defined in ordercheck parameter interface
    bool                                                   emitWarningOnNonChargedQty();

    // defined in orderrelease parameter interface
    bool                                                   releaseRedYellowOrdersPostponed();

	// definded in parameteredit interface
	void                                                   setOrderSort( const util::SortOrder& orderSort );
	void                                                   setDaysToColor( basar::Int32 );
	void                                                   setReserveQuantityReleaseFactor( basar::Decimal );
    void                                                   setReserveQuantityOrderFactor( basar::Decimal );
    
	void												   setNotSalesEffectiveReserveCheck( bool );
	void                                                   setNotSalesEffectiveReserveQtyReleaseFactor( basar::Decimal );
	void                                                   setNotSalesEffectiveReserveQtyOrderFactor( basar::Decimal );

	void                                                   setFilterStartDate( const basar::Date& );
	void                                                   setFilterEndDate( const basar::Date& );
    void                                                   setLargeScaleOrderLimit( basar::Int32 );
	void                                                   setCSVExportSeparator( basar::VarString );
	void												   setDiscountPctTrailer( basar::VarString );
	void												   setOrderrouting( basar::VarString );

public:
	static const libutil::misc::ClassInfo&                 getClassInfo();

protected:
	// from ComponentInitialization
	void                                                   doInit( const ParameterInitParams& );
	void                                                   doShutdown();

	// from DMBase
	AccessorExecuteParameterization                        getFindByPatternExecuteParameters() const;
	basar::VarString                                       getSearchYIteratorAttributeNames()  const;

private:
	void                                                   resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator     getSearchYit();
	basar::db::aspect::AccessorPropertyTableRef            getPropTab();

	basar::db::aspect::AccessorPropertyTable_YIterator     addNewParameter( const basar::VarString& paramName );
	basar::db::aspect::AccessorPropertyTable_YIterator     findParameterByName( const basar::VarString& parameterName );
	basar::db::aspect::AccessorPropertyTable_YIterator     getParameterByName( const basar::VarString& parameterName );
	void                                                   setParameter( const basar::VarString& paramName, basar::Int32 value );
	void                                                   setParameter( const basar::VarString& paramName, const basar::VarString& value );

	void                                                   orderSortValid( const util::SortOrder& orderSort );
	void                                                   daysToColorValid( basar::Int32 days );
	void                                                   reserveQuantityFactorValid( basar::Decimal reserveQtyFactor );
	void                                                   notSalesEffectiveReserveCheckValid( basar::VarString check );
    void                                                   notSalesEffectiveReserveQuantityFactorValid( basar::Decimal reserveQtyFactor );
	void                                                   transitStorageLocationSlotValid( const basar::VarString& transitStorageLocationSlot );
	void                                                   filterStartDateValid( const basar::Date& startDate  );
	void                                                   filterEndDateValid( const basar::Date& endDate );
	void                                                   csvExportSeparatorValid( const basar::VarString separator );

	bool                                                   isSaveable( const basar::db::aspect::AccessorPropertyTable_YIterator yit );
	void                                                   saveParameter( const basar::db::aspect::AccessorPropertyTable_YIterator yit );

private:
	ParameterDM ( const ParameterDM& );
	ParameterDM operator = ( const ParameterDM& );

private:
	ParameterInitParams				                                     m_InitParams;
    libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_DBTransactionFactory;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_PARAMETERDM_H
