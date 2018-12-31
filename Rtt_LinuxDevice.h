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

#include "Rtt_MPlatformDevice.h"
#include "Rtt_DeviceOrientation.h"
#include "Input/Rtt_PlatformInputDeviceManager.h"
#include "Rtt_LinuxInputDeviceManager.h"

namespace Rtt
{

class MCallback;
class String;

class LinuxDevice : public MPlatformDevice
{
	public:
		typedef MPlatformDevice Super;

		LinuxDevice(Rtt_Allocator& allocator);
		virtual ~LinuxDevice();

		virtual const char* GetModel() const override;
		virtual const char* GetName() const override;
		virtual const char* GetUniqueIdentifier(IdentifierType t) const override;
		virtual EnvironmentType GetEnvironment() const override;
		virtual const char* GetPlatformName() const override;
		virtual const char* GetPlatformVersion() const override;
		virtual const char* GetProductName() const;
		virtual const char* GetArchitectureInfo() const override;
		virtual PlatformInputDeviceManager& GetInputDeviceManager() override;
		virtual void Vibrate() const override;
		virtual void BeginNotifications(EventType type) const override;
		virtual void EndNotifications(EventType type) const override;
		virtual bool DoesNotify(EventType type) const override;
		virtual bool HasEventSource(EventType type) const override;
		virtual void SetAccelerometerInterval(U32 frequency) const override;
		virtual void SetGyroscopeInterval(U32 frequency) const override;
		virtual void SetLocationAccuracy(Real meters) const override;
		virtual void SetLocationThreshold(Real meters) const override;
		virtual void SetOrientation(DeviceOrientation::Type orientation);
		virtual DeviceOrientation::Type GetOrientation() const override;
		virtual const char* GetPlatform() const override;
		virtual const char* GetManufacturer() const override;

	private:
		Rtt_Allocator& fAllocator;
		DeviceNotificationTracker fTracker;
		DeviceOrientation::Type fOrientation;
		LinuxInputDeviceManager fInputDeviceManager;
};

// ----------------------------------------------------------------------------

} // namespace Rtt

// ----------------------------------------------------------------------------
