//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Bundles the parameters of the execution method of the accessor
 *  \author Bischof Bjoern
 *  \date   15.09.2012
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_ACCESSOR_EXECUTE_PARAMETERIZATION_H
#define GUARD_DOMMOD_ACCESSOR_EXECUTE_PARAMETERIZATION_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
    //-------------------------------------------------------------------------------------------------//
    // struct declaration section
    //-------------------------------------------------------------------------------------------------//
    struct AccessorExecuteParameterization
    {
        public:
            AccessorExecuteParameterization( const basar::VarString& accessMethod, const bool clearBeforeSelect = true, 
                                            const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT)
            : m_AccessMethod( accessMethod ), m_ClearBeforeSelect( clearBeforeSelect ), m_FlagExcept( flagExcept ), m_DBHint( dbHint)
            {}
            
            AccessorExecuteParameterization( const AccessorExecuteParameterization& rhs )
            : m_AccessMethod( rhs.m_AccessMethod ), m_ClearBeforeSelect( rhs.m_ClearBeforeSelect ), m_FlagExcept( rhs.m_FlagExcept ), m_DBHint( rhs.m_DBHint)
            {}
        
        private:
            AccessorExecuteParameterization& operator=( const AccessorExecuteParameterization& rhs );
        
        public:
            const basar::VarString                     m_AccessMethod;
            const bool                                 m_ClearBeforeSelect;
            const bool                                 m_FlagExcept;
            const basar::db::aspect::DatabaseHintEnum  m_DBHint;
    };

} // end namespace domMod
} // end namespace libabbauw

#endif
