#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEAD_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEAD_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorderhead.h"
#include "transferorderheadinitparams.h"

#include "dmbase.h"
#include <libutil/misc/componentinitialization.hpp>

#include <libbasardbaspect_accessor.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class ConnectionRef;
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libutil
{
	class ClassInfo;
}

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{

			class TransferOrderHead : public libutil::misc::ComponentInitialization< TransferOrderHeadInitParams, TransferOrderHead >,
				public ITransferOrderHead,
				private DMBase
			{

			public:
				TransferOrderHead(basar::db::aspect::ConnectionRef);
				~TransferOrderHead();

				// defined in transfer order head interface
				basar::db::aspect::AccessorPropertyTable_YIterator       get();
				basar::db::aspect::AccessorPropertyTable_YIterator       get() const;

				void                                                     findById(basar::Int32 orderNo);

				void                                                     markForDelete();
				bool                                                     save();

				basar::Decimal                                           getDiscountPct() const;
				basar::VarString                                         getDiscountPctRange() const;

				basar::Int32                                             getOrderNo() const;
				OrderStatus                                              getOrderStatus() const;

				bool                                                     isChanged() const;
				bool                                                     isEmpty() const;

				void                                                     clear();

			public:
				static const libutil::misc::ClassInfo& getClassInfo();

			protected:
				// from ComponentInitialization
				void                                                     doInit(const TransferOrderHeadInitParams&);
				void                                                     doShutdown();

				// from DMBase
				AccessorExecuteParameterization                          getFindByPatternExecuteParameters() const;
				basar::VarString                                         getSearchYIteratorAttributeNames()  const;

			private:
				TransferOrderHead(const TransferOrderHead&);
				TransferOrderHead operator = (const TransferOrderHead&);

				void                                                     resetSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator       getSearchYit();

			private:
				TransferOrderHeadInitParams				                 m_InitParams;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEAD_H
