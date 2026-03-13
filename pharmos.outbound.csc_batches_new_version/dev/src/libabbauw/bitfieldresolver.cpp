#include "bitfieldresolver.h"
#include <libbasarcmnutil_bstring.h>
#include <libbasarcmnutil_decimal.h>

namespace libabbauw
{
namespace util
{

BitFieldResolver::BitFieldResolver()
{
}

basar::VarString BitFieldResolver::resolveDeliveryNote(basar::Int32 etartschalter1, basar::Int32 etartschatler2, basar::Int32 stock)
{
	basar::VarString result = "";

	if( 0 == etartschalter1 ) return result;

	if( isBitSet(etartschalter1, BIT15 ) ) result.append("E");	//Nachfolgeartikel
	if( isBitSet(etartschalter1, BIT13 ) ) result.append("A");	//ausser Handel
	if( isBitSet(etartschalter1, BIT12 ) ) result.append("AV");	//Auslaufartikel
	if( isBitSet(etartschalter1, BIT4) ) result.append("H");	//vom Hersteller nicht geliefiert
	if( isBitSet(etartschalter1, BIT5) ) result.append("L");	//vom Hersteller nicht lieferbar
	if( !isBitSet(etartschatler2, BIT1) && 0 == stock ) result.append("F");	//Artikel wird nicht geführt
	
	return result;
}

basar::VarString BitFieldResolver::resolveNotice(basar::Int32 etartschalter1, basar::Int32 etartklasse1)
{
	basar::VarString result = "";
	
	if( 0 == etartschalter1 && 0 == etartklasse1 ) return result;

	if( isBitSet(etartklasse1, BIT10) ) result.append("N"); //BTM
	
	if( isBitSet(etartschalter1, BIT6) ||
		isBitSet(etartschalter1, BIT7) ||
		isBitSet(etartschalter1, BIT8)    ) result.append("K"); //Kühlartikel
	
	return result;
}

bool BitFieldResolver::isBitSet(basar::Int32 bitField, basar::Int32 bitPosition)
{
	//Formula trunc(bitfield/bit) - 2 * trunc(bitfield/(bit*2))

	basar::Int32 mainResult = 0;

	basar::Decimal truncResult1( bitField/bitPosition     );
	basar::Decimal truncResult2( bitField/(bitPosition*2) );

	truncResult1.trunc(2);
	truncResult2.trunc(2);
	
	basar::Int32 result1 = truncResult1.toInt32();
	basar::Int32 result2 = truncResult2.toInt32();

	mainResult = result1 - 2 * result2;
	
	if( 0 == mainResult ) return false;
	else return true;
}

} // end namespace util
} // end namespace libabbauw
