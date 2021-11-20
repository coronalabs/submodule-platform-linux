#include "Rtt_LinuxFileUtils.h"
#include "Core/Rtt_Assert.h"
#include "Rtt_LinuxCrypto.h"
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace Rtt
{
	const char *LinuxFileUtils::GetStartupPath(string *exeFileName)
	{
		static char buf[PATH_MAX + 1];
		ssize_t count = readlink("/proc/self/exe", buf, PATH_MAX);
		buf[count] = 0;

		// remove file name
		char *filename = strrchr(buf, '/');
		Rtt_ASSERT(filename);

		if (exeFileName)
		{
			*exeFileName = filename + 1;
		}

		*filename = 0;

		return buf;
	}

	const char *LinuxFileUtils::GetHomePath()
	{
		const char *homeDir = NULL;

		if ((homeDir = getenv("HOME")) == NULL)
		{
			homeDir = getpwuid(getuid())->pw_dir;
		}

		return homeDir;
	}

	char *LinuxFileUtils::CalculateMD5(string filename)
	{
		LinuxCrypto crypto;
		U8 digest[MCrypto::kMaxDigestSize];
		size_t digestLen = crypto.GetDigestLength(MCrypto::kMD5Algorithm);
		Rtt::Data<const char> data(filename.c_str(), (int)filename.length());
		crypto.CalculateDigest(MCrypto::kMD5Algorithm, data, digest);

		char *hex = (char *)calloc(sizeof(char), digestLen * 2 + 1);

		for (unsigned int i = 0; i < digestLen; i++)
		{
			char *p = hex;
			p += sprintf(hex + 2 * i, "%02x", digest[i]);
		}

		return hex;
	}
};
