#ifndef GUARD_LIBLOCKMANAGER_KEY_H
#define GUARD_LIBLOCKMANAGER_KEY_H

#include "ikey.h"
#include <libbasar_definitions.h>

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class Key : public IKey
    {
    public:
        explicit Key( basar::Int32 id );
        explicit Key( basar::Int32 id, basar::Int16 branchNo );
        ~Key();

        // set name of column where the key should be serialized
        void            setIdentifierName( const basar::VarString& );

        // from ISerializable interface
        DataCollection  serialize() const;

    private:
        // forbidden
        Key();

    private:
        basar::Int32     m_id;
        basar::VarString m_identifier;
        basar::Int16     m_branchNo;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_KEY_H
