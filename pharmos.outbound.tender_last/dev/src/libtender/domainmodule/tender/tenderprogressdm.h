#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROGRESSDM_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROGRESSDM_H

#include "itenderprogress.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class TenderProgressDM : public ITenderProgress
            {
                public:
                    static const libutil::misc::ClassInfo& getClassInfo();

                public:
                    TenderProgressDM();
                    virtual ~TenderProgressDM();

                    void                                                        injectTenderProgressAccessor( libutil::infrastructure::accessor::IAccessorPtr );

                    virtual void                                                findArticleQuantityByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                findByTender( basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                findTenderProgressForOMGCalcultion( basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                save( basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual basar::db::aspect::AccessorPropertyTableRef         get()const;
                    virtual void                                                resetSearchYit();
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
                    virtual bool                                                isEmpty() const;

                private:
                    TenderProgressDM(const TenderProgressDM& TenderProgressDM);
                    TenderProgressDM& operator= (const TenderProgressDM& TenderProgressDM);

                    libutil::domMod::SearchYIteratorPtr                         getSearchYIterator() const;
                    libutil::infrastructure::accessor::IAccessorPtr             getTenderProgressAccessor() const;

                    mutable libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
                    libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
            };

        } // end namespace tender
    }	  // end namespace domMod
}	 	  // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROGRESSDM_H
