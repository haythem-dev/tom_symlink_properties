#include "waitcursor.h"

#pragma warning (push)
#pragma warning(disable: 4127 4512)
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#pragma warning (pop)

namespace CSC_Batches
{
namespace gui
{
WaitCursor::WaitCursor( QWidget * parent ) 
: m_Parent( parent ), 
  m_Cursor( parent->cursor() ) 
{
	m_Parent->setCursor( Qt::WaitCursor );
    QApplication::processEvents(); // workaround a Qt5 glitch: Mouse cursor not changed when a dialog box is opened before
}

WaitCursor::~WaitCursor()
{
	m_Parent->setCursor( m_Cursor );
}

} // end namespace gui
} // end namespace CSC_Batches

