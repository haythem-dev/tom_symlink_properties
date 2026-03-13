#ifndef GUARD_TENDER_IMPORT_FILE_READER_PTR_H
#define GUARD_TENDER_IMPORT_FILE_READER_PTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
namespace useCase
{
class TenderImportFileReader;
typedef boost::shared_ptr<tender::useCase::TenderImportFileReader> TenderImportFileReaderPtr;

} // end namespace useCase
} // end namespace tender

#endif // GUARD_TENDER_IMPORT_FILE_READER_PTR_H
