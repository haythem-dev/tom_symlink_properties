#ifndef GUARD_LIBTENDER_DOMMOD_PARTNER_IPHARMACYCHAINCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_PARTNER_IPHARMACYCHAINCOLLECTIONDM_H

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

namespace libtender
{
namespace domMod
{
namespace pharmacychain
{
class IPharmacyChainCollectionDM
{
public:
	virtual ~IPharmacyChainCollectionDM() {}

	virtual void                                                findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void                                                findByPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void												findCustomerGroupIDByPharmacyChain( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	
	virtual basar::db::aspect::AccessorPropertyTableRef         get() const          = 0;
	virtual void                                                resetSearchYit()     = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
	virtual void                                                setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;
	virtual bool                                                isEmpty() const      = 0;
};

} //end namespace pharmacychain
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_PARTNER_IPHARMACYCHAINCOLLECTIONDM_H
