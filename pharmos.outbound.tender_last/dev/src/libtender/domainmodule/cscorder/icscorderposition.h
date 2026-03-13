#ifndef GUARD_DOMMOD_CSCORDER_ICSCORDERPOSITION_H
#define GUARD_DOMMOD_CSCORDER_ICSCORDERPOSITION_H

//-----------------------------------------------------------------------------------------

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

class ICscOrderPosition
{
public:
	virtual ~ICscOrderPosition() {}

	virtual void				setIbtPossible(bool) = 0;
	virtual void				setRestDeliveryPossible(bool) = 0;
	virtual void				setDispoPossible(bool) = 0;
	virtual void				setPartialDeliveryPossible(bool) = 0;

	virtual basar::Int32		getOrderQuantity() = 0;
	virtual basar::Int32		getIBTQuantity() = 0;
	virtual basar::VarString	getArticleCode() = 0;

	virtual basar::Int32		getDeliveryQuantity() = 0;
	virtual basar::Int32		getLackReason() = 0;
	virtual basar::VarString	getText() = 0;

	virtual void				save(bool importing = false) = 0;
};

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_DOMMOD_CSCORDER_ICSCORDERPOSITION_H
