#ifndef GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDER_TRANSFERORDERHEADPROPERTYSTATECHECKER_H
#define GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDER_TRANSFERORDERHEADPROPERTYSTATECHECKER_H

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
            class HeadInPreparationPropertyStateChecker
            {
                public:
                    HeadInPreparationPropertyStateChecker( basar::db::aspect::AccessorPropertyTable_YIterator );
                    ~HeadInPreparationPropertyStateChecker();

                    virtual bool                                        isValid()      const;
                    virtual bool                                        isSelectable() const;
                    virtual bool                                        isInsertable() const;
                    virtual bool                                        isUpdatable()  const;
                    virtual bool                                        isDeletable()  const;
                    virtual bool                                        isSavable()    const;
                    virtual bool                                        isComplete()   const;

                ////////////////////////////////////////////
                // private member methods
                ////////////////////////////////////////////
                private:
                    HeadInPreparationPropertyStateChecker( const HeadInPreparationPropertyStateChecker& );
                    HeadInPreparationPropertyStateChecker& operator=( const HeadInPreparationPropertyStateChecker& );

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

#endif //GUARD_LIBABBAUW_ACCESSOR_TRANSFERORDER_TRANSFERORDERHEADPROPERTYSTATECHECKER_H
