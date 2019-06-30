
if (MaxXML != undefined) or (MaxUtils != undefined) then
( 
	--First close the attach point tool if it is already opened
	try (closeRolloutFloater attachTool) catch ()
	
	--Set up Rollouts
	attachTool = newRolloutFloater "Attach Point Tool" 310 180
	
	--Setup a struct that contains some of the global variables to be used
	struct attachToolVars (clearBufferState = false, newAttachedName, attachToObj, attachText, attachName, errorNum = 0)
	thisATV = attachToolVars ()
	
	--Main Rollout tool
	Rollout attachToolRollout "Attach!"
	(
        
		--Define the user interface controls
		checkBox cbEffect "Effect" pos:[10,10]
		checkBox cbLibObj "Library Object" pos:[10,28]
		checkBox cbVisibility "Visibility" pos:[10,46]
		checkBox cbMouseRect "Mouse Rect" pos:[10,64]
		checkBox cbPlatform "Platform" pos:[10,82]
		checkBox cbNoCrash "No Crash" pos:[10,100]
		checkBox cbEmpty "Empty" pos:[10,118]
		checkBox clearBuffer "Remove FS9 Attach?" pos:[140,10]
		spinner spnOffsetFromObj "Offset from geometry:" range:[-10.0,10.0,0.0] scale:0.1 width:80 pos:[140,30] align:#left enabled:false
		editText txtAttachPointName "Attach name:" width:180 pos:[110,60]
		button attachPointGo "Attach to selected geometry!" width:180 pos:[110,85]
		checkButton attachPointNew "Create new attach point!" width:180 pos:[110,110]
		
		--Set up function to put the attach point XML information into the User Defined Properties
		fn attachPart attachType attachName attachText clearBufferState currentObject =
		(
			if(currentObject != unsupplied) then
			(
				fullXML = ("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?> <FSMakeMdlData version=\"9.0\"> " + (thisATV.attachText as string) + "</FSMakeMdlData>")
				if clearBuffer.state then
				(
					setUserPropBuffer currentObject ""
				)
				setUserProp currentObject "FSXML" fullXML
				
				--If the object is actually an attach point, thenchange the name, otherwise leave the part name alone.
				if (cbEffect.checked == true) or (cbLibObj.checked == true) or (cbPlatform.checked == true) or (cbEmpty.checked == true) then
				(
					currentObject.name = (attachType + attachName)
				)
			)
			
			else
			(
				fullXML = ("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?> <FSMakeMdlData version=\"9.0\"> " + (thisATV.attachText as string) + "</FSMakeMdlData>")
				if clearBuffer.state then
				(
					setUserPropBuffer $ ""
				)
				setUserProp $ "FSXML" fullXML
				
				--If the object is actually an attach point, thenchange the name, otherwise leave the part name alone.
				if (cbEffect.checked == true) or (cbLibObj.checked == true) or (cbPlatform.checked == true) or (cbEmpty.checked == true) then
				(
					$.name = (attachType + attachName)
				)
			)
		)
		
		--Define a function that will check to see if there is an attach num in the file.  If there isn't one, add one that is 0.  If there is one, return it.
		fn fnUpdateAttachNum = 
		(
			local attachNumID = fileProperties.findProperty #custom "AttachNum"
			if attachNumID == 0 then
			(
				fileProperties.addProperty #custom "AttachNum" 0
				return 0
			)
			else
			(
				local attachNum = fileProperties.getPropertyValue #custom attachNumID
				return attachNum
			)
		)
		
		--Define a function that creates the new attach name based on the effect or object plus the current increment number.  The increment number only gets changed 
		--when you select a new effect or object.
		fn fnIncrementNameNum = 
		(
			local thisAttachNum = fnUpdateAttachNum ()
			return (thisATV.newAttachedName as string + "_" + thisAttachNum as string)
		)
		
		--Define all of the rollouts possible to use starting with:
		--THE EFFECTS ROLLOUT
		Rollout effectsRollout "Attach Effect"
		(
			--Set up array of effects from the content directory.  If it can't be found automatically, then prompt the user.
			fn defineFXList = 
			(
				local effectsList = #()
				if usingAcesBuild then
				(
					for f in getFiles (FSContent + "effects\\*.fx") do
					(
						append effectsList (getFilenameFile f)
					)
				)
				else
				(
					local thisFSPath = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4" "SetupPath"
					global thisEffectsDir = thisFSPath + "effects"
					if not doesFileExist thisEffectsDir then
					(
						thisEffectsDir = getSavePath caption:"I can't find your Prepar3D effects directory.  Please locate the effects directory you want to choose from."
					)
					if thisEffectsDir != undefined then
					(
						for f in getFiles (thisEffectsDir + "\\*.fx") do
						(
							append effectsList (getFilenameFile f)
						)
					)
				)
				return effectsList
			)
		
			dropdownlist attachedEffect "Effect to attach:" items:(defineFXList ()) height:30
			edittext effectParams "Effect Params:" fieldwidth:184 height:100
			
			--When the effect drop down list is changed, update the name text box to use the name of that effect
			on attachedEffect selected newAttachedEffect do
			(
				thisATV.newAttachedName = attachedEffect.selected as stringStream
				try 
				(
					skipToString thisATV.newAttachedName "fx_"
					thisATV.newAttachedName = readLine thisATV.newAttachedName
					txtAttachPointName.text = fnIncrementNameNum ()
				)
				catch 
				(
					try
					(
						seek thisATV.newAttachedName 0
						skipToString thisATV.newAttachedName "Cntrl_"
						thisATV.newAttachedName = readLine thisATV.newAttachedName
						txtAttachPointName.text = fnIncrementNameNum ()
					)
					catch (txtAttachPointName.text = fnIncrementNameNum ())
				)
			)
			
			--When the rollout is opened, adjust the size of the tool and enable the offset spinner.
			on effectsRollout open do
			(
				attachTool.size = [310,attachTool.size.y + 185]
				spnOffsetFromObj.enabled = true
			)
			on effectsRollout close do
			(
				attachTool.size = [310,attachTool.size.y - 185]
			)
		)
		
		--THE LIBRARY OBJECT ROLLOUT
		Rollout libObjRollout "Attach Library Object"
		(
			editText txtObjXFile "Object .x file:"
			button btnChooseNewXFile "Choose new library object." width:290
			
			--When the choose button is pressed, update the text box and add it to the name above
			on btnChooseNewXFile pressed do
			(
				local tempLibObj = getOpenFileName caption:"Choose .x file to attach" types:"Prepar3D .x files (*.x)|*.x"
				if tempLibObj != undefined then
				(
					txtObjXFile.text = tempLibObj
					thisATV.newAttachedName = getFileNameFile tempLibObj
					txtAttachPointName.text = fnIncrementNameNum ()
				)
			)
			
			--Define a function that gets the guid from the object X file.
			fn getGUID = 
			(
				local xFileIn = txtObjXFile.text, maxFileIn, thisObjGUID
					
				--Get the GUID from the X File of the object
				if doesFileExist xFileIn then
				(
					--See if the Max file is in the same directory
					maxFileIn = (getFilenamePath xFileIn as string) + ((getFileNameFile xFileIn as string) + ".max")
					
					--Get the GUID from the .x file
					local FSXFileIn = openFile xFileIn
					skipToString FSXFileIn "GuidToName {"
					skipToString FSXFileIn "GuidToName {"
					skipToString FSXFileIn "\""
					thisObjGUID = (readDelimitedString FSXFileIn "\"")
					close FSXFileIn
				)
				return #(maxFileIn, thisObjGUID)
			)
			
			--When the rollout is opened, adjust the size of the tool and enable the offset spinner.
			on libObjRollout open do
			(
				attachTool.size = [310,attachTool.size.y + 70]
				spnOffsetFromObj.enabled = true
			)
			on libObjRollout close do
			(
				attachTool.size = [310,attachTool.size.y - 70]
				if effectsRollout.open == false then spnOffsetFromObj.enabled = false
			)
		)
		
		--Setup a struct for collecting up the part information based on mouse rect or visibility
		struct partInfoParser 
		(
		
			--Setup an array of part names that have visbility associated with them.
			arrPartNames = #(), strPartType,
			
			--Get the part names from the XML file by checking for the correct type (mouise rect or visiblity) and then loading it and parsing it.
			fn fnGetPartNamesFromXML arrPartNames strPartType =
			(
				visXMLParser = MaxXML ()
                local filePath
                
                -- Look for legacy UseLocalModelDef value and add LOCAL ModelDefLocationType
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
				
				for i = 1 to (fileProperties.getNumProperties #custom) do
				(
					local propertyName = (fileProperties.getPropertyName #custom i)
					
					if propertyName == "ModelDefLocationType" then
					(
						local propertyValue = (fileProperties.getPropertyValue #custom i)
						if(propertyValue == "DEFAULT") then
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
						)
						if(propertyValue == "LOCAL") then
						(
							filePath = maxFilePath
						)
						if(propertyValue == "CUSTOM") then
						(
							local temp = fileProperties.findProperty #custom "ModelDefLocation"
							filePath = fileProperties.getPropertyValue #custom temp
							filePath = filePath + "\\"
						)
					)
				)
			
				if filePath == undefined then
				(
				
					if usingAcesBuild then
					(
						filePath = MaxUtils.GetPluginPath()
					)
					else
					(
						local FSSDKDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath"
						filePath = FSSDKDir + "Modeling\\3ds Max\\bin\\"
					)
				)

				if not doesFileExist filePath then
				(
					filePath = getSavePath caption:"The path location of the modeldef.xml file is undefined, please navigate to the directory that contains the modeldef.xml file."
				)

				modelDefFile = filePath + "\\modeldef.xml"
				
				--If the modeldef file can't be found, prompt to find it.
				if not doesFileExist modelDefFile then
				(
					modelDefFile = getOpenFileName caption:"Locate the modeldef.xml file in the Prepar3D SDK." filename:"*.xml"
				)
				
				if modelDefFile != undefined then
				(
					visXMLDoc = visXMLParser.loadDocument modelDefFile
					visXMLRoot = visXMLParser.GetRootNode()
					visXMLRootInfo = visXMLRoot.GetChildNodes()
					visXMLInfo = visXMLRootInfo[2]
					fsVisInfo = visXMLInfo.selectNodes ("/ModelInfo/PartInfo")
					for s = 1 to fsVisInfo.count do
					(
						fsVisAttribs = fsVisInfo[s].getChildNodes()
						for i = 1 to fsVisAttribs.count do
						(
							if (fsVisAttribs[i].getNodeName()) == strPartType do
							(
								fsVisPartName = fsVisAttribs[1].getNodeValue()
								append arrPartNames fsVisPartName
							)
						)
					)
					sort arrPartNames
				)
				else
				(
					messageBox "No viable modeldef.xml file was found."
				)
			)
		)
		
		--THE VISIBILITY ROLLOUT
		Rollout visRollout "Apply part visibility"
		(
			dropDownList ddlVisSettings "Visibility Objects:" items:#()
			on visRollout open do
			(
				--Adjust the size
				attachTool.size = [310,attachTool.size.y + 80]
				
				--Setup local version of struct
				visInfoParser = partInfoParser ()
				visInfoParser.strPartType = "Visibility"
				visInfoParser.fnGetPartNamesFromXML visInfoParser.arrPartNames visInfoParser.strPartType
				ddlVisSettings.items = visInfoParser.arrPartNames
			)
			on visRollout close do
			(
				attachTool.size = [310,attachTool.size.y - 80]
			)
		)
		
		--THE MOUSE RECT ROLLOUT
		Rollout mouseRectRollout "Apply Mouse Rect Info"
		(
			dropDownList ddlMouseRectSettings "Mouse Rect Objects:" items:#()
			on mouseRectRollout open do
			(
				--Adjust the size
				attachTool.size = [310,attachTool.size.y + 80]
				
				--Setup local version of struct
				mrInfoParser = partInfoParser ()
				mrInfoParser.strPartType = "MouseRect"
				mrInfoParser.fnGetPartNamesFromXML mrInfoParser.arrPartNames mrInfoParser.strPartType
				ddlMouseRectSettings.items = mrInfoParser.arrPartNames
			)
			on mouseRectRollout close do
			(
				attachTool.size = [310,attachTool.size.y - 80]
			)
		)
		
		--THE PLATFORM ROLLOUT
		Rollout platformRollout "Set up a platform"
		(
			dropdownlist surfaceType "Surface Type:" items:#("CONCRETE","GRASS","WATER","GRASS_BUMPY","ASPHALT","SHORT_GRASS","LONG_GRASS","HARD_TURF","SNOW","ICE","URBAN","FOREST","DIRT","CORAL","GRAVEL","OIL_TREATED","STEEL_MATS","BITUMINUS","BRICK","MACADAM","PLANKS","SAND","SHALE","TARMAC","WRIGHT_FLYER_TRACK","LAST") height:30
			on platformRollout open do
			(
				attachTool.size = [310,attachTool.size.y + 80]
				thisATV.newAttachedName = surfaceType.selected
				txtAttachPointName.text = fnIncrementNameNum ()
			)
			on platformRollout close do
			(
				attachTool.size = [310,attachTool.size.y - 80]
			)
			
			--Redefine the name of the platform based on the type of platform
			on surfaceType selected newSurfaceType do
			(
				thisATV.newAttachedName = surfaceType.selected
				txtAttachPointName.text = fnIncrementNameNum ()
			)
		)
			
		--If the user manually changes the name, make sure that becomes the new attach name that gets incremented.
		on txtAttachPointName entered newTXTAttachPointName do
		(
			thisATV.newAttachedName = newTXTAttachPointName
			txtAttachPointName.text = fnIncrementNameNum ()
		)
		
		--Define a function that will open or close the rollouts for any of the various rollout sets.
		fn fnOpenCloseRollout cbEffect effectsRollout = 
		(
			cbEmpty.checked = false
			--Add the rollout if the box is checked.
			if cbEffect.state == true then addRollout effectsRollout attachTool
			else try (removeRollout effectsRollout) catch ()
		)
		
		--When any of the check boxes are checked or unchecked, either add or remove their rollouts as appropriate
		on cbEmpty changed newCBEmpty do
		(
			cbEffect.checked = false; removeRollout effectsRollout
			cbLibObj.checked = false; removeRollout libObjRollout
			cbVisibility.checked = false; removeRollout visRollout
			cbMouseRect.checked = false; removeRollout mouseRectRollout
			cbPlatform.checked = false; removeRollout platformRollout
			cbNoCrash.checked = false
		)
		on cbEffect changed newCBEffect do
		(
			fnOpenCloseRollout cbEffect effectsRollout
		)
		on cbLibObj changed newCBLibObj do
		(
			fnOpenCloseRollout cbLibObj libObjRollout
		)
		on cbVisibility changed newCBVisibility do
		(
			fnOpenCloseRollout cbVisibility visRollout
		)
		on cbMouseRect changed newCBMouseRect do
		(
			fnOpenCloseRollout cbMouseRect mouseRectRollout
		)
		on cbPlatform changed newCBPlatform do
		(
			fnOpenCloseRollout cbPlatform platformRollout
		)
		on cbNoCrash changed newCBNoCrash do
		(
			cbEmpty.checked = false
		)
		
		--Pull the defining of the effects and library object into its own function so it can be used by the simple attach mechanism
		fn fnDefineEffectLib =
		(
			--Define the effects parameters.
			if effectsRollout.open == true then
			(
				if effectsRollout.attachedEffect.selected != undefined then
				(
					format "%" "<AttachedObject> " to:thisATV.attachText
					format "%" ("<Effect effectName=\"" + effectsRollout.attachedEffect.selected + "\" effectParams=\"" + effectsRollout.effectParams.text + "\"/> ") to:thisATV.attachText
					format "%" "</AttachedObject> " to:thisATV.attachText
				)
				else
				(
					messageBox "Please selecte an effect from the drop down list.  If there are no effects available, please check the effects check-box again and chose a directory that has effects in it."
				)
			)
			
			--Get the GUID from the selected object and then define the library object parameters.
			if libObjRollout.open == true then
			(
				thisObjGUID = libObjRollout.getGUID ()
				if thisObjGUID[2] != undefined then
				(
					format "%" "<AttachedObject> " to:thisATV.attachText
					format "%" ("<LibraryObject name=\"" + thisObjGUID[2] + "\" scale=\"1.0\"/> ") to:thisATV.attachText
					format "%" "</AttachedObject> " to:thisATV.attachText
					return thisObjGUID[1]
				)
				--Fail the attach point operation and set the failed flag it it can't find a GUID.
				else 
				(
					messageBox "Attach point failed! I can't find a GUID for this object.  Make sure you have an object with a GUID selected."
					return false
				)
			)
		)
		
		--Define a function that sets up the text for the attach part based on the state of the UI.
		fn fnDefineAttachText = 
		(
			--Clear the attach text string so that all the new info can be output to it.
			thisATV.attachText = "" as stringStream
			
			--Only add in attach point info if there actually is an attach point
			if (effectsRollout.open == true) or (libObjRollout.open == true) or (cbEmpty.checked == true) then
			(
				--Put the attach point name into the XML blob.
				format "%" ("<Attachpoint name=\"attachpt_" + txtAttachPointName.text + "\"> ") to:thisATV.attachText
				
				--Call effects and library functionality
				fnDefineEffectLib ()
				
				--Close out the Attach Point info blob.
				format "%" "</Attachpoint> " to:thisATV.attachText
			)
			
			--If there's a platform, put in the platform information as well.
			if platformRollout.open == true then
			(
				format "%" ("<Platform name=\"platform_" + txtAttachPointName.text + "\" surfaceType=\"" + platformRollout.surfaceType.selected + "\" > </Platform> ") to:thisATV.attachText
			)
			
			--If No Crash is checked, then give it no crash boxes
			if cbNoCrash.checked == true then
			(
				format "%" ("<NoCrash/> ") to:thisATV.attachText
			)
			
			--Add in visibility part name info
			if visRollout.open == true then
			(
				format "%" ("<Visibility name=\"" + visRollout.ddlVisSettings.selected + "\"> </Visibility> ") to:thisATV.attachText
			)
			
			--Add in mouse rect part name info
			if mouseRectRollout.open == true then
			(
				format "%" ("<MouseRect name=\"" + mouseRectRollout.ddlMouseRectSettings.selected + "\"> </MouseRect> ") to:thisATV.attachText
			)
		)
		
		--Define a function that attaches all of the children of an object and then selects them
		fn fnAttachChildren = 
		(
			local arrChildList = #()
			local arrDummyList = #()
			for i = (execute ("$" + $.name + "/...*")) do
			(
				if classOf i != Dummy then 
				(
					append arrChildList i
				)
				else
				(
					append arrDummyList i
				)
			)
			--Get rid of any dummy nodes
			for i = 1 to arrDummyList.count do
			(
				if isDeleted arrDummyList[i] != true then
				(
					delete arrDummyList[i]
				)
			)
			
			--Select the first non-dummy and attach the rest to it
			select arrChildList[1]
			for i = 2 to arrChildList.count do
			(
				if isDeleted arrChildList[i] != true then
				(
					convertTo $ TriMeshGeometry
					try (attach $ arrChildList[i]) catch ()
				)
			)
		)
		
		--Define a function that will replace the attach point object with the real library object
		fn fnSetupAttachPt currentObject = 
		(
			local libObj = fnDefineAttachText ()
			
			if (libObjRollout.open == true) or ((libObj != OK) and (libObj != undefined)) then
			(
				local oldAttachPt
				local oldAttachPtPos
				local oldAttachPtRot
				
				if(currentObject != unsupplied) then
				(
					oldAttachPt = currentObject
					oldAttachPtPos = currentObject.position
					loldAttachPtRot = currentObject.rotation
				)
				
				--Do the getGUID function on library objects
				if (libObjRollout.open == true) then
				(
					local thisObjGUID = libObjRollout.getGUID ()
					libObj = thisObjGUID[1]
				)
				
				try 
				(
					arrThisObjObjs = (getMAXFileObjectNames libObj)
				) 
				catch
				(
					messageBox "Please enter a legitimate object .x file."
					return false
				)
				if arrThisObjObjs != undefined then
				(
					--If there is not a 100 LOD in the model file (which there always should be one), then fail and return false
					local intFind100LOD
					for i = 1 to arrThisObjObjs.count do
					(
						if (matchPattern arrThisObjObjs[i] pattern:"*_LOD_100") then intFind100LOD = arrThisObjObjs[i]
					)
					if intFind100LOD != undefined then
					(
						
						if(currentObject != unsupplied) then
						(
							mergeMaxFile libObj #mergeDups #useSceneMtlDups 
							clearSelection ()
							for i = 1 to arrThisObjObjs.count do
							(
								unhide (execute ("currentObject" + arrThisObjObjs[i]))
								selectMore (execute ("currentObject" + arrThisObjObjs[i]))
							)
							deselect (execute ("currentObject" + intFind100LOD + "...*"))
							delete currentObject
							select (execute ("currentObject" + intFind100LOD))
							currentObject.pivot = [0,0,0]
							
							--Attach the children into a single object
							fnAttachChildren ()
							
							--Since the merge was successful, get rid of the texture on the attach point, give the attach point its new position and rotation and then delete the old attach point
							currentObject.material = undefined
							currentObject.rotation = oldAttachPtRot
							currentObject.position = oldAttachPtPos
							delete oldAttachPt
						)
					)
					else
					(
						if thisATV.errorNum == 0 then
						(
							messageBox "I can't find the 100 LOD in this file, so I am using the standard sphere as the attach point instead of the object's 100 LOD."
						)
						thisATV.errorNum = 1
					)
				)
				else
				(
					if thisATV.errorNum == 0 then
					(
						messageBox "There is no Max file matching this .x file.  The geometry will not be merged.  A simple primitive will be used instead."
					)
					thisATV.errorNum = 1
				)
			)
			--Make sure rollouts are open before doing any attach point stuff
			if (effectsRollout.open == true) or (libObjRollout.open == true) or (visRollout.open == true) or (mouseRectRollout.open == true) or (platformRollout.open == true) or (cbEmpty.checked == true) or (cbNoCrash.checked == true) then
			(
				--Setup the names to be correct
				local thisAttachName = "attachpt_"
				if (platformRollout.open == true) then (thisAttachName = "platform_")
				
				--Update the name to match the new attachnum
				txtAttachPointName.text = fnIncrementNameNum ()
				
				--Setup the attach point based on the settings.
				attachPart thisAttachName txtAttachPointName.text thisATV.attachText clearBufferState currentObject
			)
		)
		
		--Define a function that will increase the attach num by 1 so that all the attach names in the file are unqiue
		fn fnIncrementAttachNum =
		(
			--Increment the attach num by one so that the next attach point also has a unique name.
			local attachNumID = fileProperties.findProperty #custom "AttachNum"
			local attachNum = fileProperties.getPropertyValue #custom attachNumID
			
			attachNum += 1

			fileProperties.deleteProperty #custom "AttachNum"
			fileProperties.addProperty #custom "AttachNum" attachNum
		)
		
		--Change user properties to attach effect on existing geometry
		on attachPointGo pressed do
		(
			if $ != undefined then
			(
				--Reset the error number so that errors will be displayed
				thisATV.errorNum = 0
					
				--Don't check for LOD name if the Visibility or Mouse Rect rollout is open.
				if (visRollout.open == true) or (mouseRectRollout.open == true) or (cbNoCrash.checked == true) then
				(	
					--Define the attach text and setup attach point if it is a library object
					fnSetupAttachPt ($)
				)
				
				--Check to see if we're trying to attach to an LOD, if we are, fail and warn the user.
				if (effectsRollout.open == true) or (libObjRollout.open == true) or (platformRollout.open == true) or (cbEmpty.checked == true) then
				(
					for object in getCurrentSelection() do
					(				
						if skipToString (object.name as stringstream) (getfilenamefile maxfilename) != undefined then
						(
							messageBox "Please do not attach to your main objects, this will blow them away."
						)
						else
						(					
							--Setup the text and then if the library object rollout is open, then replace the existing attach point with the new object
							fnSetupAttachPt(object)
							
							
							--Increment the attach num.
							fnIncrementAttachNum ()
							
							txtAttachPointName.text = fnIncrementNameNum ()
						)
					)
					

				)
				
			)
			else
			(
				messageBox "Please select an object to turn into an attach point!"
			)
		)
		
		on clearBuffer changed newBufferState do
		(
			thisAtv.clearBufferState = newBufferState
		)
		
		--Create a function that creates a sphere for effects or library objects or empty attach points, and then sets that sphere up to be a real attach point
		fn fsAttachFxObj = 
		(
			--Create a sphere that will be replaced with a lbrary object or used for effects or other attach types.
			sphere name:"tempAttachPt" pos:[0,0,0] radius:0.5 segs:4 wireColor:(color 246 7 255) isSelected:true
			
			--Setup the attach point and merge in the library object if there is one, then link it to the root node of the scene.
			fnSetupAttachPt ($)
			
			if rootnode.children.count == 2 then ($.parent = rootnode.children[1])
		)
		
		
		--When button is pressed, call function that allows the user to create easy point-and-click attach points.  Then increment the name of the attachpoint.
		on attachPointNew changed newAttachState do
		(
			--Reset the error number to 0 so the first error will be displayed
			thisATV.errorNum = 0
			
			if $ != undefined then
			(
				if attachPointNew.state == on do
				(
					if (effectsRollout.open == true) or (libObjRollout.open == true) or (cbEmpty.checked == true) then
					(
						--Define the object that is going to be used to do the hit testing on the point and click attach points
						thisATV.attachToObj = $
						
						--Define the attach text information
						fnDefineAttachText ()
						thisATV.attachName = txtAttachPointName.text
						
						--Setup a mouse tool that uses hit testing on the orginal selected object to align the attach point to the face normal
						tool objHitTest
						(
							--Define locals
							local fnDefineViewDirRay, alignToHitTest
							
							--Define a function that will get the camera position and view direction ray from the active viewport.
							fn fnDefineViewDirRay = 
							(
								local coordSysTM = Inverse(getViewTM())
								local viewDir = -coordSysTM.row3
								local viewPt = coordSysTM.row4
								return (ray viewPt viewDir)
							)
							
							--Define a function that aligns the attach point with the hit tested object
							fn alignToHitTest =
							(
								local vdr = fnDefineViewDirRay ()
								local p3ip = worldPoint
								local vdr = case (viewport.getType ()) of
								(
									default:undefined
									#view_top : (ray [p3ip.x,p3ip.y,vdr.pos.z = thisATV.attachToObj.max.z + 10] [0,0,-(vdr.pos.z + 10 - p3ip.z)])
									#view_bottom : (ray [p3ip.x,p3ip.y,vdr.pos.z = thisATV.attachToObj.min.z - 10] [0,0,-(vdr.pos.z - 10 - p3ip.z)])
									#view_right : (ray [vdr.pos.x = thisATV.attachToObj.max.x + 10,p3ip.y,p3ip.z] [-(vdr.pos.x + 10 - p3ip.x),0,0])
									#view_left : (ray [vdr.pos.x = thisATV.attachToObj.min.x - 10,p3ip.y,p3ip.z] [-(vdr.pos.x - 10 - p3ip.x),0,0])
									#view_front : (ray [p3ip.x,vdr.pos.y = thisATV.attachToObj.min.y - 10,p3ip.z] [0,-(vdr.pos.y - 10 - p3ip.y),0])
									#view_back : (ray [p3ip.x,vdr.pos.y = thisATV.attachToObj.max.y + 10,p3ip.z] [0,-(vdr.pos.y + 10 - p3ip.y),0])
									#view_persp_user : (ray vdr.pos -[vdr.pos.x - p3ip.x,vdr.pos.y - p3ip.y,vdr.pos.z - p3ip.z])
								)
								
								--Check to see if a valid viewport type was used, if not, then fail
								if vdr != undefined then
								(
									try
									(
										objIntersectPoint = (intersectRay thisATV.attachToObj vdr)
										if objIntersectPoint != undefined then
										(
											faceIntersectID = (intersectRayEx thisATV.attachToObj vdr)[2]
											faceIntNorm = (getFaceNormal thisATV.attachToObj faceIntersectID)
											objIntersectPoint.pos = [objIntersectPoint.pos.x + faceIntNorm.x * spnOffsetFromObj.value,objIntersectPoint.pos.y + faceIntNorm.y * spnOffsetFromObj.value,objIntersectPoint.pos.z + faceIntNorm.z * spnOffsetFromObj.value]
											return #(objIntersectPoint.pos,faceIntNorm)
										)
									) 
									catch 
									(
										return false
									)
								)
								else
								(
									viewport.setType #view_persp_user
									alignToHitTest ()
								)
							)
							
							on stop do
							(
								--Turn off the attach point tool and delete the current object
								attachPointNew.state = off
								if $ != thisATV.attachToObj then delete $
							)
							
							--Align the object before the first click
							on freeMove do
							(
								local thisObjAlign = alignToHitTest ()
								if thisObjAlign != undefined and thisObjAlign != false then
								(
									$.pos = thisObjAlign[1]
									$.dir = thisObjAlign[2]
								)
								if thisObjAlign == false then
								(
									messageBox "Your object needs to be an editable mesh in order for this to work.  Please collpase the stack or convert the object."
									#stop
								)
							)
							
							--On mouse click, do if left button attach the obj and load a new one, if right button, quit the tool and delete the current attach point
							on mousePoint click do
							(
								local thisObjAlign = alignToHitTest ()
								if thisObjAlign != undefined then
								(
									if click != 2 then 
									(
										--Increment the attach num.
										fnIncrementAttachNum ()
										txtAttachPointName.text = fnIncrementNameNum ()
										
										--Create the new attach point.
										fsAttachFxObj ()
									)
								)
							)
							
							--Align the object after the first click
							on mouseMove clickNo do
							(
								local thisObjAlign = alignToHitTest ()
								if thisObjAlign != undefined then
								(
									$.pos = thisObjAlign[1]
									$.dir = thisObjAlign[2]
								)
							)
							
							--When the tool is started, bring in the first attach point
							on start do
							(
								--Bring in the first attach point
								fsAttachFxObj ()
							)
						)
						startTool objHitTest
						
						--Delete any orphaned attach points and then reselect the hit testing object
						delete $tempAttachPt*
						select thisATV.attachToObj
					)
				)
			)
			else
			(
				messageBox "Please select an object to attach to!"
				attachPointNew.state = off
			)
		)
	)
	addRollout attachToolRollout attachTool
)
else
(
	messageBox "Either the MaxXML or the MaxUtils plugin cannot be found.  Please put those plugins into a folder along with the modeldef.xml file and assign a plugin path to that folder."
)