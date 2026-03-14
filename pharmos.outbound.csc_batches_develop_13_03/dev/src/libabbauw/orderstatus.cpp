#include "orderstatus.h"

namespace libabbauw
{
namespace domMod
{
    OrderStatus::OrderStatus( const basar::I18nString& str ) : m_Status( END_OF_ENUM )
    {
        for( int i = ZU; i != END_OF_ENUM; ++i )
        {
            Status testStatus = static_cast< Status >( i );
            if( toString( testStatus ) == str )
            {
                m_Status = testStatus;
                break;
            }
        }
    }

    OrderStatus::OrderStatus( Status s ) : m_Status( s )
    {
    }

    OrderStatus::Status OrderStatus::getStatus() const
    {
        return m_Status;
    }

    basar::I18nString OrderStatus::toString() const
    {
        return toString( m_Status );
    }

    bool OrderStatus::operator ==( OrderStatus rhs )
    {
        return m_Status == rhs.m_Status;
    }

    bool OrderStatus::operator !=( OrderStatus rhs )
    {
        return m_Status != rhs.m_Status;
    }

    basar::I18nString OrderStatus::toString( Status s ) const
    {
	    static basar::ConstString colorMapping[END_OF_ENUM] =
	    {
		    "ZU", "FE", "ER", "", "", "", "", "", "", "ST"
	    };

	    if( s != END_OF_ENUM )
		    return colorMapping[s];
	    else
		    return "";
    }
} // end namespace domMod
} // end namespace libabbauw
