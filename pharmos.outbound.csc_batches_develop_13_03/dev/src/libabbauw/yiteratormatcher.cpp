#include "yiteratormatcher.h"

#include <libbasarproperty.h>

namespace libabbauw
{
namespace util
{
    YIteratorMatcher::YIteratorMatcher( basar::db::aspect::AccessorPropertyTable_YIterator source )
        : m_Source( source )
    {
    }

    basar::db::aspect::AccessorPropertyTable_YIterator YIteratorMatcher::matchTo( basar::db::aspect::AccessorPropertyTable_YIterator destination )
    {
        destination.match( m_Source );

        // copy states from original object
        for( basar::db::aspect::AccessorPropertyTable_XIterator xit = destination.begin(); ! xit.isEnd(); ++xit )
        {
            if( m_Source.contains( xit.getName() ) )
            {
                const basar::SupportedStateEnum origState = m_Source.getState( xit.getName() ).getState();
                if( origState != basar::SS_CLEAN && origState != basar::SS_UNKNOWN )
                {
                    xit.setPropertyState( origState );
                }
            }
        }

        return destination;
    }

} // end namespace util
} // end namespace libabbauw
