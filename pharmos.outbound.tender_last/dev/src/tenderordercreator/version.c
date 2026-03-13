#include "tender_version.h"

#define PROJECTNAME		"tenderordercreator"

#ifdef NDEBUG
#define PROJECTCONFIG	"rel"
#else
#define PROJECTCONFIG	"dbg"
#endif

const char versioninfo[] = "@(#)" PROJECTNAME " v " TENDER_VERSION " " PROJECTCONFIG;
