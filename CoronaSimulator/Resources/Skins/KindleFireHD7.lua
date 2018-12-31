-------------------------------------------------------------------------------
-- 
-- KindleFire
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Walter
--
-------------------------------------------------------------------------------

simulator =
{
	device = "android-tablet",
	screenOriginX = 72,
	screenOriginY = 70,
	screenWidth = 800,
	screenHeight = 1280,
	androidDisplayApproximateDpi = 240, -- hdpi
	deviceImage = "KindleFireHD7.png",
	displayManufacturer = "Amazon",
	displayName = "KFTT",
	statusBarDefault = "", 
	statusBarTranslucent = "", 
	statusBarBlack = "", 
	statusBarLightTransparent = "",
	statusBarDarkTransparent = "",
	windowTitleBarName = 'Kindle Fire HD 7" (2012)',
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
