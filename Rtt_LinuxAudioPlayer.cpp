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

#include "Core/Rtt_Build.h"
#include "Rtt_LinuxAudioPlayer.h"

// ----------------------------------------------------------------------------

namespace Rtt
{

// ----------------------------------------------------------------------------

#pragma region Constructors/Destructors
/// <summary>Creates an audio player used by Corona's "media" API's in Lua.</summary>
/// <param name="handle">Provides the Lua listener to be invoked when playback ends.</param>
/// <param name="allocator">Allocator needed to create memory for this player.</param>
EmscriptenAudioPlayer::EmscriptenAudioPlayer(const ResourceHandle<lua_State> & handle, Rtt_Allocator & allocator)
:	PlatformAudioPlayer(handle)
{
}

/// <summary>Destroys this player and disposes its allocated resources.</summary>
EmscriptenAudioPlayer::~EmscriptenAudioPlayer()
{
}

#pragma endregion


#pragma region Public Member Functions
/// <summary>Loads the given audio file into memory.</summary>
/// <param name="filePath">Path and name of the audio file to be loaded. Cannot be NULL or empty.</param>
/// <returns>
///  <para>Returns true if the given audio file was loaded successfully.</para>
///  <para>Returns false if failed to load the file or if given an invalid argument.</para>
/// </returns>
bool EmscriptenAudioPlayer::Load(const char * filePath)
{
	return true;
}

/// <summary>Plays the audio file that was loaded via the Load() function.</summary>
void EmscriptenAudioPlayer::Play()
{
}

/// <summary>Stops audio playback and resets back to the beginning of the audio track.</summary>
void EmscriptenAudioPlayer::Stop()
{
}

/// <summary>Pauses audio playback.</summary>
void EmscriptenAudioPlayer::Pause()
{
}

/// <summary>Resumes audio playback if currently paused.</summary>
void EmscriptenAudioPlayer::Resume()
{
}

/// <summary>Sets the volume level.</summary>
/// <param name="volume">
///  <para>A value ranging between 0.0 and 1.0.</para>
///  <para>Set to 0.0 to mute audio.</para>
///  <para>Set to 1.0 to set the volume at the highest level.</para>
/// </param>
void EmscriptenAudioPlayer::SetVolume(Rtt_Real volume)
{
}

/// <summary>Gets the volume level for this audio track.</summary>
/// <returns>Returns the current volume level. Ranges between 0.0 and 1.0, where 0.0 is muted.</returns>
Rtt_Real EmscriptenAudioPlayer::GetVolume() const
{
	return 0;
}

#pragma endregion

// ----------------------------------------------------------------------------

} // namespace Rtt

// ----------------------------------------------------------------------------
