//-------------------------------------------------------------------------------------------------//
/*! \file	cscorderdm.cpp
 *  \brief  implementation of domain module (cscorder)
 *  \author Beatrix Trömel
 *  \date   05.08.2008
 *  history: 16.08.2013 kes new method createCSCOrderABBA(...) and enhanced createCSCOrder(...)
 */
//-------------------------------------------------------------------------------------------------////-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#ifdef WIN32
#pragma warning (push)
#pragma warning(disable: 4127 4512)
#endif
#include <QtCore/QCoreApplication>
#ifdef WIN32
#pragma warning (pop)
#endif

#include <libbasardbaspect.h>
#include <libbasar_definitions.h>
#include <kscclientwrapper/kscclientwrapper.h>

#include "libcscbatch_definitions.h"
#include "batchorderdm.h"
#include "cscorderdm.h"
#include "cscinfosacc.h"
#include "basarutils.h"
#include "icscsession.h"
#include "libabbauw/iorderreleaseparameter.h"
#include "libcscbatch_loggerpool.h"	// for LoggerPool
//+++ Spion löschen:
#include <sstream>
#include <iostream>
#include <fstream> //REMOVE
using namespace std;

//-------------------------------------------------------------------------------------------------//
// usings
//-------------------------------------------------------------------------------------------------//
using domMod::KSCClientDMConnParams;
using basar::appl::HANDLER_OK;

namespace libcscbatch {
namespace domMod {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	CSCOrderDM::CSCOrderDM( const ConnectionRef conn, const basar::Int16 branchNo )
		: m_connection( conn ), m_KSCClientDM( LoggerPool::loggerDomModules ), m_Initialized(0), m_branchNo( branchNo )
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::CSCOrderDM().");
	}

    CSCOrderDM::CSCOrderDM( const ConnectionRef conn, const basar::Int16 branchNo, const basar::VarString KscServer, const basar::Int32 KscPort )
        : m_connection( conn ), m_KSCClientDM( LoggerPool::loggerDomModules ), m_Initialized(0), m_branchNo( branchNo )
    {
        BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::CSCOrderDM().");

        //-- init CSC Connection:
		m_KscConnection.server=KscServer;   // is taken from UsrAdmin
		m_KscConnection.port=KscPort;	    // is taken from UsrAdmin
		m_KscConnection.device="CSCBATCH";  // USE CAPITAL LETTERS ONLY!!!
    }

