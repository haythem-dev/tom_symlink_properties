#include "omgtransferdm.h"
#include "tenderbatchprocessing_property_definitions.h"
#include <loggerpool/loggerpool.h>
#include <infrastructure/remoteprocedure/remoteprocedureacc_definitions.h>
#include <libutil/exception.h>
#include <libutil/util.h>
#include <libutil/accessor.h>

#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include <libbasarcmnutil_exceptions.h>
#include <libbasardbaspect_exceptions.h>
#include <libbasardbsql_exceptions.h>

namespace tenderbatchprocessing 
{
namespace domMod 
{
namespace omgtransfer 
{

using namespace basar::db::aspect;

const libutil::misc::ClassInfo& OMGTransferDM::getClassInfo()
{
    static const libutil::misc::ClassInfo info( "OMGTransferDM", libutil::misc::Version( 1, 0, 0, 0 ) );
    return info;
}

OMGTransferDM::OMGTransferDM( )
: m_Logger( tenderbatchprocessing::LoggerPool::getLoggerDomModules() ),
  m_ReturnCode( 0 ),
  m_PositionsProcessed( 0 )
{
    METHODNAME_DEF( OMGTransferDM, OMGTransferDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

OMGTransferDM::~OMGTransferDM()
{
    METHODNAME_DEF( OMGTransferDM, ~OMGTransferDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

void OMGTransferDM::injectRemoteProcedureAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
    METHODNAME_DEF( OMGTransferDM, injectRemoteProcedureAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_Accessor = accessor;
}

void OMGTransferDM::addPositions( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd )
{
	METHODNAME_DEF( OMGTransferDM, addPositions )
	BLOG_TRACE_METHOD( m_Logger, fun );

	using namespace basar::db::aspect;
	using namespace std;

	basar::Int32 counter = 0;
	std::stringstream posStream;

	for( AccessorPropertyTable_YIterator yit = yitStart; yit != yitEnd; ++yit )
	{
		posToStream( posStream, yit );
		++counter;
	}

	std::stringstream headStream;
	headToStream( headStream, yitStart, counter );

	std::stringstream ss;
	ss << headStream.str() << posStream.str();

BLOG_TRACE( m_Logger, ss.str().c_str() );

	getOMGTransferAccessor()->getPropertyTable().clear();
	AccessorPropertyTable_YIterator yitCICS = getOMGTransferAccessor()->getPropertyTable().insert( basar::FOR_INSERT );
	yitCICS.setString( properties::PROCEDURE_NAME, "PVF410" );
	yitCICS.setString( properties::PROCEDURE_DATA, ss.str().c_str() );
}

void OMGTransferDM::headToStream( std::ostream & str, basar::db::aspect::AccessorPropertyTable_YIterator yit, const basar::Int32 posCounter ) const
{
	using namespace std;
	str << setw( 4 ) << setfill( ' ' ) << left << "2000";		// comm area length
	str << setw( 10 ) << setfill( ' ' ) << left << "Tender";	// application name
	str << setw( 2 ) << setfill( ' ' ) << left << "IN";			// function: IN == Insert, TT == test, display only
	str << setw( 2 ) << setfill( '0' ) << right << yit.getInt16( properties::BRANCH_NO );
	str << setw( 2 ) << setfill( '0' ) << right << "00";		// return code
	str << setw( 4 ) << setfill( '0' ) << right << "0000";		// error number
	str << setw( 2 ) << setfill( '0' ) << right << "00";		// count positions processed
	str << setw( 2 ) << setfill( '0' ) << right << posCounter;	// count positions attached
}

void OMGTransferDM::posToStream( std::ostream & str, basar::db::aspect::AccessorPropertyTable_YIterator yit ) const
{
	using namespace std;
	str << setw( 10 ) << setfill( '0' ) << right << yit.getInt32( properties::TENDER_NO );
	str << setw( 7 ) << setfill( '0' ) << right << yit.getInt32( properties::ARTICLE_NO );
	str << setw( 7 ) << setfill( '0' ) << right << yit.getInt32( properties::CUSTOMER_NO );
	str << setw( 5 ) << setfill( '0' ) << right << yit.getInt32( properties::DELIVERY_QTY );

	basar::VarString price( yit.getDecimal( properties::BASE_PRICE ).toString( 2 ) );
	price.exchange( ".", "" );
	price.exchange( ",", "" );
	str << setw( 7 ) << setfill( '0' ) << right << price.c_str();

	basar::VarString discount( yit.getDecimal( properties::DISCOUNT_PCT ).toString( 2 ) );
	discount.exchange( ".", "" );
	discount.exchange( ",", "" );
	str << setw( 5 ) << setfill( '0' ) << right << discount.c_str();

	str << setw( 5 ) << setfill( '0' ) << right << yit.getInt32( properties::SUPPLIER_NO );
	str << setw( 34 ) << setfill( ' ' ) << left << " ";	// filler
}

void OMGTransferDM::save()
{
	METHODNAME_DEF( OMGTransferDM, save )
	BLOG_TRACE_METHOD( m_Logger, fun );

    using namespace infrastructure::accessor::remoteProcedure::lit;
	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	try
	{
		m_ReturnCode = 0;
		m_PositionsProcessed = 0;
		executeResultInfo = m_Accessor->execute( CALL_REMOTE_PROCEDURE, getOMGTransferAccessor()->getPropertyTable().begin(), true, false );
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, CALL_REMOTE_PROCEDURE, __FILE__, __LINE__ );
	}
	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, CALL_REMOTE_PROCEDURE, executeResultInfo )
	}

	AccessorPropertyTable_YIterator yitResult = getOMGTransferAccessor()->getPropertyTable().begin();
	if( yitResult.isEnd() )
	{
		throw;
	}

	basar::VarString result( yitResult.getString( properties::PROCEDURE_RESULT ).substr( 0, 28 ) );

	/*
fehler:
"2500Tender    IN120300000204"

erfolg:
"2500Tender    IN120000000404"

	str << setw( 4 ) << setfill( ' ' ) << left << "2500";		// comm area length
	str << setw( 10 ) << setfill( ' ' ) << left << "Tender";	// application name
	str << setw( 2 ) << setfill( ' ' ) << left << "IN";			// function: IN == Insert, TT == test, display only
	str << setw( 2 ) << setfill( '0' ) << right << yit.getInt16( properties::BRANCH_NO );
	str << setw( 2 ) << setfill( '0' ) << right << "00";		// return code
	str << setw( 4 ) << setfill( '0' ) << right << "0000";		// error number
	str << setw( 2 ) << setfill( '0' ) << right << "00";		// count positions processed
	str << setw( 2 ) << setfill( '0' ) << right << posCounter;	// count positions attached
	*/

	basar::VarString temp( result.substr( 18, 2 ) );
	m_ReturnCode = temp.stoi();

	temp = result.substr( 24, 2 );
	m_PositionsProcessed = temp.stoi();
}

bool OMGTransferDM::hasError() const
{
	return ( 0 == m_ReturnCode );
}

basar::Int32 OMGTransferDM::getPositionsTransferred() const
{
	return m_PositionsProcessed;
}

basar::Int32 OMGTransferDM::getMaxPositionsCount() const
{
	return 24;
}

libutil::infrastructure::accessor::IAccessorPtr OMGTransferDM::getOMGTransferAccessor() const
{
    CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    return m_Accessor;
}

/*
CALL STP.TCASTP ('PVF410','2500
Stelle	Name	Format	Im Beispiel	Erklärung
1-10	CALLID	Char 10	„-> T E S T“	Hier kann in 10 Stellen das aufrufende Programm eingetragen werden
11-12	FUNCTION	Char 2	IN	Bisher gibt es die Funktion IN für Insert, sowie TT – ein Testaufruf ohne Aktion, nur mit Display
13-14	BRANCHNO	Num 2	12	Die Filialnummer habe ich aus der Artikeltabelle genommen
15-16	RC	Char 2	00	Returncode aus dem Pharmos (Details folgen in einer weiteren Mail)
17-20	DRFEH-K	Char 4	0003	Fehlernummer aus dem Pharmos (Details in einer weiteren Mail)
21-22	PROCESSED	Num2	00	Anzahl der verarbeiteten Positionen (bei Fkt IN Anzahl der geschriebenen)
23-24	INDMAX	Num 2	02	Bitte die Anzahl der gefüllten Positionen mitgeben – max 20

Hier beginnt einer 20er Tabelle mit den Positionen	
25-31	ARTICLENO	Num 7	0021806	PZN – 7 stellig
32-34	EGR	Num 3	040	Einkaufsgruppe
35-42	DATE	JHJJMMTT	20030305	Datum
43-48	TIME	HHMMSS	110353	Uhrzeit
49-55	IDF	Num 7	0001560	IDF
56-62	RENR	Num 7	0150070	Rechnungsnummer
63-67	MGLFG	Num 5	00010	Menge
68-74	GEP	5VK2NK, ohne Komma	0000605	Bitte den Preis ohne Kommastellen übergeben (als 5,23€ als 0000523)
75-79	GEWRAB	3VK2NK, ohne Komma	01666	Ditto
80-84	HNR	Num 5	00007	Herstellernummer
85-93	EINH	Char 9	“   375 ML”	Einheit
94-123	ABEZ	Char 30	“EAU PRECIEUSE DEPENSIER       ”	30 stelliger Text
124-126	DRFO	Char 3	“LOE”	Darreichungsform
127-144	FILLER	Char 18		leer
145-264 	2. Tabellenelement	120	Start: 25+ ((ElementNo -1)*120)	
2424-2500	Rest	Char76		leer


*/

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender
