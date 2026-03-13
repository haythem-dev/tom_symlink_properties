#ifndef GUARD_PARAMETER_ACC_H
#define GUARD_PARAMETER_ACC_H

namespace libabbauw
{
namespace acc_parameter
{
using basar::db::aspect::SQLStringBuilder;

BEGIN_QUERY_BUILDER_DECLARATION( SelectParameter )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertParameterValue )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertParameterManifestation )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdateParameter )
END_BUILDER_DECLARATION

} // end namespace acc_parameter
} // end namespace libabbauw

#endif // GUARD_PARAMETER_ACC_H
