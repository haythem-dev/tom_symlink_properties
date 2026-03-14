#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IORDERTYPEGETTER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IORDERTYPEGETTER_H

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			class IOrderTypeGetter
			{
			public:
				virtual ~IOrderTypeGetter() {}

				virtual bool hasOrderTypeOM() const = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IORDERTYPEGETTER_H
