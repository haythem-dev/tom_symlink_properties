#ifndef GUARD_TENDER_IMPORT_UC_PTR_H
#define GUARD_TENDER_IMPORT_UC_PTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
namespace useCase
{
class TenderImportUC;
typedef boost::shared_ptr<tender::useCase::TenderImportUC> TenderImportUCPtr;

} // end namespace useCase
} // end namespace tender

#endif // GUARD_TENDER_IMPORT_UC_PTR_H
