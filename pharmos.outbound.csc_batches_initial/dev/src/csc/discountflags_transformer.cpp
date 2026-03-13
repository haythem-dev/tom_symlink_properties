#include <QtCore/QCoreApplication>
#include "discountflags_transformer.h"

namespace CSC_Batches
{
	namespace viewConn
	{

		DiscountFlagsTransformer::DiscountFlagsTransformer() {}

		DiscountFlagsTransformer::DiscountFlagsTransformer(const basar::I18nString pPropertyName, const basar::I18nString translatedName)
		{
			m_PropertyName = pPropertyName;
			if ( !translatedName.empty() ) {
				m_TranslatedPropertyName = translatedName;
			}
			else {
				m_TranslatedPropertyName = m_PropertyName;
			}
		}

		DiscountFlagsTransformer::~DiscountFlagsTransformer() {}

		void DiscountFlagsTransformer::RightToLeft(GuiPropTabYit guiIt, const CSC_Batches::viewConn::AccPropTabYit accIt)
		{
			basar::I18nString flag = getPropertyValue(accIt);
			if ("0" == flag || "" == flag)
			{
				guiIt.setString(m_TranslatedPropertyName, QObject::tr("GEP").toLocal8Bit().constData());
			}
			else
			{
				guiIt.setString(m_TranslatedPropertyName, QObject::tr("AGP").toLocal8Bit().constData());
			}
		}

		//----------------------------------------------------------------------------
		//! \throw  no-throw
		//! 
		void DiscountFlagsTransformer::LeftToRight(const GuiPropTabYit guiIt, CSC_Batches::viewConn::AccPropTabYit accIt)
		{
			basar::I18nString flag = guiIt.getString(m_TranslatedPropertyName);
			if (flag == QObject::tr("GEP").toLocal8Bit().constData())
			{
				setPropertyValue(accIt, "0");
			}
			else
			{
				setPropertyValue(accIt, "1");
			}
		}

		//! \return pointer to created transformer
		boost::shared_ptr <basar::gui::tie::Transformer> DiscountFlagsTransformer::create()
		{
			return boost::shared_ptr < basar::gui::tie::Transformer >(new DiscountFlagsTransformer(*this));
		}

		void DiscountFlagsTransformer::setPropertyValue(CSC_Batches::viewConn::AccPropTabYit yitAcc, basar::VarString newValue)
		{
			yitAcc.setInt16(m_PropertyName, (basar::Int16)newValue.stoi());
		}

		basar::VarString DiscountFlagsTransformer::getPropertyValue(CSC_Batches::viewConn::AccPropTabYit yitAcc)
		{
			basar::VarString oldValue;

			oldValue.itos(yitAcc.getInt16(m_PropertyName));
			return oldValue;
		}

		void DiscountFlagsTransformer::translateProperty(const basar::I18nString& pPropertyName, basar::I18nString& translatedProperty, const basar::I18nString& context)
		{
			if (context != "") {
				translatedProperty = QCoreApplication::translate(context.c_str(), pPropertyName.c_str()).toLocal8Bit().constData();
			}
			else {
				translatedProperty = pPropertyName;
			}
		}
	} // viewConn
} // CSC_Batches