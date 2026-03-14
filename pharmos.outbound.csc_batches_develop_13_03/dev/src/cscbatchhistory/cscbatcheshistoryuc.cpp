#include "cscbatcheshistoryappl.h"  // don't include cyclicly, so put this into cpp
#include "cscbatcheshistoryuc.h"

#include "properties_definitions.h"

namespace cscbatchhistory 
{
	namespace useCase 
	{
		CscBatchesHistoryUC::CscBatchesHistoryUC()
		{
			BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::CscBatchesHistoryUC().");
		}

		CscBatchesHistoryUC::~CscBatchesHistoryUC()
		{
			BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::~CscBatchesHistoryUC().");
		}

		basar::appl::EventReturnType CscBatchesHistoryUC::run(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
			try
			{
				basar::appl::EventReturnType result(basar::appl::HANDLER_ERROR);

				if (!yit.isContainedAndSet("branchno"))
				{
					basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'branchno'";
					BLOG_ERROR(LoggerPool::loggerUseCases, msg);
					return(result);
				}

				if (!yit.isContainedAndSet("lessthanorderdate"))
				{
					basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'lessthanorderdate'";
					BLOG_ERROR(LoggerPool::loggerUseCases, msg);
					return(result);
				}

				if (!yit.isContainedAndSet("deletedateforarchive"))
				{
					basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'deletedateforarchive'";
					BLOG_ERROR(LoggerPool::loggerUseCases, msg);
					return(result);
				}

				//--- initialize domain modules with opened connection: 
				m_ArchiveDM.init(getManager().getConnectionCsc());
				m_CopyBatchOrdersDM.init(getManager().getConnectionCsc());
				m_OrderHeadProtocolDM.init(getManager().getConnectionCsc());
				m_OrderPosProtocolDM.init(getManager().getConnectionCsc());

				//Delete old datasets in archive table 
				basar::db::aspect::AccessorPropertyTable_YIterator yitSearchArchive;
				yitSearchArchive = m_ArchiveDM.getSearchYit();

				yitSearchArchive.setInt16("branchno", static_cast<basar::Int16>(yit.getString("branchno").stoi()));
				yitSearchArchive.setInt32("deletedateforarchive", yit.getString("deletedateforarchive").stoi());

				if (!m_ArchiveDM.deleteOrderProposalArchive(yitSearchArchive))
				{
					result.message = "Error while deleting aorderproposal!";
					BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
					m_ArchiveDM.shutdown();
					return result;
				}

				if (!m_ArchiveDM.deleteOrderPosArchive(yitSearchArchive))
				{
					result.message = "Error while deleting aorderpos!";
					BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
					m_ArchiveDM.shutdown();
					return result;
				}

				if (!m_ArchiveDM.deleteOrderHeadArchive(yitSearchArchive))
				{
					result.message = "Error while deleting aorderhead!";
					BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
					m_ArchiveDM.shutdown();
					return result;
				}

				//Adaptation from yitMain to yitSearchCopyBatchOrder to archive orderpos & orderhead
				basar::db::aspect::AccessorPropertyTable_YIterator yitSearchCopyBatchOrder;
				yitSearchCopyBatchOrder = m_CopyBatchOrdersDM.getSearchYit();

				yitSearchCopyBatchOrder.setInt16("branchno", static_cast<basar::Int16>(yit.getString("branchno").stoi()));
				yitSearchCopyBatchOrder.setInt32("lessthanorderdate", yit.getString("lessthanorderdate").stoi());

				//Get orders that should be archived in the next step and save them as backups.
				m_OrderHeadProtocolDM.setBackup(m_CopyBatchOrdersDM.getOrderHeadsWhichShouldBeArchived(yitSearchCopyBatchOrder));
				m_OrderPosProtocolDM.setBackup(m_CopyBatchOrdersDM.getOrderPositionsWhichShouldBeArchived(yitSearchCopyBatchOrder));

				//--- copy orders and positions into archive tables:
				if (!m_CopyBatchOrdersDM.copyBatchOrders(yitSearchCopyBatchOrder))
				{
					result.message = "Error when archiving batch orders!";
					BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
					m_CopyBatchOrdersDM.shutdown();
					return result;
				}

				//Protocol it
				m_OrderPosProtocolDM.saveOrderPosProtocol();
				m_OrderHeadProtocolDM.saveOrderHeadProtocol();
				/*
				//Delete old protocols
				basar::Date date = basar::Date::getCurrent();
				date.addDays( -static_cast<basar::Int16>(yit.getString("minagefororderprotocol").stoi()) );

				m_OrderPosProtocolDM.getSearchYit().setInt16( cscbatchhistory::properties::BRANCHNO, static_cast<basar::Int16>(yit.getString("branchno").stoi()));
				m_OrderPosProtocolDM.getSearchYit().setDateTime( cscbatchhistory::properties::MINAGEFORORDERPROTOCOL, dt );

				m_OrderHeadProtocolDM.getSearchYit().setInt16( cscbatchhistory::properties::BRANCHNO, static_cast<basar::Int16>(yit.getString("branchno").stoi()));
				m_OrderHeadProtocolDM.getSearchYit().setDateTime( cscbatchhistory::properties::MINAGEFORORDERPROTOCOL, dt );
				*/
				//Delete old protocols
				basar::Date date = basar::Date::getCurrent();
				date.addDays(-static_cast<basar::Int16>(yit.getString("minagefororderprotocol").stoi()));

				m_OrderPosProtocolDM.getSearchYit().setInt16("branchno", static_cast<basar::Int16>(yit.getString("branchno").stoi()));
				m_OrderPosProtocolDM.getSearchYit().setString("minagefororderprotocol", date.toString("YYYYmmdd"));

				m_OrderHeadProtocolDM.getSearchYit().setInt16("branchno", static_cast<basar::Int16>(yit.getString("branchno").stoi()));
				m_OrderHeadProtocolDM.getSearchYit().setString("minagefororderprotocol", date.toString("YYYYmmdd"));


				//Attention! First positions, then the headers!
				m_OrderPosProtocolDM.deleteOrderPosProtocol(m_OrderPosProtocolDM.getSearchYit());
				m_OrderHeadProtocolDM.deleteOrderHeadProtocol(m_OrderHeadProtocolDM.getSearchYit());

				//--- release domain module
				m_OrderPosProtocolDM.shutdown();
				m_OrderHeadProtocolDM.shutdown();
				m_CopyBatchOrdersDM.shutdown();

				if (yit.isContainedAndSet("minimumagefororderproposal"))
				{
					//xz:--- initialize domain modules with opened connection for orderproposal
					m_OrderProposalDM.init(getManager().getConnectionCsc());

					//xz: Adaptation from yitMain(string type) to yitSearchOrderProposal to archive orderproposal(integer type)
					basar::db::aspect::AccessorPropertyTable_YIterator yitSearchOrderProposal;
					yitSearchOrderProposal = m_OrderProposalDM.getSearchYit();
					yitSearchOrderProposal.setInt16("branchno", static_cast<basar::Int16>(yit.getString("branchno").stoi()));
					yitSearchOrderProposal.setInt32("minimumagefororderproposal", yit.getString("minimumagefororderproposal").stoi());

					//xz:--- copy ordersproposal into archive tables:
					if (!m_OrderProposalDM.copyOrderProposal(yitSearchOrderProposal))
					{
						result.message = "Error when archiving orderproposal!";
						BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
						m_OrderProposalDM.shutdown();
						return result;
					}

					//--- release domain module
					m_OrderProposalDM.shutdown();
				}

				return basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				e.what();
				BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
				throw;
			}
			catch (...)
			{
				BLOG_ERROR(LoggerPool::loggerUseCases, "CscBatchesHistoryUC::run().");
				throw;
			}
		}

	}
}
