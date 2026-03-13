#ifndef GUARD_TEST_UTILS_H
#define GUARD_TEST_UTILS_H


namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class ConnectionRef;
		}

		namespace sql
		{
			class ConnectionRef;
		}
	}
}

void initRandom();

namespace unittest
{
class Trace
{
public:
	Trace( basar::ConstString msg, basar::ConstString indent )
	: m_indent( indent ), m_msg( msg )
	{
#ifndef NDEBUG
		std::cout << m_indent << "---Start " << msg << std::endl;
#endif
	}

	~Trace()
	{
#ifndef NDEBUG
		std::cout << m_indent << "---End " << m_msg << "\n" << std::endl;
#endif
	}
private:
	Trace ( const Trace & r );
	Trace operator = ( const Trace & r );

	basar::ConstString	m_indent;
	basar::ConstString	m_msg;
};

#define TRACE_IT_INDENT(msg,indent) Trace trace_it(msg,indent)
#define TRACE_IT(msg) Trace trace_it(msg,"")

#define FIXTURE_TEST_START(baseFixture,testName) TEST_FIXTURE( baseFixture, testName ) \
{ const char * fun = ""#testName; TRACE_IT( ""#testName ); try

#define FIXTURE_TEST_END catch( basar::Exception & e ) \
{ processException( e, fun ); }}

} // end namespace unittest

#endif	// GUARD_TEST_UTILS_H
