#ifndef GUARD_LIBABBAUW_UTIL_BITFIELDRESOLVER_H
#define GUARD_LIBABBAUW_UTIL_BITFIELDRESOLVER_H

#include <libbasardbaspect_accessorpropertytable_yiterator.h>


namespace libabbauw
{
namespace util
{
    class BitFieldResolver
    {
    public:
		BitFieldResolver();

		basar::VarString resolveDeliveryNote(basar::Int32 etartschalter1, basar::Int32 etartschatler2, basar::Int32 stock);
		basar::VarString resolveNotice(basar::Int32 etartschalter1, basar::Int32 etartklasse1);

    private:
		
        BitFieldResolver( const BitFieldResolver& );
        BitFieldResolver& operator = ( const BitFieldResolver& );

		bool isBitSet(basar::Int32 bitField, basar::Int32 bitPosition);
		

		enum POSTOBIT
		{
			BIT1 =	1,
			BIT2 =	2,
			BIT3 =	4,
			BIT4 =	8,
			BIT5 =	16,
			BIT6 =	32,
			BIT7 =	64,
			BIT8 =	128,
			BIT9 =	256,
			BIT10 = 512,
			BIT11 = 1024,
			BIT12 = 2048,
			BIT13 = 4096,
			BIT14 = 8192,
			BIT15 = 16384,
			BIT16 = 32768
		};
    };

} // end namespace libabbauw
} // end namespace util

#endif // GUARD_LIBABBAUW_UTIL_BITFIELDRESOLVER_H
