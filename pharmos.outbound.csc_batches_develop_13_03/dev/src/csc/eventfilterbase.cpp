#include "eventfilterbase.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "batchordervc.h"
#pragma warning (pop)

namespace CSC_Batches {
  namespace viewConn {
	EventFilterBase::EventFilterBase(QObject* parent) : QObject(parent), m_dialog(dynamic_cast < BatchOrderVC* >(parent)) {

	}

	EventFilterBase::~EventFilterBase() {
	}

	bool EventFilterBase::eventFilter(QObject *obj, QEvent *event) {
		if(event->type() != QEvent::KeyPress)
			return false;	

		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		//---------------------- Return 
		if((keyEvent->key() == Qt::Key_Return)) {
			m_dialog->on_btnPositions_clicked();
			return true;
		}// if: Qt::Key_Return

		//---------------------- Esc
		if(keyEvent->key() == Qt::Key_Escape) {
			// do nothing, so Dialog is not closed
			return true;
		}// if: Qt::Key_Escape


		obj;
		return false;
	}// eventFilter

	bool EventFilterBase::keyPressEvent(QObject *obj, QKeyEvent *keyEvent) {
		obj;
		keyEvent;
return false;
	}// keyPressEvent
  }
}
