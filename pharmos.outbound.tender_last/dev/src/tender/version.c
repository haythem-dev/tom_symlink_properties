#include <libbasar_pkgversion.h>
#include "tender_version.h"
#include "tender_definitions.h"

#ifdef NDEBUG
#define PROJECTCONFIG	"rel"
#else
#define PROJECTCONFIG	"dbg"
#endif

const char versioninfo[] = tender_APPLICATION_NAME " v " TENDER_VERSION " " PROJECTCONFIG;

const char BASAR_versioninfo[] = PRODUCT_NAME " v " PRODUCT_MAJORNO "."PRODUCT_MINORNO "." PRODUCT_EXTENSION "." PRODUCT_BUILDNO PRODUCT_CONFIG;
