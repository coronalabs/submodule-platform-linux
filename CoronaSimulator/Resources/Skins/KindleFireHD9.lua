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
	screenOriginX = 102,
	screenOriginY = 100,
	screenWidth = 1200,
	screenHeight = 1920,
	androidDisplayApproximateDpi = 240, -- hdpi
	deviceImage = "KindleFireHD9.png",
	displayManufacturer = "Amazon",
	displayName = "KFJWI",
	statusBarDefault = "", 
	statusBarTranslucent = "", 
	statusBarBlack = "", 
	statusBarLightTransparent = "",
	statusBarDarkTransparent = "",
	windowTitleBarName = 'Kindle Fire HD 8.9" (2012)',
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
