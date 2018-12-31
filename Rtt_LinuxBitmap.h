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

#include "Display/Rtt_PlatformBitmap.h"
#include "Core/Rtt_Array.h"
#include "Core/Rtt_String.h"

namespace Rtt
{

class PlatformFont;

// ----------------------------------------------------------------------------

class LinuxBaseBitmap : public PlatformBitmap
{
	public:
		typedef PlatformBitmap Super;

		typedef enum _Channel
		{
			kRed, kGreen, kBlue, kAlpha,
		}
		Channel;

	public:
		LinuxBaseBitmap();
		LinuxBaseBitmap(Rtt_Allocator *context, int w, int h, uint8_t* rgba);
		virtual ~LinuxBaseBitmap();

		virtual const void * GetBits( Rtt_Allocator *context ) const;
		virtual void FreeBits() const override {};
		virtual U32 Width() const;
		virtual U32 Height() const;
		virtual Format GetFormat() const;
		virtual bool IsProperty( PropertyMask mask ) const;
		virtual void SetProperty( PropertyMask mask, bool newValue );
		bool LoadFileBitmap(Rtt_Allocator &context, const char *path);
		static bool SaveBitmap(Rtt_Allocator *context, PlatformBitmap * bitmap, const char * filePath);

	protected:
		Rtt_INLINE bool IsPropertyInternal( PropertyMask mask ) const { return (fProperties & mask) ? true : false; }

		mutable U8 *fData;
		mutable S32 fWidth;
		mutable S32 fHeight;
		Format fFormat;
		U8 fProperties;
};

class LinuxFileBitmap : public LinuxBaseBitmap
{
	public:
		typedef LinuxBaseBitmap Super;

		LinuxFileBitmap(Rtt_Allocator& context, const char *filePath);
		virtual ~LinuxFileBitmap();

	private:
		String fPath;
};


class LinuxMaskFileBitmap : public LinuxBaseBitmap
{
	public:
		typedef LinuxBaseBitmap Super;

		LinuxMaskFileBitmap(Rtt_Allocator& context, const char *filePath);
		virtual ~LinuxMaskFileBitmap();
		
	private:
		String fPath;
};


class LinuxTextBitmap : public LinuxBaseBitmap
{
	public:
		typedef LinuxBaseBitmap Super;

		LinuxTextBitmap(Rtt_Allocator & context, const char str[], const PlatformFont& font, int width, int height, const char alignment[], Real& baselineOffset);
		virtual ~LinuxTextBitmap();

		virtual void FreeBits() const override {};
		virtual PlatformBitmap::Format GetFormat() const;
		virtual U8 GetByteAlignment() const;

	private:

		void wrapText();

		int fWrapWidth;
		String fAlignment;
};

// ----------------------------------------------------------------------------

} // namespace Rtt

// ----------------------------------------------------------------------------
