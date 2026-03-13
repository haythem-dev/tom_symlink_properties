//-------------------------------------------------------------------------------------------------//
/*! \file	batchorderposacc.h
 *  \brief  accessor for order positions
 *  \author Beatrix Trömel
 *  \date   09.06.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_BATCHORDERPOSACC_H
#define GUARD_BATCHORDERPOSACC_H

#include <libbasardbaspect.h>
#include "libcscbatch_loggerpool.h"

//----------------------------------------------------------------------------
//! namespace for xyz-accessor (each accessor has to have it´s own namespace!)
namespace batchorderposacc {
	using basar::db::aspect::SQLStringBuilder;

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelBatchOrderPosByBranchNoAndOrderNo)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(SaveBatchOrderPos)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelArticleByBranchNoAndArticleNo)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelPZNByArticleCode)
	END_BUILDER_DECLARATION
}// batchorderposacc
#endif	//GUARD
