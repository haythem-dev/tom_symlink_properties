#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERCOLLECTIONDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "itendercollectiondm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            //-------------------------------------------------------------------------------------------------//
            // class declaration section
            //-------------------------------------------------------------------------------------------------//
            class TenderCollectionDM : public ITenderCollectionDM
            {
                //--------------------------------------------------------------------------------------------------//
                // static declaration section
                //--------------------------------------------------------------------------------------------------//
                public:
                    static const libutil::misc::ClassInfo& getClassInfo();

                //--------------------------------------------------------------------------------------------------//
                // object declaration section
                //--------------------------------------------------------------------------------------------------//
                public:
                    TenderCollectionDM();
                    virtual ~TenderCollectionDM();

                    void                                                        injectTenderAccessor( libutil::infrastructure::accessor::IAccessorPtr );
                    void                                                        injectTenderFactory ( libtender::componentManager::ITenderConnectedFactoryPtr );

                    virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                findByEndDate(basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                findSuccessorTender(basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual void                                                findValidTenders( basar::db::aspect::AccessorPropertyTable_YIterator );
					virtual void												findExpiredTenders( basar::db::aspect::AccessorPropertyTable_YIterator );
                    virtual basar::db::aspect::AccessorPropertyTableRef         get()const;
                    virtual void                                                resetSearchYit();
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
                    virtual ITenderPtr                                          getTender( const basar::db::aspect::AccessorPropertyTable_YIterator yitTender ) const;
                    virtual bool                                                isEmpty() const;

                private:
                    /////////////////////////////////////////////////////////////////////////
                    // private member methods
                    /////////////////////////////////////////////////////////////////////////
                    TenderCollectionDM(const TenderCollectionDM& TenderCollectionDM);
                    TenderCollectionDM& operator= (const TenderCollectionDM& TenderCollectionDM);

                    libutil::domMod::SearchYIteratorPtr                         getSearchYIterator() const;
                    libutil::infrastructure::accessor::IAccessorPtr             getTenderAccessor() const;
                    libtender::componentManager::ITenderConnectedFactoryPtr     getTenderConnectedFactory() const;


                    /////////////////////////////////////////////////////////////////////////
                    // private member variables
                    /////////////////////////////////////////////////////////////////////////
                    mutable libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
                    libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
                    libtender::componentManager::ITenderConnectedFactoryPtr     m_TenderConnectedFactory;
            };
        } // namespace tender
    } // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERCOLLECTIONDM_H
