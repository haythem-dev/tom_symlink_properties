#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_IORDERGETTER_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_IORDERGETTER_H

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
	namespace domMod
	{
		namespace cscorder
		{
			class IOrderGetter
			{
			public:
				virtual ~IOrderGetter() {}

				virtual basar::db::aspect::AccessorPropertyTable_YIterator  get() const = 0;
			};

		} // end namespace cscorder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CSCORDER_IORDERGETTER_H
