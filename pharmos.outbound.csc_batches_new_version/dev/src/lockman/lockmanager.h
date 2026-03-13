#ifndef GUARD_LIBLOCKMANAGER_LOCKMANAGER_H
#define GUARD_LIBLOCKMANAGER_LOCKMANAGER_H

#include "ilockmanager.h"
#include "ilockaccessorptr.h"

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class LockManager : public ILockManager
    {
    public:
        LockManager();
        ~LockManager();

    // from ILockManager interface
    public:
        void                       lock    ( const ILockEntry& );
        bool                       tryLock ( const ILockEntry& );
        void                       unlock  ( const IKey&       );
        bool                       isLocked( const IKey&       ) const;

        void                       eraseStaleLocks();

    public:
        void                       injectAccessor( accessor::ILockAccessorPtr );

    private:
        // forbid copying of this class
        LockManager( const LockManager& );
        LockManager& operator =( const LockManager& );

        accessor::ILockAccessorPtr getAccessor() const;

    private:
        accessor::ILockAccessorPtr m_Accessor;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKMANAGER_H
