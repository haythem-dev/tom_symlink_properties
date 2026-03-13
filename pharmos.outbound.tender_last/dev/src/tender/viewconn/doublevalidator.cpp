#include "doublevalidator.h"

namespace tender
{
namespace viewConn
{
DoubleValidator::DoubleValidator( QObject * parent )
: QRegExpValidator( QRegExp( getRegExp() ), parent )
{
}

DoubleValidator::~DoubleValidator()
{
}

const QString DoubleValidator::getRegExp()
{
	QString sign = QLocale::system().decimalPoint();
	QString positiveVal = "(([0-9]{1,7})([" + sign + "][0-9]{0,2}){0,1})";
	return positiveVal;
}

} // end namespace viewConn
} // end namespace tender

/*
{
    QString sign = QLocale::system().decimalPoint ();
    QString positiveVal             =  "([0-9]{1,7})(["+ sign + "][0-9]{0,2}){0,1}";
    QString negativValGTmin1 =  "(-0)(["           + sign + "][0-9]{0,2}){0,1}";
    QString negativValmin1      = "(-1)(["            + sign + "][0]{0,2}){0,1}";
  
    QString all;
    all.append("((");
    all.append(positiveVal);
     all.append(")|(");
     all.append(negativValGTmin1);
     all.append(")|(");
     all.append(negativValmin1);
    all.append("))"); 
        
    QRegExpValidator * c = new QRegExpValidator ( QRegExp(all),ui.lineEdit_2 );
    ui.lineEdit_2->setValidator(c);
    }

*/