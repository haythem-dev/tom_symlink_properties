//-------------------------------------------------------------------------------------------------//
/*! \file	eventfilterbase.h
 *  \brief  
 *  \author Beatrix Trömel
 *  \date   25.06.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef EVENTFILTERBASE_H
#define EVENTFILTERBASE_H

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
//#pragma warning(disable: 4512)
#include <QtCore/QObject>
#pragma warning (pop)

class QKeyEvent;
class QFocusEvent;

namespace CSC_Batches {
  namespace viewConn {
		class BatchOrderVC;

		class EventFilterBase : public QObject {
		public:
			EventFilterBase(QObject* parent = 0);
			virtual ~EventFilterBase();
		protected:
			/*!
				\brief The default key handler.
			*/
			bool eventFilter(QObject *obj, QEvent *event);
			virtual bool keyPressEvent(QObject *obj, QKeyEvent *keyEvent);

			BatchOrderVC* m_dialog;
		private:
			EventFilterBase(const EventFilterBase& EventFilterBase);
			EventFilterBase& operator= (const EventFilterBase& EventFilterBase);
		};
  }
}
#endif
