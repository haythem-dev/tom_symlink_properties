#ifndef GUARD_DOMMOD_I_TENDER_IMPORT_COLLECTION_PTR_H
#define GUARD_DOMMOD_I_TENDER_IMPORT_COLLECTION_PTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace tender
{
class ITenderImportCollection;
typedef boost::shared_ptr<libtender::domMod::tender::ITenderImportCollection> ITenderImportCollectionPtr;

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_DOMMOD_I_TENDER_IMPORT_COLLECTION_PTR_H