	CSCOrderDM::~CSCOrderDM()
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::~CSCOrderDM().");
		shutdown();
	}

	void CSCOrderDM::injectParameter( libabbauw::domMod::parameter::IOrderReleaseParameterPtr parameterDM )
	{
		m_ParameterDM = parameterDM;
	}

    void CSCOrderDM::setCSCSession( CSC_Batches::infrastructure::session::ICSCSessionPtr session )
    {
        //-- init CSC Connection:
        m_KscConnection.server = session->getServer();
		m_KscConnection.port   = session->getPort();
		m_KscConnection.device = session->getDevice();
    }

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** init *****************************************************
	void CSCOrderDM::init() 
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::init().");

		try 
		{		
			if(0 == m_Initialized) 
			{
                m_KSCClientDM.init( m_branchNo, m_KscConnection );
				m_BatchOrderAbbaACC	= basar::db::aspect::Manager::getInstance().createAccessorInstance("SelBatchOrderAccInstance", "SelBatchOrdersAcc", ConnectionRef());	// null-Connection! No DB-searches planned, just to use the proptab 

				// init domain modules:
				m_batchOrderDM.init(m_connection);
			}
			++m_Initialized;
		}
        catch(KSCClientDMException& e) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::init()->KSCClientDMException."); 
			throw e;
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::init()."); 
			throw; 
		}
	}

	//*************************************** shutdown *****************************************************
	void CSCOrderDM::shutdown()
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::shutdown().");
		try 
		{		
			// release accessor instance if instance was created WITH storing (default: without) in instance list
			
			if( 0==m_Initialized )
				return;
			
			--m_Initialized;
			if( 0==m_Initialized )
			{
				if(!m_BatchOrderAbbaACC.getPropertyTable().isNull())
					m_BatchOrderAbbaACC.getPropertyTable().clear();
				m_BatchOrderAbbaACC.reset();

				//--- shutdown domain modules:
				m_KSCClientDM.shutdown();
				m_batchOrderDM.shutdown();
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::shutdown()."); 
			throw;
		}
	}

	//*************************************** createCSCOrderABBAUW ******************************************************
	//	User friendly method for releasing a single order from ABBA-UW
	//  For datasourcetype: "UW" = transfer orders (UW, OM, UN, MD etc. from project ABBA-UW)
	// 
	//	Parameters:
	//	datasourcetype = "UW"		
	//	ordertype:	e.g."UW00", "OM22" ..., contains booking- and pickingtype!
	//  postpone:	true:  Order will be postponed in KSC (State ZU)
	//              false: Order will be out to Commission&invoicing at once
	//
	//  Watch out: orderstate has to be set to "FE" before calling this method!
	//             This method sets it to "ER" on success.

	basar::appl::EventReturnType CSCOrderDM::createCSCOrderABBAUW(	basar::Int16		branchno,				// yit
                                                                    basar::Int16        branchnoorig,           // yit
																	basar::Int32		orderno,				// yit
																	basar::Int32		pharmacyno, 			// yit
                                                                    basar::Int32		purchaseorderno,    	// yit P_17000393
																	const basar::I18nString&	datasourcetype, // yit 
																	const basar::I18nString&	ordertype, 		// yit	
																	const basar::I18nString&	headertext,		// yit
																	const basar::Date&		deferredpaymentdate,	// yitExtra
																	const basar::Date&		edideliverydate,		// yitExtra
																	const bool			postpone				// true: postpone order, false: close order completely
																	)				
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCOrderABBAUW().");

		basar::appl::EventReturnType	result(basar::appl::HANDLER_ERROR);

		try 
		{
			// Prepare Yit: One line with additional info for KSC-Client. 
			// The PropertyTable of Yit is defined as SelBatchOrdersAcc in batchorderacc.cpp
			m_BatchOrderAbbaACC.getPropertyTable().clear();					
			m_BatchOrderAbbaACC.getPropertyTable().insert(basar::FOR_UNKNOWN);
			AccessorPropertyTable_YIterator yit = m_BatchOrderAbbaACC.getPropertyTable().begin();

			// Fill yit with params
			yit.setInt16("branchno",		branchno);		
            yit.setInt16("branchnoorig",    branchnoorig); // this method is now used for NO orders too (branchnoorig is needed there)
			yit.setInt32("orderno",			orderno);		
			yit.setInt32("pharmacyno",		pharmacyno);	
            yit.setInt32("purchaseorderno", purchaseorderno);
			yit.setString("datasourcetype",	datasourcetype);		
			yit.setString("ordertype",		ordertype);		
			yit.setString("headertext",		headertext);

			// Prepare YitExtra: One line with additional info for KSC-Client. 
			// All properties are strings because of createAccessorYIterator()
			basar::I18nString	strYit = "";
			basar::I18nString	strYit1 = "";

			strYit1.format("%s=%d;", "deferredpaymentdate", deferredpaymentdate.getDate());		
			strYit.append(strYit1);						
			strYit1.format("%s=%d;", "edideliverydate",		edideliverydate.getDate());		
			strYit.append(strYit1);						
			AccessorPropertyTable_YIterator yitExtra = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
			
			result = createCSCOrder( yit, yitExtra, postpone );

			return result;
		}
		catch(KSCClientDMException e) 
		{
			// kes 15.04.14: Help to gather returntexts om KSC errors, do not throw them one by one
			result.message = e.what();
			return result;
			// orig: throw e;
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCOrderABBAUW()."); 
			throw;
		}

	}

	//*************************************** createCSCServerOrder ******************************************************
	//
	//  datasourcetype: "SH" or "CL" or else are Stammhaus- or Clearing-Orders
	//					"UW" are transfer orders (UW, OM, UN, MD etc. from project ABBA-UW)
	//	                  
	//  Parameters:
	//   Yit:	Contains the properties, that are defined in batchorderacc.cpp
	//			for Accessor SelBatchOrdersAcc. This Accessor is used e.g. for
	//			Access-Method SelBatchOrdersByPattern, that fills up the CH/SH-GUI-Screen
	//			of CSC_Batches. 
	//
	//			yit-values that must be set for datasourcetype SH/CL:
	//			"branchno", "orderno", "pharmacyno","datasourcetype",  "ordertype" = e.g."NO00",  
	//			"headertext", "branchnoorig"
	//			
	//
	// 			yit-values that must be set for datasourcetype UW:
	//			"branchno", "orderno", "pharmacyno","datasourcetype",  "ordertype" = e.g."UW00",  
	//			"headertext" 
	//
	//	
	//   YitExtra:  Contains extra properties, depending on the kind of order.
	//			This Yit is e.g. formed in CSC_Batches_Auto, file batchorderautouc.cpp, 
	//			method releaseOrders(). It is called "yitUpdateKscExtra" there. 
	//
	//			yitExtra-values that must be set for datasourcetype SH/CL:
	//			"deferredpaymentdate" (integer), "edideliverynote" (string)
	//
	//			yitExtra-values that must be set for datasourcetype UW:
	//			"deferredpaymentdate" (integer), "edideliverydate" (integer)
	//
	//  postpone:	true: Order will be postponed in KSC (State ZU)
	//              false: Order will be out to Commission&invoicing at once
	//				Watch out: default if not set = false!!


	basar::appl::EventReturnType CSCOrderDM::createCSCOrder(const AccessorPropertyTable_YIterator yit, 
															const AccessorPropertyTable_YIterator yitExtra,
															const bool		postpone)  // default =false, if not set
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCOrder().");


		// yitHead: Iterator for a Property-Table-line, where the KSCClient-Order will be built.
		//			YitHead is constructed by AccKSCClientOrder, that is defined in clientorderacc.cpp
		basar::db::aspect::AccessorPropertyTable_YIterator yitHead; // has to be before try-block because it is also used in catch-block
		bool OpenOrder=false;

		try 
		{
			basar::appl::EventReturnType	result(basar::appl::HANDLER_ERROR);
			basar::I18nString				str	=	"";
			::domMod::KSCClientDMReturn		kscRet; 

			kscRet.stateInfo = ::domMod::INPUT_ERROR; // initialization
			
			if(yit.isNull() || yit.isEnd()) 
			{
				result.message = QCoreApplication::translate("CSCOrderDM","No data passed for creating a CSC order!").toLocal8Bit().constData();
				return result;
			}

			// Find out the type of order: CL, SH or UW/OM
			basar::Int32 BatchType = getBatchType(yit.getString("datasourcetype"));

			//--- create empty order: 
			// Adds one line to AccKSCClientOrder-PropertyTable and gets an Iterator for it
			// yitHead (For Property description list see AccKSCClientOrder in clientorderacc.cpp): 
			yitHead=m_KSCClientDM.addEmptyOrder();


			//--- set properties for the orderhead:
			if (isTransferOrder(yit))
			{
				// --- set properties for the head of a UW- or OM-order from ABBA-UW:
				yitHead.setInt16("branchno",yit.getInt16("branchno"));
				yitHead.setInt32("customerno",yit.getInt32("pharmacyno"));
				yitHead.setString("ordertype",yit.getString("ordertype").substr(0,2));		// e.g. "UW", "OM", "CM"
				yitHead.setString("pickingtype",yit.getString("ordertype").substr(2,1));
				yitHead.setString("bookingtype",yit.getString("ordertype").substr(3,1));	
				yitHead.setString("orderheadline1"	,yit.getString("headertext"));	// For textmessage on invoice "*UEWAUF VON <manufacturername>"
																					// in kdauftragbem.bemerkungen Nr. 1? 
				yitHead.setInt32("batchtyp",BatchType);

				yitHead.setString("reference","");		// ABBA-UW uses reference only for storage of ksc-orderno.  

				// items for later on called method setBatchValueDate()
				str = yitExtra.getString("deferredpaymentdate");		// valuta date
				int intDeferredpaymentdate	= str.stoi();
				// P-16000164: A valuedate for today or even earlier days does not make sense
				basar::DateTime dtToday;
				int intToday				= dtToday.getDate();		
				if ( intDeferredpaymentdate <= intToday )
				{
					intDeferredpaymentdate = 0;
				}
				yitHead.setInt32("deferredpaymentdate",intDeferredpaymentdate);

				yitHead.setInt32("batchschreiben",9);					// "9" means: Ueberweiser
				yitHead.setInt16("originbranchno",0);

				// In yitExtra: edideliverydate is string (not int32 because of oldfashioned way of creation, e.g. in batchorderuc.cpp)
				// In yitHead:  edideliverydate is int32
				str = yitExtra.getString("edideliverydate");		// delivery date
				yitHead.setInt32("edideliverydate",str.stoi());


				// Implemented for AT: If parameter "ORDERROUTING"='OFF' in table KSC-DB/parameter, 
				// then switch off orderrouting (Anrufweiterleitung) for the next openORDER()-Call.
				// Any other setting for this parameter or no setting: don't touch the settings for orderrouting.
				basar::VarString orderrouting = m_ParameterDM->getOrderrouting();
				orderrouting.trim();
				basar::VarString strParamValueOrderroutingOff(PARAM_VALUE_OFF);
				if(0 == orderrouting.compare(strParamValueOrderroutingOff) ) 
				{
					
					// The call of setNoRouting() is active only for the next openOrder().  
					m_KSCClientDM.kscSetNoRouting();	// So soll es sein
					BLOG_TRACE(LoggerPool::loggerDomModules, "### SETNOROUTING() called");	
				}
			}
			else	/*DATASRC_CL, _SH and everything else */
			{
				// --- set properties for the head of a CL/SH resp. non-UW-order:
				yitHead.setInt16("branchno",yit.getInt16("branchno"));
				yitHead.setInt32("customerno",yit.getInt32("pharmacyno"));
				yitHead.setString("ordertype",yit.getString("ordertype").substr(0,2));
				yitHead.setString("pickingtype",yit.getString("ordertype").substr(2,1));
				yitHead.setString("bookingtype",yit.getString("ordertype").substr(3,1));
				yitHead.setString("ordernote",yit.getString("headertext"));
				yitHead.setInt16("originbranchno",yit.getInt16("branchnoorig"));
				yitHead.setInt32("batchtyp",BatchType);
				str = yitExtra.getString("deferredpaymentdate");
				yitHead.setInt32("deferredpaymentdate",str.stoi());
				yitHead.setString("edideliverynote","0");
			}

			//test the value of yit temporarily at debug breakpoints
			#ifdef _DEBUG
			if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
			{
				std::stringstream strstream;
				yitHead.toStream(strstream);
				BLOG_TRACE(LoggerPool::loggerDomModules, "### SPION createCSCOrder yitHead");
				BLOG_TRACE(LoggerPool::loggerDomModules, strstream.str());
			}
			#endif

			//--- create order in KSC:
			kscRet=m_KSCClientDM.openOrder(yitHead,true);
			
			if(::domMod::OK!=kscRet.stateInfo) 
			{
				// Attention! Do not invoke cancelOrder, if openOrder failed (otherwise kscclient will completely stop)
				basar::I18nString strOrderMessage;
				strOrderMessage.itos(yit.getInt32("orderno"));
				QString strMessage="";
				strMessage.append(QCoreApplication::translate("CSCOrderDM","ERROR when inserting an order into CSC!! Order could not be opened!").toLocal8Bit().constData());
				strMessage.append(QCoreApplication::translate("CSCOrderDM"," Order no. ").toLocal8Bit().constData());
				strMessage.append(strOrderMessage.c_str());
				// kes: 14.01.14 Do not forget textinfo from KSC-Server! And set result to error!
				strMessage.append(QCoreApplication::translate("CSCOrderDM"," Textinfo: ").toLocal8Bit().constData());
				strMessage.append(kscRet.textInfo.c_str());
				result.message=strMessage.toLocal8Bit().constData();
				result.ret	=basar::appl::HANDLER_ERROR;
				BLOG_ERROR(LoggerPool::loggerDomModules, result.message);

				// kes, 15.01.14: After getting a negative result there is no other way than cancelling 
				//                the order to continue after trying to open an order with improper order data.
				//                So try to cancel!
				if(::domMod::INPUT_ERROR ==kscRet.stateInfo) 
				{
					m_KSCClientDM.cancelOrder();
				}

				return result;
			}

			OpenOrder=true;

			//--- get CSC order no:
			basar::Int32 OrderNoKsc=kscRet.itInfo.getInt32("orderno");

			// purchasorderno as relationno	to KSC
			if (yit.isContainedAndSet("purchaseorderno") && yit.getInt32("purchaseorderno") > 0)
			{
				kscRet=m_KSCClientDM.setOrderRelationNo(yit.getInt32("purchaseorderno"), ORDERRELATIONNO_TYPE_PURCHASEORDER);
			}

			//--- ABBA-UW: tell valuedate and Batchmodus to KSC 
			if (isTransferOrder(yit))
			{
				::domMod::KSCClientDMReturn kscRetSet;
				kscRetSet.stateInfo	= ::domMod::INPUT_ERROR; // initialization

				kscRetSet=m_KSCClientDM.setBatchValueDate(yitHead); // Params to be set: branchnoorigin=0, deferredpaymentdate, Batchschreiben=9(=Ueberweiser)		

				if(::domMod::OK!=kscRetSet.stateInfo) 
				{
					basar::I18nString strOrderMessage;
					strOrderMessage.itos(yit.getInt32("orderno"));
					QString strMessage="";
					strMessage.append(QCoreApplication::translate("CSCOrderDM","FATAL ERROR when setting Batch and valuedate for an order!").toLocal8Bit().constData());
					strMessage.append(QCoreApplication::translate("CSCOrderDM"," Order no. ").toLocal8Bit().constData());
					strMessage.append(strOrderMessage.c_str());
					// kes: 14.01.14 Do not forget textinfo from KSC-Server! And set result to error!
					strMessage.append(QCoreApplication::translate("CSCOrderDM"," Textinfo: ").toLocal8Bit().constData());
					strMessage.append(kscRetSet.textInfo.c_str());
					result.message=strMessage.toLocal8Bit().constData();
					result.ret	=basar::appl::HANDLER_ERROR;
					BLOG_ERROR(LoggerPool::loggerDomModules, result.message);

					// kes, 15.01.14: After getting a negative result there is no other way than cancelling 
					//                the order to continue after trying to open an order with improper order data.
					//                So try to cancel!
					if(::domMod::INPUT_ERROR ==kscRetSet.stateInfo) 
					{
						::domMod::KSCClientDMReturn		kscRetCancel; 
						kscRetCancel=m_KSCClientDM.cancelOrder();
					}

					return result;
				}
			}

			//--- create items in CSC:
			result = createCSCItems(yit.getInt32("orderno"),
										yitHead.getInt32("customerno"), 
										yit.getString("ordertype").substr(0,2));
			if(result.ret == basar::appl::HANDLER_ERROR) 
			{
				kscRet=m_KSCClientDM.cancelOrder();
				return result;
			}

			// Close the order 
			if (isTransferOrder(yit))
			{
				// For ABBA-UW-orders
				if (postpone)
				{
					// release order postponed (state=ZU, manual release in ae.exe-GUI necessary)
					kscRet=m_KSCClientDM.postponeOrder(true);
				}
				else
				{
					// release order completely (state=ER, ready for picking & invoicing)
					kscRet=m_KSCClientDM.closeOrder(true);
				}

				// Evaluate result
				OpenOrder=false;
				if(::domMod::OK!=kscRet.stateInfo)
				{
					kscRet=m_KSCClientDM.openOrder(yitHead,true);  // open before cancelling
					kscRet=m_KSCClientDM.cancelOrder();
					basar::I18nString strOrderMessage;
					strOrderMessage.itos(yit.getInt32("orderno"));
					QString strMessage="";
					strMessage.append(QCoreApplication::translate("CSCOrderDM","FATAL ERROR! Order could not be closed!").toLocal8Bit().constData());
					strMessage.append(QCoreApplication::translate("CSCOrderDM"," Order no. ").toLocal8Bit().constData());
					strMessage.append(strOrderMessage.c_str());

					// kes: 14.01.14 Do not forget textinfo from KSC-Server!And set result to error!
					strMessage.append(QCoreApplication::translate("CSCOrderDM"," Textinfo: ").toLocal8Bit().constData());
					strMessage.append(kscRet.textInfo.c_str());
					result.message	=strMessage.toLocal8Bit().constData();
					result.ret		=basar::appl::HANDLER_ERROR;
					BLOG_ERROR(LoggerPool::loggerDomModules, result.message);

					basar::I18nString strOrderMessage1;
					strOrderMessage1.itos(kscRet.stateInfo);
					strOrderMessage="Return Value CloseOrder() resp. postponeOrder: ";
					strOrderMessage.append(strOrderMessage1);
					BLOG_ERROR(LoggerPool::loggerDomModules, strOrderMessage);
					return result;
				}
			}
			else	/*DATASRC_CL, _SH and more */
			{	
				// TEST: Beginn
                
				basar::I18nString strMsg1 = yit.getString("datasourcetype");
				basar::I18nString strMsg2 ="Datasourcetype before release: <";
				strMsg2.append(strMsg1);
				strMsg2.append(">");
				BLOG_TRACE(LoggerPool::loggerDomModules, strMsg2.c_str());
                

				// TEST: End

				// für Datenherkunft CL: postpone

				if( yit.getString("datasourcetype") == DATASRC_DI || yit.getString("datasourcetype") == DATASRC_BC )
					kscRet=m_KSCClientDM.closeOrder(true);
				else
					kscRet=m_KSCClientDM.postponeOrder(true);  // im KSC landet das als zurückgestellt

				// Evaluate result
				OpenOrder=false;
				if(::domMod::OK!=kscRet.stateInfo) 
				{
					kscRet=m_KSCClientDM.openOrder(yitHead,true);  // open before cancelling
					kscRet=m_KSCClientDM.cancelOrder();
					basar::I18nString strOrderMessage;
					strOrderMessage.itos(yit.getInt32("orderno"));
					QString strMessage="";
					strMessage.append(QCoreApplication::translate("CSCOrderDM","FATAL ERROR! Order could not be closed!").toLocal8Bit().constData());
					strMessage.append(QCoreApplication::translate("CSCOrderDM"," Order no. ").toLocal8Bit().constData());
					strMessage.append(strOrderMessage.c_str());

					result.message=strMessage.toLocal8Bit().constData();
					BLOG_ERROR(LoggerPool::loggerDomModules, result.message);
					basar::I18nString strOrderMessage1;
					strOrderMessage1.itos(kscRet.stateInfo);
					strOrderMessage="";
					strOrderMessage.append("Return Value postponeOrder: ");
					strOrderMessage.append(strOrderMessage1);
					BLOG_ERROR(LoggerPool::loggerDomModules, strOrderMessage);
					return result;
				}
			}

			// set orderacquisitiondate (=releasedate) to today
			basar::DateTime dtToday;

			result = changeBatchOrderStatus(yit.getInt32("orderno"), dtToday.getDate(), OrderNoKsc);
			if(result.ret == basar::appl::HANDLER_ERROR) 
			{
				kscRet=m_KSCClientDM.openOrder(yitHead,true);  // open before cancelling
				kscRet=m_KSCClientDM.cancelOrder();
				return result;
			}

			return basar::appl::HANDLER_OK;
		}
		catch(KSCClientDMException e) 
		{

			std::stringstream str;
			str << "CSCOrderDM::createCSCOrder(): KSCClientDMException:"	<< e.what();
			BLOG_ERROR(LoggerPool::loggerDomModules, str.str().c_str());

			throw e;
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCOrder()."); 
			if(!OpenOrder)
				m_KSCClientDM.openOrder(yitHead,true);  // open before cancelling
			m_KSCClientDM.cancelOrder();
			throw;
		}
	}

	//*************************************** createCSCItems ******************************************************
	/*	
		AccKSCClientItem:			defined in clientitemacc.cpp (?)
			int32 articleno; \
			string ean; \						
				string articlecode; \
			int32 orderqty; \
			int16 interbranchtransfer; \		
			int16 subsequentdelivery; \			
			int16 articlerequirementpurchase; \	
			int16 partialdelivery; \			
			int16 sequenceno; \					
			string itemnote;")
         		int32 idforigin; \
		 		int32 ordernoorigin; \
		 		int32 posnoorigin; \
			for ABBA:	 int32 nonchargedqty; \
	*/
	basar::appl::EventReturnType CSCOrderDM::createCSCItems(basar::Int32 OrderNo, basar::Int32 idfOrigin, basar::I18nString strOrdertype) 
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCItemsABBA().");

		try 
		{
			basar::appl::EventReturnType result(basar::appl::HANDLER_ERROR);
			AccessorPropertyTableRef PropTabSearch;
			
			//--- create Yit for searching all positions for this batch order:
			basar::I18nString strYit="";
			basar::I18nString strYit1="";
			strYit1.format("%s=%d;", "branchno", m_branchNo);
			strYit.append(strYit1);
			strYit1.format("%s=%d;", "orderno", OrderNo);
			strYit.append(strYit1);
			AccessorPropertyTable_YIterator yitOrderPositions = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);

			//--- get all positions for this order:
			// (PropTabSearch-Properties are defined in batchorderposacc.cpp)

            m_batchOrderDM.getOrderPosPropTab().clear();
			PropTabSearch = m_batchOrderDM.findPositionsByBranchNoAndOrderNo(yitOrderPositions);

			//--- put all positions one by one into CSC:
			AccessorPropertyTable_YIterator yit;

			for (yit=PropTabSearch.begin(); !yit.isEnd(); ++yit) 
			{
				if(!nsBasarUtils::BasarUtils::isPropSet(yit,"orderno")) 
				{  // orderno is unset
					result.message=QCoreApplication::translate("CSCOrderDM","FATAL ERROR when inserting a position into CSC! Order number is not set!").toLocal8Bit().constData();
					return result;
				}

				// Generate a single-line proptab containing the customerno with yitExtra pointing to it.
				strYit1.format("%s=%d;", "idforigin", idfOrigin);
				strYit.append(strYit1);
				AccessorPropertyTable_YIterator yitExtra = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);	

				// yit:		 Points to the current item-line of PropTabSearch 
				// yitExtra: Points to a single line, containing only the idfnr for this order
				result = createCSCItem(yit, yitExtra, strOrdertype);

				if(result.ret == basar::appl::HANDLER_ERROR) 
				{
					return result;
				}
			}

			return basar::appl::HANDLER_OK;
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCItems()."); 
			throw;
		}
	}

	//*************************************** createCSCItem ******************************************************
	// Parameter:
	// yit:		 Belongs to one item-line in PropTabSearch. 
	//           Its Properties are defined in batchorderposacc.cpp
	// yitExtra: Belongs to a single line, containing only the idfnr for this order
	// strOrdertype: OrderType as string, e.g. "UW", "OM", ..
	//
	// ToDo Keep Parameter strOrderType better inside of yitExtra
	//
	basar::appl::EventReturnType CSCOrderDM::createCSCItem(AccessorPropertyTable_YIterator yit, AccessorPropertyTable_YIterator yitExtra, basar::I18nString strOrdertype) 
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCItem().");

		try 
		{
			basar::appl::EventReturnType	result(basar::appl::HANDLER_ERROR);
			basar::I18nString				str="";
			::domMod::KSCClientDMReturn		kscRet; 

			//--- create empty item. Later on yitPos collects data for KscClient
			//    The PropertyTable for yitPos is defined in kscclientitemacc.cpp
			basar::db::aspect::AccessorPropertyTable_YIterator yitPos=m_KSCClientDM.addEmptyItem();
			
			//--- set properties for item:
			if ( isTransferOrder(strOrdertype) )
			{
				// set item properties for UW/OM order

				// ARTICLENO and ARTICLECODE
				// kes 12.02.14: Set only articlecode, otherwise articlecode is overwritten by PZN7-articleno in countries with pzn8 etc.! 
				// yitPos.setInt32("articleno",yit.getInt32("pzn"));

				if(!nsBasarUtils::BasarUtils::isPropSet(yit,"article_code")) 
				{  
					// article_code is unset, so utilize pzn instead of it
					basar::I18nString strHelp;
					strHelp.itos(yit.getInt32("pzn"));
					yitPos.setString("articlecode", strHelp);		
				}
				else
				{
					yitPos.setString("articlecode", yit.getString("article_code"));
				}

				// AMGE (= pure ordered quantity without noncharged quantity) 
				// kes 13.2.14: Die orderqty für den KSC-Server ist die reine Fakturmenge (=AMGE, =Liefermenge minus Naturalrabatt)
				//              in orderpos/orderedqty ist die Summe aus AMGE und NR gespeichert. Daher: NR abziehen.
				basar::Int32 orderedqty		= yit.getInt32("orderedqty");		// zu liefernde Gesamtmenge incl. NR
				basar::Int32 rebateinkind	= yit.getInt32("nonchargedqty");	// Naturalrabatt
				yitPos.setInt32("orderqty", orderedqty - rebateinkind);

				// NONCHARGEDQUANTITY (Naturalrabatt)
				// kes 12.02.14: field nonchargedqty is no longer in use
                // yitPos.setInt32  ("nonchargedqty",	yit.getInt32	("nonchargedqty"));		// NR, old field 
                yitPos.setInt32  ("rebateinkind",	rebateinkind);		// NR, Naturalrabatt

				// DISCOUNTPCT (Barrabatt) OR TEXTFIELD
				// kes 12.02.14 orig: yitPos.setDecimal("discountpct",	yit.getDecimal	("discountpct"));		// BR
				// kes 12.02.14: Decrease-Rebate is given as negative number normal case), increase as positive number (very seldom).
				//               ksc-server needs positive number for decrease rebate. Therefore multiply by -1.
				// ToDo: The transfer mechanism for the BR (nowadays by Position-textfield e.g. "BR22;00")
				//       has to be modernized (transfer by decimal-field discountpct and kindofdiscount). 
				//       Until this  has taken place we transfer the BR in both ways. The evaluation of kindofdiscount and 
				//       discountpct is not implemented in KSCserver yet
				// ToDo: When these fields kindofdiscount and discountpct will be evaluated by KSC-Server, there has to 
				//       be a soution for the case "discountpct=0.
				//       In this case the "normal" rebate conditions are to be set, NOT discountPct=0!
				// For ABBA-UW we do NOT send the original textfield from orderpos but the rebate BR-string!
				// kes 17.02.15: For ABBA-UW DE we transfer the textfield in case that discountPct=0.00.
				//       As long as the textfield-text does not start with "BR", it is directly transfered to KSC-DB/kdauftrag/bemerkungen.
				//       This is necessary to transfer the Positiontext after Splitt or sometimes the FTX-11/AAA-Text
				basar::Decimal		decBR = yit.getDecimal	("discountpct");
				decBR.round(2);								// round two two digits
				basar::Decimal decNull(0);					

				// The BR-rebate must not be transferred if rebate is 0 (no rebate)
				if ( decBR != decNull)
				{
					// discountPCT contains a real value != zero
					if ( decBR < decNull )						// eliminate minus-sign
					{
						decBR *= -1;
					}

					basar::VarString	strBR = "BR";
					strBR = strBR + decBR.toNonLocString(2);	// Two digits behind dot
					if ( isUWOrder(strOrdertype) )			
					{
						// For UW-Orders add Discount-rebate trailer:
						// If parameter DISCOUNT_PCT_TRAILER is set in DB-table 'parameter': 
						// (programmname='CSCBatches     ',zweck='User Setting        ',parametername='DISCOUNT_PCT_TRAILER',auspraegung='H',wert=0)
						// Add first char of "auspraegung" Trailer to "BR"-text.
						// CR16D072 ABBA for AT: For AT it must be 'H' for Hausrabatt! 'H' causes Hausrabatt to be added in KSC-Client.

						basar::VarString trailer = m_ParameterDM->getDiscountPctTrailer();
						
						if(!trailer.empty())
						{
							// Austria adds additional "Hausrabatt" to all its items. The rebate is calculated as manufacturer rebate plus Hausrabatt.
							strBR = strBR + trailer.substr(0,1);	// 'H' causes Hausrabatt to be added in KSC-Client
						}
					}
					// only for CH branches add the discount flags at the end of the article hint
					if ((m_branchNo >= 12) && (m_branchNo <= 16))
					{
					// Attention: the first 9 characters of article hint will be displayed as is in the invoice so any changes
					// to it must be checked before with the sales team.
					// to be compatible to HOST, the discount string in the comment field must be 9 characters
					// before we can add additional informations e.g. the discount flags for CH
					for (int intLength = (int)strBR.length(); intLength < 9; intLength++)
					{
						strBR = strBR + " ";
					}

					basar::I18nString strDiscountCalcFrom;
					basar::I18nString strDiscountApplyTo;

					strDiscountCalcFrom.itos(yit.getInt16("discountcalcfrom"));
					strDiscountApplyTo.itos(yit.getInt16("discountapplyto"));

					strBR = strBR + ";" + strDiscountCalcFrom + ";" + strDiscountApplyTo; // should be like "BR23.12  ;0;1" now
					}
					strBR.exchange(".",";");					// change decimal-point to semicolon
					yitPos.setString("itemnote",strBR);			// should be like "BR23;12" now
																// field itemnote will be transferred to field KSC-DB/kdauftragpos/bemerkungen
				}
				else
				{
					// discountPCT is zero
					
					// ... transfer textfield to itemnote. We do not overwrite a rebate cause it is zero.
					// As long as textfield-text does not start with "BR" this text is not interpreted 
					// as a rebate but just transferred to kdauftragpos.bemerkungen
					// This is used e.g. for messages concerning splitted orderitems.
					if ( yit.getString("textfield").length() >0)
					{
						basar::VarString	strItemnote = yit.getString("textfield");	

						// Because CSC-Client crashes on itemtexts longer than 29: truncate it
						if (CSC_POS_ITEMNOTE_LENGTH < strItemnote.length())
						{
							strItemnote.resize(CSC_POS_ITEMNOTE_LENGTH);
						}
						yitPos.setString("itemnote",strItemnote);
					}

				}

				// ADDITIONAL FIELDS FOR FUTURE REBATE HANDLING
				// ToDo: discountpct and kindofdiscount are not in use yet. Leave them empty as long as itemnote is missused for it instead
				//       But leaving them uninitialized causes Flag "NL" to be set in OM orderpositions. (kes, 09.05.14)
				yitPos.setDecimal("discountpct",	decNull);		// unused, init with 0.   Later on: yitPos.setDecimal("discountpct",	decBR);
				yitPos.setInt16  ("kindofdiscount",	0 );			// unused, init with 0.   Later on: kind of BR: for UW in CH always BR (not defined yet)

				
				// DIFFERENT ORDER CONTROL SETTINGS
				// ToDo CH: Settings are valid for CH, maybe other countries prefer different settings
				// These values overwrite the settings made in ae.exe for this ordertype
				yitPos.setInt16("interbranchtransfer",0);			// Verbundabwicklung:				nein
				yitPos.setInt16("subsequentdelivery",1);			// Nachlieferung kompletter Zeilen: ja
				yitPos.setInt16("articlerequirementpurchase",0);	// Dispo für Besorgungsartikel:		nein
				yitPos.setInt16("partialdelivery",0);				// Teillieferung innerhalb Zeile:	nein	--> Dieser Parameter hat vermutlich keine Auswirkung, Teillieferungen in der Zeile finden trotzdem statt:
																	//												Regel: Wenn in der Auftragsart NL gesetzt ist bekommt der Kunde auch Teillieferungen in der Zeile 

				// FIELDS THAT ARE NOT IN USE FOR ABBA-UW
				yitPos.setInt32("idforigin",0); 
				yitPos.setInt32("ordernoorigin",0);	
				yitPos.setInt32("posnoorigin",0);		

				// ONLY FOR DEBUG REASONS:
				#ifdef _DEBUG
				if (LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
				{
					std::stringstream strstream;
					yitPos.toStream(strstream);
					BLOG_TRACE(LoggerPool::loggerDomModules, "### SPION CSCOrderDM::createCSCItem yitPos");
					BLOG_TRACE(LoggerPool::loggerDomModules, strstream.str());
				}
				#endif

				// HAND OVER ITEM TO KSCCLIENT
				kscRet = m_KSCClientDM.addItemCodeRab(yitPos,true);
				// TODO addItemCodeBatch: The batch (Charge) is not trasferred for UW/OM yet.
			}
			else
			{
				// set item properties for CL/SH order or other types
		
				yitPos.setInt32("articleno",yit.getInt32("pzn"));
				if(!nsBasarUtils::BasarUtils::isPropSet(yit,"article_code")) 
				{  // article_code is unset
					basar::I18nString strHelp;
					strHelp.itos(yit.getInt32("pzn"));
					yitPos.setString("articlecode", strHelp);		
				}
				else 
				{
					yitPos.setString("articlecode", yit.getString("article_code"));
				}

				yitPos.setInt32("orderqty",yit.getInt32("orderedqty"));

				yitPos.setInt16("interbranchtransfer",0);			// Verbundabwicklung
				yitPos.setInt16("subsequentdelivery",0);			// Nachlieferung kompletter Zeilen
				yitPos.setInt16("articlerequirementpurchase",0);	// Dispo für Besorgungsartikel
				yitPos.setInt16("partialdelivery",0);				// Teillieferung innerhalb von Zeilen

				yitPos.setString("itemnote",yit.getString("textfield"));

				str=yitExtra.getString("idforigin");
				yitPos.setInt32("idforigin",static_cast<basar::Int32>(str.stoi()));  

				yitPos.setInt32("ordernoorigin",yit.getInt32("customerorderno"));
				yitPos.setInt32("posnoorigin",yit.getInt32("customerposno"));

				if (nsBasarUtils::BasarUtils::isPropSet(yit,"batch"))
				{
					yitPos.setString("batch", yit.getString("batch"));
					kscRet = m_KSCClientDM.addItemCodeBatch(yitPos,true);
				}
				else
				{
					kscRet = m_KSCClientDM.addItem(yitPos,true);
				}
			}

			// Handle return value
			if(::domMod::OK!=kscRet.stateInfo) 
			{
				basar::I18nString strOrderMessage;
				strOrderMessage.itos(yit.getInt32("pzn"));
				QString strMessage="";
				strMessage.append(QCoreApplication::translate("CSCOrderDM","FATAL ERROR when inserting an item of a KSC order!").toLocal8Bit().constData());
				strMessage.append(QCoreApplication::translate("CSCOrderDM"," Article no. ").toLocal8Bit().constData());
				strMessage.append(strOrderMessage.c_str());
				result.message=strMessage.toLocal8Bit().constData();
				return result;
			}

			return basar::appl::HANDLER_OK;
		}
		catch(...)
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::createCSCItemABBA()."); 
			throw;
		}
	}

