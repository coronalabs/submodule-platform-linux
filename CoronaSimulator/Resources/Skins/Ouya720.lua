-------------------------------------------------------------------------------
-- 
-- Ouya - 1280x720
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
	screenWidth = 720,
	screenHeight = 1280,
	androidDisplayApproximateDpi = 320, -- xhdpi
	displayManufacturer = "OUYA",
	displayName = "OUYA Console",
	supportsScreenRotation = false,
	isUprightOrientationPortrait = false,
	windowTitleBarName = "Ouya",

	safeScreenInsetTop = 64,
	safeScreenInsetLeft = 36,
	safeScreenInsetBottom = 64,
	safeScreenInsetRight = 36,
	safeLandscapeScreenInsetTop = 36,
	safeLandscapeScreenInsetLeft = 64,
	safeLandscapeScreenInsetBottom = 36,
	safeLandscapeScreenInsetRight = 64,
}
simulator.defaultFontSize = 18.0 * (simulator.androidDisplayApproximateDpi / 160)
