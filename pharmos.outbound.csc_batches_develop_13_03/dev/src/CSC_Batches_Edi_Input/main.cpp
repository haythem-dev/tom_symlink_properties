//----------------------------------------------------------------------------
/*! \file   
 *  \brief  main entrance
 *  \author Bischof Bjoern
 *  \date   07.03.2013
 */
//----------------------------------------------------------------------------
#include "abbauwappl.h"
#include "loggerpool.h"

#include <time.h>

/*
#ifndef WIN32
#include "gitversioninfo.h"
#endif
*/
extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}

// Only for Testing
/*
#include "edifactline.h"
#include "unbline.h"
#include "unhline.h"
#include "bgmline.h"
#include "dtmline.h"
#include "ediline_definitions.h"
#include "nadline.h"
#include "unsline.h"
#include "untline.h"
#include "unzline.h"
#include "linline.h"
#include "imdline.h"
#include "qtyline.h"
#include "pcdline.h"
#include "irrelevantline.h"

#include "unb_00_line.h"
#include "unh_01_line.h"
#include "bgm_02_line.h"
#include "dtm_03_line.h"
#include "nad_04_line.h"
#include "alc_05_line.h"
#include "pcd_06_line.h"
#include "lin_08_line.h"
#include "imd_09_line.h"
#include "qty_10_line.h"
#include "ftx_11_line.h"
#include "uns_97_line.h"
#include "irrelevant_de_line.h"

//#include "linetypelist.h"

// Forwards
//void testUNBLineclass();
//void testUNHLineclass();
//void testBGMLineclass();
//void testDTMLineclass();
//void testNADLineclass();
//void testUNSLineclass();
//void testUNTLineclass();
//void testUNZLineclass();
//void testLINLineclass();
//void testIMDLineclass();
//void testQTYLineclass();
//void testPCDLineclass();
//void testIRRLineclass();

void testUNB_00_Lineclass();
void testUNH_01_Lineclass();
void testBGM_02_Lineclass();
void testDTM_03_Lineclass();
void testNAD_04_Lineclass();
void testALC_05_Lineclass();
void testPCD_06_Lineclass();
void testLIN_08_Lineclass();
void testIMD_09_Lineclass();
void testQTY_10_Lineclass();
void testFTX_11_Lineclass();
void testUNS_97_Lineclass();
void testIRR_DE_Lineclass();
*/
// End only for testing

//----------------------------------------------------------------------------
// start-up of ABBAUW-CSC_Batches_Edi_Input.exe
// 
// Arguments with example: -countrycode CH -transferdir /transfer/ksc/data/ch/edi/uw -inputdir ../data/edi_input -forbranchnos 06,08,17
//----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	basar::cmnutil::CmdLineTokenizer cmdline;

	cmdline.setCmdLine(argc, argv);

	if (cmdline.isParam("-version"))
	{
		std::cout << versioninfo << std::endl;
		std::cout << BASAR_versioninfo << std::endl;
        /*
#ifndef WIN32
		std::cout << GitVersionInfo::instance() << std::endl;
#endif
        */
		exit( 0 );
	}
	if (cmdline.isParam("-cscinfo"))
	{
		std::cout << versioninfo << std::endl;
		std::cout << BASAR_versioninfo << std::endl;
		/*
#ifndef WIN32
		std::cout << GitVersionInfo::instance() << std::endl;
#endif
		*/
		exit(0);
	}
	int retvalue = 0;		// return = 0: everything ok 
							// return = 1: some error(s) occurred, couldn't handle one or more flatfiles				

	// TESTEST Beginn  (should be implemented as unit tests)
	//testUNH_01_Lineclass();
	//testUNBLineclass();
	//testUNHLineclass();
	//testBGMLineclass();
	//testDTMLineclass();
	//testNADLineclass();
	//testUNSLineclass();
	//testUNTLineclass();
	//testUNZLineclass();
	//testLINLineclass();
	//testIMDLineclass();
	//testQTYLineclass();
	//testPCDLineclass();
	//testIRRLineclass();
	//testUNB_00_Lineclass();
	//testUNH_01_Lineclass();
	//testBGM_02_Lineclass();
	//testLIN_08_Lineclass();
	//testDTM_03_Lineclass();
	//testNAD_04_Lineclass();
	//testALC_05_Lineclass();
	//testPCD_06_Lineclass();
	//testIMD_09_Lineclass(); 
	//testQTY_10_Lineclass();	
	//testFTX_11_Lineclass();
	//testUNS_97_Lineclass();
	//testIRR_DE_Lineclass();
	//return retvalue;
	// TESTEST Ende

	std::string msg = "Version: ";
	msg += versioninfo;
	msg += "Basarversion: ";
	msg += BASAR_versioninfo;

	BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, msg);

    try 
	{
        abbaUW::ABBAUWAppl::getInstance().init( argc, argv );
		
		if ( !abbaUW::ABBAUWAppl::getInstance().run())
		{
			retvalue = 1;
		}

        abbaUW::ABBAUWAppl::getInstance().shutdown();  
        abbaUW::ABBAUWAppl::clear();  
    } 
	catch( ... )
	{
		retvalue = 1;
    }

    return retvalue;
}	

