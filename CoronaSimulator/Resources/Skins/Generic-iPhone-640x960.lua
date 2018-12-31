-------------------------------------------------------------------------------
-- 
-- Generic-iPhone4.lua
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Walter
--
-------------------------------------------------------------------------------

simulator =
{
	device = "borderless-ios-phone",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 640,
	screenHeight = 960,
	iosPointWidth = 320,
	iosPointHeight = 480,
	safeLandscapeScreenInsetStatusBar = 20 * 2,
	safeScreenInsetStatusBar = 20 * 2,
	displayManufacturer = "Corona Labs Inc.",
	displayName = "iPhone",
	statusBarDefault = "iPhone4StatusBarBlack.png",
	statusBarTranslucent = "iPhone4StatusBarWhite.png",
	statusBarBlack = "iPhone4StatusBarBlack.png",
	statusBarLightTransparent = "iPhone4StatusBarWhite.png",
	statusBarDarkTransparent = "iPhone4StatusBarBlack.png",
	windowTitleBarName = "iPhone @2x",
	defaultFontSize = 17 * 2,		-- Converts default font point size to pixels.
}
