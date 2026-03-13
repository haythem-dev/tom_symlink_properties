#ifndef GUARD_DOMMOD_CSCORDER_CSCORDERPOSITION_H
#define GUARD_DOMMOD_CSCORDER_CSCORDERPOSITION_H

//-----------------------------------------------------------------------------------------

#include "icscorderposition.h"

#include <kscclient/orderinfo.h>

//-----------------------------------------------------------------------------------------

namespace log4cplus {
	class Logger;
}

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

//-----------------------------------------------------------------------------------------

class CscOrderPosition : public ICscOrderPosition
{
public:
	~CscOrderPosition();

	virtual basar::Int32 getDeliveryQuantity();
	virtual basar::Int32 getIBTQuantity();
	virtual basar::Int32 getLackReason();
	virtual basar::VarString getText();

	virtual basar::Int32		getOrderQuantity();
	virtual basar::VarString	getArticleCode();

	virtual void setIbtPossible(bool); /* auxiliary */
	virtual void setRestDeliveryPossible(bool); /* restdelivery */
	virtual void setDispoPossible(bool); /* dispose */
	virtual void setPartialDeliveryPossible(bool); /* part delivery */

	virtual void save(bool importing = false);

private:
	CscOrderPosition(const basar::VarString& articleCode, const basar::Int32 qty, const log4cplus::Logger& logger);
	CscOrderPosition(const orderinfoS& info, const log4cplus::Logger& logger);
	CscOrderPosition(const CscOrderPosition &);
	CscOrderPosition & operator = ( const CscOrderPosition & );

	const log4cplus::Logger&	getLogger() const;

	orderinfoS					m_OrderInfo;
	const log4cplus::Logger&	m_Logger;

	basar::VarString			m_ArticleCode;
	basar::Int32				m_OrderQuantity;
	bool						m_IbtPossible; /* auxiliary */
	bool						m_RestDelPossible; /* restdelivery */
	bool						m_DispoPossible; /* dispose */
	bool						m_PartDelPossible; /* part delivery */

	friend class CscOrderDM;
};

//-----------------------------------------------------------------------------------------

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------

#endif // GUARD_DOMMOD_CSCORDER_CSCORDERPOSITION_H

//-----------------------------------------------------------------------------------------

