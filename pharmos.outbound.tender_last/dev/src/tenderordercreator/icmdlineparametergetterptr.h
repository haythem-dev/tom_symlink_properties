#ifndef GUARD_ICMDLINEPARAMETERGETTERPTR_H
#define GUARD_ICMDLINEPARAMETERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderordercreator
{
    namespace infrastructure
    {
        class ICmdLineParameterGetter;

        typedef boost::shared_ptr<ICmdLineParameterGetter> ICmdLineParameterGetterPtr;
    }
}

#endif
