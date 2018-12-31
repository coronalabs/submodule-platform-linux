-------------------------------------------------------------------------------
-- 
-- Ouya - 1920x1080
-- Copyright (c) 2013 Corona Labs Inc. All rights reserved.
-- 
-- Reviewers:
-- 		Joshua Quick
--
-------------------------------------------------------------------------------

simulator =
{
	device = "android-tv",
	screenOriginX = 0,
	screenOriginY = 0,
	screenWidth = 1080,
	screenHeight = 1920,
	androidDisplayApproximateDpi = 320, -- xhdpi
	displayManufacturer = "OUYA",
	displayName = "OUYA Console",
	supportsScreenRotation = false,
	isUprightOrientationPortrait = false,
	windowTitleBarName = "Ouya",

	safeScreenInsetTop = 96,
	safeScreenInsetLeft = 54,
	safeScreenInsetBottom = 96,
	safeScreenInsetRight = 54,
	safeLandscapeScreenInsetTop = 54,
	safeLandscapeScreenInsetLeft = 96,
	safeLandscapeScreenInsetBottom = 54,
	safeLandscapeScreenInsetRight = 96,
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
