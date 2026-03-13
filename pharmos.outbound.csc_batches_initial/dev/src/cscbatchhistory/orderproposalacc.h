//-------------------------------------------------------------------------------------------------//
/*! \file	orderproposalacc.h
 *  \brief  implemtation of accessor (orderproposal)
 *  \author Xin Zhou
 *  \date   30.06.2013
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_ORDERPROPOSALACC_H
#define GUARD_ORDERPROPOSALACC_H

#include <libbasardbaspect.h>
#include "loggerpool.h"

namespace orderproposalacc {

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(saveArchiveOrderProposal)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for deleting data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(deleteOrderProposal)
	END_BUILDER_DECLARATION


}// namespace
#endif	//GUARD
