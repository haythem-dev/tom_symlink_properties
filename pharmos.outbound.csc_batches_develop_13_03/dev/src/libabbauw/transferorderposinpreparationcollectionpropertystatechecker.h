#ifndef GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONCOLLECTIONPROPERTYSTATECHECKER_H
#define GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONCOLLECTIONPROPERTYSTATECHECKER_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
    namespace accessor
    {
        namespace transferOrder
        {
            class PosInPreparationCollectionPropertyStateChecker
            {
                public:
                    PosInPreparationCollectionPropertyStateChecker( basar::db::aspect::AccessorPropertyTable_YIterator );
                    ~PosInPreparationCollectionPropertyStateChecker();

                    virtual bool                                        isValid()          const;
                    virtual bool                                        isSelectable()     const;
                    virtual bool                                        isInsertable()     const;
                    virtual bool                                        isUpdatable()      const;
                    virtual bool                                        isDeletable()      const;
                    virtual bool                                        isSavable()        const;
                    virtual bool                                        isComplete()       const;

                ////////////////////////////////////////////
                // private member methods
                ////////////////////////////////////////////
                private:
                    PosInPreparationCollectionPropertyStateChecker( const PosInPreparationCollectionPropertyStateChecker& );
                    PosInPreparationCollectionPropertyStateChecker& operator=( const PosInPreparationCollectionPropertyStateChecker& );

                    bool                                                isClean() const;
                    const log4cplus::Logger&                            getLogger() const;

                ////////////////////////////////////////////
                // private member variables
                ////////////////////////////////////////////
                private:
                    const log4cplus::Logger&                            m_Logger;
                    basar::db::aspect::AccessorPropertyTable_YIterator  m_Yit;
            };
        }
    }
}   // end namespace libabbauw

#endif //GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONCOLLECTIONPROPERTYSTATECHECKER_H
