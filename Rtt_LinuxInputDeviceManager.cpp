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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "Rtt_LinuxInputDevice.h"
#include "Rtt_LinuxInputDeviceManager.h"
#include "Rtt_Event.h"

namespace Rtt 
{

	LinuxInputDeviceManager::LinuxInputDeviceManager(Rtt_Allocator *allocatorPointer)
		: PlatformInputDeviceManager(allocatorPointer)
	{
	}

	LinuxInputDeviceManager::~LinuxInputDeviceManager()
	{
	}

	void LinuxInputDeviceManager::init()
	{
		registerDevice("/dev/input/js0");
		registerDevice("/dev/input/js1");
	}

	void LinuxInputDeviceManager::registerDevice(const char* dev)
	{
		LinuxInputDevice* devicePointer = static_cast<LinuxInputDevice*>( this->Add(InputDeviceType::kJoystick) );
		devicePointer->init(dev);
	}

	PlatformInputDevice* LinuxInputDeviceManager::CreateUsing(const InputDeviceDescriptor &descriptor)
	{
		return Rtt_NEW(GetAllocator(), LinuxInputDevice(descriptor));
	}

	LinuxInputDevice* LinuxInputDeviceManager::GetBySerialNumber(const char* sn)
	{
		if (NULL == sn)
		{
			return NULL;
		}

		const ReadOnlyInputDeviceCollection &collection = GetDevices();
		for (int index = 0; index < collection.GetCount(); index++)
		{
			LinuxInputDevice* devicePointer = (LinuxInputDevice*)collection.GetByIndex(index);
			if (devicePointer != NULL) //fixme && ([devicePointer->fSerialNumber isEqualToString:sn]))
			{
				return devicePointer;
			}
		}
		return NULL;
	}

	void LinuxInputDeviceManager::Destroy(PlatformInputDevice* devicePointer)
	{
		Rtt_DELETE(devicePointer);
	}

	void LinuxInputDeviceManager::dispatchEvents(Runtime* runtime)
	{
		const ReadOnlyInputDeviceCollection &collection = GetDevices();
		for (int index = 0; index < collection.GetCount(); index++)
		{
			LinuxInputDevice *devicePointer = (LinuxInputDevice*)collection.GetByIndex(index);
			if (devicePointer != NULL && devicePointer->GetConnectionState() == InputDeviceConnectionState::kConnected)
			{
				devicePointer->dispatchEvents(runtime);
			}
		}
	}

}
