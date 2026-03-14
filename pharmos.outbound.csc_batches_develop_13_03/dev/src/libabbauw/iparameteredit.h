#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETEREDIT_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETEREDIT_H

#include "parameter_definitions.h"
#include "iparameter.h"

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
	namespace domMod
	{
		namespace parameter
		{

			class IParameterEdit : public IParameter
			{
			public:

				virtual ~IParameterEdit() {}

				// We created an interface for these methods to assure that nobody can change the parameters except the parameter screen.
				virtual void                                                setOrderSort(const util::SortOrder&) = 0;
				virtual void                                                setDaysToColor(basar::Int32) = 0;
				virtual void                                                setReserveQuantityReleaseFactor(basar::Decimal) = 0;
				virtual void                                                setReserveQuantityOrderFactor(basar::Decimal) = 0;
				virtual void                                                setNotSalesEffectiveReserveCheck(bool) = 0;
				virtual void                                                setNotSalesEffectiveReserveQtyReleaseFactor(basar::Decimal) = 0;
				virtual void                                                setNotSalesEffectiveReserveQtyOrderFactor(basar::Decimal) = 0;
				virtual void                                                setFilterStartDate(const basar::Date&) = 0;
				virtual void                                                setFilterEndDate(const basar::Date&) = 0;
				virtual void                                                setLargeScaleOrderLimit(basar::Int32) = 0;
				virtual void                                                setCSVExportSeparator(basar::VarString) = 0;
			};

		} // end namespace parameter
	} // end namespace domMod
} // end namespace libabbauw

#endif	// GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETEREDIT_H
