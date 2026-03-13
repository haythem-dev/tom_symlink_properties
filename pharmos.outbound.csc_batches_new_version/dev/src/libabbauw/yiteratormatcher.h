#ifndef GUARD_LIBABBAUW_UTIL_YITERATORMATCHER_H
#define GUARD_LIBABBAUW_UTIL_YITERATORMATCHER_H

#include <libbasardbaspect_accessorpropertytable_yiterator.h>

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace util
{

    class YIteratorMatcher
    {
    public:
        YIteratorMatcher( basar::db::aspect::AccessorPropertyTable_YIterator source );

        basar::db::aspect::AccessorPropertyTable_YIterator matchTo( basar::db::aspect::AccessorPropertyTable_YIterator destination );

    private:
        // forbidden
        YIteratorMatcher();
        YIteratorMatcher( const YIteratorMatcher& );
        YIteratorMatcher& operator = ( const YIteratorMatcher& );

    private:
        basar::db::aspect::AccessorPropertyTable_YIterator m_Source;
    };

} // end namespace util
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_UTIL_YITERATORMATCHER_H
