//----------------------------------------------------------------------------
/*! \file	customerblockedacc.h
 *  \brief  string builder declarations for accessor to kundeschalter 
 *  \author Beatrix Trömel
 *  \date   17.10.2008
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CUSTOMERBLOCKEDACC_H
#define GUARD_CUSTOMERBLOCKEDACC_H

#include <libbasardbaspect.h>
#include "libcscbatch_loggerpool.h"

//----------------------------------------------------------------------------
//! namespace for xyz-accessor (each accessor has to have it´s own namespace!)
namespace customerblockedacc {
	using basar::db::aspect::SQLStringBuilder;

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelCustomerBlocked)
	END_BUILDER_DECLARATION
}// namespace
#endif	//GUARD
