#include "bvostatusdataexportservice.h"

#include "libabbauw_properties_definitions.h"

#include <sstream>

namespace CSC_Batches
{
    namespace services
    {
        BVOStatusDataExportSerivce::~BVOStatusDataExportSerivce()
        {
        }

        BVOStatusDataExportSerivce::BVOStatusDataExportSerivce()
        {
        }

        basar::VarString BVOStatusDataExportSerivce::serializeData( basar::db::aspect::AccessorPropertyTable_YIterator dataYit )
        {
            std::stringstream os;

            char separator = getSeparator();
			separator;

			os << dataYit.getInt32( libabbauw::properties::PURCHASEORDERSUPPLIERNO.getName() ); //Beslanr
            os << separator;
			os << dataYit.getInt32( libabbauw::properties::PHARMACYNO.getName() ); //IDFNr
            os << separator;
			os << dataYit.getString( libabbauw::properties::PZN.getName() ); //Artikelnr
            os << separator;
            os << dataYit.getString( libabbauw::properties::ARTICLENAME.getName() ); //Artikel
            os << separator;
            os << dataYit.getString( libabbauw::properties::UNIT.getName() ); //Einheit
			os << separator;
			os << dataYit.getInt32( libabbauw::properties::ORDERPROPOSALQTY.getName() ); // Menge
			os << separator;

			os << dataYit.getString( libabbauw::properties::DELIVERYNOTE.getName() ); // Lieferinfo
			os << separator;

			if(dataYit.getDate( libabbauw::properties::ORDERDATE.getName() ).isValid())
			{
				os << dataYit.getDate( libabbauw::properties::ORDERDATE.getName() ); // Auftragsdat
				os << separator;
			}
			else
			{
				os << ""; 
				os << separator;
			}

			if(dataYit.getDate( libabbauw::properties::EDIDELIVERYDATE.getName() ).isValid())
			{
				os << dataYit.getDate( libabbauw::properties::EDIDELIVERYDATE.getName() ); // Lieferdat
				os << separator;
			}
			else
			{
				os << ""; 
				os << separator;
			}

			os << dataYit.getString( libabbauw::properties::ORDERTYPE.getName() ); // AART
			os << separator;

			if(dataYit.getDateTime( libabbauw::properties::ORDER_PROP_PROCESSED_TIME.getName() ).isValid())
			{
				os << dataYit.getDateTime( libabbauw::properties::ORDER_PROP_PROCESSED_TIME.getName() ); //BVO erzeugt 
				os << separator;
			}
			else
			{
				os << ""; 
				os << separator;
			}

			os << dataYit.getInt32( libabbauw::properties::DATE_OF_PURCHASE.getName() ); // Bestellung erzeugt 
			os << separator;

			os << dataYit.getInt32( libabbauw::properties::PURCHASEORDERNO.getName() ); //Best.-Nr.
			os << separator;
			os << dataYit.getDate( libabbauw::properties::NEXT_MAIN_ORDER_DATE.getName() ); //nächste Hauptb.
			os << separator;

            os << '\n';

            return os.str();
        }

        basar::VarString BVOStatusDataExportSerivce::getExportTitle()
        {
            return "BWStatus";
        }

        basar::VarString BVOStatusDataExportSerivce::getTitleLine()
        {
            std::stringstream os;

            char separator = getSeparator();

            os << "Beslanr" << separator;
			os << "IDFNr" << separator;
			os << "Artikelnr" << separator;
			os << "Artikel" << separator;
			os << "Einheit" << separator;
			os << "Menge" << separator;
			os << "Lieferinfo" << separator;
			os << "Auftragsdat" << separator;
			os << "Lieferdat" << separator;
			os << "AART" << separator;
			os << "BVO erzeugt" << separator;
			os << "Bestellung erzeugt" << separator;
			os << "Best.-Nr." << separator;
			os << "nächste Hauptb." << separator;

			os << '\n';

            return os.str();
        }
    }
}