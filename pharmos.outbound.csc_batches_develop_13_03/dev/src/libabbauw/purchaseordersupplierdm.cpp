//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmndm.h"

#include "purchaseordersupplierdm.h"
#include "purchaseordersupplierdm_definitions.h"
#include "purchaseordersupplieracc.h"

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace purchaseordersupplierdm
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    PurchaseOrderSupplierDM::PurchaseOrderSupplierDM( const basar::db::aspect::ConnectionRef conn )
        : DMBase( conn, ACC_PURCHASEORDERSUPPLIER, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerPurchaseOrderSupplier )
	{
        METHODNAME_DEF( PurchaseOrderSupplierDM, PurchaseOrderSupplierDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
		
        acc_purchaseordersupplier::SelectPurchaseOrderSupplier dummy;
	}

	PurchaseOrderSupplierDM::~PurchaseOrderSupplierDM()
	{
        METHODNAME_DEF( PurchaseOrderSupplierDM, ~PurchaseOrderSupplierDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

	AccessorExecuteParameterization PurchaseOrderSupplierDM::getFindByPatternExecuteParameters() const
	{
        return AccessorExecuteParameterization( SELECT_PURCHASEORDERSUPPLIER );
	}

	basar::VarString PurchaseOrderSupplierDM::getSearchYIteratorAttributeNames() const
    {
		return properties::PURCHASEORDERSUPPLIERNO.toDescriptionString();
	}
	
	void PurchaseOrderSupplierDM::findPurchaseOrderSupplierById( basar::Int32 purchaseOrderSupplierNo )
	{
		METHODNAME_DEF( PurchaseOrderSupplierDM, findPurchaseOrderSupplierById )
        BLOG_TRACE_METHOD(getLogger(), fun );

        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = getSearchYit();
        yitSearchTmp.setInt32( properties::PURCHASEORDERSUPPLIERNO, purchaseOrderSupplierNo );
		DMBase::findByPattern( yitSearchTmp );
	}

	basar::db::aspect::AccessorPropertyTable_YIterator PurchaseOrderSupplierDM::getSearchYit()
	{
		return DMBase::getSearchYit();
	}

    void PurchaseOrderSupplierDM::resetSearchYit()
    {
	    DMBase::resetSearchYit();
    }

    bool PurchaseOrderSupplierDM::isEmpty() const
    {
        return DMBase::isEmpty();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator PurchaseOrderSupplierDM::get()
	{
		return DMBase::getPropTab().begin();
	}

    basar::db::aspect::AccessorPropertyTable_YIterator PurchaseOrderSupplierDM::get() const
	{
		return DMBase::getPropTab().begin();
	}

    void PurchaseOrderSupplierDM::clear()
    {
        METHODNAME_DEF( PurchaseOrderSupplierDM, clear )
        BLOG_TRACE_METHOD(getLogger(), fun );

        DMBase::getPropTab().clear();
    }

	const libutil::misc::ClassInfo & PurchaseOrderSupplierDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "PurchaseOrderSupplierDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

} // end namespace purchaseordersupplierdm
} // end namespace domMod
} // end namespace libabbauw
