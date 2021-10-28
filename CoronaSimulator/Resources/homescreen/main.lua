------------------------------------------------------------------------------
--
-- This file is part of the Corona game engine.
-- For overview and more information on licensing please refer to README.md 
-- Home page: https://github.com/coronalabs/corona
-- Contact: support@coronalabs.com
--
------------------------------------------------------------------------------
local widget = require("widget")
local simErr, simulator = pcall(require, "simulator")
local json = require("json")
local lfs = require("lfs")
local sFormat = string.format
local recentProjectsPath = sFormat("%s/.Solar2D/Sandbox/Simulator/Documents/recents.json", os.getenv("HOME")) 
local backgroundColor = {0.14, 0.14, 0.149, 1}
local buttonBackgroundColor = {0.2, 0.2, 0.21, 1}
local mainFont = "OpenSans-Regular.ttf"
local lightFont = "OpenSans-Light.ttf"
local buttonWidth = 150
local buttonHeight = 30
local buttonFontSize = 14
local buttonYPadding = 5
local recentProjectIconSize = 30
local headerFontSize = 14
local sidePadding = 12
local recentProjectTitles = {}
local popupMenu = nil
local recentProjectsGroup = display.newGroup()
local handleObjectMouseEvents = nil
local createButton = nil
local openRecentProject = nil
local createRecentProjectList = nil
local createPopupMenu = nil

display.setDefault("background", backgroundColor[1], backgroundColor[2], backgroundColor[3])

local function scriptPath()
	local str = debug.getinfo(2, "S").source:sub(2):match("(.*/)")
	return str:match("(.*/)")
 end

