#ifndef GUARD_TENDER_VIEWCONN_DOUBLE_VALIDATOR_H
#define GUARD_TENDER_VIEWCONN_DOUBLE_VALIDATOR_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include <QtCore/QLocale>
	#include <QRegExpValidator>
#pragma warning (pop)

namespace tender
{
namespace viewConn
{
class DoubleValidator : public QRegExpValidator
{
public:
	DoubleValidator( QObject * parent );
	virtual ~DoubleValidator();

private:
	static const QString getRegExp();
};

} // end namespace viewConn
} // end namespace tender

#endif // GUARD_TENDER_VIEWCONN_DOUBLE_VALIDATOR_H
