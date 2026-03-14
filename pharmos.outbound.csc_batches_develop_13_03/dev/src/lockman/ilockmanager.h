#ifndef GUARD_LIBLOCKMANAGER_ILOCKMANAGER_H
#define GUARD_LIBLOCKMANAGER_ILOCKMANAGER_H

namespace libLockManager
{
//---------------------------------------------------------------------------//
// forward declaration section
//---------------------------------------------------------------------------//
    class ILockEntry;
    class IKey;

//---------------------------------------------------------------------------//
// interface definition section
//---------------------------------------------------------------------------//
    class ILockManager
    {
    public:
        virtual ~ILockManager() {}

        virtual void lock    ( const ILockEntry&  )       = 0;
        //virtual bool tryLock ( const ILockEntryPtr& )       = 0;
        virtual void unlock  ( const IKey &       )       = 0;
        virtual bool isLocked( const IKey &       ) const = 0;

        virtual void eraseStaleLocks()                    = 0;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKMANAGER_H
