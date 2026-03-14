//----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  global definitions for obst
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------------------------//

#ifndef GUARD_META_CLASS_MACRO_DEFINITIONS_H
#define GUARD_META_CLASS_MACRO_DEFINITIONS_H

//----------------------------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>

//----------------------------------------------------------------------------------------------//
// 'meta' class macro
//----------------------------------------------------------------------------------------------//
#define CLASSNAME_DECL						   static const basar::I18nString s_CLASSNAME;
#define CLASSNAME_DEF( CLASS )	               const basar::I18nString CLASS::s_CLASSNAME = #CLASS;
#define METHODNAME_DEF( CLASS, FUNNAME )       static basar::ConstString fun = #CLASS"::"#FUNNAME"()";

//----------------------------------------------------------------------------------------------//
#endif
