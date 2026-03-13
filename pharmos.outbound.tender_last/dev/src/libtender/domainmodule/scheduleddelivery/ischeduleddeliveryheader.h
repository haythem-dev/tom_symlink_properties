#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYHEADER_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYHEADER_H

#include <libbasar_definitions.h>

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

namespace libtender	{
namespace domMod {
namespace scheduledDelivery	{

class IScheduledDeliveryHeader
{
public:
	virtual ~IScheduledDeliveryHeader() {}

	virtual basar::Int32 getScheduledDeliveryId() const = 0;
	virtual basar::Int16 getBranchNo() const = 0;
	virtual basar::Int32 getTenderId() const = 0;
	virtual basar::Int32 getCustomerNo() const = 0;
	virtual basar::Date getDeliveryDate() const = 0;
	virtual basar::Int32 getCscOrderNo() const = 0;
	virtual basar::Date getCscOrderDate() const = 0;

	virtual void  setCscOrderNo(basar::Int32) = 0;
	virtual void  setCscOrderDate(basar::Date) = 0;

	virtual void findByKey ( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator get() = 0;
	virtual const basar::db::aspect::AccessorPropertyTable_YIterator get() const = 0; 

	virtual basar::Int32 getPrimaryKey() const = 0;

	virtual void save() = 0;
	virtual bool isEmpty() const = 0;
};

		} //end namespace scheduledDelivery
	}	  //end namespace domMod
}		  //end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYHEADER_H
