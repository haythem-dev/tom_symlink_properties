#pragma warning (push)
#pragma warning(disable: 4800)
#include <QtGui/QPainter>
#pragma warning (pop)

#include <QtWidgets/QItemDelegate>

namespace CSC_Batches
{
	namespace viewConn
	{
		// Qt-style: Paints the text color in fields where background color is set
		//           to ensure better readability
		class ItemColorFormatter : public QItemDelegate
		{
		public:
			ItemColorFormatter(QObject* parent, int column) : QItemDelegate(parent), m_Column(column)
			{
			}

			void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
			{
				if (index.column() == m_Column)
				{
					QStyleOptionViewItem viewOption(option);

					QColor color = index.data(Qt::BackgroundRole).value< QColor >();
					if (!color.isValid())
					{
						color = QColor(Qt::white);
					}

					viewOption.palette.setColor(QPalette::Highlight, color);
					QItemDelegate::paint(painter, viewOption, index);
				}
				else
				{
					QItemDelegate::paint(painter, option, index);
				}
			}

		private:
			int m_Column;
		};
	}
}