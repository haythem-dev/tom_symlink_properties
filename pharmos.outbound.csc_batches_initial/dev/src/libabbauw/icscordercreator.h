#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERCREATOR_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERCREATOR_H

#include <boost/shared_ptr.hpp>

// this is a violation of use of systemeventmanager but the providing (interface implementing) class
// needs this
#include <libbasarappl_systemeventmanager.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
		}
	}
}

namespace libabbauw
{
namespace domMod
{
namespace cscorder
{

class ICSCOrderCreator
{
public:
	virtual ~ICSCOrderCreator() {}

    virtual void init()     = 0;
    virtual void shutdown() = 0;

	virtual basar::appl::EventReturnType createCSCOrderABBAUW(	basar::Int16		branchno,
                                                                basar::Int16        branchnoorig,
																basar::Int32		orderno,
																basar::Int32		pharmacyno,
                                                                basar::Int32        purchaseorderno, //P_17000393
																const basar::I18nString&	datasourcetype,
																const basar::I18nString&	ordertype,
																const basar::I18nString&	headertext,
																const basar::Date&		deferredpaymentdate,
																const basar::Date&		edideliverydate,
																const bool			postpone			  // true: postpone order, false: close order completely
															) = 0;
};

} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERCREATOR_H
