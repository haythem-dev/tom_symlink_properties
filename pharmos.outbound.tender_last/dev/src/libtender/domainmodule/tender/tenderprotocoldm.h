#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROTOCOLDM_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROTOCOLDM_H

#include "itenderprotocol.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class TenderProtocolDM : public ITenderProtocol
            {
                public:
                    static const libutil::misc::ClassInfo& getClassInfo();

                public:
                    TenderProtocolDM();
                    virtual ~TenderProtocolDM();

                    void                                                        injectTenderProtocolAccessor( libutil::infrastructure::accessor::IAccessorPtr );

                    virtual basar::db::aspect::AccessorPropertyTableRef			get()const;
                    virtual void                                                resetSearchYit();
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
                    virtual bool                                                isEmpty() const;

					virtual void												saveProtocol();

                private:
                    TenderProtocolDM(const TenderProtocolDM& TenderProtocolDM);
                    TenderProtocolDM& operator= (const TenderProtocolDM& TenderProtocolDM);

					bool														isSavable( basar::db::aspect::AccessorPropertyTable_YIterator ) const;
					bool														hasSavableStates( basar::db::aspect::AccessorPropertyTable_YIterator ) const;
					void														checkNumberHeader() const;

                    libutil::domMod::SearchYIteratorPtr                         getSearchYIterator() const;
                    libutil::infrastructure::accessor::IAccessorPtr             getTenderProtocolAccessor() const;

                    mutable libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
                    libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
            };

        } // end namespace tender
    }	  // end namespace domMod
}	 	  // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROTOCOLDM_H
