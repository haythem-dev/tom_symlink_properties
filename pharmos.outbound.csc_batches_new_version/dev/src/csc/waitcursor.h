#ifndef GUARD_CSC_BATCHES_GUI_WAIT_CURSOR_H
#define GUARD_CSC_BATCHES_GUI_WAIT_CURSOR_H

class QWidget;
#include <QtGui/QCursor>

namespace CSC_Batches
{
namespace gui
{
/*! \class WaitCursor waitcursor.h gui/waitcursor.h
    \brief Changes the cursor to Qt::WaitCursor
    \details WaitCursor changes the cursor to Qt::WaitCursor when object is created.
	On destruction of WaitCursor object the cursor is set to the active one before
	change to Qt::WaitCursor.
	\example example_waitcursor.cpp
*/
class WaitCursor
{
public:
	WaitCursor( QWidget * parent );
	~WaitCursor();

private:
	QWidget *	m_Parent;
	QCursor		m_Cursor;
};

} // end namespace gui
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_GUI_WAIT_CURSOR_H

