#include "tendertypecollection.h"

#ifdef WIN32
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
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
			TenderTypeCollection::TenderTypeCollection( infrastructure::CountryIDEnum countryID )
			{
				using namespace libtender::infrastructure;
			
				switch( countryID )
				{
					case CID_BG:
					{
						m_Map[ TT_HOSPITAL ] = "Hospital";
						m_Map[ TT_STATE ] = "State";
						break;
					}
			
					case CID_FR:
					{
						m_Map[ TT_PREWHOLESALE ] = "Prewholesale";
						m_Map[ TT_PLATFORM ] = "Platform";
						m_Map[ TT_SRA ] = "SRA";
						m_Map[ TT_CAP ] = "CAP";
						break;
					}
			
					case CID_RS:
					default:
					{
						m_Map[ TT_STANDARD ] = "Standard";
						break;
					}
				}
			}
			
			TenderTypeCollection::const_iterator TenderTypeCollection::begin() const
			{
				return m_Map.begin();
			}
			
			TenderTypeCollection::const_iterator TenderTypeCollection::end() const
			{
				return m_Map.end();
			}
			
			basar::I18nString TenderTypeCollection::getBusinessTypeName( TenderTypeEnum tenderTypeEnum )
			{
				TenderTypeCollection::const_iterator it = m_Map.find( tenderTypeEnum );
			
				if ( m_Map.end() == it )
				{
					throw;
				}
			
				return it->second;
			}
			
			//Is needed for translations... 
			void TenderTypeCollection::setStandardBusinessType(QString standard)
			{
				m_Map[ TT_STANDARD ] = standard.toStdString();
			}

		} 
	} 
} 
