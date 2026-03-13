//-------------------------------------------------------------------------------------------------//
/*! \file	csciniacc.cpp
 *  \brief  string builder definition for order-accessor + registration of order-accessor
 *  \author	Beatrix Trömel
 *  \date   29.01.2009
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSCINIACC_H
#define GUARD_CSCINIACC_H

#include <libbasardbaspect.h>

namespace csciniacc {
	using basar::db::aspect::SQLStringBuilder;

	//---------------------------------------------------------------------------
	//! class for selecting data by id \n final class
	BEGIN_QUERY_BUILDER_DECLARATION(SelEnterByBranch)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(SaveEnterByBranch)
	END_BUILDER_DECLARATION

	//---------------------------------------------------------------------------
	//! class for saving data by id \n final class
	BEGIN_WRITE_BUILDER_DECLARATION(DeleteEnterByBranch)
	END_BUILDER_DECLARATION
}// csciniacc
#endif	//GUARD
