-------------------------------------------------------------------------------
-- 
-- Galaxy S5
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Perry
--
-------------------------------------------------------------------------------

simulator =
{
	device = "android-phone",
	screenOriginX = 75,
	screenOriginY = 223,
	screenWidth = 1080,
	screenHeight = 1920,
	androidDisplayApproximateDpi = 480, -- xxhdpi
	deviceImage = "Samsung-Galaxy-S5.png",
	displayManufacturer = "samsung",
	displayName = "SM-G900S",
	supportsScreenRotation = true,
	hasAccelerometer = true,
	windowTitleBarName = "Samsung Galaxy S5",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
