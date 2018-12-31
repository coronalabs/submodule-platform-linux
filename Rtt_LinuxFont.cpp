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
#include "Core/Rtt_Build.h"
#include "Rtt_LinuxFont.h"

namespace Rtt
{

LinuxFont::LinuxFont(Rtt_Allocator &allocator, const char *fontName, Rtt_Real size, bool isBold)
:	Super(),
	fName( & allocator, fontName ? fontName : "" ),
	fSize( size ),
	fIsBold( isBold )
{
}

LinuxFont::LinuxFont(Rtt_Allocator &allocator, PlatformFont::SystemFont fontType, Rtt_Real size)
:	Super(),
	fName( & allocator, "" ),
	fSize( size ),
	fIsBold( false )
{
}

LinuxFont::LinuxFont(const LinuxFont &rhs)
:	Super(),
	fName(rhs.fName.GetAllocator(), rhs.fName.GetString()),
	fSize(rhs.fSize),
	fIsBold(rhs.fIsBold)
{
}

LinuxFont::~LinuxFont()
{
}

PlatformFont* LinuxFont::CloneUsing(Rtt_Allocator *allocator) const
{
	if (!allocator)
	{
		return NULL;
	}
	return Rtt_NEW(allocator, LinuxFont(*this));
}

const char* LinuxFont::Name() const
{
	return fName.GetString();
}

void LinuxFont::SetSize(Rtt_Real newValue)
{
	fSize = newValue;
}

Rtt_Real LinuxFont::Size() const
{
	return fSize;
}

void* LinuxFont::NativeObject() const
{
	// todo
	return NULL;
}

bool LinuxFont::IsBold() const
{
	return fIsBold;
}

// ----------------------------------------------------------------------------

} // namespace Rtt

// ----------------------------------------------------------------------------
