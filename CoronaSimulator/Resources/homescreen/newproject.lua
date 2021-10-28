#!/usr/bin/env lua

------------------------------------------------------------------------------
--
-- This file is part of the Corona game engine.
-- For overview and more information on licensing please refer to README.md 
-- Home page: https://github.com/coronalabs/corona
-- Contact: support@coronalabs.com
--
------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------
--
-- newproject.lua	
--
-- Takes arguments and creates a config.lua and build.settings accordingly, then
-- it copies the two files, as well as all files from specified template into a
-- new folder.
--
-----------------------------------------------------------------------------------------

-- Usage:
--
--	lua newproject.lua game (options: blank, composer, game, app, ebook)
--
--	lua newproject.lua ebook width=320 height=480 ... (all other params listed below:)
--
--	params with default values:
--	
--	width=320
--	height=480
--	scale=letterbox
--	fps=30
--	orientation=portrait
--	statusBarHidden=false
--	hideIconShine=true
--	exitOnSuspend=true
--	savePath=output		-- path to save all template files, DO NOT include slash at end "/"
--  templateDir=
--

local lfs = require("lfs")

function string.starts( s, prefix )
	return string.sub(s,1,string.len(prefix)) == prefix
end

function string.ends( s, suffix )
	return suffix == '' or string.sub(s,-string.len(suffix)) == suffix
end

-----------------------------------------------------------------------------------------

local arg = ...

local buildSettingsTemplate = "buildsettings_template.txt"
local configLuaTemplate = "config_template.txt"

-----------------------------------------------------------------------------------------

local function pathForFile( file, baseDir )
	local result = file
	if ( baseDir ) then
		result = baseDir .. "/" .. file
	end
	return result
end

local function copyTemplateFiles( projectTemplate, savePath, baseDir, params )
	local params = params or {}
	local srcDir = pathForFile( projectTemplate or "blank", baseDir )
	local savePath = savePath or "output"
	local width = params.width or "320"
	local height = params.height or "480"
	local orientation = params.orientation or "portrait"

	-- if orientation isn't portrait, swap width and height
	if orientation ~= "portrait" then
		local tmp = width
		width = height
		height = tmp
	end

	-- copy all template files into output directory
	for originalFilename in lfs.dir( srcDir ) do
		if originalFilename ~= "." and originalFilename ~= ".." then

			local path = srcDir .. "/" .. originalFilename
			local fileType = lfs.attributes(path, "mode")
			print("Copying ", fileType, originalFilename)

			if fileType == "directory" then
				-- recurse to copy the directory
				lfs.mkdir(savePath.."/"..originalFilename)
				copyTemplateFiles( projectTemplate.."/"..originalFilename, savePath.."/"..originalFilename, baseDir, params )
			else
				local contents

				-- read file contents into a variable
				local file = io.open( path, "rb" )
				if file then
					contents = file:read( "*a" )
					io.close( file );
					file = nil
				else
					print("ERROR: Cannot open '"..path.."' for reading")
				end

				-- save contents to new location
				local newPath = savePath .. "/" .. originalFilename
				local file = io.open( newPath, "wb+" )
				if file ~= nil then
					if originalFilename:ends(".ccscene") then
						contents = string.gsub( contents, "{TARGET_WIDTH}", width )
						contents = string.gsub( contents, "{TARGET_HEIGHT}", height )
					end
					file:write( contents )
					io.close( file );
					file = nil
				else
					print("ERROR: Cannot open '"..newPath.."' for writing.  Try saving to a different directory")
				end
			end
		end
	end

end

-----------------------------------------------------------------------------------------

