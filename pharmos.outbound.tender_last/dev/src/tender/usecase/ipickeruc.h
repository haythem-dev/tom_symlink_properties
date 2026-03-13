#ifndef GUARD_TENDER_USECASE_IPICKERUC_H
#define GUARD_TENDER_USECASE_IPICKERUC_H

#include <libutil/irunnable.h>
#include <libutil/searchyiterator.h>

namespace tender
{
namespace useCase
{
	class IPickerUC : public libutil::useCase::IRunnable
    {
        public:
            virtual ~IPickerUC(){};

	        virtual void run() = 0;
	        virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked() = 0;
			virtual basar::db::aspect::AccessorPropertyTableRef getFound() 
			{
				basar::db::aspect::AccessorPropertyTableRef propTab = basar::db::aspect::AccessorPropertyTableRef();
                return propTab;
			};
			virtual void setPicked(basar::db::aspect::AccessorPropertyTable_YIterator picked) { };

            virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator()
            {
                basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator();
                return yit;
            };

            virtual void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit ) {};
            virtual void setCheckMode(bool) {};
			virtual void setSearchWithoutGUIMode(bool) {};

			virtual bool isPharmacyChainValid()		{ return false; };
			virtual bool isCustomerGroupIDValid()	{ return false; };
	};

} //end namespace useCase
} //end namespace tender

#endif // end GUARD_TENDER_USECASE_IPICKERUC_H