//*************************************** setBatchType ***********************************************
basar::Int32 CSCOrderDM::getBatchType(const basar::VarString& datasourcetype)
{

	// for different batchtypes see pxverbund pxocldef.h:
	/*----------------------------------------------------------------------------*/
	/*  Enumeration of origintypes                                                */
	/*----------------------------------------------------------------------------*/
	enum ORIGINTYPE
	{
		cORTYPE_UNKNOWN   = 0,                //
		cORTYPE_AUXILIARY    ,                // type auxiliary                              1
		cORTYPE_CLEARING,                     // type clearing                               2
		cORTYPE_INSTALLER,                    // type stock installer                        3
		cORTYPE_STOCK,                        // type stock                                  4
		cORTYPE_STOCK_DISTRIBUTION,           // type stock distibution                      5
		cORTYPE_INDUSTRIAL_RETURN,            // type industrial return                      6
		cORTYPE_PARENT_COMPANY,               // type parent company                         7
		cORTYPE_KOMMI_ERROR,                  // type kommi errror                           8
		cORTYPE_TRANSFER,                     // type ueberweiser = UE                       9
		cORTYPE_PIN,                          // type pin order                              10
		cORTYPE_BATCH_TRANSFER,               // type ueberweiser aus Batch = UW             11
		cORTYPE_GLOBUS_ORDER,                 // type Globus Order = GO                      12
		cORTYPE_EDIFAC_ORDER,                 // type Edifac Order = ES                      13
		cORTYPE_FR_CUSTOMER_ORDER,            // type French Prewhole Customer Order    = CO 14
		cORTYPE_FR_INDUSTRIAL_RETURN,         // type French Prewhole Industrial Return = RI 15
		cORTYPE_BATCH_CALLBACK,          	  // type Batch Callback (Chargenrueckrufe) = BC 16
		cORTYPE_DIRECT_CLOSURE,               // type with direct order close = DC           17
		cORTYPE_IBT_EXCHANGE                  // type Verbundumlistungen = VU                18
	};

//	static const char* ORIGINNAMES[] = {"VB", "CL", "ER", "LA", "LV", "IR","SH","KF","UE","PI","UW","GO","ES", "CO", "RI", "BC", "DC", "VU"};

	basar::Int32 BatchType = cORTYPE_UNKNOWN;

	if (DATASRC_CL == datasourcetype)
	{
		BatchType=2;
	}
	else if (DATASRC_SH == datasourcetype)
	{
		BatchType=7;
	}
	else if (DATASRC_MW == datasourcetype)
	{
		BatchType=7;
	}
	else if (DATASRC_UW == datasourcetype)
	{
		BatchType=11;					
	}
	else if (DATASRC_UE == datasourcetype)
	{
		BatchType=9;
	}
	else if (DATASRC_ER == datasourcetype)
	{
		BatchType=3;
	}
	else if (DATASRC_IR == datasourcetype)  // WADI
	{
		BatchType=6;
	}
	else if (DATASRC_BC == datasourcetype)
	{  
		BatchType=16;
	}
	else if (DATASRC_VU == datasourcetype)
	{  
		BatchType = cORTYPE_IBT_EXCHANGE;
	}
	return BatchType;

}// getBatchType


	//*************************************** isTransferOrder ***********************************************
	//
	//  Description:	Tells us, whether this order is of a ordertype, that is treated as a transfer order (Überweiser, z.B. UW) 
	//	Parameter  :	yit : Contains order related data 
	//	Returns    :	true : yes, this order is a Transfer order
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isTransferOrder(const AccessorPropertyTable_YIterator yit) 
	{
		const char *function="CSCOrderDM::isTransferOrder(yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if (isUWOrder(yit) || isUNOrder(yit) || isMDOrder(yit)|| isOMOrder(yit) || isHZOrder(yit) || isAKOrder(yit) || isCMOrder(yit) || isGMOrder(yit))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isTransferOrder Overload*************************************
	//
	//  Description:	Tells us, whether this order is of a ordertype, that is treated as a transfer order (Überweiser, z.B. UW) 
	//	Parameter  :	ordertype : Contains ordertype-string, e.g. "UW", "NO" etc.
	//	Returns    :	true : yes, this order is a Transfer order
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isTransferOrder(const basar::I18nString ordertype)
	{

		const char *function="CSCOrderDM::isTransferOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if (isUWOrder(ordertype) || isUNOrder(ordertype) || isMDOrder(ordertype)|| isOMOrder(ordertype) || isHZOrder(ordertype) || isAKOrder(ordertype) || isCMOrder(ordertype) || isGMOrder(ordertype))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isUWOrder ***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype UW (= Ueberweiser / transfer order)
	//	Parameter  :	yit : Contains order related data 
	//	Returns    :	true : yes, this order is of ordertype UW
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isUWOrder(const AccessorPropertyTable_YIterator yit) {

		const char *function="CSCOrderDM::isUWOrder(yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try {
			if(!nsBasarUtils::BasarUtils::isPropSet(yit,"ordertype")) 
			{  
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR( LoggerPool::loggerDomModules, str.str().c_str() );
				throw;
			}

            basar::VarString orderType = yit.getString("ordertype").substr(0,2);
			return ORDERTYPE_UW == orderType;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}// catch
	}// isUWOrder

	//*************************************** isUWOrder Overload*************************************
	//
	//  Description:	Tells us, whether this order is of ordertype UW (= Ueberweiser / transfer order)
	//	Parameter  :	ordertype : Contains ordertype-string, e.g. "UW", "NO" etc.
	//	Returns    :	true : yes, this order is of ordertype UW
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isUWOrder(const basar::I18nString ordertype) {

		const char *function="CSCOrderDM::isUWOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try {

			if(ORDERTYPE_UW == ordertype){
				return true;
			}
			else 
			{
				return false;
			}
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}// catch
	}// isUWOrder


	//*************************************** isOMOrder ***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype OM (= OMG order)
	//	Parameter  :	yit : Contains order related data 
	//	Returns    :	true : yes, this order is of ordertype OM
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isOMOrder(const AccessorPropertyTable_YIterator yit)
	{
		const char *function="CSCOrderDM::isOMOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(!nsBasarUtils::BasarUtils::isPropSet(yit,"ordertype")) 
			{  
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR( LoggerPool::loggerDomModules, str.str().c_str() );

				throw;
			}

			if(ORDERTYPE_OM == yit.getString("ordertype").substr(0,2))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isOMOrder Overload***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype OM (= OMG order)
	//	Parameter  :	ordertype : Contains ordertype-string, e.g. "UW", "NO" etc.
	//	Returns    :	true : yes, this order is of ordertype OM
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isOMOrder(const basar::I18nString ordertype) 
	{
		const char *function="CSCOrderDM::isOMOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(ORDERTYPE_OM == ordertype)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isUNOrder ***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype UN (= UN / PRISMA order)
	//	Parameter  :	yit : Contains order related data 
	//	Returns    :	true : yes, this order is of ordertype UN
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isUNOrder(const AccessorPropertyTable_YIterator yit)
	{
		const char *function="CSCOrderDM::isUNOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(!nsBasarUtils::BasarUtils::isPropSet(yit,"ordertype")) 
			{  
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR( LoggerPool::loggerDomModules, str.str().c_str() );

				throw;
			}

			if(ORDERTYPE_UN == yit.getString("ordertype").substr(0,2))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isUNOrder Overload***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype UN (= UN / PRISMA order)
	//	Parameter  :	ordertype : Contains ordertype-string, e.g. "UW", "NO", "UN" etc.
	//	Returns    :	true : yes, this order is of ordertype UN
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isUNOrder(const basar::I18nString ordertype) 
	{
		const char *function="CSCOrderDM::isUNOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(ORDERTYPE_UN == ordertype)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isMDOrder ***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype MD 
	//	Parameter  :	yit : Contains order related data 
	//	Returns    :	true : yes, this order is of ordertype MD
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isMDOrder(const AccessorPropertyTable_YIterator yit) 
	{
		const char *function="CSCOrderDM::isMDOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(!nsBasarUtils::BasarUtils::isPropSet(yit,"ordertype")) 
			{  
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR( LoggerPool::loggerDomModules, str.str().c_str() );

				throw;
			}

			if(ORDERTYPE_MD == yit.getString("ordertype").substr(0,2))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** isMDOrder Overload***********************************************
	//
	//  Description:	Tells us, whether this order is of ordertype MD 
	//	Parameter  :	ordertype : Contains ordertype-string, e.g. "UW", "NO", "UN", "MD" etc.
	//	Returns    :	true : yes, this order is of ordertype MD
	//                  false: no, it is not
	//	
	//***************************************************************************************************************
	bool CSCOrderDM::isMDOrder(const basar::I18nString ordertype) 
	{
		const char *function="CSCOrderDM::isMDOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(ORDERTYPE_MD == ordertype)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	bool CSCOrderDM::isHZOrder(const AccessorPropertyTable_YIterator yit) 
	{
		const char *function="CSCOrderDM::isHZOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(!nsBasarUtils::BasarUtils::isPropSet(yit,"ordertype")) 
			{  
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR( LoggerPool::loggerDomModules, str.str().c_str() );

				throw;
			}

			if(ORDERTYPE_HZ == yit.getString("ordertype").substr(0,2)){
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	bool CSCOrderDM::isHZOrder(const basar::I18nString ordertype) 
	{
		const char *function="CSCOrderDM::isHZOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(ORDERTYPE_HZ == ordertype)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	bool CSCOrderDM::isAKOrder(const AccessorPropertyTable_YIterator yit) 
	{
		const char *function="CSCOrderDM::isAKOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(!nsBasarUtils::BasarUtils::isPropSet(yit,"ordertype")) 
			{  
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR( LoggerPool::loggerDomModules, str.str().c_str() );

				throw;
			}

			if(ORDERTYPE_AK == yit.getString("ordertype").substr(0,2))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	bool CSCOrderDM::isAKOrder(const basar::I18nString ordertype) 
	{
		const char *function="CSCOrderDM::isAKOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			if(ORDERTYPE_AK == ordertype)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	bool CSCOrderDM::isGMOrder(const AccessorPropertyTable_YIterator yit)
	{
		const char* function = "CSCOrderDM::isGMOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try
		{
			if (!nsBasarUtils::BasarUtils::isPropSet(yit, "ordertype"))
			{
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR(LoggerPool::loggerDomModules, str.str().c_str());

				throw;
			}

			if (ORDERTYPE_GM == yit.getString("ordertype").substr(0, 2))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch (...)
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function);
			throw;
		}
	}

	bool CSCOrderDM::isGMOrder(const basar::I18nString ordertype)
	{
		const char* function = "CSCOrderDM::isGMOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try
		{
			if (ORDERTYPE_GM == ordertype)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch (...)
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function);
			throw;
		}
	}

	bool CSCOrderDM::isCMOrder(const AccessorPropertyTable_YIterator yit)
	{
		const char* function = "CSCOrderDM::isCMIOrder(Yit).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try
		{
			if (!nsBasarUtils::BasarUtils::isPropSet(yit, "ordertype"))
			{
				// ordertype is not set
				std::stringstream str;
				str << function << ": yit without ordertype";
				BLOG_ERROR(LoggerPool::loggerDomModules, str.str().c_str());

				throw;
			}

			if (ORDERTYPE_DG == yit.getString("ordertype").substr(0, 2))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch (...)
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function);
			throw;
		}
	}

	bool CSCOrderDM::isCMOrder(const basar::I18nString ordertype)
	{
		const char* function = "CSCOrderDM::isCMOrder(String).";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try
		{
			if (ORDERTYPE_DG == ordertype)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch (...)
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function);
			throw;
		}
	}

	//*************************************** isCustomerEnabled ******************************************************
	bool CSCOrderDM::isCustomerEnabled(basar::Int32 pharmacyno) 
	{
		const char *function="CSC_Batches::isCustomerEnabled().";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			basar::appl::EventReturnType result(basar::appl::HANDLER_ERROR);
			//--- create yit string:
			basar::I18nString strYit = "";
			basar::I18nString strYit1 = "";
			// set branchno:
			strYit1.format("%s=%d;", "branchno", m_branchNo);
			strYit.append(strYit1);						
			// set order:
			strYit1.format("%s=%d;", "pharmacyno", pharmacyno);		
			strYit.append(strYit1);			
			AccessorPropertyTable_YIterator yitSelect = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
		
			AccessorPropertyTableRef PropTabRef=m_batchOrderDM.SelCustomerBlocked(yitSelect);
			if(PropTabRef.empty()) 
			{  
				return true;  // customer is blocked only if flag is set
			}// if(PropTabRef.empty())
			if("1"==PropTabRef.begin().getString("blocked"))
				return false;
			return true;  // customer is blocked only if flag is set
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

	//*************************************** changeBatchOrderStatus ***********************************************
	//
	// Descr.:	Changes the OrderStatus from "FE" to "ER".
	//          and adds ksc-orderno to referencefield 
	//			and set orderacquisitiondate 
	//
	//***************************************************************************************************************
	basar::appl::EventReturnStruct CSCOrderDM::changeBatchOrderStatus(	basar::Int32 OrderNo, 
																		basar::Int32 OrderAcquisitionDate, 
																		basar::Int32 OrderNoKsc) 
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CSCOrderDM::changeBatchOrderStatus().");

		try 
		{
			basar::appl::EventReturnType result(basar::appl::HANDLER_ERROR);
			AccessorPropertyTable_YIterator yitUpdate;

			result = buildYitForUpdate(OrderNo, OrderAcquisitionDate, OrderNoKsc, yitUpdate);
			if(result.ret == basar::appl::HANDLER_ERROR) 
			{
				return result;
			}

			// Accessor SaveBatchOrderByPattern defined in batchorderacc.cpp
			m_batchOrderDM.saveBatchOrderByPattern(yitUpdate); //+++ besser saveBatchOrderStatuschange???
			return basar::appl::HANDLER_OK;
		}
		catch(...)
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, "CSCOrderDM::changeBatchOrderStatus()."); 
			throw;
		}
	}

	//*************************************** buildYitForUpdate ***********************************************
	//
	// Descr.:	Builds a Yit needed for updating.
	//			(All properties are of type string)
	// Returns:	Yit-String
	//
	//***************************************************************************************************************
	basar::appl::EventReturnStruct CSCOrderDM::buildYitForUpdate(	basar::Int32 OrderNo, 
																	basar::Int32 OrderAcquisitionDate,
																	basar::Int32 OrderNoKsc,
																	AccessorPropertyTable_YIterator &yitUpdate) 
	{
		const char *function="CSCOrderDM::buildYitForUpdate().";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);

		try 
		{
			basar::appl::EventReturnType result(basar::appl::HANDLER_ERROR);

			//--- create yit string:
			basar::I18nString strYit = "";
			basar::I18nString strYit1 = "";
			
			// set branchno:
			strYit1.format("%s=%d;", "branchno", m_branchNo);
			strYit.append(strYit1);						
			
			// set order:
			strYit1.format("%s=%d;", "orderno", OrderNo);		
			strYit.append(strYit1);				

			// set status FE:  (ToDo: comment wrong??! )
			strYit1.format("%s=%d;", "status", CSC_Batches::ER);
			strYit.append(strYit1);			
	
			// set orderacquisitiondate:
			strYit1.format("%s=%d;", "orderacquisitiondate", OrderAcquisitionDate);  
			strYit.append(strYit1);				

			// set ordernoksc:
			strYit1.format("%s=%d;", "ordernoksc", OrderNoKsc);  
			strYit.append(strYit1);				

			yitUpdate = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
			return basar::appl::HANDLER_OK;
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw;
		}
	}

}
}
