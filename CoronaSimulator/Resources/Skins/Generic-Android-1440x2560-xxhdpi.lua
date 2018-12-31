-------------------------------------------------------------------------------
-- 
-- Generic-Android-1440x2560-xxhdpi.lua
-- Copyright (c) 2016 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Ajay
--
-------------------------------------------------------------------------------

-- Based on the LG G3 and Samsung Galaxy Note 4
-- https://material.io/devices/
simulator =
{
	device = "borderless-android-device",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 1440,
	screenHeight = 2560,
	androidDisplayApproximateDpi = 480, -- xxhdpi
	displayManufacturer = "Corona Labs Inc.",
	displayName = "GenericAndroidDevice",
	windowTitleBarName = "Android (xxhdpi)",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