local function createConfig( projectTemplate, configFile, params )
	local params = params or {}
	
	-- extract parameters or use default values
	local projectTemplate = projectTemplate or "blank"
	local configFile = configFile or "config_template.txt"
	local templateBase = params.templateBase or "" --system.ResourceDirectory
	local width = params.width or "320"
	local height = params.height or "480"
	local scale = params.scale or "letterbox"
	local fps = params.fps or "60"
	local savePath = params.savePath or "output"
	local saveBase = params.saveBase or "" --system.DocumentsDirectory
	
	-- open template file
	--local templatePath = system.pathForFile( configFile, templateBase )
	local templatePath = pathForFile( configFile, params.templateBaseDir )
	local template
	local file = io.open( templatePath, "r" )

	if file ~= nil then
		template = file:read( "*a" )
		io.close( file )
		file = nil
	else
		print("ERROR: Cannot open '"..templatePath.."' for reading")
	end
	
	-- replace template 'tags' with parameters
	template = string.gsub( template, "{CONTENT_WIDTH}", width )
	template = string.gsub( template, "{CONTENT_HEIGHT}", height )
	template = string.gsub( template, "{CONTENT_SCALE}", '"' .. scale .. '"' )
	template = string.gsub( template, "{CONTENT_FPS}", fps )
	
	-- save the config file
	--local finalPath = system.pathForFile( savePath, saveBase )
	local finalPath = savePath .. "/config.lua"
	local file = io.open( finalPath, "w+" )
	if file ~= nil then
		file:write( template )
		io.close( file )
		file = nil
	else
		print("ERROR: Cannot open '"..finalPath.."' for writing.  Try saving to a different directory")
	end
end

-----------------------------------------------------------------------------------------

local function createBuildSettings( projectTemplate, buildSettingsFile, params )
	local params = params or {}
	
	-- extract parameters or use default values
	local projectTemplate = projectTemplate or "blank"
	local buildSettingsFile = buildSettingsFile or "buildsettings_template.txt"
	local templateBase = params.templateBase or "" --system.ResourceDirectory
	local orientation = params.orientation or "portrait"
	--local supportedOrientations = params.supportedOrientations or { "portrait" }
	local statusBarHidden = params.statusBarHidden or false
	local hideIconShine = params.hideIconShine or true
	local exitOnSuspend = params.exitOnSuspend or true
	local savePath = params.savePath or "output"
	local saveBase = params.saveBase or ""	--system.DocumentsDirectory
	
	-- open template file
	--local templatePath = system.pathForFile( buildSettingsFile, templateBase )
	local templatePath = buildSettingsFile
	local templatePath = pathForFile( buildSettingsFile, params.templateBaseDir )
	local template
	local file = io.open( templatePath, "r" )
	if file ~= nil then
		template = file:read( "*a" )
		io.close( file )
		file = nil
	else
		print("ERROR: Cannot open '"..templatePath.."' for reading")
	end

	-- take supported orientations table and create a string
	--[[
	local orientations = "{ "
	for i=1,#supportedOrientations do
		orientations = orientations .. '"' .. supportedOrientations[i] .. '", '
	end
	orientations = orientations .. " }"
	--]]
	
	-- replace template 'tags' with parameters
	template = string.gsub( template, "{ORIENTATION_DEFAULT}", '"' .. orientation .. '"' )
	--template = string.gsub( template, "{ORIENTATION_SUPPORTED}", orientations )
	template = string.gsub( template, "{PLIST_STATUSBAR}", tostring(statusBarHidden) )
	template = string.gsub( template, "{PLIST_ICON}", tostring(hideIconShine) )
	template = string.gsub( template, "{PLIST_EXITSUSPEND}", tostring(exitOnSuspend) )
	
	-- save the final build.settings file
	--local finalPath = system.pathForFile( savePath, saveBase )
	local finalPath = savePath .. "/build.settings"
	local file = io.open( finalPath, "w+" )
	if file ~= nil then
		file:write( template )
		io.close( file )
		file = nil
	else
		print("ERROR: Cannot open '"..filePath.."' for writing.  Try saving to a different directory")
	end
end

-----------------------------------------------------------------------------------------

local params
local projectTemplate = "blank"

if ( arg.isSimulator ) then
	-- simulator supplies arg as key/value pairs already
	params = arg
	projectTemplate = params.template or projectTemplate
else
	-- parse command-line arguments
	params = {}
	projectTemplate = arg[1] or projectTemplate
	
	if #arg > 1 then
		for i=2,#arg do
			local a = arg[i]
			local start = string.find( a, "=" )
			local paramName = string.sub( a, 1, start-1 )
			local paramVal = string.sub( a, start+1 )
			
			params[paramName] = paramVal
		end
	end
end

createConfig( projectTemplate, configLuaTemplate, params )
createBuildSettings( projectTemplate, buildSettingsTemplate, params )
copyTemplateFiles( projectTemplate, params.savePath or "output", params.templateBaseDir, params )
