--This rollout contains the GUID and Friendly name information and tools.
rollout GUIDAndFriendlyRoll "File Properties (GUID, Friendly Name, Shadow Map, Bounds)"
(
	editText guidTxt "GUID:" fieldWidth:215 enabled:false pos:[10,10]
	button createGUIDX "Create" align:#left width:50 pos:[263,8]
	editText FriendlyTxt "Friendly:" fieldWidth:205 enabled:false pos:[10,35]
	button changeFriendly "Edit" align:#left width:50 pos:[263,33]
	button getMaxGUIDX "Get GUID from .x file" align:#left width:140 pos:[10,60]
	
	radioButtons ModelLoc "ModelDef Location:" labels:#("Default","Local","Custom") columns:3 pos:[10,90] 
	edittext NewModelLoc "Location:" enabled:false pos:[10,130] width:280 height:20
	button btnBrowseLoc	"Browse" enabled:false pos:[10,160] width:90 height:20
	
	checkBox boolChooseXFile "Choose .X File to get GUID?" checked:false pos:[160,185]
	checkBox boolShadowMapReady "Model has shadow map?" checked:false pos:[10,185]

    checkBox checkOverrideBoundingBox "Override Bounding Box?" checked:false pos:[10,200]
    label lblMin "Min:" enabled:false pos:[10,220] align:#left
    editText txtBBMinX "X:" fieldWidth:60 enabled:false pos:[50,220]
    editText txtBBMinY "Y:" fieldWidth:60 enabled:false pos:[140,220]
    editText txtBBMinZ "Z:" fieldWidth:60 enabled:false pos:[230,220]
    label lblMax "Max:" enabled:false pos:[10,245] align:#left
    editText txtBBMaxX "X:" fieldWidth:60 enabled:false pos:[50,245]
    editText txtBBMaxY "Y:" fieldWidth:60 enabled:false pos:[140,245]
    editText txtBBMaxZ "Z:" fieldWidth:60 enabled:false pos:[230,245]
    
    checkBox checkOverrideRadius "Override Radius?" checked:false pos:[10,270]
    editText txtRadius "Radius:" fieldWidth:60 enabled:false pos:[204,270]
	
	global changeNow = true
	--Setup a function that populates the GUID and Friendly Name fields from the file properties
	fn fnChooseModelDef =
	(
		local filePath
		
		-- Lookup the ModelDefLocationType value

        for i = 1 to (fileProperties.getNumProperties #custom) do
        (
            local propertyName = (fileProperties.getPropertyName #custom i)
			
			if propertyName == "UseLocalModelDef" then
			(
				local propertyValue = (fileProperties.getPropertyValue #custom i)
				if propertyValue == "TRUE" then
				(
					fileProperties.addProperty #custom "ModelDefLocationType" "LOCAL"
					fileProperties.deleteProperty #custom "UseLocalModelDef"
				)
				if propertyValue == "FALSE" then
				(
					if usingAcesBuild then
					(
						filePath = MaxUtils.GetPluginPath() + "\\"
					)
					else
					(
						local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
						filePath = FSSDKDir + "Modeling\\3ds Max\\bin\\"
					)
					fileProperties.addProperty #custom "ModelDefLocationType" "DEFAULT"
					fileProperties.deleteProperty #custom "UseLocalModelDef"
				)
			)
        )
        
		if ModelLoc.state == 1 then
		(
			fileProperties.addProperty #custom "ModelDefLocationType" "DEFAULT"
			if usingAcesBuild then
			(
				filePath = MaxUtils.GetPluginPath() + "\\"
			)
			else
			(
				local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
				filePath = FSSDKDir + "Modeling\\3ds Max\\bin\\"
			)

		)
		if ModelLoc.state == 2 then
		(
			fileProperties.addProperty #custom "ModelDefLocationType" "LOCAL"
			filePath = maxFilePath
		)
		if ModelLoc.state == 3 then
		(
			try 
			(
				if(newModelLoc.text == "") then
				(
					fileProperties.addProperty #custom "ModelDefLocationType" "CUSTOM"
					filePath = getSavePath caption: "Select your ModelDef.xml directory"
					filePath = filePath + "\\"
					newModelLoc.text = filePath
				)
				else
				(
					fileProperties.addProperty #custom "ModelDefLocationType" "CUSTOM"
					filePath = newModelLoc.text
				)
				
				fileProperties.addProperty #custom "ModelDefLocation" filePath
			)
            catch()
		)
		
		if (filePath == undefined OR filePath == "") then
		(
			messageBox "Unable to find modeldef.xml. Please check that the file is in the specified folder and try again. Setting the current path back to default."
			fileProperties.addProperty #custom "ModelDefLocationType" "DEFAULT"
			if usingAcesBuild then
			(
				filePath = MaxUtils.GetPluginPath() + "\\"
			)
			else
			(
				local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
				filePath = FSSDKDir + "Modeling\\3ds Max\\bin\\"
			)
		)
		
		if not doesFileExist (filePath + "modeldef.xml") then
		(
			messageBox "Unable to find modeldef.xml. Please check that the file is in the specified folder and try again. Setting the current path back to default."
			fileProperties.addProperty #custom "ModelDefLocationType" "DEFAULT"
			if usingAcesBuild then
			(
				filePath = MaxUtils.GetPluginPath() + "\\"
			)
			else
			(
				local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
				filePath = FSSDKDir + "Modeling\\3ds Max\\bin\\"
			)
		)
	)
    
	fn fnPopulateGF =
	(
		try
		(
			local guidIndex = fileProperties.findProperty #custom "Guid"
			local friendlyIndex = fileProperties.findProperty #custom "FriendlyName"
			local thisGUID = fileProperties.getPropertyValue #custom guidIndex
			guidTxt.text = thisGUID
			local thisFriendly = fileProperties.getPropertyValue #custom friendlyIndex
			FriendlyTxt.text = thisFriendly
		) catch ()

		try
		(
			local smIndex = fileProperties.findProperty #custom "ShadowMapReady"
			local shadowmapReady = fileProperties.getPropertyValue #custom smIndex
			boolShadowMapReady.checked = (shadowmapReady == "TRUE");
		) catch (boolShadowMapReady.checked = false)
        
        try
        (
            local overrideRadiusIndex = fileProperties.findProperty #custom "OverrideRadius"
            local overrideRadiusValue = fileProperties.getPropertyValue #custom overrideRadiusIndex
            checkOverrideRadius.checked = true
            txtRadius.enabled = true
        )
        catch
        (
            checkOverrideRadius.checked = false
            txtRadius.enabled = false
        )
        try
		(
			try
			(
				local currentLoc = fileProperties.findProperty #custom "ModelDefLocationType"
				local currentLocVal = fileProperties.getPropertyValue #custom CurrentLoc
				if currentLocVal == "DEFAULT" then
				(
					ModelLoc.state = 1
				)
				if currentLocVal == "LOCAL" then
				(
					ModelLoc.state = 2
				)
				if currentLocVal == "CUSTOM" then
				(
					local currentPath = fileProperties.findProperty #custom "ModelDefLocation"
					local currentPathVal = fileProperties.getPropertyValue #custom CurrentPath
					
					
					ModelLoc.state = 3
					NewModelLoc.text = currentPathVal
					btnBrowseLoc.enabled = true
				)
				if currentLocVal == undefined then
				(
					ModelLoc.state = 1
				)
			)catch()
			try
			(
				local currentLoc2 = fileProperties.findProperty #custom "UseLocalModelDef"
				local currentLocVal2 = fileProperties.getPropertyValue #custom CurrentLoc2
				
				if currentLocVal2 == "FALSE" then
					ModelLoc.state = 1
				if currentLocVal2 == "TRUE" then
					ModelLoc.state = 2
			
			)catch()
			

			
		)
		catch
		(
			ModelLoc.state = 1
		)
		
        try
        (
            local radiusIndex = fileProperties.findProperty #custom "Radius"
            local radiusValue = fileProperties.getPropertyValue #custom radiusIndex
            txtRadius.text = radiusValue
        ) catch (txtRadius.text = "1.0")

        try
        (
            local overrideBBIndex = fileProperties.findProperty #custom "OverrideBoundingBox"
            local overrideBBValue = fileProperties.getPropertyValue #custom overrideBBIndex
            local overrideBBBoolean = (overrideBBValue == "TRUE")
            lblMin.enabled = overrideBBBoolean
            lblMax.enabled = overrideBBBoolean
            txtBBMinX.enabled = overrideBBBoolean
            txtBBMinY.enabled = overrideBBBoolean
            txtBBMinZ.enabled = overrideBBBoolean
            txtBBMaxX.enabled = overrideBBBoolean
            txtBBMaxY.enabled = overrideBBBoolean
            txtBBMaxZ.enabled = overrideBBBoolean
            checkOverrideBoundingBox.checked = overrideBBBoolean
        )
        catch
        (
            lblMin.enabled = false
            lblMax.enabled = false
            txtBBMinX.enabled = false
            txtBBMinY.enabled = false
            txtBBMinZ.enabled = false
            txtBBMaxX.enabled = false
            txtBBMaxY.enabled = false
            txtBBMaxZ.enabled = false
            checkOverrideBoundingBox.checked = false
        )

        try
        (
            local bbMinXIndex = fileProperties.findProperty #custom "BoundingBoxMinX"
            local bbMinXValue = fileProperties.getPropertyValue #custom bbMinXIndex
            txtBBMinX.text = bbMinXValue
        ) catch (txtBBMinX.text = "-1.0")
        
        try
        (
            local bbMinYIndex = fileProperties.findProperty #custom "BoundingBoxMinY"
            local bbMinYValue = fileProperties.getPropertyValue #custom bbMinYIndex
            txtBBMinY.text = bbMinYValue
        ) catch (txtBBMinY.text = "-1.0")
        
        try
        (
            local bbMinZIndex = fileProperties.findProperty #custom "BoundingBoxMinZ"
            local bbMinZValue = fileProperties.getPropertyValue #custom bbMinZIndex
            txtBBMinZ.text = bbMinZValue
        ) catch (txtBBMinZ.text = "-1.0")
        
        try
        (
            local bbMaxXIndex = fileProperties.findProperty #custom "BoundingBoxMaxX"
            local bbMaxXValue = fileProperties.getPropertyValue #custom bbMaxXIndex
            txtBBMaxX.text = bbMaxXValue
        ) catch (txtBBMaxX.text = "1.0")
        
        try
        (
            local bbMaxYIndex = fileProperties.findProperty #custom "BoundingBoxMaxY"
            local bbMaxYValue = fileProperties.getPropertyValue #custom bbMaxYIndex
            txtBBMaxY.text = bbMaxYValue
        ) catch (txtBBMaxY.text = "1.0")
        
        try
        (
            local bbMaxZIndex = fileProperties.findProperty #custom "BoundingBoxMaxZ"
            local bbMaxZValue = fileProperties.getPropertyValue #custom bbMaxZIndex
            txtBBMaxZ.text = bbMaxZValue
        ) catch (txtBBMaxZ.text = "1.0")
	)
	
	--When the rollout is opened, populate the GUID and Friendly Name
	on GUIDAndFriendlyRoll open do
	(
		fnPopulateGF ()
	)
	
	--Setup a function that allows you to add a new GUID or Friendly name, but queries the user to replace if one already exists
	local strGUIDOrFriendly, strNewGUIDOrFriendly
	fn queryGUIDAndFriendly strGUIDOrFriendly strNewGUIDOrFriendly = 
	(
		if (fileProperties.findProperty #custom strGUIDOrFriendly) == 0 then
		(
			fileProperties.addProperty #custom strGUIDOrFriendly strNewGUIDOrFriendly
		)
		else
		(
			if (queryBox ("This file already has a " + strGUIDOrFriendly + " associated with it.  Should I replace it?")) then
			(
				fileProperties.deleteProperty #custom strGUIDOrFriendly
				fileProperties.addProperty #custom strGUIDOrFriendly strNewGUIDOrFriendly
			)
		)
	)

    --Get ShadowMapReady flag from .x file
	on boolShadowMapReady changed newBoolShadowMap do
	(
		try
		(
			fileProperties.deleteProperty #custom "ShadowMapReady"
			if boolShadowMapReady.checked == true then
			(
				fileProperties.addProperty #custom "ShadowMapReady" "TRUE"
			)
		)
		catch (messageBox "Problem setting shadow map flag!")
		fnPopulateGF ()
	)
    
    -- Process Override Radius CheckBox has changed
    on checkOverrideRadius changed newCheckOverrideRadius do
    (
		try
		(
			fileProperties.deleteProperty #custom "OverrideRadius"
			if checkOverrideRadius.checked == true then
			(
				fileProperties.addProperty #custom "OverrideRadius" "TRUE"
                
                fileProperties.deleteProperty #custom "Radius"
                fileProperties.addProperty #custom "Radius" txtRadius.text
                
                txtRadius.enabled = true
			)
		)
		catch (messageBox "Problem setting override radius flag!")
		fnPopulateGF ()
    )

    -- Process Override Bounding Box CheckBox has changed
    on checkOverrideBoundingBox changed newCheckOverrideBoundingBox do
    (
		try
		(
            fileProperties.deleteProperty #custom "OverrideBoundingBox"
			if checkOverrideBoundingBox.checked == true then
			(
				fileProperties.addProperty #custom "OverrideBoundingBox" "TRUE"
                
                fileProperties.deleteProperty #custom "BoundingBoxMinX"
                fileProperties.addProperty #custom "BoundingBoxMinX" txtBBMinX.text
                
                fileProperties.deleteProperty #custom "BoundingBoxMinY"
                fileProperties.addProperty #custom "BoundingBoxMinY" txtBBMinY.text
                
                fileProperties.deleteProperty #custom "BoundingBoxMinZ"
                fileProperties.addProperty #custom "BoundingBoxMinZ" txtBBMinZ.text
                
                fileProperties.deleteProperty #custom "BoundingBoxMaxX"
                fileProperties.addProperty #custom "BoundingBoxMaxX" txtBBMaxX.text
                
                fileProperties.deleteProperty #custom "BoundingBoxMaxY"
                fileProperties.addProperty #custom "BoundingBoxMaxY" txtBBMaxY.text
                
                fileProperties.deleteProperty #custom "BoundingBoxMaxZ"
                fileProperties.addProperty #custom "BoundingBoxMaxZ" txtBBMaxZ.text
                
                lblMin.enabled = true
                lblMax.enabled = true
                txtBBMinX.enabled = true
                txtBBMinY.enabled = true
                txtBBMinZ.enabled = true
                txtBBMaxX.enabled = true
                txtBBMaxY.enabled = true
                txtBBMaxZ.enabled = true
            )
        )
        catch (messageBox "Problem setting override bounding box flag!")
        fnPopulateGF ()
    )
    
    -- Process Override Radius value has changed
    on txtRadius entered newTxtRadius do
    (
        fileProperties.deleteProperty #custom "Radius"
        fileProperties.addProperty #custom "Radius" newTxtRadius
    )
    
    -- Process Override Bounding Box values have changed
    on txtBBMinX entered newTxtBBMinX do
    (
        fileProperties.deleteProperty #custom "BoundingBoxMinX"
        fileProperties.addProperty #custom "BoundingBoxMinX" newTxtBBMinX
    )
    on txtBBMinY entered newTxtBBMinY do
    (
        fileProperties.deleteProperty #custom "BoundingBoxMinY"
        fileProperties.addProperty #custom "BoundingBoxMinY" newTxtBBMinY
    )
    on txtBBMinZ entered newTxtBBMinZ do
    (
        fileProperties.deleteProperty #custom "BoundingBoxMinZ"
        fileProperties.addProperty #custom "BoundingBoxMinZ" newTxtBBMinZ
    )
    on txtBBMaxX entered newTxtBBMaxX do
    (
        fileProperties.deleteProperty #custom "BoundingBoxMaxX"
        fileProperties.addProperty #custom "BoundingBoxMaxX" newTxtBBMaxX
    )
    on txtBBMaxY entered newTxtBBMaxY do
    (
        fileProperties.deleteProperty #custom "BoundingBoxMaxY"
        fileProperties.addProperty #custom "BoundingBoxMaxY" newTxtBBMaxY
    )
    on txtBBMaxZ entered newTxtBBMaxZ do
    (
        fileProperties.deleteProperty #custom "BoundingBoxMaxZ"
        fileProperties.addProperty #custom "BoundingBoxMaxZ" newTxtBBMaxZ
    )
	
	--Get GUID from .x file to make sure we aren't blowing away the real GUID.
	on getMaxGUIDX pressed do
	(
		try
		(
			local strThisFileX
			if boolChooseXFile.checked == true then
			(
				strThisFileX = openFile (getOpenFileName caption:"Choose .x file to grab the GUID and friendly from." filename:(maxFilePath + "*.x"))
			)
			else
			(
				strThisFileX = openFile (maxFilePath + (getFilenameFile maxFilename) + ".x")
			)
			skipToString strThisFileX "GuidToName {"
			skipToString strThisFileX "GuidToName {"
			if (skipToString strThisFileX "GuidToName {") != undefined then
			(
				strThisFileGUIDFromX = "badGUID"
				strThisFileFriendlyFromX = "badFriendly"
			)
			else
			(
				seek strThisFileX 0
				skipToString strThisFileX "GuidToName {"
				skipToString strThisFileX "GuidToName {"
				skipToString strThisFileX "{"
				strThisFileGUIDFromX = readDelimitedString strThisFileX "}"
				skipToNextLine strThisFileX
				skipToString strThisFileX "\""
				strThisFileFriendlyFromX = readDelimitedString strThisFileX "\""
			)
			
			--Use query function to test for existing GUID or Friendly Name
			if strThisFileGUIDFromX == "badGUID" then
			(
				messageBox "The GUID in this .X file is screwed up.  I'm not going to grab it."
			)
			else
			(
				queryGUIDAndFriendly "Guid" strThisFileGUIDFromX
			)
			if strThisFileFriendlyFromX == "badFriendly" then
			(
				messageBox "The Friendly Name in this .X file is screwed up.  I'm not going to grab it."
			)
			else
			(
				queryGUIDAndFriendly "FriendlyName" strThisFileFriendlyFromX
			)
			
			--Close the X file so it can be overwritten
			close strThisFileX
		)
		catch (messageBox "Either you didn't select an .x file, or I can't find it.  Please try again.")
		fnPopulateGF ()
	)
	
	--Create a new GUID and Firendly Name for this file
	on createGUIDX pressed do
	(
		local newGUIDX = fnDefineFSGUID ()
		queryGUIDAndFriendly "Guid" (newGUIDX as string)
		fnPopulateGF ()
	)

	on ModelLoc changed newLoc do
	(
		if newLoc == 3 then
		(
			btnBrowseLoc.enabled = true
		)
		else 
		(
			btnBrowseLoc.enabled = false
		)
		fnChooseModelDef ()
	)
	on btnBrowseLoc pressed do
	(
		try 
		(
			local filePath = getSavePath caption: "Select your ModelDef.xml directory"
			filePath = filePath + "\\"
			--ModelLoc.state = 3
		
			if not doesFileExist (filePath + "modeldef.xml") then
			(
				messageBox "Unable to find modeldef.xml. Please check that the file is in the specified folder and try again. Setting the current path back to default."
				fileProperties.addProperty #custom "ModelDefLocationType" "DEFAULT"
				
				if usingAcesBuild then
				(
					filePath = MaxUtils.GetPluginPath() + "\\"
				)
				else
				(
					local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
					filePath = FSSDKDir + "Modeling\\3ds Max\\bin\\"
				)
			)
			if filePath != undefined then
			(
				if ModelDefLocationType == "CUSTOM" then
				(
					fileProperties.addProperty #custom "ModelDefLocation" filePath
					NewModelLoc.text = filePath
				)
			)
		)
		catch()
		
	)
	
	
	--After editing the Friendly name, update that information and repopulate the GUID and Friendly name
	on friendlyTxt entered newFriendlyTxt do
	(
		queryGUIDAndFriendly "FriendlyName" newFriendlyTxt
		friendlyTxt.enabled = false
		fnPopulateGF ()
	)
	
	--When the edit friendly is pressed, allow the editing of friendly names.
	on changeFriendly pressed do
	(
		friendlyTxt.enabled = true
	)
)