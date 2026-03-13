//----------------------------------------------------------------------------
/*! \file
 *  \brief  Re-Interpret 0 <-> GEP, 1 <-> AGP between GUI and model
*   for the discount flags used in CH
 *  \author Olaf Wasmuth
 *  \date   18.10.2022
 */
 //----------------------------------------------------------------------------
#ifndef GUARD_DISCOUNTFLAGS_TRANSFORMER_H
#define GUARD_DISCOUNTFLAGS_TRANSFORMER_H

// #pragma warning (push)
// #pragma warning (disable: 4231)
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarguitie.h>
#include <libbasarproperty.h>

namespace basar {
	namespace db {
		namespace aspect {
			class AccessorPropertyTable_YIterator;
		}
	}
}

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
	namespace viewConn
	{
		typedef basar::gui::tie::GuiPropertyTable_YIterator			GuiPropTabYit;
		typedef basar::db::aspect::AccessorPropertyTable_YIterator	AccPropTabYit;

		//-------------------------------------------------------------------------------------------------//
		// class declaration
		//-------------------------------------------------------------------------------------------------//
		class DiscountFlagsTransformer : public basar::gui::tie::Transformer
		{

		public:
			DiscountFlagsTransformer();
			DiscountFlagsTransformer(const basar::I18nString pPropertyName, const basar::I18nString translatedName);
			~DiscountFlagsTransformer();

			virtual void 				RightToLeft(GuiPropTabYit guiIt, const AccPropTabYit accIt);
			virtual void 				LeftToRight(const GuiPropTabYit guiIt, AccPropTabYit accIt);

			virtual boost::shared_ptr < basar::gui::tie::Transformer > create();

		protected:
			virtual basar::VarString	getPropertyValue(CSC_Batches::viewConn::AccPropTabYit yitAcc);
			virtual void				setPropertyValue(CSC_Batches::viewConn::AccPropTabYit yitAcc, basar::VarString newValue);
			virtual	void				translateProperty(const basar::I18nString& pPropertyName, basar::I18nString& translatedProperty, const basar::I18nString& context);

			basar::I18nString	m_PropertyName;
			basar::I18nString	m_TranslatedPropertyName;

		};
	} // viewconn
} // CSC_Batches

#endif //GUARD_DISCOUNTFLAGS_TRANSFORMER_H