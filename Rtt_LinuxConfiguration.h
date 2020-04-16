#ifndef LINUXCONFIGURATION_H
#define LINUXCONFIGURATION_H

#include <string>

#pragma once

namespace Rtt{
	
	class LinuxConfiguration
	{
		
	public:
		LinuxConfiguration();
		~LinuxConfiguration();
		std::string ProjectFolder;
		std::string BuildFolder;
		std::string HomeDir;
		std::string ApplicationPath;
		std::string ApplicationName;
	private:
		void Init();
	};
	
	
} // namespace Rtt



#endif