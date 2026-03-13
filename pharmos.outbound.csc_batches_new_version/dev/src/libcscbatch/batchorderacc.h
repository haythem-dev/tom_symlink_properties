//-------------------------------------------------------------------------------------------------//
/*! \file	batchorderacc.cpp
 *  \brief  string builder definition for order-accessor + registration of order-accessor
 *  \author	Beatrix Trömel
 *  \date   18.07.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_BATCHORDERACC_H
#define GUARD_BATCHORDERACC_H

#include <libbasardbaspect.h>

namespace batchorderacc {
	using basar::db::aspect::SQLStringBuilder;

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelBatchOrdersByPattern)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(SaveBatchOrderByPattern)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(SaveBatchOrderStatusChange)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(SaveBatchOrderForRelease)
	END_BUILDER_DECLARATION

}// batchorderacc
#endif	//GUARD
