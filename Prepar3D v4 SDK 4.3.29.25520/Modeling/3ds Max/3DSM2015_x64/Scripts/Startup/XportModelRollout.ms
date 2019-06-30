
--Include the other rollouts being called so that they are available
try (fileIn (acesScriptsFolder + "\\Startup\\ExportCityPlacementRollout.ms")) catch ()
try (fileIn (acesScriptsFolder + "\\Startup\\ExportPlacementRollout.ms")) catch ()
try (fileIn (acesScriptsFolder + "\\Startup\\ModelViewerRollout.ms")) catch ()

--Define master export rollout.  This is the main UI for exporting files from Max to FS.
Rollout FSBuildingExport "Export Tools"
(	
	group "Root path info:"
	(
		label lblAcesRoot "Aces root path:" pos:[10,25] align:#left
		button btnChangeRoot "Change root path" width:100 pos:[200,20]
	)
	group "Export Model"
	(
		editText txtTextureDir "Tex Dir:" width:100 pos:[200,70] text:"texture"
		button thisModel "Export this Max file!" align:#left width:100 pos:[200,95]
		button allFSModels "Export all max files!" align:#left width:100 pos:[200,120]
		radioButtons rbBuildContentType "Build type:" labels:#("debug","retail","hybrid") columns:1 pos:[135,70] default:3
		checkBox checkPrompt "Prompt on export?" checked:false pos:[10,70]
        checkBox expNewFile "Choose export file?" checked:false pos:[10,90]
		checkBox checkXToMDL "Build MDL file?" checked:false pos:[10,110]
        checkBox checkBuild "Build on Export?" checked:false pos:[10,130]
	)

	--When the change root path button is pressed, define the new root path.
	on btnChangeRoot pressed do
	(
		local newPathName = getSavePath caption:"Choose new path to use"
		if newPathName != undefined then
		(
			FSRoot = newPathName
			lblAcesRoot.text = ("Current root path:   " + FSRoot)
		)
		--Redefine all of the Aces global based on the new path.
		fnSetupAcesGlobals ()
	)
	
	--Make sure the user cannot build on export and build only the MDL file
	on checkBuild changed newCheckBuild do
	(
		if newCheckBuild == true then checkXToMDL.checked = false
	)
	on checkXToMDL changed newCheckXToMDL do
	(
		if newCheckXToMDL == true then checkBuild.checked = false
	)
	
	
	--Define a function to add and remove rollouts to the FSExportAndLOD floater
	fn fnAddRollout rolloutType newSize =
	(
		addRollout rolloutType FSExportAndLOD
		FSExportAndLOD.size = [330,newSize]
	)
	fn fnRemoveRollout rolloutType = (try (removeRollout rolloutType FSExportAndLOD) catch ())
	
	--define export function for prompted export
	fn exportModel f =
	(
		setsilentmode true
		loadMaxFile f;
		exportFile (getFilenamePath (f) + getFilenameFile (f)  + ".x") using:"FSModelExp"
	)
	fn exportModelNew f =
	(
		setsilentmode true
		loadMaxFile f;
		newXFileName = getSaveFileName caption:"Export max file as:" filename:(getFilenamePath (f) + "*.x")
		if newXFileName != undefined then
		(
			exportFile newXFileName using:"FSModelExp"
		)
	)
	
	--define export function for non-prompted export
	fn exportModelNP f =
	(
		setsilentmode true
		loadMaxFile f;
		exportFile (getFilenamePath (f) + getFilenameFile (f)  + ".x") #noprompt using:"FSModelExp"
	)
	fn exportModelNPNew f =
	(
		setsilentmode true
		loadMaxFile f;
		newXFileName = getSaveFileName caption:"Export max file as:" filename:(getFilenamePath (f) + "*.x")
		if newXFileName != undefined then
		(
			exportFile newXFileName #noprompt using:"FSModelExp"
		)
	)
	
	--Create a function for checking the various export conditions and then exporting them appropriately
	fn fnExportFsModels fsFile =
	(
		if checkPrompt.checked == true then
		(
			if expNewFile.checked == true then
			(
				exportModelNew fsFile
			)
			else
			(
				exportModel fsFile
			)
		)
		else
		(
			if expNewFile.checked == true then
			(
				exportModelNPNew fsFile
			)
			else
			(
				exportModelNP fsFile
			)
		)
	)
	
	--This will parse all of the .x files in parallel directories and export them if they are newer than their associated max files.
	on allFSModels pressed do
	(
		local checkBuildState = checkBuild.checked
		if getSaveRequired () then
		(
			if checkForSave () then
			(
				--First gather all the max files in parallel directories
				local thisFilePath = maxFilePath as stringStream
				local thisEndPath = (getFilenameFile (subString (thisFilePath as string) 1 ((thisFilePath as string).count - 1)))
				local allFilePath = substring (thisFilePath as string) 1 ((thisFilePath as string).count - (thisEndPath.count + 1))
				for maxDirs in getDirectories (allFilePath + "*") do
				(	
					for f in getFiles (maxDirs + "*.max") do 
					(
						fileName = getFilenameFile f
						filePath = getFilenamePath f
						if doesFileExist (filePath + fileName + ".x") == false then
						(
							fnExportFsModels f
						)
						else
						(
							expFileVar = (maxUtils.compareFileTimeStamp f (filePath + fileName + ".x"))
							if expFileVar == 1 then
							(
								fnExportFsModels f 
							)
						)
					)
				)
				if checkBuildState == true then
				(
					buildDir = maxFilePath
					local thisDefineBuildType = FSDefineObjList ()
					local buildType = thisDefineBuildType.fnDefineBuildType rbBuildContentType.state
					local texBuild = buildAcesTexDir buildType buildDir txtTextureDir.text
					buildLocalContent buildDir "pause" buildType texBuild
					checkBuild.checked = true
				)
			)
		)
	)
	--Export an .x file from this Max file only.
	on thisModel pressed do
	(
		if checkPrompt.checked == true then
		(
			if expNewFile.checked == true then
			(
				newXFileName = getSaveFileName caption:"Export max file as:" filename:(maxFilePath + "*.x")
				if newXFileName != undefined then
				(
					exportFile newXFileName using:"FSModelExp"
				)
			)
			else
			(
				exportFile (maxFilePath + (getFileNameFile maxFileName) + ".x") using:"FSModelExp"
			)
		)
		else
		(
			if expNewFile.checked == true then
			(
				newXFileName = getSaveFileName caption:"Export max file as:" filename:(maxFilePath + "*.x")
				if newXFileName != undefined then
				(
					exportFile newXFileName #noprompt using:"FSModelExp"
				)
			)
			else
			(
				exportFile (maxFilePath + (getFileNameFile maxFileName) + ".x") #noprompt using:"FSModelExp"
			)
		)
		
		--If the build on export flag is checked, check the build type and build that build type
		if checkBuild.checked == true then
		(
			--Define the buildType for this object based on the selected drop down item
			local thisDefineBuildType = FSDefineObjList ()
			local buildType = thisDefineBuildType.fnDefineBuildType rbBuildContentType.state
		
			if expNewFile.checked == true then
			(
				if newXFileName != undefined then
				(
					buildDir = getFilenamePath newXFileName
					local texBuild = buildAcesTexDir buildType buildDir txtTextureDir.text
					buildLocalContent buildDir "pause" buildType texBuild
				)
			)
			else
			(
				buildDir = maxFilePath
				local texBuild = buildAcesTexDir buildType buildDir txtTextureDir.text
				buildLocalContent buildDir "pause" buildType texBuild
			)
		)
		
		--If the Build only mdl flag is checked, only run XtoMDL on the .x file and then copy it to the appropriate distrib directory
		if checkXToMDL.checked == true then
		(
			--Define some local variables
			local buildType, xToMdlCmd
			
			--Define the buildType for this object based on whether it is in the Aces build or whether it is using the SDK
			if usingAcesBuild then
			(
				local thisDefineBuildType = FSDefineObjList ()
				buildType = thisDefineBuildType.fnDefineBuildType rbBuildContentType.state
			)
			else
			(
				xToMdlCmd = ("call \"" + acesPluginDir as string + "\\XToMDL.exe\" /XANIM /DICT:\"" + acesPluginDir as string + "\\..\\..\\bin\\modeldef.xml\" ")
			)
			
			--create a function to define the copy to location for the finished .mdl file in the aces build
			fn fnCopyMdlTo buildDir buildType newXFilename =
			(
				local thisBuildDir = buildDir as stringStream
				skipToString thisBuildDir "objects\\"
				local thisMarker = (filePos thisBuildDir)
				local thisDir = (subString buildDir (thisMarker + 1) buildDir.count)
				if (matchPattern (readDelimitedString (thisDir as stringStream) "\\") pattern:"Global") or (matchPattern (readDelimitedString (thisDir as stringStream) "\\") pattern:"cities") then
				(
					messageBox "Building only the MDL file for cities or Global doesn't make any sense.  Please use the \"Build on Export\" check box for cities or Global."
				)
				else
				(
					if doesFileExist (FSRoot + "\\public\\distrib\\" + buildType + "\\i386\\SimObjects\\" + thisDir) then
					(
						--DOSCommand (acesCmdLine + "&pushd " + buildDir + "&xtomdl /XANIM " + newXFilename)
                        messageBox "Aces command line no longer supported"
						deleteFile (FSRoot + "\\public\\distrib\\" + buildType + "\\i386\\SimObjects\\" + thisDir + (getFilenameFile newXFilename) + ".MDL")
						if (copyFile ((getFilenamePath newXFilename) + (getFilenameFile newXFilename) + ".MDL") (FSRoot + "\\public\\distrib\\" + buildType + "\\i386\\SimObjects\\" + thisDir + (getFilenameFile newXFilename) + ".MDL")) then
						(
							deleteFile ((getFilenamePath newXFilename) + (getFilenameFile newXFilename) + ".MDL")
						)
					)
					else
					(
						messageBox ("There is no \"" + buildType + "\" directory in distrib.  Please select a build type that exists on this enlistment.")
					)
				)
			)
			
			--Create a function that builds the .x file into an .mdl and copies it into the appropriate location in the retail version of Prepar3D
			fn fnVendorMDL newXFilename xToMdlCmd =
			(
				DOSCommand (xToMdlCmd + "\"" + newXFilename + "\"&pause")
			)
			
			if expNewFile.checked == true then
			(
				if newXFileName != undefined then
				(
					buildDir = getFilenamePath newXFileName
					if usingAcesBuild then 
					(
						fnCopyMdlTo buildDir buildType newXFilename
					)
					else
					(
						fnVendorMDL newXFilename xToMdlCmd
					)
				)
			)
			else
			(
				buildDir = maxFilePath
				if usingAcesBuild then 
				(
					fnCopyMdlTo buildDir buildType (maxFilePath + (getFileNameFile maxFileName) + ".x")
				)
				else
				(
					fnVendorMDL (maxFilePath + (getFileNameFile maxFileName) + ".x") xToMdlCmd
				)
			)
		)
	)
	group "Model Viewer and Placements"
	(
		dotNetControl modelPlacement "toolStrip" pos:[10,180] width:295 height:20
	)
	
	on FSBuildingExport open do
	(
		--Populate the root path when the rollout is opened
		if usingAcesBuild then 
		(
			lblAcesRoot.text = ("Current root path:   " + FSRoot)
		)
		else
		(
            txtTextureDir.enabled = false
			btnChangeRoot.enabled = false
			lblAcesRoot.enabled = false
			checkBuild.enabled = false
			rbBuildContentType.enabled = false
			allFSModels.enabled = false
		)
		
		--Define the color
		modelPlacement.backColor = modelPlacement.backColor.FromARGB 255 192 222 249
		modelPlacement.foreColor = modelPlacement.foreColor.FromARGB 255 50 10 10
		
		--Add the Menu items
		modelPlacement.items.add("Model Viewer")
		modelPlacement.items.add("Placement File")
		modelPlacement.items.add("City Placement")
	)
	
	--When the radiobuttons are changed, load the appropriate rollout into the UI so it can be used.
	on modelPlacement click arg do
	(
		local hitNode = modelPlacement.getItemAt (dotNetObject "System.Drawing.Point" arg.x arg.y)
		if hitNode != undefined then
		(
			if hitNode.checked == false then
			(
				hitNode.checked = true
				local hitTest = case hitNode.text of 
				(
					"Model Viewer" : (if usingAcesBuild then (addRollout FSModelView FSExportAndLOD; FSExportAndLOD.size.y += 160) else (messageBox "This function is not supported at this time."))
					"Placement File" : (addRollout FSPlacementRoll FSExportAndLOD; FSExportAndLOD.size.y += 220)
					"City Placement" : (if usingAcesBuild then (addRollout FSCityPlacement FSExportAndLOD; FSExportAndLOD.size.y += 220) else (messageBox "This function is not supported at this time."))
				)
			)
			else
			(
				hitNode.checked = false
				local hitTest = case hitNode.text of 
				(
					"Model Viewer" : (try (removeRollout FSModelView FSExportAndLOD; FSExportAndLOD.size.y -= 160) catch ())
					"Placement File" : (try (removeRollout FSPlacementRoll FSExportAndLOD; FSExportAndLOD.size.y -= 220) catch ())
					"City Placement" : (try (removeRollout FSCityPlacement FSExportAndLOD; FSExportAndLOD.size.y -= 220) catch ())
				)
			)
		)
	)
	
	--If the Export model rollout is closed, make sure to close the subRollouts.
	on FSBuildingExport close do
	(
		try (if FSModelView.open then (removeRollout FSModelView FSExportAndLOD; FSExportAndLOD.size.y -= 160)) catch ()
		try (if FSPlacementRoll.open then (removeRollout FSPlacementRoll FSExportAndLOD; FSExportAndLOD.size.y -= 220)) catch ()
		try (if FSCityPlacement.open then (removeRollout FSCityPlacement FSExportAndLOD; FSExportAndLOD.size.y -= 220)) catch ()
	)
)