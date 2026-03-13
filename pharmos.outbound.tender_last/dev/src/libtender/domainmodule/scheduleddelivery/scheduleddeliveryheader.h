#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYHEADER_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYHEADER_H

//-----------------------------------------------------------------------------------------

#include "ischeduleddeliveryheader.h"

#include <libutil/accessor.h>
#include <libutil/classinfo.h>
#include <libutil/validator.h>

//-----------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace scheduledDelivery {
//-----------------------------------------------------------------------------------------

class ScheduledDeliveryHeader : public IScheduledDeliveryHeader
{
public:
	ScheduledDeliveryHeader();
	~ScheduledDeliveryHeader();

	virtual basar::Int32 getScheduledDeliveryId() const;
	virtual basar::Int16 getBranchNo() const;
	virtual basar::Int32 getTenderId() const;
	virtual basar::Int32 getCustomerNo() const;
	virtual basar::Date getDeliveryDate() const;
	virtual basar::Int32 getCscOrderNo() const;
	virtual basar::Date getCscOrderDate() const;
	virtual void  setCscOrderNo(basar::Int32);
	virtual void  setCscOrderDate(basar::Date);

	void injectScheduledDeliveryHeaderAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor );

	virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTable_YIterator get();
	virtual const basar::db::aspect::AccessorPropertyTable_YIterator get() const;

	virtual basar::Int32 getPrimaryKey() const;

	virtual void save();
	virtual bool isEmpty() const;

private:
	ScheduledDeliveryHeader( const ScheduledDeliveryHeader & r );
	ScheduledDeliveryHeader& operator = ( const ScheduledDeliveryHeader & r );

	libutil::infrastructure::accessor::IAccessorPtr    getAccessor() const;
	basar::db::aspect::AccessorPropertyTable_YIterator getHeader()   const;

	bool isSavable() const;
	bool hasSavableStates() const;

	void checkNumberHeader() const;
	void createNew() const;

	const log4cplus::Logger & getLogger() const;

	const log4cplus::Logger 					    m_Logger;
	libutil::infrastructure::accessor::IAccessorPtr m_Accessor;
};

//-----------------------------------------------------------------------------------------

} //end namespace scheduleddelivery
} //end namespace domMod
} //end namespace libtender

//-----------------------------------------------------------------------------------------

#endif //end GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYHEADER_H

//-----------------------------------------------------------------------------------------
