-------------------------------------------------------------------------------
-- 
-- Generic-Android-800x1280-tvdpi.lua
-- Copyright (c) 2016 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Ajay
--
-------------------------------------------------------------------------------

-- Based on the Nexus 7 (2012), Kindle Fire HD 8 (2015) and (2016)
-- https://material.io/devices/
-- https://developer.amazon.com/public/solutions/devices/fire-tablets/specifications/01-device-and-feature-specifications
simulator =
{
	device = "borderless-android-device",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 800,
	screenHeight = 1280,
	androidDisplayApproximateDpi = 213, -- tvdpi
	displayManufacturer = "Corona Labs Inc.",
	displayName = "GenericAndroidDevice",
	windowTitleBarName = "Android (tvdpi)",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
