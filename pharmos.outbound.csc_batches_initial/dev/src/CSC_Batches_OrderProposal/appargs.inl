//--------------------------------------------------------------------------------------------------//
/*! \brief 
 *  \author     Bjoern Bischof
 *  \date       24.12.2010
 *  \version    00.00.00.01
 */ 
//--------------------------------------------------------------------------------------------------//

inline std::ostream& operator<< ( std::ostream& strm, const abbaUW::cmdLine::AppArgs& args )
{
	args.toStream( strm );
	return strm;
}
