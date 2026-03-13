#include "util.h"

namespace tender
{
namespace viewConn
{
void ComboBox::setCurrentIndex( QComboBox * comboBox, const QVariant & userData )
{
	for( int i = 0; i < comboBox->count(); ++i )
	{
		if( comboBox->itemData( i ) == userData )
		{
			comboBox->setCurrentIndex( i );
			break;
		}
	}
}

} // end namespace viewConn
} // end namespace tender
