//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md 
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#include "Core/Rtt_Build.h"
#include "Rtt_LinuxDevice.h"

// ----------------------------------------------------------------------------

namespace Rtt
{

// ----------------------------------------------------------------------------

#pragma region Constructors/Destructors
LinuxDevice::LinuxDevice( Rtt_Allocator& allocator )
:	fAllocator( allocator ),
	fInputDeviceManager( &allocator ),
	fOrientation( DeviceOrientation::kUnknown )
{
}

LinuxDevice::~LinuxDevice()
{
}

#pragma endregion


#pragma region Public Member Functions
void LinuxDevice::SetOrientation( DeviceOrientation::Type orientation )
{
	fOrientation = orientation;
}

void LinuxDevice::Vibrate() const
{
}

void LinuxDevice::SetAccelerometerInterval( U32 frequency ) const
{
}

void LinuxDevice::SetGyroscopeInterval( U32 frequency ) const
{
}

bool LinuxDevice::HasEventSource( EventType type ) const
{
	bool hasEventSource = false;
	
	switch (type)
	{
		case MPlatformDevice::kAccelerometerEvent:
			break;
		case MPlatformDevice::kGyroscopeEvent:
			break;
		case MPlatformDevice::kOrientationEvent:
		case MPlatformDevice::kLocationEvent:
		case MPlatformDevice::kHeadingEvent:
		case MPlatformDevice::kMultitouchEvent:
		case MPlatformDevice::kKeyEvent:
			hasEventSource = true;
			break;
		default:
			Rtt_ASSERT_NOT_REACHED();
			break;
	}
	return hasEventSource;
}

void LinuxDevice::BeginNotifications( EventType type ) const
{
	fTracker.BeginNotifications( type );

	switch( type )
	{
		case MPlatformDevice::kOrientationEvent:
		case MPlatformDevice::kLocationEvent:
		case MPlatformDevice::kAccelerometerEvent:
		case MPlatformDevice::kGyroscopeEvent:
		case MPlatformDevice::kHeadingEvent:
		case MPlatformDevice::kMultitouchEvent:
			break;
		default:
			Rtt_ASSERT_NOT_REACHED();
			break;
	}
}

void LinuxDevice::EndNotifications( EventType type ) const
{
	fTracker.EndNotifications( type );

	switch( type )
	{
		case MPlatformDevice::kOrientationEvent:
		case MPlatformDevice::kLocationEvent:
		case MPlatformDevice::kAccelerometerEvent:
		case MPlatformDevice::kGyroscopeEvent:
		case MPlatformDevice::kHeadingEvent:
		case MPlatformDevice::kMultitouchEvent:
			break;
		default:
			Rtt_ASSERT_NOT_REACHED();
			break;
	}
}

bool LinuxDevice::DoesNotify( EventType type ) const
{
	return fTracker.DoesNotify( type );
}

const char* LinuxDevice::GetModel() const
{
	return "Linux";
}

const char* LinuxDevice::GetName() const
{
	return "";
}

const char* LinuxDevice::GetUniqueIdentifier( IdentifierType t ) const
{
	const char *result = "";

	switch ( t )
	{
		case MPlatformDevice::kDeviceIdentifier:
			break;
		case MPlatformDevice::kHardwareIdentifier:
			break;
		case MPlatformDevice::kOSIdentifier:
			break;
		case MPlatformDevice::kUdidIdentifier:
			break;
		default:
			break;
	}
	return result;
}

MPlatformDevice::EnvironmentType LinuxDevice::GetEnvironment() const
{
	return kDeviceEnvironment;
}

const char* LinuxDevice::GetPlatformName() const
{
	return "LINUX";
}
	
const char* LinuxDevice::GetPlatformVersion() const
{
	return "1.0";
}

const char* LinuxDevice::GetProductName() const
{
	return "";
}

const char* LinuxDevice::GetArchitectureInfo() const
{
	return "Lua";
}

PlatformInputDeviceManager& LinuxDevice::GetInputDeviceManager()
{
	return fInputDeviceManager;
}

void LinuxDevice::SetLocationAccuracy( Real meters ) const
{
}

void LinuxDevice::SetLocationThreshold( Real meters ) const
{
}

DeviceOrientation::Type LinuxDevice::GetOrientation() const
{
	return fOrientation;
}

	const char* LinuxDevice::GetPlatform() const
{
	return "linux";
}
	
	const char* LinuxDevice::GetManufacturer() const
{
	return "CoronaLabs";
}
	
#pragma endregion

// ----------------------------------------------------------------------------

} // namespace Rtt

// ----------------------------------------------------------------------------
