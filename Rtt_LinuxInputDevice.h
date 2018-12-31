//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Corona Labs Inc.
// Contact: support@coronalabs.com
//
// This file is part of the Corona game engine.
//
// Commercial License Usage
// Licensees holding valid commercial Corona licenses may use this file in
// accordance with the commercial license agreement between you and 
// Corona Labs Inc. For licensing terms and conditions please contact
// support@coronalabs.com or visit https://coronalabs.com/com-license
//
// GNU General Public License Usage
// Alternatively, this file may be used under the terms of the GNU General
// Public license version 3. The license is as published by the Free Software
// Foundation and appearing in the file LICENSE.GPL3 included in the packaging
// of this file. Please review the following information to ensure the GNU 
// General Public License requirements will
// be met: https://www.gnu.org/licenses/gpl-3.0.html
//
// For overview and more information on licensing please refer to README.md
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __Rtt_LinuxInputDevice__
#define __Rtt_LinuxInputDevice__

#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "Input/Rtt_PlatformInputDeviceManager.h"

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

namespace Rtt
{
	
	class Runtime;
	
	class LinuxInputDevice : public PlatformInputDevice
	{
	public:
		typedef PlatformInputDevice Super;

		LinuxInputDevice(const InputDeviceDescriptor &descriptor);
		virtual ~LinuxInputDevice();
				
		virtual const char* GetProductName() override;
		virtual const char* GetDisplayName() override;
		virtual const char* GetPermanentStringId() override;
		virtual InputDeviceConnectionState GetConnectionState() override;
		virtual void Vibrate() override;
		virtual bool CanVibrate() override;
		virtual const char* GetDriverName() override;
		PlatformInputAxis* OnCreateAxisUsing(const InputAxisDescriptor &descriptor) override;
		
		bool AddNamedAxis(const char* axis);
		void init(const char* dev);
		void dispatchEvents(Runtime* runtime);
		const char* getAxisName(int i);

		InputDeviceConnectionState fConnected;
		std::string fSerialNumber;
		bool fCanVibrate;
		int fd;
		int fAxesCount;
		std::string fDriverName;
		uint8_t* fAxesMap;
	};
	
	class LinuxInputAxis : public PlatformInputAxis
	{
	public:
		bool centerPoint0;

		LinuxInputAxis(const InputAxisDescriptor &descriptor);
		virtual ~LinuxInputAxis(){};
		
		virtual Rtt_Real GetNormalizedValue(Rtt_Real rawValue) override;
	};
	
} // namespace Rtt


#endif /* __Rtt_LinuxInputDevice__ */
