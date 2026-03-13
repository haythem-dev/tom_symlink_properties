#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERSTATUS_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERSTATUS_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>

namespace libabbauw
{
namespace domMod
{
    class OrderStatus
    {
    public:
        enum values
		{
			ZU,
			FE,
			ER,
			ST = 9,
			END_OF_ENUM		// this HAS TO be the last entry in this enum!
		};
		typedef enum values Status;

    public:
        explicit OrderStatus( const basar::I18nString& str );
        explicit OrderStatus( Status );

        Status           getStatus() const;
        basar::I18nString toString() const;

        bool operator ==( OrderStatus );
        bool operator !=( OrderStatus );

    private:
        OrderStatus();

        basar::I18nString toString( Status ) const;

    private:
        Status m_Status;
    };

} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERSTATUS_H
