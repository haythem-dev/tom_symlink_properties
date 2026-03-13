#ifndef GUARD_TENDER_VIEWCONN_ITENDERCONTRACTINFO_H
#define GUARD_TENDER_VIEWCONN_ITENDERCONTRACTINFO_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QtWidgets/QGroupBox>
#pragma warning (pop)

#include <libtender/domainmodule/tender/tenderdm.h>
#include <libtender/domainmodule/tender/tenderstateenum.h>
#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>
#include <libtender/domainmodule/tender/tendertypeenum.h>
//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//class QFrame;

namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

//-------------------------------------------------------------------------------------------------//
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class ITenderContractInfo: public QGroupBox
        {
            public:
                virtual ~ITenderContractInfo(){};

				virtual void injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping ) = 0;
				virtual void injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping  ) {};

                virtual void matchToTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender ) = 0;
                virtual void matchFromTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender, bool hasTenderArticles ) = 0;

                virtual void matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer ) = 0;
                virtual void matchFromPharmacyGroup( basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyGroup ) {};
				virtual void matchFromPartner( basar::db::aspect::AccessorPropertyTable_YIterator yitPartner ) {};
				virtual void matchFromPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyChain ) {};

				virtual void matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit ) = 0;
                virtual void matchToPharmacyGroupSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupSearchYit ) {};
				virtual void matchToPartnerSearch( basar::db::aspect::AccessorPropertyTable_YIterator partnerSearchYit ) {};
				virtual void matchToPharmacyChainSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChainSearchYit ) {};

				virtual void setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;
                virtual void setTenderState( libtender::domMod::tender::TenderStateEnum ) {};
                virtual void setPharmacyGroupId( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};
				virtual void setPartnerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};
				virtual void setPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};

				virtual const libtender::domMod::tender::TenderTypeCollectionPtr getTypeMapping() = 0;
				virtual const libtender::domMod::tender::TenderStateEnum getStateIndex() = 0;
				virtual const libtender::domMod::tender::TenderStateEnum getStateData() = 0;
				virtual const libtender::domMod::tender::TenderTypeEnum	 getType() = 0;
				virtual const basar::Date getDateTo() = 0;

				virtual bool isFilled() = 0;
				virtual bool isCustomerSelected() = 0;

                virtual bool isPartnerInFocus() { return false; };
                virtual bool isPharmacyChainInFocus() { return false; };
                virtual bool isCustomerInFocus() = 0;
                virtual bool isPharmacyGroupInFocus() { return false; };

				virtual bool isContractPeriodValid() = 0;

				virtual void resetAllFields() = 0;
				virtual void resetCustomer() = 0;
                virtual void resetPharmacyGroup() {};
				virtual void prepareForCloning() {};

				virtual void setFocusOnFirstEmptyField() = 0;
				virtual void disableAll() = 0;
				virtual void disableAllExceptState( bool disable, bool hasTenderArticles ) = 0;

            private:
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ITENDERCONTRACTINFO_H
