-------------------------------------------------------------------------------
-- 
-- Generic-Android-800x1280-hdpi.lua
-- Copyright (c) 2016 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Ajay
--
-------------------------------------------------------------------------------

-- Based on the Fire HD 7 (2014) and Fire HD 6 (2014)
-- https://developer.amazon.com/public/solutions/devices/fire-tablets/specifications/01-device-and-feature-specifications
simulator =
{
	device = "borderless-android-device",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 800,
	screenHeight = 1280,
	androidDisplayApproximateDpi = 240, -- hdpi
	displayManufacturer = "Corona Labs Inc.",
	displayName = "GenericAndroidDevice",
	windowTitleBarName = "Android (hdpi)",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
