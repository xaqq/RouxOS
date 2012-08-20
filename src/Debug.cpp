#include	"KBase.hh"
#include	"StaticString.hh"
#include	"PrintEngine.hh"
#include	"Debug.hh"

void	assert_failed(const char* fc, const char* f, int l, const char* c)
{
  kprint(StaticString("\e4ASSERT FAILED: ")
	 .append(c).append(" inside '").append(fc).append("' in file ")
	 .append(f).append(":").append(StaticString::number((size_t)l)));
  while (42);
}