/*
void testUNB_00_Lineclass()
{
	domMod::line::Unb_00_Line line;
	bool ret = false;

	try {

	// Valid UNB_00-Line
	ret = false;
	line.process("00UNOB203721440                           PH                76539419                           PH                14070906282090141");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid UNB_00-Line
	ret = false;
	line.process("00UNOC207141006                           PH                76539419                           PH                14070907310709072111");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid UNB_00-Line from HAGEDA mit TEST
	ret = false;
	line.process("00UNOB203721440                           PH                76539419                           PH                14070906212090126                                                                       _.,1yxc           ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid UNB_00-Line
	ret = false;
	line.process("00UNOB244242992                           PH                76539419                           PH                1407090817");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Wrong UNB-TYP
	ret = false;
	line.process("UNB400856148    UEWABFS  93AUNPHOUEW                                          ");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Wrong SYNTAX
	ret = false;
	line.process("00UNIX44242992                           PH                76539419                           PH                1407090817");
	ret = line.isValidLine(); 
	assert(ret == false);

	// UNB-Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("1");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}

void testUNH_01_Lineclass()
{
	domMod::line::Unh_01_Line line;
	bool ret = false;

	try {

	// Valid UNH_01-Line
	ret = false;
	line.process("010400856148    UEWAUFS  93AUNPHOUEW                                          ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid UNH_01-Line
	ret = false;
	line.process("012090272       UEWAUF  S93A");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Wrong UNH-TYP
	ret = false;
	line.process("010400856148    UEWABFS  93AUNPHOUEW                                          ");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Wrong UNH-VERSION
	ret = false;
	line.process("010400856148    UEWAUFZ  93AUNPHOUEW                                          ");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Wrong UNH-FREIGABE
	ret = false;
	line.process("010400856148    UEWAUFS  94AUNPHOUEW                                          ");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Not UNH-Line (exception)
	//ret = true;
	//unhline.process("UNI");
	//ret = unhline.isValidLine(); 
	//assert(ret == false);

	// UNH-Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("1");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}

void testBGM_02_Lineclass()
{
	domMod::line::Bgm_02_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("02640            blabla                       4902482101   ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("02640");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Wrong DOKCODE
	ret = false;
	line.process("02644                                         4902482101");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Not BGM-Line (exception)
	ret = true;
	line.process("BGM640");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}


void testDTM_03_Lineclass()
{
	domMod::line::Dtm_03_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("0313720140709                           102");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("032  20140709                           102                                                                                                                                                                              Brücken-Apotheke                   Frau Möller                                                                                                                                    Brückenstr. 12a                                                                             Rüdersdorf                                  15562    DE");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("0320920140709");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("03ZZZ");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Wrong DATCODE
	ret = false;
	line.process("0377720140709");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Not DTM-Line (exception)
	ret = true;
	line.process("0720920140709");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}



void testNAD_04_Lineclass()
{
	domMod::line::Nad_04_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("04MF 68758258         BFA");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("04BY 3372096          BGA                                                                                                                                                                                  Brücken-Apotheke                   Frau Möller                                                                                                                                    Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE");
	ret = line.isValidLine(); 
	assert(ret == true);

		// Valid Line
	ret = false;
	line.process("04DS 3003016          BGA                                                                                                                                                                                  PHOENIX Pharmahandel GmbH & Co. KG Vertriebszentrum Berlin                                                                                                                        Lengeder Str. 42                                                                                         Berlin                                      13407    DE");
	ret = line.isValidLine(); 
	assert(ret == true);

		// Valid Line
	ret = false;
	line.process("04CN 3372096          BGA                                                                                                                                                                                  Brücken-Apotheke                   Frau Möller                                                                                                                                    Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Wrong CODELIST
	ret = false;
	line.process("04CN 3372096          BF                                                                                                                                                                                  Brücken-Apotheke                   Frau Möller                                                                                                                                    Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Not NAD-Line (exception)
	ret = true;
	line.process("1235647654765                                       jhvljvfljgljflghfljhgfljhfgljhgfljhgfljhgljhgghgljhglglj");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}



void testALC_05_Lineclass()
{
	domMod::line::Alc_05_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("05H");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("05K");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("05F");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("05M");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Invalid Line
	ret = false;
	line.process("05X");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line
	ret = false;
	line.process("05");
	ret = line.isValidLine(); 
	assert(ret == false);



	// Not ALC-Line (exception)
	ret = true;
	line.process("1235647654765                                       jhvljvfljgljflghfljhgfljhfgljhgfljhgfljhgljhgghgljhglglj");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}


void testPCD_06_Lineclass()
{
	domMod::line::Pcd_06_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("061        25");
	ret = line.isValidLine(); 
	assert(ret == true);

	ret = false;
	line.process("06  2      30      x");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("061      2.22                                                                                                                                                                        Brücken-Apotheke                   Frau Möller                                                                                                                                    Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("06 1 123456.8   COD      blablablab                                                                                                                                                                        Brücken-Apotheke                   Frau Möller                                                                                                                                    Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("06                                                                                                                                                                                PHOENIX Pharmahandel GmbH & Co. KG Vertriebszentrum Berlin                                                                                                                        Lengeder Str. 42                                                                                         Berlin                                      13407    DE");
	ret = line.isValidLine(); 
	assert(ret == true);

		// Valid Line
	ret = false;
	line.process("06");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Wrong Percentvalue
	ret = false;
	line.process("062   A    25");	
	ret = line.isValidLine(); 
	assert(ret == false);


	// Not PCD-Line (exception)
	ret = true;
	line.process("1235647654765                                       jhvljvfljgljflghfljhgfljhfgljhgfljhgfljhgljhgghgljhglglj");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);
	}

	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}


void testLIN_08_Lineclass()
{
	domMod::line::Lin_08_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("08000001   04704206                           P");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("08000010   06312077                           P  ZZZ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("08");
	ret = line.isValidLine(); 
	assert(ret == true);


	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Not LIN-Line (exception)
	ret = true;
	line.process("LIN876");
	ret = line.isValidLine(); 
	assert(ret == false);	
	
	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}
	

void testIMD_09_Lineclass()
{
	domMod::line::Imd_09_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("09baölblablajhzghgfjhjgfd   ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("09F                  Venostasin Gel 0.01 1x100");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line
	ret = false;
	line.process("09");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("0");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Not IMD-Line (exception)
	ret = true;
	line.process("BGM640");
	ret = line.isValidLine(); 
	assert(ret == false);	
	
	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}
	
void testQTY_10_Lineclass()
{
	domMod::line::Qty_10_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("1021 000000000000010");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line  NR
	ret = false;
	line.process("10130000000000000003");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Invalid Line
	ret = false;
	line.process("1021 ");
	ret = line.isValidLine(); 
	assert(ret == true);


	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("1");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Not QTY-Line (exception)
	ret = true;
	line.process("QTY876");
	ret = line.isValidLine(); 
	assert(ret == false);	
	
	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}

void testFTX_11_Lineclass()
{
	domMod::line::Ftx_11_Line line;
	bool ret = false;

	try {

	// Valid Line
	ret = false;
	line.process("11AAQ.1.2.3.4.5.6FREIER FREITEXThgfjgfjgfjgfjgfdjgfdjfgd gggggggggggggggggggggggggggggk............");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line  
	ret = false;
	line.process("11AAQ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Invalid Line
	ret = false;
	line.process("11BBQ            Freier Text aber kein Headertext  ");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("1");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Not FTX-Line (exception)
	ret = true;
	line.process("17AAQ");
	ret = line.isValidLine(); 
	assert(ret == false);	
	
	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}
	
	

void testUNS_97_Lineclass()
{
	domMod::line::Uns_97_Line line;
	bool ret = false;

	try {

	// Valid UNS_97-Line
	ret = false;
	line.process("97S      blabla                                   ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid UNS_97-Line
	ret = false;
	line.process("97D");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Wrong UNS-Qualifier (exception)
	ret = false;
	line.process("07P ");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Wrong UNS-number (exception)
	ret = false;
	line.process("98D ");
	ret = line.isValidLine(); 
	assert(ret == false);

	
	// UNS-Line too short (exception)
	ret = true;
	line.process("97");
	ret = line.isValidLine(); 
	assert(ret == false);

	}



	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}
  


void testIRR_DE_Lineclass()
{
	domMod::line::IrrLine_DE line;
	bool ret = false;


	try {

		// Valid Line Normalausstattung
		ret = false;
		line.process("98UNT            A12807");
		ret = line.isValidLine(); 
		assert(ret == true);

		// Valid Line Normalausstattung
		ret = false;
		line.process("99UNZ            A12807");
		ret = line.isValidLine(); 
		assert(ret == true);


		// Valid Line Normalausstattung
		ret = false;
		line.process("07204          77          ");
		ret = line.isValidLine(); 
		assert(ret == true);


		// Valid Line Normalausstattung
		ret = false;
		line.process("12AEP       77,77          ");
		ret = line.isValidLine(); 
		assert(ret == true);


		try {
			// Invalid Line : Valid line 11 FTX
			ret = true;
			line.process("11DS 3002809          BGAZZZ                                                   ");
			ret = line.isValidLine(); 
			assert(ret == false);
		} 
		catch (...) {		
			int i=99;i;
		}



		try {
			// Invalid Line : Unknown intentifier
			ret = true;
			line.process("04DS 3002809          BGAZZZ                                                   ");
			ret = line.isValidLine(); 
			assert(ret == false);
		} 
		catch (...) {		
			int i=99;i;
		}



		try {
			// Invalid Line : Line too short and even shorter than line identifier (exception)
			ret = true;
			line.process("0");
			ret = line.isValidLine(); 
			assert(ret == false);
		} 
		catch (...) {		
			int i=99;i;
		}



	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}


//   Tests of EDIfact-line interpreter
void testUNBLineclass()
{
	domMod::line::UnbLine unbline;
	bool ret = false;

	try {

	// Valid UNB-Line
	ret = false;
	unbline.process("UNB7601001002470 7601001358034 UEWAUF");
	ret = unbline.isValidLine(); 
	assert(ret == true);

	// Only Test-UNB
	ret = true;
	unbline.process("UNB7601001002470 7601001358034 UEWAUF1");
	ret = unbline.isValidLine(); 
	assert(ret == false);

	// No UEWAUF-fixed text
	ret = true;
	unbline.process("UNB7601001002470 7601001358034 ABCDEF");
	ret = unbline.isValidLine(); 
	assert(ret == false);

	// Not for AMEDIS
	ret = true;
	unbline.process("UNB7601001002470 7601001358777 UEWAUF");
	ret = unbline.isValidLine(); 
	assert(ret == false);


	// Not UNB-Line (exception)
	//ret = true;
	//unbline.process("XNB7601001002470 7601001358034 UEWAUF");
	//ret = unbline.isValidLine(); 
	//assert(ret == false);

	// UNB-Line too short(exception)
	//ret = true;
	//unbline.process("UNB7601001002470 76358034 UEWAUF");
	//ret = unbline.isValidLine(); 
	//assert(ret == false);

	// UNB-Line exception: Shorter than line identifier (exception)
	//ret = true;
	//unbline.process("UN");
	//ret = unbline.isValidLine(); 
	//assert(ret == false);


	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		
	}
	catch (	...)
	{
		int i=99;i;

		
	}
	return;
}



void testUNHLineclass()
{
	domMod::line::UnhLine unhline;
	bool ret = false;

	try {

	// Valid UNH-Line
	ret = false;
	unhline.process("UNH");
	ret = unhline.isValidLine(); 
	assert(ret == true);

	// Not UNH-Line (exception)
	//ret = true;
	//unhline.process("UNI");
	//ret = unhline.isValidLine(); 
	//assert(ret == false);

	// UNH-Line too short and even shorter than line identifier (exception)
	ret = true;
	unhline.process("UN");
	ret = unhline.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;


	}
	catch (	...)
	{
		int i=99;i;


	}
	return;
}


void testBGMLineclass()
{
	domMod::line::BgmLine line;
	bool ret = false;

	try {

	// Valid BGM-Line
	ret = false;
	line.process("BGM64070231");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid BGM-Line long form with filler 
	ret = false;
	line.process("BGM64012345678901234567890123456789012345xxx____________________________________");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Not BGM-Line (exception)
	//ret = true;
	//line.process("BBB");
	//ret = line.isValidLine(); 
	//assert(ret == false);

	// BGM-Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("BG");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

	}
	catch (	...)
	{
		int i=99;i;


	}
	return;
}

void testDTMLineclass()
{
	domMod::line::DtmLine line;
	bool ret = false;

	try {

	// Valid DTM-Line 137
	ret = false;
	line.process("DTM13720120709102");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line long form with filler 2
	ret = false;
	line.process("DTM2  20120809102                                                               ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line long form with filler 209 and without 102
	ret = false;
	line.process("DTM4  20120817                                                                  ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Line long form with aphanumeric Date (exception)
	ret = true;
	line.process("DTM4  2012A817");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Line long form with invalid Date (exception)
	ret = true;
	line.process("DTM4  2012172017102");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Not DTM-Line (exception)
	//ret = true;
	//line.process("BBB");
	//ret = line.isValidLine(); 
	//assert(ret == false);

	// BGM-Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("BG");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;


	}
	catch (	...)
	{
		int i=99;i;
	}
	return;
}

void testNADLineclass()
{
	domMod::line::NadLine line;
	bool ret = false;


	try {

	// Valid NAD-Line MF w/o 104
	ret = false;
	line.process("NADMF 7601001002470    ZELLER MAX SOHNE AG");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid NAD-Line DS 104
	ret = false;
	line.process("NADDS 7601001000209 104Amedis-UE SA");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid NAD-Line BY 
	ret = false;
	line.process("NADBY 7601001024588 104Pharmacie de Troistorrents");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Invalid Line : Wrong Role
	ret = true;
	line.process("NADSE 7601001000209 104Amedis-UE SA");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Wrong Codepfl
	ret = true;
	line.process("NADDS 7601001000209 10 Amedis-UE SA");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : EAN missing?????
	ret = true;
	line.process("NADDS               10 Amedis-UE SA");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Not NAD-Line (exception)
	ret = true;
	line.process("XXXDS               10 Amedis-UE SA");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("NA");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;


	}
	catch (	...)
	{
		int i=99;i;


	}
	return;
}

void testUNSLineclass()
{
	domMod::line::UnsLine line;
	bool ret = false;


	try {

	// Valid UNS-Line Qualifier=D
	ret = false;
	line.process("UNSD");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid UNS-Line Qualifier=S
	ret = false;
	line.process("UNSS                                                                            ");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Invalid Line : Wrong Qualifier
	ret = true;
	line.process("UNSK");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Invalid Line : Not NAD-Line (exception)
	ret = true;
	line.process("XXXD");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("UN");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;


	}
	catch (	...)
	{
		int i=99;i;


	}
	return;
}

void testUNTLineclass()
{
	domMod::line::UntLine line;
	bool ret = false;


	try {

	// Valid UNT-Line 
	ret = false;
	line.process("UNT");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Invalid Line : Not UNT-Line (exception)
	ret = true;
	line.process("XXX");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("UN");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;


	}
	catch (	...)
	{
		int i=99;i;


	}
	return;
}

void testUNZLineclass()
{
	domMod::line::UnzLine line;
	bool ret = false;


	try {

	// Valid UNZ-Line 
	ret = false;
	line.process("UNZ");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Invalid Line : Not UNZ-Line (exception)
	ret = true;
	line.process("XXX");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("UN");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;


	}
	catch (	...)
	{
		int i=99;i;


	}
	return;
}

void testLINLineclass()
{
	domMod::line::LinLine line;
	bool ret = false;


	try {

	// Valid LIN-Line ohne articlenoCode 
	ret = false;
	line.process("LIN0000017680307240113 ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid LIN-Line with articlenoCode "EN"
	ret = false;
	line.process("LIN0000027680307240380 EN ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid LIN-Line with articlenoCode "MF"
	ret = false;
	line.process("LIN000003Manufact  4711MF ");
	ret = line.isValidLine(); 
	assert(ret == true);


	// Valid LIN-Line without posnr 
	ret = false;
	line.process("LIN      7680307240380 EN ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid LIN-Line without posnr, articleno and articlenoCode 
	ret = false;
	line.process("LIN                                               ");
	ret = line.isValidLine(); 
	assert(ret == true);


	// Invalid LIN-Line with alphanumeric posnr 
	ret = false;
	line.process("LIN000A047680307240380 EN ");
	ret = line.isValidLine(); 
	assert(ret == true);


	// Invalid Line : Invalid articlenoCode
	ret = true;
	line.process("LIN000004Ma    ct  4711MFD");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Invalid Line : Not LIN-Line (exception)
	ret = true;
	line.process("XXX");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("UN");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}

void testIMDLineclass()
{
	domMod::line::ImdLine line;
	bool ret = false;


	try {

	// Valid IMD-Line Vollausstattung
	ret = false;
	line.process("IMDPHARMA GINVAPAST                          ZAHNFLEISCHPASTE, 10 G             ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid line without optional parts
	ret = false;
	line.process("IMDPHARMA ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid line short form without second optional part
	// Watch out: Debugger shows uninitialized content in private memberstrings after resetMembers(). Only debugger-problem!!???
	ret = false;
	line.process("IMDPHARMA TEBODONT SPRAY");
	ret = line.isValidLine(); 
	assert(ret == true);



	// Invalid Line : Invalid Wabesch
	ret = true;
	line.process("IMDBEARINGHCV1721 Huelle-Kaefig Rollenlager           ");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Invalid Wabesch: Too short
	ret = true;
	line.process("IMDPHARMA");
	ret = line.isValidLine(); 
	assert(ret == false);

	// Invalid Line : Line too short and even shorter than line identifier (exception)
	ret = true;
	line.process("IM");
	ret = line.isValidLine(); 
	assert(ret == false);


	// Invalid Line : Not IMD-Line (exception)
	ret = true;
	line.process("XXX");
	ret = line.isValidLine(); 
	assert(ret == false);

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}

void testQTYLineclass()
{
	domMod::line::QtyLine line;
	bool ret = false;


	try {

	// Valid Line Vollausstattung, Typ 21
	ret = false;
	line.process("QTY21 00010PCE                                                                  ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line Vollausstattung, Typ 47
	ret = false;
	line.process("QTY47 00005PCE                                                                  ");
	ret = line.isValidLine(); 
	assert(ret == true);


	// Valid line without optional parts
	ret = false;
	line.process("QTY21 00005");
	ret = line.isValidLine(); 
	assert(ret == true);


	try {
		// Invalid Line : Invalid Qualifier
		ret = true;
		line.process("QTY77 00030PCE                                                                  ");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}

	try {
		// Invalid Line : Invalid Unit
		ret = true;
		line.process("QTY21 00030Coils                                                                  ");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}


	try {
		// Invalid Line : Invalid Quantity not alphanumeric
		ret = true;
		line.process("QTY47 0A030PCE                                                                  ");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}

	try {
		// Invalid Line : Invalid Quantity negative
		ret = true;
		line.process("QTY47 00-30PCE                                                                  ");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}

	try {
		// Invalid Line : Line too short and even shorter than line identifier (exception)
		ret = true;
		line.process("QT");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}


	try {
		// Invalid Line : Not IMD-Line (exception)
		ret = true;
		line.process("XXX");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}


void testPCDLineclass()
{
	domMod::line::PcdLine line;
	bool ret = false;


	try {

	// Valid Line Normalausstattung
	ret = false;
	line.process("PCD12 0000023.45                                                                ");
	ret = line.isValidLine(); 
	assert(ret == true);

	// Valid Line Vollausstattung, decrease
	ret = false;
	line.process("PCD12 0000078.981                                                               ");
	ret = line.isValidLine(); 
	assert(ret == true);


	try {
		// Invalid Line : Invalid percent value alphanumeric
		ret = true;
		line.process("PCD12 00A0023.45                                                              ");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}


	try {
		// Invalid Line : Invalid Qualifier
		ret = true;
		line.process("PCD12 0000023.45A                                                              ");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}



	try {
		// Invalid Line : Line too short and even shorter than line identifier (exception)
		ret = true;
		line.process("PC");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}


	try {
		// Invalid Line : Not PCD-Line (exception)
		ret = true;
		line.process("XXX");
		ret = line.isValidLine(); 
		assert(ret == false);
	} 
	catch (...) {		
		int i=99;i;
	}

	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}



void testIRRLineclass()
{
	domMod::line::IrrLine line;
	bool ret = false;


	try {

		// Valid Line Normalausstattung
		ret = false;
		line.process("TDT12                                                                           ");
		ret = line.isValidLine(); 
		assert(ret == true);



		try {
			// Invalid Line : Unknown intentifier
			ret = true;
			line.process("ABC12                                                                        ");
			ret = line.isValidLine(); 
			assert(ret == false);
		} 
		catch (...) {		
			int i=99;i;
		}



		try {
			// Invalid Line : Line too short and even shorter than line identifier (exception)
			ret = true;
			line.process("TD");
			ret = line.isValidLine(); 
			assert(ret == false);
		} 
		catch (...) {		
			int i=99;i;
		}



	}
	catch (	basar::cmnutil::BasarException be)
	{
		int i=99;i;

		// ToDo: Logging
	}
	catch (	...)
	{
		int i=99;i;

		// ToDo: Logging
	}
	return;
}
*/
