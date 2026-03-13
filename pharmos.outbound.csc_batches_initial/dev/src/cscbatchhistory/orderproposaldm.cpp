//-------------------------------------------------------------------------------------------------//
/*! \file	orderproposaldm.cpp
 *  \brief  implemtation of domain module (orderproposal)
 *  \author Xin Zhou
 *  \date   30.06.2013
 */
 //-------------------------------------------------------------------------------------------------//

 //**************************************************************************************************
 // includes
 //**************************************************************************************************
#include "orderproposaldm.h"

namespace domMod 
{
	//**************************************************************************************************
	//         C'tor / D'tor
	//**************************************************************************************************
	OrderProposalDM::OrderProposalDM() : m_Initialized(0) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::OrderProposalDM().");
	}

	OrderProposalDM::~OrderProposalDM() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::~OrderProposalDM().");
	}

	//**************************************************************************************************
	//         methods
	//**************************************************************************************************
	//*************************************** init *****************************************************
	void OrderProposalDM::init(const basar::db::aspect::ConnectionRef activeConn) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::init().");
		try 
		{
			if (0 == m_Initialized) 
			{
				m_OrderProposalACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("AccOrderProposalInstance", "OrderProposalAcc", activeConn);
			}
			++m_Initialized;
		}// try
		catch (...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::init().");
			throw;
		}// catch
	}// init

	//*************************************** shutdown *****************************************************
	void OrderProposalDM::shutdown() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::shutdown().");
		try 
		{
			// release accessor instance, if instance was created WITH storing (default: without) in instance list
			// basar::db::aspect::Manager::getInstance().releaseAccessorInstance("AccInstance");

			if (0 == m_Initialized)
				return;

			--m_Initialized;
			if (0 == m_Initialized) 
			{
				if (!m_OrderProposalACC.getPropertyTable().isNull())
					m_OrderProposalACC.getPropertyTable().clear();
				m_OrderProposalACC.reset();
			}
		}
		catch (...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::shutdown().");
			throw;
		}
	}// shutdown

	//*************************************** copyOrderProposal *****************************************************	
	bool OrderProposalDM::copyOrderProposal(const basar::db::aspect::AccessorPropertyTable_YIterator yit) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::copyOrderProposal().");

		//xz: check if the parameter for archiving is big enough (>=2)
		basar::Int32 ageforproposal = yit.getInt32("minimumagefororderproposal");
		if (ageforproposal < 2)
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "The minimum age for archiving orderproposal is set too small");
			return false;
		}

		try 
		{
			m_OrderProposalACC.beginTransaction();

			basar::db::aspect::ExecuteResultInfo res;

			/*
			//xz: check the value of yit in debugausgabe
			std::stringstream ss;
			yit.toStream(ss);
			ss;
			*/

			// PAY ATTENTION HERE!  
			//--- copy order proposal:
			basar::db::aspect::ExecuteResultInfo resPos = m_OrderProposalACC.execute("saveArchiveOrderProposal", yit);
			if (resPos.hasError()) 
			{  
				m_OrderProposalACC.rollbackTransaction();
				return false;
			}

			res = m_OrderProposalACC.execute("deleteOrderProposal", yit);
			if (res.hasError() != resPos.hasError()) 
			{
				m_OrderProposalACC.rollbackTransaction();
				return false;
			}

			m_OrderProposalACC.commitTransaction();
			return true;
		}
		catch (...) {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::copyOrderProposal().");
			throw;
		}
	}// copyOrderProposal  


	basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalDM::getSearchYit()
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::getSearchYit().");

		return getSearchYIteratorPtr()->get();
	}

	searchyiterator::SearchYIteratorPtr OrderProposalDM::getSearchYIteratorPtr()
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::getSearchYIteratorPtr().");

		if (0 == m_SearchYIterator.get()) 
		{
			m_SearchYIterator = searchyiterator::SearchYIteratorPtr(
				new searchyiterator::SearchYIterator("int16 branchno; int32 minimumagefororderproposal",
					cscbatchhistory::LoggerPool::loggerDomModules
				)
			);
		}
		return m_SearchYIterator;
	}

	//*************************************** map2SearchYit *****************************************************
	/*
	basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalDM::map2SearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalDM::map2SearchYit().");

		// to check the yit in debug modus
			std::stringstream ss;
			yit.toStream(ss);
			ss;

		basar::Date lessThanProposalDate;
		basar::Int16 branchNo = yit.getString( "branchno" ).stoi();
		lessThanProposalDate.fromString("yyyymmdd", yit.getString( "ltorderproposal" ));

		basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_OrderProposalACC.getPropertyTable().insert( basar::FOR_CLEAN );

		yitSearch.setInt16( "branchno", branchNo );
		yitSearch.setDate( "ltorderproposal", lessThanProposalDate );

		return yitSearch;
	}
	*/

} // namespace domMod

