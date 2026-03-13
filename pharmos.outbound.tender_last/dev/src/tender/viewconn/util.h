#ifndef GUARD_TENDER_VIEWCONN_UTIL_H
#define GUARD_TENDER_VIEWCONN_UTIL_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include <QComboBox>
#pragma warning (pop)

namespace tender
{
namespace viewConn
{
class ComboBox
{
public:
	static void setCurrentIndex( QComboBox * comboBox, const QVariant & userData );

private:
	ComboBox();
};

} // end namespace viewConn
} // end namespace tender

#endif // GUARD_TENDER_VIEWCONN_UTIL_H
