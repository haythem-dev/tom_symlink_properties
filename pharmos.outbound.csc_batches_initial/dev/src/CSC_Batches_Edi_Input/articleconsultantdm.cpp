//-------------------------------------------------------------------------------------
/*! \file  articleconsultantdm.cpp
*  \brief  Domain module for database operations regarding articles
*  \brief  Main purpose: get the additional article information to fill the fields of an orderhead/-pos-entry 
*  \author P. Kessling
*  \date   19.04.2013, 
*  history 19.04.13  pk new, similar to batchorderdm.h
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <string>
#include <sstream>		// for stringstream
#include <iomanip>		// for stringstream
#include <exception>	// for exceptions
#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros


#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasarappl_systemeventmanager.h>
#include <libbasarcmnutil_bstring.h> 
#include <libbasarcmnutil_datetime.h>
#include <libbasardbaspect.h>



#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "dataacquisitionexception.h"

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"
#include "edi_input_definitions.h"
#include "articleconsultantdm.h"

//-------------------------------------------------------------------------------------------------//
// using section
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::cmnutil::ParameterList;
using basar::db::aspect::ExecuteResultInfo;
//-------------------------------------------------------------------------------------------------//
// namespace definition section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace libAbbauw
	{
		namespace articleConsultantDM
		{
			//--------------------------------------------------------------------------------------------------//
			// static definition section
			//--------------------------------------------------------------------------------------------------//
			//--------------------------------------------------------------------------------------------------//
			// object definition section
			//--------------------------------------------------------------------------------------------------//

			//**************************************************************************************************
			//         Konstruktoren / Destruktoren
			//**************************************************************************************************
			ArticleConsultantDM::ArticleConsultantDM() : m_Initialized(0) 
			{
				METHODNAME_DEF( ArticleConsultantDM, ArticleConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			ArticleConsultantDM::~ArticleConsultantDM() 
			{
				METHODNAME_DEF( articleConsultantDM, ~ArticleConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}


			//**************************************************************************************************
			//         Methoden
			//**************************************************************************************************
			//*************************************** init *****************************************************
			void ArticleConsultantDM::init(const ConnectionRef activeConn) 
			{
				METHODNAME_DEF( ArticleConsultantDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {		

					if(0 == m_Initialized) {
						// Watch out: Proptab-methods like empty(), size() dont work when caching != FULL_CACHING 
						m_ACCArticleInstRef = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_ARTICLE, libabbauw::domMod::ACC_ARTICLE, activeConn, basar::db::aspect::FULL_CACHING);  
					}

					++m_Initialized;
				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << "createAccessorInstance failed!" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// init

			//*************************************** shutdown *****************************************************
			void ArticleConsultantDM::shutdown() 
			{
				METHODNAME_DEF( ArticleConsultantDM, shutdown )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {		
					// release accessor instance if instance was created WITH storing (default: without) in instance list
					// basar::db::aspect::Manager::getInstance().releaseAccessorInstance("AccInstance");

					if(0 == m_Initialized)
					{
						return;
					}			
					--m_Initialized;

					if(0==m_Initialized) {

						if(!m_ACCArticleInstRef.getPropertyTable().isNull())
						{
							m_ACCArticleInstRef.getPropertyTable().clear();
						}
						m_ACCArticleInstRef.reset();

					}// if(0==m_Initialized)

				}
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

			}// shutdown


			//*************************************** findManufacturernoByArticleno *****************************************************
			basar::Int32  ArticleConsultantDM::findManufacturernoByArticleno(const basar::Int32 articleno) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findManufacturernoByArticleno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {	
					if ( 0 == articleno)
					{
						return 0;
					}

					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setInt32(libabbauw::properties::ARTICLENO.getName(), articleno);

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_MANUFACTURERNO_BY_ARTICLENO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<" SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int32 ret = 0;
					if (!m_ACCArticleInstRef.getPropertyTable().empty())
					{
						ret = m_ACCArticleInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::MANUFACTURERNO.getName());
					}
					return ret;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception"<< std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ));
				}// catch
			}// findManufacturernoByArticleno


			//*************************************** findWholesalerpurchasepriceByArticleno *****************************************************
			const basar::Decimal  ArticleConsultantDM::findWholesalerpurchasepriceByArticleno(const basar::Int32 articleno) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findWholesalerpurchasepriceByArticleno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {
					basar::Decimal ret(0);	// What to do without price: Set to 0. (kes/Gehrmann 17.06.13),price is only shown as an information
					if ( 0 == articleno)
					{
						return ret;
					}

					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setInt32(libabbauw::properties::ARTICLENO.getName(), articleno);

 
					// price should be valid at least until today
					basar::DateTime dtToday; 
					basar::Int32 intToday = dtToday.getDate();			

					yitSearch.setInt32(libabbauw::properties::VALIDTODATE.getName(), intToday);	

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_WHOLESALERPURCHASEPRICE_BY_ARTICLENO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{				
						// if price was found in articlecodes
						ret = m_ACCArticleInstRef.getPropertyTable().begin().getDecimal(libabbauw::properties::WHOLESALERPURCHASEPRICE.getName());
					}

					return ret;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findWholesalerpurchasepriceByArticleno



			//*************************************** findPreferredArticleCodeByArticleEan (Original, running in CH) *****************************************************
			// Note: If CH decides to use GTIN, this method will probably be the same as for DE.
			/*
			const basar::I18nString  ArticleConsultantDM::findPreferredArticleCodeByArticleEanCH(const basar::I18nString articleean) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findPreferredArticleCodeByArticleEanCH )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::I18nString retStr("");

				try {		
					if ( 0 == articleean.length())
					{
						return retStr;
					}

					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLE_CODE.getName(),	articleean);
					yitSearch.setInt16 (libabbauw::properties::CODE_TYPE.getName(),		libabbauw::domMod::lit::ARTICLE_CODE_TYPE_EAN);	


					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
							// if this articleEan was found in articlecodes
							retStr = m_ACCArticleInstRef.getPropertyTable().begin().getString(libabbauw::properties::ARTICLE_CODE.getName());
					}
					return retStr;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findPreferredArticleCodeByArticleEanCH
			*/

			
			//*************************************** findPreferredArticleCodeByArticleEan *****************************************************
			const basar::I18nString  ArticleConsultantDM::findPreferredArticleCodeByArticleEan(const basar::I18nString articleean) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findPreferredArticleCodeByArticleEan )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::I18nString retStr("");

				try {		
					if ( 0 == articleean.length())
					{
						return retStr;
					}

					// articleEan must be numeric
					basar::I18nString i18nstringArticleEan(articleean);					// just to have a non-const variable to work with
					i18nstringArticleEan.trim(basar::cmnutil::BString::FIRST_END);		// Erase space-characters at front and end
		
					if ( false == i18nstringArticleEan.isDigit())
					{
						basar::VarString msg;
						msg = "EAN contains non-digit characters: <" + i18nstringArticleEan + ">";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}

					// articleEan must have the correct length
					if (   libabbauw::domMod::lit::ARTICLE_CODE_TYPE_EAN_LENGTH  != i18nstringArticleEan.length() 
						&& libabbauw::domMod::lit::ARTICLE_CODE_TYPE_GTIN_LENGTH != i18nstringArticleEan.length())
					{
						// i18nstringArticleEan is too long or too short to be a EAN
						// Typically this happens when a article-PZN (7 or 8 chars) is coded wrongly with e.g. "EAN" like a EAN in the EDIFact-LIN-ARTNRC-Field
						// Just log:
						basar::VarString msg;
						msg = "EAN with wrong length found: <" + i18nstringArticleEan  + ">";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a EAN or GTIN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}


					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLE_CODE.getName(),	i18nstringArticleEan);		// TODO: Or better look for non-trimmed articleean as it was before in CH??


					if ( libabbauw::domMod::lit::ARTICLE_CODE_TYPE_EAN_LENGTH == i18nstringArticleEan.length() )			
					{
						// We got a EAN with 13 digits
						yitSearch.setInt16 (libabbauw::properties::CODE_TYPE.getName(),		libabbauw::domMod::lit::ARTICLE_CODE_TYPE_EAN);	
					}
					else 
					{
						// We got a GTIN (=EAN with 14 digits)
						yitSearch.setInt16 (libabbauw::properties::CODE_TYPE.getName(),		libabbauw::domMod::lit::ARTICLE_CODE_TYPE_GTIN);	
					}

					
					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
							// if this articleEan was found in articlecodes
							retStr = m_ACCArticleInstRef.getPropertyTable().begin().getString(libabbauw::properties::ARTICLE_CODE.getName());
					}
					return retStr;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findPreferredArticleCodeByArticleEan


									

			//*************************************** findPreferredArticleCodeByArticleMF *****************************************************
			basar::Int32  ArticleConsultantDM::findArticlenoInternByArticleMF(const basar::I18nString articlemf) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findArticlenointernByArticleMF )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::Int32 ret(0);

				// Unknown article
				try {		
					if ( 0 == articlemf.length())
					{
						return ret;
					}

					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLENOMF.getName(),	articlemf);


					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_ARTICLENO_INTERN_BY_ARTICLENOMF, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
						ret = m_ACCArticleInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::ARTICLENO.getName());
					}
					return ret;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findArticlenoInternByArticleMF



			//*************************************** findPreferredArticleCodeByArticleMF *****************************************************
			// Remark kes: both accessors could easier be put together into one ACCMETH.
			const basar::I18nString	ArticleConsultantDM::findPreferredArticleCodeByArticleMF(const basar::I18nString articlemf)
			{
				METHODNAME_DEF( ArticleConsultantDM, findPreferredArticleCodeByArticleMF )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {
					basar::I18nString retstr("");

					// Unknown article
					if ( 0 == articlemf.length())
					{
						return retstr;
					}

					// Get internal articleno
					basar::Int32 articleno_intern	= findArticlenoInternByArticleMF(articlemf);
					if ( 0 == articleno_intern)
					{
						return retstr;
					}


					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					// kes 25.09.14: Richtige Property setzen:
					yitSearch.setInt32(libabbauw::properties::ARTICLENO.getName(),	articleno_intern);
					// orig war: yitSearch.setString(libabbauw::properties::ARTICLENOMF.getName(),	articlemf);

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLENO_INTERN, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
						retstr = m_ACCArticleInstRef.getPropertyTable().begin().getString(libabbauw::properties::ARTICLE_CODE.getName());
					}
					return retstr;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findPreferredArticleCodeByArticleMF




			//*************************************** findPreferredArticleCodeByArticlePzn *****************************************************
			// Can cope with both PZN-types: PZN7 and PZN8   
			// CH: There is only PZN-7				(In CH ex code_types 11 PZN-7, 12 (few Austria codes), 13 EAN)
			const basar::I18nString  ArticleConsultantDM::findPreferredArticleCodeByArticlePznInhCH(const basar::I18nString articlepzn) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findPreferredArticleCodeByArticlePzn )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::I18nString retStr("");

				try 
				{	
					// Got something?
					if ( 0 == articlepzn.length())
					{
						return retStr;
					}

					// articlepzn must be numeric
					basar::I18nString i18nstringArticlepzn(articlepzn);					// just to have a non-const variable to work with
					i18nstringArticlepzn.trim(basar::cmnutil::BString::FIRST_END);		// Erase space-characters at front and end
		
					if ( false == i18nstringArticlepzn.isDigit())
					{
						basar::VarString msg;
						msg = "PZN contains non-digit characters: <" + i18nstringArticlepzn + ">";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}

					// Reduce articlepzn-string to the pure number without leading zeroes.
					// (A wrongly sent EAN would need Long64, therefore a simple conversion to a number and back is not possible(lack of 64-methods.)
					basar::UInt32 index(0);		// counts leading zeroes
					while( index < i18nstringArticlepzn.length() )
					{
						if ('0' != i18nstringArticlepzn[index] )
						{
							// index points to first digit that is >0
							break;
						}
						++index;
					}
					
					if (index == i18nstringArticlepzn.length() )
					{
						basar::VarString msg;
						msg = "PZN contains only zeroes!";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a valid PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}

					if (0 < index)
					{
						i18nstringArticlepzn.erase(0, index);	// strip leading zeroes
					}



					// Set properties: ARTICLE_CODE ... 
					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLE_CODE.getName(),	i18nstringArticlepzn);

					// ... and CODE_TYPE (Code type depends on type of PZN)
					if ( i18nstringArticlepzn.length() <= libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN7_LENGTH )
					{
						// PZN7 
						yitSearch.setInt16 (libabbauw::properties::CODE_TYPE.getName()
							,libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN7);	
					}
					else if ( i18nstringArticlepzn.length() == libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN8_LENGTH )			
					{
						// PZN8: currently  existing only in Germany
						yitSearch.setInt16 (libabbauw::properties::CODE_TYPE.getName()
							,libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN8);	
					}
					else
					{
						// PZN is too long: Typically this happens when a article-EAN (13 chars) is coded wrongly with "ZZZ" like a PZN
						// log 
						basar::VarString msg;
						msg = "PZN too long: <" + articlepzn  + ">";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}


					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE, yitSearch);

					if( resInfo.hasError() ){

						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					// Watch out: empty() doesnt work with caching policy other than FullCaching
					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
						retStr = m_ACCArticleInstRef.getPropertyTable().begin().getString(libabbauw::properties::ARTICLE_CODE.getName());
					}
					return retStr;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun<< " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findPreferredArticleCodeByArticleMF


			//*************************************** findPreferredArticleCodeByArticlePznInhDE *****************************************************
			// DE: There is mainly PZN-8            (In DE ex code_types 11 PZN-7,			               13 EAN,   16 PZN-8)
			const basar::I18nString  ArticleConsultantDM::findPreferredArticleCodeByArticlePznInhDE(const basar::I18nString articlepzn) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findPreferredArticleCodeByArticlePzn )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::I18nString retStr("");

				try 
				{	
					// Got something?
					if ( 0 == articlepzn.length())
					{
						return retStr;
					}

					// articlepzn must be numeric
					basar::I18nString i18nstringArticlepzn(articlepzn);					// just to have a non-const variable to work with
					i18nstringArticlepzn.trim(basar::cmnutil::BString::FIRST_END);		// Erase space-characters at front and end
		
					if ( false == i18nstringArticlepzn.isDigit())
					{
						basar::VarString msg;
						msg = "PZN contains non-digit characters: <" + i18nstringArticlepzn + ">";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}

					// Reduce articlepzn-string to the pure number without leading zeroes.
					// (A wrongly sent EAN would need Long64, therefore a simple conversion to a number and back is not possible(lack of 64-methods.)
					basar::UInt32 index(0);		// counts leading zeroes
					while( index < i18nstringArticlepzn.length() )
					{
						if ('0' != i18nstringArticlepzn[index] )
						{
							// index points to first digit that is >0
							break;
						}
						++index;
					}
					
					if (index == i18nstringArticlepzn.length() )
					{
						basar::VarString msg;
						msg = "PZN contains only zeroes!";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a valid PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}

					if (0 < index)
					{
						i18nstringArticlepzn.erase(0, index);	// strip leading zeroes
					}

					// Sort out numbers that are too long for PZN-8 or PZN-7
					if ( i18nstringArticlepzn.length() > libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN8_LENGTH )			
					{
						// PZN is too long: Typically this happens when a article-EAN (13 chars) is coded wrongly with "P  " like a PZN
						// log 
						basar::VarString msg;
						msg = "PZN too long: <" + articlepzn  + ">";		
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						// This is not a PZN: Don't try to decode it from DB
						return retStr;	// is still empty 
					}




					// prepare Accessor: ARTICLE_CODE ... 
					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLE_CODE.getName(),	i18nstringArticlepzn);

					// In DE first look for PZN-8 encoding.
					// In case of "not found": look for PZN-7 encoding.

					// call execute if your connection has been established!	
					// PZN8: currently  existing only in Germany
					yitSearch.setInt16 (libabbauw::properties::CODE_TYPE.getName()	,libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN8);	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE, yitSearch);

					if( resInfo.hasError() ){

						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution 1 has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					// Watch out: empty() doesnt work with caching policy other than FullCaching
					if ( m_ACCArticleInstRef.getPropertyTable().empty() )
					{
						// Didn't find a PZN-8. Perhaps a PZN-7 possible?
						if ( i18nstringArticlepzn.length() <= libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN7_LENGTH )
						{
							// ... then try 
							m_ACCArticleInstRef.getPropertyTable().clear();					
							m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
							AccessorPropertyTable_YIterator yitSearchPzn7 = m_ACCArticleInstRef.getPropertyTable().begin();
							yitSearchPzn7.setString(libabbauw::properties::ARTICLE_CODE.getName(),	i18nstringArticlepzn);

							yitSearchPzn7.setInt16 (libabbauw::properties::CODE_TYPE.getName()	,libabbauw::domMod::lit::ARTICLE_CODE_TYPE_PZN7);	

							ExecuteResultInfo	exResInfo  
								= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE, yitSearchPzn7);

							if( resInfo.hasError() ){

								BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (exResInfo.getError()));

								std::stringstream str;
								str << fun <<"SQL execution 2 has errors: " << exResInfo.m_error<< std::endl;
								throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
							}
						}
					}

					// Got something?
					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
						retStr = m_ACCArticleInstRef.getPropertyTable().begin().getString(libabbauw::properties::ARTICLE_CODE.getName());
					}
					return retStr;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun<< " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}



			//*************************************** findPreferredArticlenoByPreferredArticleCode *****************************************************
			basar::Int32 ArticleConsultantDM::findPreferredArticlenoByPreferredArticleCode(const basar::I18nString preferred_article_code) 
			{
				METHODNAME_DEF( ArticleConsultantDM, findPreferredArticlenoByPreferredArticleCode )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try 
				{
					if ( 0 == preferred_article_code.length() )
					{
						return 0;
					}

					m_ACCArticleInstRef.getPropertyTable().clear();					
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLE_CODE.getName(),	preferred_article_code);

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLENO_BY_PREFERRED_ARTICLECODE, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int32 ret(0);
					if ( !m_ACCArticleInstRef.getPropertyTable().empty() )
					{
						ret = m_ACCArticleInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::ARTICLENO.getName());
					}
					return ret;

				}
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}
			}


			basar::Int16 ArticleConsultantDM::findCMIArticles(const basar::I18nString articlepzn)
			{
				METHODNAME_DEF(ArticleConsultantDM, findCMIArticles)
				BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun);

				try
				{
					if (0 == articlepzn.length())
					{
						return 0;
					}

					m_ACCArticleInstRef.getPropertyTable().clear();
					m_ACCArticleInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);
					AccessorPropertyTable_YIterator yitSearch = m_ACCArticleInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::ARTICLE_CODE.getName(), articlepzn);

					// call execute if your connection has been established!	
					ExecuteResultInfo resInfo = m_ACCArticleInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_CMI_ARTICLES, yitSearch);

					if (resInfo.hasError()) 
					{
						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun << "SQL execution has errors: " << resInfo.m_error << std::endl;
						throw libabbauw::DataAcquisitionException(basar::ExceptInfo(fun, str.str().c_str(), __FILE__, __LINE__));
					}

					basar::Int16 ret(0);
					if (!m_ACCArticleInstRef.getPropertyTable().empty())
					{
						ret = m_ACCArticleInstRef.getPropertyTable().begin().getInt16(libabbauw::properties::ISCMI.getName());
					}

					return ret;
				}
				catch (basar::Exception& e)
				{
					basar::VarString msg;
					msg.format("Basar-Exception: <%s>", e.what().c_str());

					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg);
					throw e;
				}
				catch (std::exception& e)
				{
					basar::VarString msg;
					msg.format("Exception: <%s>", e.what());

					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg);
					throw e;
				}
				catch (...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun);

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException(basar::ExceptInfo(fun, str.str().c_str(), __FILE__, __LINE__));
				}
			}


			//*************************************** getArticlePropTab *****************************************************
			AccessorPropertyTableRef ArticleConsultantDM::getArticlePropTab() {

				METHODNAME_DEF( ArticleConsultantDM, getArticlePropTab )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {		
					return m_ACCArticleInstRef.getPropertyTable();
				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun<< "Could get PropetryTable" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// getArticlePropTab



		} // namespace ArticleConsultantDM
	} // namespace libAbbauw
} // namespace domMod

