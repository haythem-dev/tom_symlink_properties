#include "inputvalidator.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QValidator>
	#include <QLineEdit>
	#include <QComboBox>
	#include <QDateEdit>
#pragma warning (pop)

#include <libbasar_definitions.h>

namespace tender
{
namespace viewConn
{
InputValidator::InputValidator( QObject * parent, const bool useDisjunction /* = false */ )
: QObject( parent ),
  m_Enabled( true ),
  m_Valid( false ),
  m_UseDisjunction( useDisjunction )
{
}

InputValidator::~InputValidator()
{
}

void InputValidator::addObject( QLineEdit * lineEdit )
{
	ObjectDescription od( lineEdit, LineEdit );
	InnerList::const_iterator iter = m_List.find( od );
	if( iter != m_List.end() )
	{
		return;
	}

	connect( lineEdit, SIGNAL( textChanged( const QString & ) ),
			 this,	   SLOT( OnLineEditTextChanged( const QString & ) ) );
	m_List.insert( od );
	eval();
}

void InputValidator::addObject( QComboBox * comboBox )
{
	ObjectDescription od( comboBox, ComboBox );
	InnerList::const_iterator iter = m_List.find( od );
	if( iter != m_List.end() )
	{
		return;
	}

	connect( comboBox, SIGNAL( currentIndexChanged( int ) ),
			 this,	   SLOT( OnComboBoxCurrentIndexChanged( int ) ) );
	m_List.insert( od );
	eval();
}

void InputValidator::addObject( QDateEdit * dateEdit )
{
	ObjectDescription od( dateEdit, DateEdit );
	InnerList::const_iterator iter = m_List.find( od );
	if( iter != m_List.end() )
	{
		return;
	}

	connect( dateEdit, SIGNAL( dateChanged( const QDate & ) ),
			 this,	   SLOT( OnDateEditDateChanged( const QDate & ) ) );
	m_List.insert( od );
	eval();
}

void InputValidator::addObject( InputValidator * inputValidator )
{
	ObjectDescription od( inputValidator, Validator );
	InnerList::const_iterator iter = m_List.find( od );
	if( iter != m_List.end() )
	{
		return;
	}

	connect( inputValidator, SIGNAL( inputValid() ),
			 this,			 SLOT( OnInputValidatorChanged() ) );
	connect( inputValidator, SIGNAL( inputInvalid() ),
			 this,			 SLOT( OnInputValidatorChanged() ) );

	m_List.insert( od );
	eval();
}

void InputValidator::removeObject( QObject * object )
{
	ObjectDescription od( object, LineEdit );
	InnerList::iterator iter = m_List.find( od );
	if( iter == m_List.end() )
	{
		return;
	}

	disconnect( object, 0, this, 0 );
	m_List.erase( iter );
	eval();
}

bool InputValidator::isValid() const
{
	return m_Valid;
}

void InputValidator::setEnabled()
{
	m_Enabled = true;
	eval();
}

void InputValidator::setDisabled()
{
	m_Enabled = false;
}

void InputValidator::OnLineEditTextChanged( const QString & )
{
	eval();
}

void InputValidator::OnComboBoxCurrentIndexChanged( int )
{
	eval();
}

void InputValidator::OnDateEditDateChanged( const QDate & )
{
	eval();
}

void InputValidator::OnInputValidatorChanged()
{
	eval();
}

void InputValidator::eval()
{
	if( false == m_Enabled )
	{
		return;
	}

	if( m_UseDisjunction )
	{
		disjunctionEval();
	}
	else
	{
		conjunctionEval();
	}
}

void InputValidator::conjunctionEval()
{
	InnerList::const_iterator iter = m_List.begin();
	for( ; iter != m_List.end(); ++iter )
	{
		const ObjectDescription & od = ( *iter );
		QObject * object = od.object;
		if( LineEdit == od.type )
		{
			QLineEdit * lineEdit = dynamic_cast<QLineEdit *>( object );
			if( isLineEditInputValid( lineEdit ) )
			{
				continue;
			}
		}
		else if( ComboBox == od.type )
		{
			QComboBox * comboBox = dynamic_cast<QComboBox *>( object );
			if( -1 != comboBox->currentIndex() )
			{
				continue;
			}
		}
		else if( DateEdit == od.type )
		{
			QDateEdit * dateEdit = dynamic_cast<QDateEdit *>( object );
			if(    dateEdit->date() > dateEdit->minimumDate() 
				&& dateEdit->date() < dateEdit->maximumDate() )
			{
				continue;
			}
		}
		else if( Validator == od.type )
		{
			InputValidator * inputValidator = dynamic_cast<InputValidator *>( object );
			if( inputValidator->isValid() )
			{
				continue;
			}
		}

		m_Valid = false;
		emit inputInvalid();
		return;
	}

	m_Valid = true;
	emit inputValid();
}

void InputValidator::disjunctionEval()
{
	InnerList::const_iterator iter = m_List.begin();
	for( ; iter != m_List.end(); ++iter )
	{
		const ObjectDescription & od = ( *iter );
		QObject * object = od.object;
		if( LineEdit == od.type )
		{
			QLineEdit * lineEdit = dynamic_cast<QLineEdit *>( object );
			if( isLineEditInputValid( lineEdit ) )
			{
				m_Valid = true;
				emit inputValid();
				return;
			}
		}
		else if( ComboBox == od.type )
		{
			QComboBox * comboBox = dynamic_cast<QComboBox *>( object );
			if( -1 != comboBox->currentIndex() )
			{
				m_Valid = true;
				emit inputValid();
				return;
			}
		}
		else if( DateEdit == od.type )
		{
			QDateEdit * dateEdit = dynamic_cast<QDateEdit *>( object );
			if(    dateEdit->date() > dateEdit->minimumDate() 
				&& dateEdit->date() < dateEdit->maximumDate() )
			{
				m_Valid = true;
				emit inputValid();
				return;
			}
		}
		else if( Validator == od.type )
		{
			InputValidator * inputValidator = dynamic_cast<InputValidator *>( object );
			if( inputValidator->isValid() )
			{
				m_Valid = true;
				emit inputValid();
				return;
			}
		}
	}

	m_Valid = false;
	emit inputInvalid();
}

bool InputValidator::isLineEditInputValid( QLineEdit * lineEdit )
{
	QString text = lineEdit->text();
	QValidator * validator = const_cast<QValidator *>( lineEdit->validator() );
	if( NULL == validator )
	{
		if( 0 < text.length() )
		{
			return true;
		}
	}
	else
	{				
		basar::Int32 pos = lineEdit->hasFocus() ? lineEdit->cursorPosition() : 0;
		if( QValidator::Acceptable == validator->validate( text, pos ) )
		{
			return true;
		}
	}

	return false;
}

} // end namespace viewConn
} // end namespace tender

