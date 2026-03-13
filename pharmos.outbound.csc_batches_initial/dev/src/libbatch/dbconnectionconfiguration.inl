//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of abstract connection base class
 *  \author     Bischof Bjoern
 *  \date       27.01.2009
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

inline std::ostream& operator<< ( std::ostream& strm, const libBatch::dbConnection::DBConnectionConfiguration& dbConnConfig )
{
	dbConnConfig.toStream( strm );
	return strm;
}
