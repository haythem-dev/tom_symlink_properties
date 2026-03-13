#include "tenderstatecollection.h"

#ifdef WIN32
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef WIN32
#pragma warning (pop)
#endif

namespace libtender
{
	namespace domMod
	{
		namespace tender
		{
			TenderStateCollection::TenderStateCollection()
			{
			    m_Map[ TS_UNCOMPLETED ] = "Capturing data";
			    m_Map[ TS_OK ]	= "Active";
			    m_Map[ TS_MANUALLY_CLOSED ] = "Closed manually";
			    m_Map[ TS_AUTOMATICALLY_CLOSED ] = "Closed auto";
			}
			
			TenderStateCollection::const_iterator TenderStateCollection::begin() const
			{
			    return m_Map.begin();
			}
			
			TenderStateCollection::const_iterator TenderStateCollection::end() const
			{
			    return m_Map.end();
			}
			
			basar::I18nString TenderStateCollection::getBusinessStateName( TenderStateEnum tenderStateEnum )
			{
			    TenderStateCollection::const_iterator it = m_Map.find( tenderStateEnum );
			    
			    if ( m_Map.end() == it )
			    {
			        throw;
			    }
			    
			    return it->second;
			}

			//Is needed for translations... 
			void TenderStateCollection::setStates(QString uncompleted, QString active, QString closed, QString closedAutomatic)
			{
				m_Map[ TS_UNCOMPLETED ]			 = uncompleted.toStdString();
			    m_Map[ TS_OK ]					 = active.toStdString();
			    m_Map[ TS_MANUALLY_CLOSED ]		 = closed.toStdString();
			    m_Map[ TS_AUTOMATICALLY_CLOSED ] = closedAutomatic.toStdString();
			}
		
		} 
	} 
} 
