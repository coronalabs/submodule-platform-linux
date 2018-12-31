-------------------------------------------------------------------------------
-- 
-- HTC Sensation
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Walter
--
-------------------------------------------------------------------------------

simulator =
{
	device = "android-phone",
	screenOriginX = 54,
	screenOriginY = 170,
	screenWidth = 540,
	screenHeight = 960,
	androidDisplayApproximateDpi = 240, -- hdpi
	deviceImage = "HTCSensation.png",
	displayManufacturer = "HTC",
	displayName = "Sensation",
	statusBarDefault = "", 
	statusBarTranslucent = "", 
	statusBarBlack = "", 
	statusBarLightTransparent = "",
	statusBarDarkTransparent = "",
	windowTitleBarName = "HTC Sensation",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
