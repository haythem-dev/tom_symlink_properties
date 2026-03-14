#include "csc_batches_version.h"

#define PROJECTNAME		"csc_batches_orderproposal"

#ifdef NDEBUG
#define PROJECTCONFIG	"rel"
#else
#define PROJECTCONFIG	"dbg"
#endif

const char versioninfo[] = PROJECTNAME " v " CSC_BATCHES_VERSION " " PROJECTCONFIG;
const char versioninfo_for_what[] = "@(#)" PROJECTNAME " v " CSC_BATCHES_VERSION " " PROJECTCONFIG;

#include <libbasar_pkgversion.h>

const char BASAR_versioninfo[] = PRODUCT_NAME " v " PRODUCT_MAJORNO "."PRODUCT_MINORNO "." PRODUCT_EXTENSION "." PRODUCT_BUILDNO PRODUCT_CONFIG;
const char BASAR_versioninfo_for_what[] = "@(#)" PRODUCT_NAME " v " PRODUCT_MAJORNO "."PRODUCT_MINORNO "." PRODUCT_EXTENSION "." PRODUCT_BUILDNO PRODUCT_CONFIG;
