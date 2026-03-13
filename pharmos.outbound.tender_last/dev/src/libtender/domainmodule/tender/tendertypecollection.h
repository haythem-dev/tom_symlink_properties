#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_TYPE_COLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_TYPE_COLLECTION_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
//#include <QObject>
#include <QtCore/QCoreApplication>
#pragma warning (pop)



#include <map>
#include "tendertypeenum.h"
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_definitions.h>
#include <infrastructure/countryidenum.h>

namespace libtender
{
namespace domMod
{
namespace tender
{
class TenderTypeCollection
{
public:
	TenderTypeCollection( infrastructure::CountryIDEnum countryID );

private:
	typedef std::map<TenderTypeEnum, basar::I18nString> InnerMap;

public:
	typedef InnerMap::const_iterator const_iterator;

	TenderTypeCollection::const_iterator begin() const;
	TenderTypeCollection::const_iterator end() const;

	basar::I18nString getBusinessTypeName( TenderTypeEnum tenderTypeEnum );

	void setStandardBusinessType(QString standard);

private:
	InnerMap		m_Map; 
	
};

} 
} 
} 

#endif 
