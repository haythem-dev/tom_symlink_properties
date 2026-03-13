#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERHEADER_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERHEADER_H

#include "itenderheader.h"
#include <libutil/accessor.h>
#include <libutil/classinfo.h>
#include <libutil/validator.h>

namespace basar
{
namespace db
{
namespace aspect
{
	//class AccessorInstanceRef;
	class AccessorPropertyTable_YIterator;
	class AccessorPropertyTableRef;
}
}
}

namespace libtender
{
namespace domMod
{
namespace tender
{
class TenderHeader : public ITenderHeader
{
public:
	static const libutil::misc::ClassInfo & getClassInfo();

public:
	TenderHeader();
	~TenderHeader();

	void injectTenderHeaderAccessor( libutil::infrastructure::accessor::IAccessorPtr );
	//void injectValidator( libutil::misc::IValidatorPtr );

	virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTable_YIterator get();
	virtual const basar::db::aspect::AccessorPropertyTable_YIterator get() const;

	virtual void save();
	virtual bool isEmpty() const;
	virtual void validate() const;
	virtual bool isValid() const;
	virtual TenderStateEnum getState() const;
	virtual void setState( const TenderStateEnum state );

	virtual basar::Int32 getPrimaryKey() const;

private:
	TenderHeader( const TenderHeader & r );
	TenderHeader& operator = ( const TenderHeader & r );

	libutil::infrastructure::accessor::IAccessorPtr getAccessor() const;
	//libutil::misc::IValidatorPtr getValidator() const;

	basar::db::aspect::AccessorPropertyTable_YIterator getHeader() const;

	bool isSavable() const;
	bool isAssignable() const;

	void checkNumberHeader() const;
	bool hasSavableStates() const;
	void createNew() const;

	const log4cplus::Logger & getLogger() const;

	const log4cplus::Logger 							m_Logger;
	libutil::infrastructure::accessor::IAccessorPtr		m_Accessor;
	//libutil::misc::IValidatorPtr						m_Validator;

};

} // end namespace tender
} // end namespace domMod
} // end namespace domMod

#endif  // GUARD_LIBTENDER_DOMMOD_TENDER_TENDERHEADER_H
