#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_STATE_COLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_STATE_COLLECTION_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
//#include <QObject>
#include <QtCore/QCoreApplication>
#pragma warning (pop)

#include <map>
#include "tenderstateenum.h"
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_definitions.h>

namespace libtender
{
namespace domMod
{
namespace tender
{
	class TenderStateCollection
	{
		public:
		    TenderStateCollection();
		
		private:
		    typedef std::map<TenderStateEnum, basar::I18nString> InnerMap;
		
		public:
		    typedef InnerMap::const_iterator const_iterator;
		    
		    TenderStateCollection::const_iterator begin() const;
		    TenderStateCollection::const_iterator end() const;
		    basar::I18nString getBusinessStateName( TenderStateEnum tenderStateEnum );

			void setStates(QString uncompleted, QString active, QString closed, QString closedAutomatic);

		private:
		    InnerMap        m_Map; 
	};

} 
} 
} 

#endif 
