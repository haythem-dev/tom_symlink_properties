#ifndef GUARD_TENDER_VIEWCONN_FLAG_TRANSFORMER_H
#define GUARD_TENDER_VIEWCONN_FLAG_TRANSFORMER_H

namespace tender
{
namespace viewConn
{
template< typename T = basar::Int16 >
class FlagTransformer : public basar::gui::tie::Transformer
{
public:
	template< typename T >
	FlagTransformer(
		const basar::VarString & guiColumnName,
		const basar::VarString & propTabColumnName,
		const T flagOnValue,
		const T flagOffValue,
		const QString & flagOnText,
		const QString & flagOffText
			)
	: m_guiColName( guiColumnName ),
	  m_propTabColName( propTabColumnName ),
	  m_FlagOnValue( flagOnValue ),
	  m_FlagOffValue( flagOffValue ),
	  m_FlagOnText( flagOnText ),
	  m_FlagOffText( flagOffText )
	{
	}

	virtual ~FlagTransformer()
	{
	}

	virtual void RightToLeft(
		basar::gui::tie::GuiPropertyTable_YIterator guiIt,
		const basar::db::aspect::AccessorPropertyTable_YIterator accIt
			)
	{
		QString txt( getFlagValue<T>( accIt ) != m_FlagOffValue ? m_FlagOnText : m_FlagOffText );
		guiIt.setString( m_guiColName, txt.toLocal8Bit().data() );
	}

	virtual void LeftToRight(
		const basar::gui::tie::GuiPropertyTable_YIterator guiIt,
		basar::db::aspect::AccessorPropertyTable_YIterator accIt
			)
	{
		QString txt( guiIt.getString( m_guiColName ).c_str() );
		setFlagValue<T>( accIt, m_FlagOnText == txt ? m_FlagOnValue : m_FlagOffValue );
	}

private:
	FlagTransformer();

	template< typename T >
	const T getFlagValue( basar::db::aspect::AccessorPropertyTable_YIterator accIt );

	template<>
	const basar::Int16 getFlagValue<basar::Int16>( basar::db::aspect::AccessorPropertyTable_YIterator accIt )
	{
		return accIt.getInt16( m_propTabColName );
	}

	template<>
	const basar::Int32 getFlagValue<basar::Int32>( basar::db::aspect::AccessorPropertyTable_YIterator accIt )
	{
		return accIt.getInt32( m_propTabColName );
	}

	template<>
	const basar::VarString getFlagValue<basar::VarString>( basar::db::aspect::AccessorPropertyTable_YIterator accIt )
	{
		return accIt.getString( m_propTabColName );
	}

	template< typename T >
	void setFlagValue( basar::db::aspect::AccessorPropertyTable_YIterator accIt, const T flag );

	template<>
	void setFlagValue<basar::Int16>( basar::db::aspect::AccessorPropertyTable_YIterator accIt, const basar::Int16 flag )
	{
		accIt.setInt16( m_propTabColName, flag );
	}

	template<>
	void setFlagValue<basar::Int32>( basar::db::aspect::AccessorPropertyTable_YIterator accIt, const basar::Int32 flag )
	{
		accIt.setInt32( m_propTabColName, flag );
	}

	template<>
	void setFlagValue<basar::VarString>( basar::db::aspect::AccessorPropertyTable_YIterator accIt, const basar::VarString flag )
	{
		accIt.setString( m_propTabColName, flag );
	}

	virtual boost::shared_ptr < basar::gui::tie::Transformer > create()
	{
		return boost::shared_ptr < basar::gui::tie::Transformer >( new FlagTransformer( *this ) );
	}

	basar::VarString	m_guiColName;
	basar::VarString	m_propTabColName;
	T					m_FlagOnValue;
	T					m_FlagOffValue;
	QString				m_FlagOnText;
	QString				m_FlagOffText;
};

} // namespace viewConn
} // namespace tender

#endif // GUARD_TENDER_VIEWCONN_FLAG_TRANSFORMER_H