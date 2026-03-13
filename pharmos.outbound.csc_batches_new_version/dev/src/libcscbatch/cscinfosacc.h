//----------------------------------------------------------------------------
/*! \file	cscinfosacc.h
 *  \brief  string builder declarations for order-accessor
 *  \author Beatrix Trömel
 *  \date   01.09.2008
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CSCINFOACC_H
#define GUARD_CSCINFOACC_H

#include <libbasardbaspect.h>
#include "libcscbatch_loggerpool.h"

//----------------------------------------------------------------------------
//! namespace for xyz-accessor (each accessor has to have it´s own namespace!)
namespace cscinfosacc {
	using basar::db::aspect::SQLStringBuilder;

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelOrderTypesByOrderNo)
	END_BUILDER_DECLARATION
}// namespace
#endif	//GUARD
