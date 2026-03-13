#ifndef GUARD_TENDER_VIEWCONN_ARTICLEPICKERVC_H
#define GUARD_TENDER_VIEWCONN_ARTICLEPICKERVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_articlepickervc.h"
#include <QObject>
#pragma warning (pop)

#include "libtender/domainmodule/tender/tenderdm.h"
#include "libtender/domainmodule/tender/tendertypecollectionptr.h"

#include <libutil/flag.h>
#include <libutil/viewconn.h>


namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
            class AccessorPropertyTableRef;
        }
    }
}

namespace tender
{
    namespace viewConn
    {
		class ArticlePickerVC : public libutil::viewConn::BaseVC
		{
			Q_OBJECT;

			public:
				ArticlePickerVC( QWidget* parent = 0 );
				~ArticlePickerVC();

				void injectTenderTypeCollection( libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection );

				virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromArticleCollection( basar::db::aspect::AccessorPropertyTableRef        propTab   ); 
				void matchToSearchCriteria     ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );

				basar::db::aspect::AccessorPropertyTable_YIterator getCurrentArticle();
				
				void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

				void displayNoArticleFound();

			private:
				void wireEvents();
				void setupArticleTable();
				void setupSearchCriteria();
				void initArticleHeaderLabels();
				basar::Int32 initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );
				void clear();

			private slots:
				void on_DialogClose_Requested();
				void on_Search_Requested();
				void on_TakeOver_Requested();

			protected:
				virtual void focusInEvent( QFocusEvent* event );
                virtual void reject();

			private:
				Ui::ArticlePickerVC                                ui;
                						                           
                libutil::misc::Flag                                m_EventsWiring;
                QStringList                                        m_LstLabels;
                std::list<basar::VarString>                        m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef                        m_ArticleMatcher;
				basar::db::aspect::AccessorPropertyTable_YIterator m_PickedArticle;
				libtender::domMod::tender::TenderTypeCollectionPtr m_TenderTypeCollection;
		};
	} //end namespace viewConn
} //end namespace tender

#endif //end GUARD_TENDER_VIEWCONN_ARTICLEPICKERVC_H
