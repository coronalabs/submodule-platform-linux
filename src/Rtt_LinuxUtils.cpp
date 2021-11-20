#include "Rtt_LinuxUtils.h"
#include "Core/Rtt_Assert.h"

using namespace std;

namespace Rtt
{
	int LinuxUtils::SortVectorByName(string a, string b)
	{
		return a < b;
	}
};
