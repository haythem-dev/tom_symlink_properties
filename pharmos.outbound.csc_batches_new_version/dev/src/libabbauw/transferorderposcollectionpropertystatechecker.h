#ifndef GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDERPOSCOLLECTIONPROPERTYSTATECHECKER_H
#define GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDERPOSCOLLECTIONPROPERTYSTATECHECKER_H

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
            class PosCollectionPropertyStateChecker
            {
                public:
                    PosCollectionPropertyStateChecker( basar::db::aspect::AccessorPropertyTable_YIterator );
                    ~PosCollectionPropertyStateChecker();

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
                    PosCollectionPropertyStateChecker( const PosCollectionPropertyStateChecker& );
                    PosCollectionPropertyStateChecker& operator=( const PosCollectionPropertyStateChecker& );

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

#endif //GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDERPROPERTYSTATECHECKER_H
