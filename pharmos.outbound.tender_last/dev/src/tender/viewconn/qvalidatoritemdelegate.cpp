//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  Definition of QValidatorItemDelegate.
 *  \author	Frank Naumann
 *  \date   
 */
//-------------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4718 4800)
	#include <QtWidgets/qlineedit.h>
	#include <QtWidgets/qcombobox.h>
#pragma warning (pop)

//#include "commonheader.h"
#include "QValidatorItemDelegate.h"

//-------------------------------------------------------------------------------------------------//

QValidatorItemDelegate::QValidatorItemDelegate(QObject* parent) : QItemDelegate(parent)
{
}

//-------------------------------------------------------------------------------------------------//

QValidatorItemDelegate::~QValidatorItemDelegate()
{
}

//-------------------------------------------------------------------------------------------------//

void QValidatorItemDelegate::setColumnValidator(int column, QValidator* validator)
{
  m_Validators.insert(column, validator);
}

//-------------------------------------------------------------------------------------------------//

QWidget* QValidatorItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QWidget* widget = QItemDelegate::createEditor(parent, option, index);

  if (m_Validators.contains(index.column()))
  {
    if (widget->inherits( "QLineEdit" ))
      static_cast<QLineEdit*>(widget)->setValidator( m_Validators.value( index.column() ));
    else if (widget->inherits("QComboBox"))
      static_cast<QComboBox*>(widget)->setValidator( m_Validators.value( index.column() ));
  }
  return widget;
}

//-------------------------------------------------------------------------------------------------//
