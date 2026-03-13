#ifndef GUARD_LIBLOCKMANAGER_ACCESSOR_ILOCKACCESSOR_H
#define GUARD_LIBLOCKMANAGER_ACCESSOR_ILOCKACCESSOR_H

namespace libLockManager
{
//---------------------------------------------------------------------------//
// forward declaration section
//---------------------------------------------------------------------------//
    class IKey;
    class ILockEntry;

//---------------------------------------------------------------------------//
// interface definition section
//---------------------------------------------------------------------------//
namespace accessor
{

    class ILockAccessor
    {
    public:
        virtual ~ILockAccessor() {}

        virtual bool          findByKey( const IKey&       ) = 0;
        virtual void          save     ( const ILockEntry& ) = 0;
        virtual void          erase    ( const IKey&       ) = 0;

        virtual void          eraseStaleLocks()              = 0;
    };

} // end namespace accessor
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ACCESSOR_ILOCKACCESSOR_H
