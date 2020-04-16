#include "Rtt_LinuxConfiguration.h"

#include <string>

namespace Rtt
{
	LinuxConfiguration::LinuxConfiguration()
	{
		this->Init();
	}
	
	LinuxConfiguration::~LinuxConfiguration()
	{
		
	}
	
	void LinuxConfiguration::Init(){
		
		ProjectFolder = HomeDir + "/Documents/Corona Projects/";
		
	};
};
