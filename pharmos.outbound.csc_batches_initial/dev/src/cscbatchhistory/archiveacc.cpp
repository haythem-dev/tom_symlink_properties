#include "archiveacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarcmnutil_i18nstring.h>

namespace archiveacc 
{
	BUILDER_DEFINITION(deleteBatchOrderHeadsArchive)
	ENSURE_PROPERTIES_ARE_SET(deleteBatchOrderHeadsArchive,"branchno;deletedateforarchive;")

	void deleteBatchOrderHeadsArchive::buildSQLString()
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerUseCases, "batchorderarchiveacc::deleteBatchOrderHeadsArvchive::buildSQLString().");
		try 
		{		
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "DELETE FROM aorderhead WHERE branchno=#branchno# AND to_char(archivedate, '%Y%m%d')::integer < #deletedateforarchive# ;";
			
			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "batchorderarchiveacc::deleteBatchOrderHeadsArvchive()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	BUILDER_DEFINITION(deleteBatchOrderPosArchive)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(deleteBatchOrderPosArchive,"branchno;deletedateforarchive;")

	void deleteBatchOrderPosArchive::buildSQLString() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerUseCases, "batchorderarchiveacc::deleteBatchOrderPosArvchive::buildSQLString().");

		try 
		{		
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "DELETE FROM aorderpos WHERE branchno=#branchno# AND to_char(archivedate, '%Y%m%d')::integer < #deletedateforarchive# ;";

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "batchorderarchiveacc::deleteBatchOrderPosArvchive()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	BUILDER_DEFINITION(deleteBatchOrderProposalsArchive)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(deleteBatchOrderProposalsArchive,"branchno;deletedateforarchive;")

	void deleteBatchOrderProposalsArchive::buildSQLString() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerUseCases, "batchorderarchiveacc::deleteBatchOrderProposalsArvchive::buildSQLString().");

		try 
		{		
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "DELETE FROM aorderproposal WHERE branchno=#branchno# AND to_char(archivedate, '%Y%m%d')::integer < #deletedateforarchive# ;";

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "batchorderarchiveacc::deleteBatchOrderProposalsArvchive()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	BEGIN_ACCESSOR_DEFINITION("ArchiveAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 deletedateforarchive; \
			")

			ACCESS_METHOD("deleteBatchOrderHeadsArchive")
			SQL_BUILDER_CREATE(deleteBatchOrderHeadsArchive)
			SQL_BUILDER_PUSH_BACK(deleteBatchOrderHeadsArchive)

			ACCESS_METHOD("deleteBatchOrderPosArchive")
			SQL_BUILDER_CREATE(deleteBatchOrderPosArchive)
			SQL_BUILDER_PUSH_BACK(deleteBatchOrderPosArchive)

			ACCESS_METHOD("deleteBatchOrderProposalsArchive")
			SQL_BUILDER_CREATE(deleteBatchOrderProposalsArchive)
			SQL_BUILDER_PUSH_BACK(deleteBatchOrderProposalsArchive)
			
	END_ACCESSOR_DEFINITION			
}

