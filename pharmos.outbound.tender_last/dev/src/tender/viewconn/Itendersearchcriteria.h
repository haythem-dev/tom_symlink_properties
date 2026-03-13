#ifndef GUARD_TENDER_VIEWCONN_ITENDERSEARCHCRITERIA_H
#define GUARD_TENDER_VIEWCONN_ITENDERSEARCHCRITERIA_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QtWidgets/QGroupBox>
#pragma warning (pop)

#include <libtender/domainmodule/tender/tenderdm.h>
#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>
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
        class ITenderSearchCriteria: public QGroupBox
        {
            public:
                virtual ~ITenderSearchCriteria(){};

                virtual void matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch ) = 0;
                virtual void prepareForNewSearch() = 0;

                virtual void injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping ) = 0;
                virtual void injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping  ) {};
                
                virtual void setPharmacyGroupId ( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};
                virtual void setArticleCode		( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;
                virtual void setCustomerNo		( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;
                virtual void setPartnerNo		( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};
                virtual void setPharmacyChainNo ( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};
                virtual void setSupplierNo      ( basar::db::aspect::AccessorPropertyTable_YIterator yit ){}
                
                virtual const libtender::domMod::tender::TenderTypeCollectionPtr getTypeMapping() = 0;
                virtual const libtender::domMod::tender::TenderStateCollectionPtr getStatusMapping() = 0;

            private:
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ITENDERSEARCHCRITERIA_H
