-------------------------------------------------------------------------------
-- 
-- Generic-Android-1536x2048.lua
-- Copyright (c) 2016 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Ajay
--
-------------------------------------------------------------------------------

-- Based on the Nexus 9
-- https://material.io/devices/
simulator =
{
	device = "borderless-android-device",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 1536,
	screenHeight = 2048,
	androidDisplayApproximateDpi = 320, -- xhdpi
	displayManufacturer = "Corona Labs Inc.",
	displayName = "GenericAndroidDevice",
	windowTitleBarName = "Android (xhdpi)",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
