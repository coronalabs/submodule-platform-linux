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

#pragma once

#include "Core/Rtt_Types.h"
#include "Core/Rtt_String.h"
#include "Rtt_PlatformImageProvider.h"
#include "Rtt_LinuxVideoObject.h"
#include "Rtt_PlatformData.h"
#include "Rtt_Runtime.h"

namespace Rtt
{

	// Local event
	class LinuxImageProviderEvent : public VirtualEvent
	{
	public:
		typedef VirtualEvent Super;
		typedef CompletionEvent Self;

	public:
		virtual const char* Name() const { return "onVideo"; }
	};

	/// Provides an image picker window. Selected image can be sent to a Lua listener function.
	class LinuxImageProvider : public PlatformImageProvider
	{
	public:

		enum LinuxVideoEventTypes
		{
			undefined = 0,
			onError = 1,
			onStart = 2,
			onStop = 3,
			onResume = 4,
			onDataAvailable = 5,
			onPause = 6,
			onCreated = 7,
		};

		enum ImageSourceTypes
		{
			IMAGE_SOURCE_PHOTO_LIBRARY = 0,
			IMAGE_SOURCE_CAMERA = 1,
			IMAGE_SOURCE_SAVED_PHOTOS_ALBUM = 2
		};
		
		enum MediaQualityTypes
		{
			MEDIA_CAPTURE_QUALITY_LOW = 0,
			MEDIA_CAPTURE_QUALITY_MEDIUM = 1,
			MEDIA_CAPTURE_QUALITY_HIGH = 2
		};
	
		struct LinuxImageProviderCompletionEvent : public CompletionEvent
		{
			LinuxImageProviderCompletionEvent()
				: fWidth(0)
				, fHeight(0)
				, fData(0)
			{}

			LinuxImageProviderCompletionEvent(int w, int h, uint8_t* buf)
				: fWidth(w)
				, fHeight(h)
				, fData(buf)
			{}

			virtual int Push( lua_State *L ) const override;

		private:
		
			int fWidth;
			int fHeight;
			uint8_t* fData;
		};

		LinuxImageProvider(const ResourceHandle<lua_State> & handle, int w, int h);
		virtual ~LinuxImageProvider();

		virtual bool Supports(int source) const;
		virtual bool Show(int source, const char* filePath, lua_State* L);

		void dispatch(LinuxVideoEventTypes eventID, int w, int h, uint8_t* buf);

	private:
	
		bool open_device();
		void close_device();
		bool init_device();
		void uninit_device();
		int xioctl(int fh, unsigned long int request, void *arg);
		void init_read(unsigned int buffer_size);
		bool init_mmap();
		void init_userp(unsigned int buffer_size);
	
		String fFile;
		int fd;
		int force_format;
	};

} // namespace Rtt
