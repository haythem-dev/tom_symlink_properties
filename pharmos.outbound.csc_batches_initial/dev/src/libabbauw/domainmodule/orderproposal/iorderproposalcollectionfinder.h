#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONFINDER_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONFINDER_H

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTableRef;
            class AccessorPropertyTable_YIterator;
        }
    }
}

//----------------------------------------------------------------------------//
// interface declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class IOrderProposalCollectionFinder
    {
        public:
            virtual ~IOrderProposalCollectionFinder() {}

			virtual basar::db::aspect::AccessorPropertyTableRef        get()                                                                 = 0;

			virtual void                                               resetSearchYit()                                                      = 0;
			virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit()                                                        = 0;

			virtual void                                               findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    };

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONFINDER_H
