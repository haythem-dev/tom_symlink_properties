#ifndef GUARD_BATCHORDERACC_H
#define GUARD_BATCHORDERACC_H

#include <libbasardbaspect.h>
#include "loggerpool.h"

namespace copybatchorderacc 
{
	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(saveArchiveBatchOrderHeads)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for deleting data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(deleteBatchOrderHeads)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(saveArchiveBatchOrderPos)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for deleting data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(deleteBatchOrderPos)
	END_BUILDER_DECLARATION


    //---------------------------------------------------------------------------
	//! class for selecting data \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(selectBatchOrderHead)
	END_BUILDER_DECLARATION
    
    //---------------------------------------------------------------------------
	//! class for selecting data \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(selectBatchOrderPos)
	END_BUILDER_DECLARATION
}

#endif
