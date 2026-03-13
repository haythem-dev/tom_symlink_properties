#ifndef GUARD_TENDER_VIEWCONN_ITENDERARTICLEINFO_H
#define GUARD_TENDER_VIEWCONN_ITENDERARTICLEINFO_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QtWidgets/QGroupBox>
#pragma warning (pop)

#include <libtender/domainmodule/tender/tenderdm.h>
#include <libtender/domainmodule/tender/tenderstateenum.h>
#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>
#include <libtender/domainmodule/tender/tendertypeenum.h>
//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//class QFrame;

namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

//-------------------------------------------------------------------------------------------------//
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class ITenderArticleInfo: public QGroupBox
        {
            public:
                virtual ~ITenderArticleInfo(){};
                virtual void setFocus() = 0;
                virtual void matchToTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitTender ) = 0;
                virtual void matchFromTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitTender ) = 0;
                virtual basar::VarString getArticleCode() = 0;
                virtual void matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator article ) = 0;
                virtual void resetArticleName() = 0;
                virtual void resetTenderPosition() = 0;
                virtual void disableArticleCode() = 0;
                virtual void enableArticleCode() = 0;
                virtual void setFocusArticleCode() = 0;
                virtual void selectArticlecode() = 0;
                virtual void setFocusQuantity() = 0;
                virtual void selectQuantity() = 0;
                virtual void enableInputValidator( bool ip) = 0;
                virtual bool hasFocus() = 0;
                virtual void addArtValidator() = 0;

            private:
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ITENDERARTICLEINFO_H
