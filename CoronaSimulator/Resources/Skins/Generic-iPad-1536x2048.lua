-------------------------------------------------------------------------------
-- 
-- Generic-iOS-2048x1536.lua
-- Copyright (c) 2014 Corona Labs, Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Walter
--
-------------------------------------------------------------------------------

simulator =
{
	device = "borderless-ios-tablet",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 768*2,
	screenHeight = 1024*2,
	safeLandscapeScreenInsetStatusBar = 20 * 2,
	safeScreenInsetStatusBar = 20 * 2,
	iosPointWidth = 768,
	iosPointHeight = 1024,
	displayManufacturer = "Corona Labs Inc.",
	displayName = "iPad",
	statusBarDefault = "iPadStatusBarBlack.png", 
	statusBarTranslucent = "iPadStatusBarWhite.png", 
	statusBarBlack = "iPadStatusBarBlack.png", 
	statusBarLightTransparent = "iPadStatusBarWhite.png",
	statusBarDarkTransparent = "iPadStatusBarBlack.png",
	windowTitleBarName = "iPad @2x",
	defaultFontSize = 17 * 2,		-- Converts default font point size to pixels.
}
