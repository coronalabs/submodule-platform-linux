-------------------------------------------------------------------------------
-- 
-- Generic-Android-1200x1920-xhdpi.lua
-- Copyright (c) 2016 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Ajay
--
-------------------------------------------------------------------------------

-- Based on the Nexus 7 (2013)
-- https://material.io/devices/
simulator =
{
	device = "borderless-android-device",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 1200,
	screenHeight = 1920,
	androidDisplayApproximateDpi = 320, -- xhdpi
	displayManufacturer = "Corona Labs Inc.",
	displayName = "GenericAndroidDevice",
	windowTitleBarName = "Android (xhdpi)",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
