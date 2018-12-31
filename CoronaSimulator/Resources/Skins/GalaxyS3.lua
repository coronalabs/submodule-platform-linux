-------------------------------------------------------------------------------
-- 
-- Galaxy S3
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Walter
--
-------------------------------------------------------------------------------

simulator =
{
	device = "android-phone",
	screenOriginX = 58,
	screenOriginY = 190,
	screenWidth = 720,
	screenHeight = 1280,
	androidDisplayApproximateDpi = 320, -- xhdpi
	deviceImage = "GalaxyS3.png",
	displayManufacturer = "samsung",
	displayName = "GT-I9300",
	statusBarDefault = "", 
	statusBarTranslucent = "", 
	statusBarBlack = "", 
	statusBarLightTransparent = "",
	statusBarDarkTransparent = "",
	windowTitleBarName = "Samsung Galaxy S3",
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