local function getLastPathComponent(str)
	local pathIndexes = {}

	for i = 1, #str do
		if (str:sub(i, i) == "/") then
			pathIndexes[#pathIndexes + 1] = i
		end
	end

	return string.sub(str, pathIndexes[#pathIndexes - 1], pathIndexes[#pathIndexes])
end

local function getAppName(str)
	local lastPathComponent = getLastPathComponent(str)
	return (lastPathComponent:sub(2, lastPathComponent:len() - 1))
end

local function getProjectPath(str)
	local pathIndexes = {}

	for i = 1, #str do
		if (str:sub(i, i) == "/") then
			pathIndexes[#pathIndexes + 1] = i
		end
	end

	return string.sub(str, 1, pathIndexes[#pathIndexes])
end

local function loadTable(path)
	local loc = location

	if not location then
		loc = defaultLocation
	end

	local file, errorString = io.open(path, "r")

	if not file then
		return {}
	else
		local contents = file:read("*a")
		local t = json.decode(contents)
		io.close(file)

		return t
	end
end

local function saveTable(t, path)
	local loc = location

	if not location then
		loc = defaultLocation
	end

	local file, errorString = io.open(path, "w")

	if not file then
		print("File error: " .. errorString)
		return false
	else
		file:write(json.encode(t))
		io.close(file)

		return true
	end
end

local function removeExistingProjectFromRecents(recentProjects, projectName)
	-- see if this project already exists in recent projects
	if (#recentProjects > 0) then
		local existingPosition = 0

		for i = 1, #recentProjects do
			if (recentProjects[i].formattedString == projectName) then
				existingPosition = i
				break;
			end
		end

		if (existingPosition > 0) then
			table.remove(recentProjects, existingPosition)
			saveTable(recentProjects, recentProjectsPath)
		end

		if (#recentProjects > 7) then
			for i = 7, #recentProjects do
				table.remove(recentProjects, i)
			end

			saveTable(recentProjects, recentProjectsPath)
		end
	end
end

local background = display.newRect(0, 0, display.contentWidth, display.contentHeight)
background.x = display.contentCenterX
background.y = display.contentCenterY
background:setFillColor(unpack(backgroundColor))
background:addEventListener("mouse", function(event)
	local phase = event.type

	if (phase == "move") then
		native.setProperty("mouseCursor", "arrow")
	end

	return true
end)

local solar2DTuxLogo = display.newImageRect("images/logo.png", display.contentWidth / 2.5, display.contentHeight / 5)
solar2DTuxLogo.anchorX = 0
solar2DTuxLogo.anchorY = 0
solar2DTuxLogo.x = sidePadding
solar2DTuxLogo.y = 10

local latestNewsTitle = display.newText(
{
	text = "Latest News:",
	font = mainFont,
	fontSize = headerFontSize,
	align = "left"
})
latestNewsTitle.anchorX = 1
latestNewsTitle.anchorY = 0
latestNewsTitle.x = display.contentWidth - 10
latestNewsTitle.y = 20
latestNewsTitle:setFillColor(1, 1, 1, 1)

local latestNewsText = display.newText(
{
	text = "Solar2DTux is developed primarily by Danny Glover & Robert Craig. If you are in a position to support our work, please visit our homepage via the 'website' button to see our GitHub/Patreon sponsor accounts. Thank you.",
	font = lightFont,
	fontSize = headerFontSize - 1,
	width = display.contentWidth / 2.2,
	height = solar2DTuxLogo.contentHeight / 1.5,
	align = "left"
})
latestNewsText.anchorX = 1
latestNewsText.anchorY = 0
latestNewsText.x = display.contentWidth - 10
latestNewsText.y = latestNewsTitle.y + latestNewsTitle.contentHeight + 2
latestNewsText:setFillColor(1, 1, 1, 1)

-- get started
local getStartedText = display.newText(
{
	text = "Get Started",
	font = mainFont,
	fontSize = headerFontSize,
	align = "left"
})
getStartedText.anchorX = 0
getStartedText.anchorY = 0
getStartedText.x = sidePadding
getStartedText.y = solar2DTuxLogo.y + solar2DTuxLogo.contentHeight + 4
getStartedText:setFillColor(1, 1, 1, 1)

local separatorLine = display.newRect(0, 0, display.contentWidth - 20, 1)
separatorLine.anchorY = 0
separatorLine.x = display.contentCenterX
separatorLine.y = getStartedText.y + getStartedText.contentHeight + 2
separatorLine:setFillColor(unpack(buttonBackgroundColor))

openRecentProject = function(event)
	local details = event.target.details
	local recentProjects = loadTable(recentProjectsPath)
	removeExistingProjectFromRecents(recentProjects, details.formattedString)
	table.insert(recentProjects, 1, {formattedString = details.formattedString, fullURLString = details.fullURL})
	saveTable(recentProjects, recentProjectsPath)

	local projectMain = io.open(details.fullURL, "r")

	local function onProjectNotFound(event)
		if (event.action == "clicked") then
			local i = event.index

			if (i == 1) then
				removeExistingProjectFromRecents(recentProjects, details.formattedString)
				createRecentProjectList()
			end
		end
	end

	if (projectMain) then
		io.close(projectMain)
		simulator.show("open", details.fullURL)
	else
		local title = "Project Not Found On Disk!"
		local message = "The project you selected was not found on your system.\n\nWould you like to remove it from the recent projects list?"
		local alert = native.showAlert(title, message, { "Yes", "No" }, onProjectNotFound)
	end
end

handleObjectMouseEvents = function(event)
	local phase = event.type
	local showHand = true

	if (event.target.isMainButton and popupMenu.isVisible) then
		showHand = false
	end

	if (phase == "move" and showHand) then
		native.setProperty("mouseCursor", "pointingHand")
	end

	return true
end

createButton = function(label, onRelease)
	local button = widget.newButton(
	{
		label = label,
		emboss = false,
		shape = "roundedRect",
		width = buttonWidth,
		height = buttonHeight,
		cornerRadius = 2,
		fontSize = buttonFontSize,
		fillColor = 
		{ 
			default = buttonBackgroundColor, 
			over = {buttonBackgroundColor[1], buttonBackgroundColor[2], buttonBackgroundColor[3], 0.7}, 
		},
		labelColor = 
		{
			default = {1, 1, 1, 1},
			over = {1, 1, 1, 0.7}
		},
		onRelease = onRelease
	})

	button:addEventListener("mouse", handleObjectMouseEvents)

	return button
end

createRecentProjectList = function()
	if (recentProjectsGroup.numChildren > 0) then
		for i = recentProjectsGroup.numChildren, 1, -1 do
			display.remove(recentProjectsGroup[i])
		end
	end

	local recentProjects = loadTable(recentProjectsPath)

	if (#recentProjects > 0) then
		for i = 1, #recentProjects do
			local icon = nil
			local projectName = recentProjects[i].formattedString
			local projectDir = getProjectPath(recentProjects[i].fullURLString)
			projectDir = projectDir:sub(1, projectDir:len() -1)
			local projectIconFile = simulator.getPreference("welcomeScreenIconFile") or "Icon.png"
			local projectIcon = sFormat("%s/%s", projectDir, projectIconFile)

			if (lfs.attributes(projectIcon) ~= nil) then
				simulator.setProjectResourceDirectory(projectDir .. "/")
				icon = display.newImageRect(projectIconFile, system.ResourceDirectory, recentProjectIconSize, recentProjectIconSize)
			else
				icon = display.newImageRect("Icon.png", system.ResourceDirectory, recentProjectIconSize, recentProjectIconSize)
			end

			local initialYPosition = (separatorLine.y + separatorLine.contentHeight) + 4
			icon.anchorX = 1
			icon.anchorY = 0
			icon.x = display.contentWidth - sidePadding
			icon.y = i == 1 and initialYPosition or (separatorLine.y + separatorLine.contentHeight) - recentProjectIconSize - 4 + ((recentProjectIconSize + 8) * i)
			recentProjectsGroup:insert(icon)

			recentProjectTitles[#recentProjectTitles + 1] = display.newText(
			{
				text = projectName,
				font = mainFont,
				fontSize = buttonFontSize,
				align = "left"
			})
			recentProjectTitles[#recentProjectTitles].anchorX = 1
			recentProjectTitles[#recentProjectTitles].anchorY = 0
			recentProjectTitles[#recentProjectTitles].x = icon.x - icon.contentWidth - sidePadding
			recentProjectTitles[#recentProjectTitles].y = icon.y
			recentProjectTitles[#recentProjectTitles].details = {formattedString = projectName, fullURL = sFormat("%s/main.lua", projectDir)}
			recentProjectTitles[#recentProjectTitles]:addEventListener("tap", openRecentProject)
			recentProjectTitles[#recentProjectTitles]:addEventListener("mouse", 
				function(event)
					local phase = event.type

					if (phase == "move" and not popupMenu.isVisible) then
						native.setProperty("mouseCursor", "pointingHand")
					else
						if (phase == "down") then
							if (event.isSecondaryButtonDown) then
								popupMenu:setEvent(event)
								popupMenu:show()
							end
						end
					end

					return true
				end
			)
			recentProjectsGroup:insert(recentProjectTitles[#recentProjectTitles])

			-- limit long paths
			if (projectDir:len() > 78) then
				projectDir = sFormat("..%s", projectDir:sub(projectDir:len() - 70, projectDir:len()))
			end

			local projectPathText = display.newText(
			{
				text = projectDir,
				font = lightFont,
				fontSize = buttonFontSize - 2,
				align = "left"
			})
			projectPathText.anchorX = 1
			projectPathText.anchorY = 0
			projectPathText.x = recentProjectTitles[#recentProjectTitles].x
			projectPathText.y = recentProjectTitles[#recentProjectTitles].y + (recentProjectIconSize * 0.5)
			recentProjectsGroup:insert(projectPathText)

			local separatorLine = display.newRect(0, 0, display.contentWidth / 1.70, 1)
			separatorLine.anchorX = 1
			separatorLine.anchorY = 0
			separatorLine.x = projectPathText.x
			separatorLine.y = projectPathText.y + projectPathText.contentHeight + 2
			separatorLine:setFillColor(unpack(buttonBackgroundColor))
			recentProjectsGroup:insert(separatorLine)
		end
	else
		local noRecentProjectsText = display.newText(
		{
			text = "No recent projects were found.\nWhy not create one?",
			font = mainFont,
			fontSize = buttonFontSize + 1,
			align = "center"
		})
		noRecentProjectsText.anchorX = 1
		noRecentProjectsText.anchorY = 0
		noRecentProjectsText.x = display.contentWidth - sidePadding
		noRecentProjectsText.y =  (separatorLine.y + separatorLine.contentHeight) + 4
		noRecentProjectsText:setFillColor(1, 1, 1, 1)
		recentProjectsGroup:insert(noRecentProjectsText)
	end

	recentProjectsGroup:toFront()
end

createPopupMenu = function()
	local group = display.newGroup()
	group.event = nil

	local modalBackground = display.newRect(0, 0, display.contentWidth, display.contentHeight)
	modalBackground.x = display.contentCenterX
	modalBackground.y = display.contentCenterY
	modalBackground.alpha = 0
	modalBackground.isHitTestable = true
	group:insert(modalBackground)
	modalBackground:addEventListener("tap", 
		function(event)
			return true
		end
	)
	modalBackground:addEventListener("touch", 
		function(event)
			return true
		end
	)

	local popupBackground = display.newRoundedRect(0, 0, display.contentWidth / 1.5, display.contentHeight / 1.5, 2)
	popupBackground.x = display.contentCenterX
	popupBackground.y = display.contentCenterY
	popupBackground:setFillColor(unpack(backgroundColor))
	popupBackground.alpha = 0.92
	group:insert(popupBackground)

	local targetNameText = display.newText(
	{
		text = "",
		font = mainFont,
		fontSize = buttonFontSize,
		align = "center"
	})
	targetNameText.x = display.contentCenterX
	targetNameText.y = display.contentCenterY - (popupBackground.contentHeight * 0.5) + targetNameText.contentHeight
	targetNameText:setFillColor(1, 1, 1, 1)
	group:insert(targetNameText)

	local targetPathText = display.newText(
	{
		text = "",
		font = mainFont,
		fontSize = buttonFontSize - 2,
		align = "center"
	})
	targetPathText.x = display.contentCenterX
	targetPathText.y = targetNameText.y + targetNameText.contentHeight + 2
	targetPathText:setFillColor(1, 1, 1, 1)
	group:insert(targetPathText)

	local targetSeparatorLine = display.newRect(0, 0, popupBackground.contentWidth - 20, 1)
	targetSeparatorLine.x = display.contentCenterX
	targetSeparatorLine.y = targetPathText.y + targetPathText.contentHeight + 2
	targetSeparatorLine:setFillColor(unpack(buttonBackgroundColor))
	group:insert(targetSeparatorLine)

	local launchTargetButton = createButton("Launch Project", 
		function(e) 
			openRecentProject(group.event)
		end
	)
	launchTargetButton.x = display.contentCenterX
	launchTargetButton.y = (targetSeparatorLine.y + targetSeparatorLine.contentHeight) + (launchTargetButton.contentHeight * 0.5) + 4
	group:insert(launchTargetButton)

	local showProjectFilesButton = createButton("Open Project Folder", 
		function(e) 
			simulator.show("showFiles", group.targetPath)
		end
	)
	showProjectFilesButton.x = display.contentCenterX
	showProjectFilesButton.y = launchTargetButton.y + buttonHeight + buttonYPadding
	group:insert(showProjectFilesButton)

	local openProjectInEditorButton = createButton("Open Project In Editor", 
		function(e) 
			simulator.show("editProject", group.targetPath)
		end
	)
	openProjectInEditorButton.x = display.contentCenterX
	openProjectInEditorButton.y = showProjectFilesButton.y + buttonHeight + buttonYPadding
	group:insert(openProjectInEditorButton)

	local showTargetSandboxButton = createButton("Open Sandbox Folder", 
		function(e) 
			simulator.show("showSandbox", group.targetName)
		end
	)
	showTargetSandboxButton.x = display.contentCenterX
	showTargetSandboxButton.y = openProjectInEditorButton.y + buttonHeight + buttonYPadding
	group:insert(showTargetSandboxButton)

	local removeFromRecentsButton = createButton("Remove From Recents", 
		function(e) 
			local recentProjects = loadTable(recentProjectsPath)
			removeExistingProjectFromRecents(recentProjects, group.targetName)
			createRecentProjectList()
			group:hide()
		end
	)
	removeFromRecentsButton.x = display.contentCenterX
	removeFromRecentsButton.y = openProjectInEditorButton.y + buttonHeight + buttonYPadding
	group:insert(removeFromRecentsButton)

	local closePopupButton = createButton("Close", 
		function(e) 
			group:hide()
		end
	)
	closePopupButton.x = display.contentCenterX
	closePopupButton.y = removeFromRecentsButton.y + buttonHeight + buttonYPadding
	group:insert(closePopupButton)

	function group:setEvent(event)
		self.event = event
		self.targetName = event.target.details.formattedString
		self.targetPath = event.target.details.fullURL
		local targetPath = event.target.details.fullURL

		if (targetPath:len() > 65) then
			targetPath = sFormat("..%s", targetPath:sub(targetPath:len() - 65, targetPath:len()))
		end

		targetNameText.text = self.targetName
		targetPathText.text = targetPath
	end

	function group:show()
		self:toFront()
		self.isVisible = true
	end

	function group:hide()
		self:toBack()
		self.isVisible = false
	end

	group.isVisible = false
	return group;
end

popupMenu = createPopupMenu()

local cloneProjectButton = createButton("Clone A Repository", 
	function(event) 
		simulator.show("clone")
	end
)
cloneProjectButton.anchorX = 0
cloneProjectButton.x = sidePadding
cloneProjectButton.y = (separatorLine.y + separatorLine.contentHeight) + (cloneProjectButton.contentHeight * 0.5) + 4
cloneProjectButton.isMainButton = true

local openProjectButton = createButton("Open Existing Project",
	function(event)
		local recentProjects = loadTable(recentProjectsPath)
		simulator.show("open")

		local projectPath = simulator.getCurrentProjectPath()

		if (projectPath == nil or projectPath:len() <= 1) then
			return
		end

		local projectName = getAppName(projectPath)
		local projectDir = projectPath

		-- see if this project already exists in recent projects
		removeExistingProjectFromRecents(recentProjects, projectName)
		table.insert(recentProjects, 1, {formattedString = projectName, fullURLString = projectDir})
		saveTable(recentProjects, recentProjectsPath)
	end
)
openProjectButton.anchorX = 0
openProjectButton.x = sidePadding
openProjectButton.y = cloneProjectButton.y + buttonHeight + buttonYPadding
openProjectButton.isMainButton = true

local createProjectButton = createButton("Create New Project",
	function(event)
		simulator.show("new")
	end
)
createProjectButton.anchorX = 0
createProjectButton.x = sidePadding
createProjectButton.y = openProjectButton.y + buttonHeight + buttonYPadding
createProjectButton.isMainButton = true

local openSampleCodeButton = createButton("View Sample Code",
	function(event)
		simulator.show("sampleCode")
	end
)
openSampleCodeButton.anchorX = 0
openSampleCodeButton.x = sidePadding
openSampleCodeButton.y = createProjectButton.y + buttonHeight + buttonYPadding
openSampleCodeButton.isMainButton = true

local reportAnIssueButton = createButton("Report An Issue",
	function(event)
		system.openURL("https://github.com/DannyGlover/Solar2DTux/issues")
	end
)
reportAnIssueButton.anchorX = 0
reportAnIssueButton.x = sidePadding
reportAnIssueButton.y = openSampleCodeButton.y + buttonHeight + buttonYPadding
reportAnIssueButton.isMainButton = true

local documentationButton = createButton("Documentation", 
	function(event) 
		system.openURL("https://docs.coronalabs.com/api") 
	end
)
documentationButton.anchorX = 0
documentationButton.x = sidePadding
documentationButton.y = reportAnIssueButton.y + buttonHeight + buttonYPadding
documentationButton.isMainButton = true

local githubButton = createButton("GitHub", 
	function(event) 
		system.openURL("https://github.com/DannyGlover/Solar2DTux") 
	end
)
githubButton.anchorX = 0
githubButton.x = sidePadding
githubButton.y = documentationButton.y + buttonHeight + buttonYPadding
githubButton.isMainButton = true

local websiteButton = createButton("Website",
	function(event) 
		system.openURL("https://solar2dtux.com/") 
	end
)
websiteButton.anchorX = 0
websiteButton.x = sidePadding
websiteButton.y = githubButton.y + buttonHeight + buttonYPadding
websiteButton.isMainButton = true

local pluginsButton = createButton("Plugins",
	function(event) 
		system.openURL("https://plugins.solar2d.com/") 
	end
)
pluginsButton.anchorX = 0
pluginsButton.x = sidePadding
pluginsButton.y = websiteButton.y + buttonHeight + buttonYPadding
pluginsButton.isMainButton = true

-- recent projects
local recentProjectsText = display.newText(
{
	text = "Recent Projects",
	font = mainFont,
	fontSize = headerFontSize,
	align = "left"
})
recentProjectsText.anchorX = 1
recentProjectsText.anchorY = 0
recentProjectsText.x = display.contentWidth - sidePadding
recentProjectsText.y = getStartedText.y
recentProjectsText:setFillColor(1, 1, 1, 1)

--[[
local recentProjects = loadTable(recentProjectsPath)

if (#recentProjects > 0) then
	for i = 1, #recentProjects do
		local icon = nil
		local projectName = recentProjects[i].formattedString
		local projectDir = getProjectPath(recentProjects[i].fullURLString)
		projectDir = projectDir:sub(1, projectDir:len() -1)
		local projectIconFile = simulator.getPreference("welcomeScreenIconFile") or "Icon.png"
		local projectIcon = sFormat("%s/%s", projectDir, projectIconFile)

		if (lfs.attributes(projectIcon) ~= nil) then
			simulator.setProjectResourceDirectory(projectDir .. "/")
			icon = display.newImageRect(projectIconFile, system.ResourceDirectory, recentProjectIconSize, recentProjectIconSize)
		else
			icon = display.newImageRect("Icon.png", system.ResourceDirectory, recentProjectIconSize, recentProjectIconSize)
		end

		local initialYPosition = (separatorLine.y + separatorLine.contentHeight) + 4
		icon.anchorX = 1
		icon.anchorY = 0
		icon.x = display.contentWidth - sidePadding
		icon.y = i == 1 and initialYPosition or (separatorLine.y + separatorLine.contentHeight) - recentProjectIconSize - 4 + ((recentProjectIconSize + 8) * i)
		
		recentProjectTitles[#recentProjectTitles + 1] = display.newText(
		{
			text = projectName,
			font = mainFont,
			fontSize = buttonFontSize,
			align = "left"
		})
		recentProjectTitles[#recentProjectTitles].anchorX = 1
		recentProjectTitles[#recentProjectTitles].anchorY = 0
		recentProjectTitles[#recentProjectTitles].x = icon.x - icon.contentWidth - sidePadding
		recentProjectTitles[#recentProjectTitles].y = icon.y
		recentProjectTitles[#recentProjectTitles].details = {formattedString = projectName, fullURL = sFormat("%s/main.lua", projectDir)}
		recentProjectTitles[#recentProjectTitles]:addEventListener("tap", openRecentProject)
		recentProjectTitles[#recentProjectTitles]:addEventListener("mouse", 
			function(event)
				local phase = event.type

				if (phase == "move" and not popupMenu.isVisible) then
					native.setProperty("mouseCursor", "pointingHand")
				else
					if (phase == "down") then
						if (event.isSecondaryButtonDown) then
							popupMenu:setEvent(event)
							popupMenu:show()
						end
					end
				end

				return true
			end
		)

		-- limit long paths
		if (projectDir:len() > 78) then
			projectDir = sFormat("..%s", projectDir:sub(projectDir:len() - 70, projectDir:len()))
		end

		local projectPathText = display.newText(
		{
			text = projectDir,
			font = lightFont,
			fontSize = buttonFontSize - 2,
			align = "left"
		})
		projectPathText.anchorX = 1
		projectPathText.anchorY = 0
		projectPathText.x = recentProjectTitles[#recentProjectTitles].x
		projectPathText.y = recentProjectTitles[#recentProjectTitles].y + (recentProjectIconSize * 0.5)

		local separatorLine = display.newRect(0, 0, display.contentWidth / 1.70, 1)
		separatorLine.anchorX = 1
		separatorLine.anchorY = 0
		separatorLine.x = projectPathText.x
		separatorLine.y = projectPathText.y + projectPathText.contentHeight + 2
		separatorLine:setFillColor(unpack(buttonBackgroundColor))
	end
else
	local noRecentProjectsText = display.newText(
	{
		text = "No recent projects were found.\nWhy not create one?",
		font = mainFont,
		fontSize = buttonFontSize + 1,
		align = "center"
	})
	noRecentProjectsText.anchorX = 1
	noRecentProjectsText.anchorY = 0
	noRecentProjectsText.x = display.contentWidth - sidePadding
	noRecentProjectsText.y =  (separatorLine.y + separatorLine.contentHeight) + 4
	noRecentProjectsText:setFillColor(1, 1, 1, 1)
end
--]]

createRecentProjectList()

local versionText = display.newText(
{
	text = ("Version: %s"):format(system.getInfo("build")),
	font = lightFont,
	fontSize = buttonFontSize,
	align = "left"
})
versionText.anchorX = 0
versionText.anchorY = 1
versionText.x = sidePadding
versionText.y = display.contentHeight - 6

local copyrightText = display.newText(
{
	text = "Copyright 2020 © Solar2D / Solar2DTux",
	font = lightFont,
	fontSize = buttonFontSize,
	align = "left"
})
copyrightText.anchorX = 1
copyrightText.anchorY = 1
copyrightText.x = display.contentWidth - sidePadding
copyrightText.y = display.contentHeight - 6

local function onResize(event)
	
end

Runtime:addEventListener("resize", onResize)

local function onMouse(event)
	local phase = event.type
	local x, y = event.x, event.y

	if (phase == "move") then
		for i = 1, #recentProjectTitles do
			local withinRange = false

			if (x >= recentProjectTitles[i].x - recentProjectTitles[i].contentWidth and x <= recentProjectTitles[i].x) then
				if (y >= recentProjectTitles[i].y and y <= recentProjectTitles[i].y + recentProjectTitles[i].contentHeight) then
					withinRange = true
				end
			end

			if (withinRange) then
				recentProjectTitles[i]:setFillColor(0.70, 0.70, 0.70)
			else
				recentProjectTitles[i]:setFillColor(1, 1, 1)
			end
		end
	end

	return true
end

Runtime:addEventListener("mouse", onMouse)
