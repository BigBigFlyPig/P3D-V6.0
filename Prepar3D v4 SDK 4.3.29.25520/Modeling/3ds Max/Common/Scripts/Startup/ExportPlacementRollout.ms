
--Transfer the function into this space
FSDefineObjList = FSDefineObjList

--Define rollout for the export of placement files.  This will be used to create a dialog for exporting placement files into FS.
Rollout FSPlacementRoll "Export a placement file for the currently opened file"
(
	group "Export Placement File"
	(
		editText placeNameText "Filename:" fieldwidth:240 pos:[10,25] text:"NewPlacement"
		editText latText "Lat:" text:"N47 25.89" fieldwidth:100 pos:[37,50]
		editText lonText "Lon:" text:"W122 18.43" fieldwidth:100 pos:[174,50]
		editText headText "Heading:" text:"0" fieldwidth:40 pos:[12,75]
		checkBox cbBuildMDLIntoBGL "Build MDL file into BGL file?" pos:[10,100]
        checkBox cbConvertTextures "Convert textures?" pos:[10,120]
		checkBox cbLoadGame "Launch Prepar3D at this location?" pos:[10,140]
		button expPlacement "Export Placement Files!" width:140 pos:[10,160]
		button delPlacement "Delete Placement Files!" width:140 pos:[165,160]
	)
	
	--If the aces build is undefined then disable the build types
	on FSPlacementRoll open do
	(
		if usingAcesBuild then 
		(
			cbLoadGame.enabled = false
		)
	)
	    
	--Export a placement .XML file based on the LAT / LON.  Then use BGLComp to compile that .XML file into .BGL.
	on expPlacement pressed do
	(
        --Determine the location of the add-on as well as the location of BGLcomp
        local thisFSPath = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4" "SetupPath"
        local acesSimLoc = (thisFSPath + "Prepar3D.exe")

        local thisAddonPath = (systemTools.getEnvVariable("USERPROFILE") + "\\Documents\\Prepar3D v4 Add-ons\\" + (getFilenameFile maxFilename) + "\\")
        local thisAddonFile = (thisAddonPath + "add-on.xml")
        local thisAddonScenery = (thisAddonPath + "scenery\\")
        local thisAddonTexture = (thisAddonPath + "texture\\")
        local thisAddonModel = (thisAddonPath + "model\\")
        local thisMDLFile = (thisAddonModel + (getFilenameFile maxFilename) + ".MDL")
        local srcMDLFile = ((getFilenamePath maxFilePath) + (getFilenameFile maxFilename) + ".MDL")
        
        local thisPlaceFile = (thisAddonScenery + placeNameText.text + ".XML")
        local thisBGLFile = (thisAddonScenery + placeNameText.text + ".BGL")
        
        local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
        local bglCompLoc = ("\"" + FSSDKDir + "World\\Scenery\\bglcomp.exe\" " + "\"" + thisPlaceFile + "\"")
        local acesImagetool = ("\"" + FSSDKDir + "ImageTool.exe\" ")
        
        local acesFlightFile = (acesArtTools + "ScriptFlights\\DefaultMDLView.FLT")
        
		try
		(
            makeDir thisAddonPath
            makeDir thisAddonScenery
            makeDir thisAddonTexture

			local placementFile = createFile thisPlaceFile
			try
			(
				--Get the GUID of the current object by grabbing it from the file Properties.
				local intGUIDIndex = fileProperties.findProperty #custom "Guid"
				--Put the GUID in the curly braces so that it's in the correct format
				local objGUID = ("{" + (fileProperties.getPropertyValue #custom intGUIDIndex) + "}")

				--Define the XML format
				sceneObjLine = "<SceneryObject lat=\"" + latText.text + "\" lon=\"" + lonText.text + "\" alt=\"0\" pitch=\"0\" bank=\"0\" heading=\"" + headText.text + "\" altitudeIsAgl=\"TRUE\" imageComplexity=\"VERY_SPARSE\">"
				libObjectLine = "<LibraryObject name=\"" + objGUID + "\" scale=\"1.0\" />"
				format "%\n%\n    %\n        %\n    %\n" FSXMLVersionLine FSXMLSchemaLine sceneObjLine libObjectLine "</SceneryObject>" to:placementFile
				
				local loadGameVar = true
				--If the inlcude MDL in BGL check box is checked then build the MDL file into the BGL file. If the MDL is not present, then warn user and exit.
				if cbBuildMDLIntoBGL.checked then
				(
                    makeDir thisAddonModel
                    
                    deleteFile thisMDLFile
                    copyFile srcMDLFile thisMDLFile
                
					if doesFileExist thisMDLFile then
					(
						format "%\n" ("    <ModelData sourceFile=\"..\\model\\" + (filenameFromPath thisMDLFile) + "\" />") to:placementFile
					)
					else
					(
						messageBox ("Could not locate " + srcMDLFile as string + " for BGL compilation.  Please re-export it.")
						loadGameVar = false
					)
				)
				
				--Finish up the XML and close the file.
				format "%\n" "</FSData>" to:placementFile
				close placementFile
				--bglcomp the newly created file in add-on scenery
				DOSCommand ("call " + bglCompLoc)

                --Now try to create the add-on file
                try
                (
                    local addOnFile = createFile thisAddonFile
                    local xmlHeader = ("<?xml version=\"1.0\" encoding=\"Windows-1252\"?>")
                    local simBaseHeader = ("<SimBase.Document Type=\"AddOnXml\" version=\"3,3\" id=\"add-on\">")
                    local addOnName = ("    <AddOn.Name>" + (getFilenameFile maxFilename) + " Placement" + "</AddOn.Name>")
                    local addOnDescription = ("    <AddOn.Description>" + (getFilenameFile maxFilename) + " Placement Add-on" + "</AddOn.Description>")
                    local addOnComponentHeader = ("    <AddOn.Component>")
                    local addOnComponentFooter = ("    </AddOn.Component>")
                    local addOnSceneryTitle = ("        <Name>" + (getFilenameFile maxFilename) + " Scenery" + "</Name>")
                    local addOnSceneryCategory = ("        <Category>Scenery</Category>")
                    local addOnSceneryPath = ("        <Path>Scenery</Path>")
                    local addOnTextureTitle = ("        <Name>" + (getFilenameFile maxFilename) + " Texture" + "</Name>")
                    local addOnTextureCategory = ("        <Category>Texture</Category>")
                    local addOnTexturePath = ("        <Path>Texture</Path>")
                    local simBaseFooter = ("</SimBase.Document>")
                    
                    format "%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n" xmlHeader simBaseHeader addOnName addOnDescription addOnComponentHeader addOnSceneryTitle addOnSceneryCategory addOnSceneryPath addOnComponentFooter addOnComponentHeader addOnTextureTitle addOnTextureCategory addOnTexturePath addOnComponentFooter simBaseFooter to:addOnFile
                    
                    close addOnFile
                )
                catch
                (
                    messageBox ("You don't have a " + thisAddonFile as string + " directory for me to export an add-on.xml file!")
                    loadGameVar = false
                )
                
                --Now try to create the texture directory read me file
                try
                (
                    local readMeFile = createFile (thisAddonTexture + "ReadMe.txt")
                    local readMeText = ("Copy any textures required by all placed objects to this directory.")
                    format "%\n" readMeText to:readMeFile
                    
                    close readMeFile
                )
                catch
                (
                    messageBox ("You don't have a " + thisAddonTexture as string + " directory for me to export a ReadMe.txt file!")
                    loadGameVar = false
                )
                
                --First process all of the texture files in this file's directory
                if cbConvertTextures.checked then
                (
                    local texArray = #()
                    for f in getFiles (maxfilepath + "*.psd") do append texArray f
                    for f in getFiles (maxfilepath + "*.bmp") do append texArray f
                    for f in getFiles (maxfilepath + "*.tga") do append texArray f
                    for f in texArray do
                    (
                        local imagetoolCmd = case of
                        (
                            (matchPattern (getFilenameFile f) pattern:"*bump*") : ("call " + acesImagetool + "-brief -nobeep -nomip -dxt5 -RedInAlpha -nogui -dds -nodither \"" + f + "\"")
                            (matchPattern (getFilenameFile f) pattern:"*specular*") : ("call " + acesImagetool + "-brief -nobeep -mip -dxt5 -nogui -dds \"" + f + "\"")
                            default : ("call " + acesImagetool + "-brief -nobeep -mip -dxt5 -nogui -dds \"" + f + "\"")
                        )
                        
                        --Run imagetool on the files
                        DOSCommand imagetoolCmd
                        
                        --Check to see if the files exist and then copy them to the add-on texture directory
                        if doesFileExist (getFilenamePath f + getFilenameFile f + ".dds") then
                        (
                            DOSCommand ("copy \"" + getFilenamePath f + getFilenameFile f + ".dds\" \"" + thisAddonTexture + "\"")
                        )
                    )
                )
                
				--If the load game checkbox is checked, then create a flight file at the location and start flightsim there
				if cbLoadGame.checked then
				(
					--If the other steps succeeded then continue to load Flightsim
					if loadGameVar then
					(						
						--Edit the flight file to start at this lat/lon
						local thisFlightFile = openFile acesFlightFile mode:"r+"
						skipToString thisFlightFile "Latitude="
						for i = (20 - latText.text.count) to 20 do append latText.text " "
						format "%\n" latText.text to:thisFlightFile
						skipToString thisFlightFile "Longitude="
						for i = (20 - lonText.text.count) to 20 do append lonText.text " "
						format "%\n" lonText.text to:thisFlightFile
						
						--Start the sim engine with the newly updated flight file
						shellLaunch ("\"" + acesSimLoc + "\"") ("\"" + acesFlightFile + "\"")
						close thisFlightFile
					)
				)
			)
			catch
			(
				messageBox "This model doesn't have a GUID assigned to it.  Please add one."
				close placementFile
			)
		)
		catch
		(
			messageBox ("You don't have a " + thisPlaceFile as string + " directory for me to export to!")
		)
	)
	on delPlacement pressed do
	(
        local thisPlaceFile = (thisAddonScenery + placeNameText.text + ".XML")
        local thisBGLFile = (thisAddonScenery + placeNameText.text + ".BGL")
		deleteFile thisPlaceFile
		deleteFile thisBGLFile
	)
)
