#ifndef GUARD_TENDER_VIEWCONN_INPUT_VALIDATOR_H
#define GUARD_TENDER_VIEWCONN_INPUT_VALIDATOR_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include <QObject>
	#include <QWidget>
#pragma warning (pop)

#include <set>

class QLineEdit;
class QComboBox;
class QDateEdit;
class QDate;

namespace tender
{
namespace viewConn
{
class InputValidator : public QObject
{
	Q_OBJECT

public:
	InputValidator( QObject * parent, const bool useDisjunction = false );
	~InputValidator();

	void addObject( QLineEdit * lineEdit );
	void addObject( QComboBox * comboBox );
	void addObject( QDateEdit * dateEdit );
	void addObject( InputValidator * inputValidator );
	void removeObject( QObject * object );

	bool isValid() const;
	static bool isLineEditInputValid( QLineEdit * lineEdit );

	void setEnabled();
	void setDisabled();

signals:
	void inputValid();
	void inputInvalid();

private slots:
	void OnLineEditTextChanged( const QString & );
	void OnComboBoxCurrentIndexChanged( int );
	void OnDateEditDateChanged( const QDate & );
	void OnInputValidatorChanged();

private:
	void eval();
	void conjunctionEval();
	void disjunctionEval();

	enum ObjectType
	{
		LineEdit = 0,
		ComboBox,
		DateEdit,
		Validator
	};

	struct ObjectDescription
	{
		QObject *	object;
		ObjectType	type;

		ObjectDescription( QObject * _object, ObjectType _type )
		: object( _object ), type( _type )
		{
		}

		inline bool operator<( const ObjectDescription & right ) const
		{
			return ( object < right.object );
		}
	};

	typedef std::set<ObjectDescription> InnerList;

	bool		m_Enabled;
	bool		m_Valid;
	bool		m_UseDisjunction;
	InnerList	m_List;
};

} // end namespace viewConn
} // end namespace tender

#endif // GUARD_TENDER_VIEWCONN_INPUT_VALIDATOR_H
