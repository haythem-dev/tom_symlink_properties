#include "countrycodemapper.h"

namespace libtender
{
namespace infrastructure
{
CountryIDEnum CountryCodeMapper::getCountryID( const basar::Int16 areaID )
{
	CountryIDEnum ret = CID_UNKNOWN;
	switch( areaID )
	{
		case 43:
		case 48:
		case 2011:
		{
			ret = CID_FR;
			break;
		}

		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 2008:
		{
			ret = CID_BG;
			break;
		}

		case 3:
		case 84:
		case 85:
		case 2009:
		{
			ret = CID_RS;
			break;
		}

        case 12:
        case 13:
        case 16:
        case 2002:
        {
            ret = CID_CH;
            break;
        }

		default:
		{
			// nothing to do
			break;
		}
	}

	return ret;
}

CountryIDEnum CountryCodeMapper::getCountryID( const basar::VarString countryStr )
{
	CountryIDEnum ret = CID_UNKNOWN;
	if (countryStr == "PFR")
	{
		ret = CID_FR;
	}
	if (countryStr == "BG")
	{
		ret = CID_BG;
	}
	if (countryStr == "RS")
	{
		ret = CID_RS;
	}
	if (countryStr == "CH")
	{
		ret = CID_CH;
	}

	return ret;
}

basar::Int16 CountryCodeMapper::getRegionID( const CountryIDEnum country )
{
	basar::Int16 regionID = 0;
	switch ( country )
	{
		case CID_FR : regionID = 2011; break;
		case CID_BG : regionID = 2008; break;
		case CID_RS : regionID = 2009; break;
        case CID_CH : regionID = 2002; break;
		default: break;
	}
	return regionID;
}

} // end namespace infrastructure
} // end namnespace libtender