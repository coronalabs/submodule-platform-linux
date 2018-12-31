-------------------------------------------------------------------------------
-- 
-- Generic-Android-1200x1920-xxhdpi.lua
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Walter
--
-------------------------------------------------------------------------------

simulator =
{
	device = "borderless-android-device",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 1200,
	screenHeight = 1920,
	androidDisplayApproximateDpi = 480, -- xxhdpi
	displayManufacturer = "Corona Labs Inc.",
	displayName = "GenericAndroidDevice",
	windowTitleBarName = "Android (xxhdpi)",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
