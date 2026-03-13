//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       27.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

inline std::ostream& operator<< ( std::ostream& strm, const libBatch::userAdmin::UserLoginConfiguration& usrConfig )
{
	usrConfig.toStream( strm );
	return strm;
}
