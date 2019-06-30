
--Define rollout floater values so that rollouts can be added as needed.  If the rollout is already open, then close the old one first.
try (closeRolloutFloater FSXShadersTool) catch ()

--Include the FSXMaterialIO.
fileIn (acesScriptsFolder + "\\Startup\\FSXMaterialIO.ms")
fileIn (acesScriptsFolder + "\\Startup\\AcesMatToDxShader.ms")

FSXShadersTool = newRolloutFloater "Shader Tool" 950 480
FSXShadersTool.pos = [50,50]

--Define the call back events script so that things update when you open a new file
callbacks.removescripts #filePostOpen
callbacks.addScript #filePostOpen filename:(acesPluginRoot + acesMax9ScriptsFolder + "\\openFileCallback.ms")

--Define struct that contains global values
struct fsxShaderGlobals
(
	thisFSProfile = fnGetFSUserProfile (),
	thisMaterial,
	newBmpLoad,
	matToChange,
	shaderProfilesList
)

--Define an instance of the global variables struct
thisShaderGlobals = fsxShaderGlobals ()

--Define the location of the material editor bitmaps
--helpDir = (getDir #help)
--rootPos = findString helpDir "help"
--global maxIconDir = ((subString helpDir 1 (rootPos - 1)) + "UI\\Icons\\")

--Define the rollout that contains the material and bitmap section of the tool.
Rollout matBitRollout "Materials and bitmaps:"
(
	label lblTreeName "Materials in the current scene:" pos:[10,10]
	dotNetControl matTree "TreeView" width:350 height:250 pos:[10,30]
	button refreshMats "Refresh Materials" pos:[10,290] width:350
	label lblCurrentMat "Currently Selected Material:" pos:[10,315] align:#left
	groupBox gbProfiles "Profiles:" width:360 height:105 pos:[5,335]
	dropDownList updaterProfiles items:#() width:350 height:25 pos:[10,355]
	button updateFromProfile "Update selected material from Profile!" width:350 pos:[10,383]
	button btnSaveExistingProfile "Overwrite selected!" width:120 pos:[10,410]
	button btnCreateNewProfile "Create new!" width:100 pos:[135,410]
	button btnDeleteProfile "Delete selected!" width:120 pos:[240,410]
	colorPicker difColor "Diffuse Color:    " pos:[370,10] width:130
	colorPicker specColor "Specular Color: " pos:[370,35] width:130
	spinner spnSpecValue "Specular Value:" pos:[370,65] width:60 align:#left range:[0,999,0] scale:1
	label textBmpType "Bitmap Type:" align:#left pos:[370,85]
	radioButtons bitmapSwitcher "" columns:1 labels:#("Diffuse", "Diffuse Alpha", "Night Texture", "Night Texture Alpha", "Specular Map", "Specular Map Alpha", "Reflection Map", "Bump Map", "Fresnel Ramp", "Detail Map") pos:[400,100]
	mapButton difBtn "" width:20 height:15 pos:[370,100]
	mapButton nightBtn "" width:20 height:15 pos:[370,130]
	mapButton specBtn "" width:20 height:15 pos:[370,160]
	mapButton reflectBtn "" width:20 height:15 pos:[370,190]
	mapButton bumpBtn "" width:20 height:15 pos:[370,205]
	mapButton fresnelBtn "" width:20 height:15 pos:[370,220]
	mapButton detailBtn "" width:20 height:15 pos:[370,235]
    -- Images no longer exist with current versions of 3dsMax.
	--button btnChangeBmp images:#(maxIconDir + "MeditTools_i.bmp",undefined,43,1,1,1,1) tooltip:"Change selected bitmap" width:24 height:24 pos:[370,260]
	--button btnClearBmp images:#(maxIconDir + "MeditTools_i.bmp",undefined,43,7,7,7,7) tooltip:"Clear selected bitmap" width:24 height:24 pos:[398,260]
	--button assignMatToSel images:#(maxIconDir + "MeditTools_i.bmp",undefined,43,5,5,5,5) tooltip:"Assign Material to selection" width:24 height:24 pos:[370,260]
	--button showMatInView images:#(maxIconDir + "MeditTools_i.bmp",undefined,43,21,21,21,21) tooltip:"Show material in viewport" width:24 height:24 pos:[398,260]
    button assignMatToSel "Assign Material to selection" width:140 pos:[370,260]
	button showMatInView "Show material in viewport" width:140 pos:[370,285]
	button cleanUpMatEditor "Clean up the material editor" width:140 pos:[370,315] tooltip:"This will remove all unused materials from the material editor and put all of the scene materials in order in the material editor."
	button addTexturePaths "Add texture paths" width:140 pos:[370,340] tooltip:"This will add all of the \"texture\" directories to the paths so that there will be no missing textures when working in house."
	
	bitmap currentBitmap height:384 width:384 pos:[520,10]
	label currentBitmapName "Bitmap Texture" pos:[520,400] align:#left
	label currentBitmapSize "Bitmap Size" pos:[720,400] align:#left
	label currentBitmapPath "Path:" pos:[520,420] width:340 align:#left
	--button btnChangeBmpPath "Force fill bitmaps on Prepar3D Material" width:260 pos:[520,326] enabled:false
	
	--When the clean up the material editor button is pressed, remove all unused materials and rearrange all of the scene materials
	on cleanUpMatEditor pressed do
	(
		fnFSCleanMatEditorWindow ()
	)
	
	--Define a function that collects the bitmap dirs form the ini file
	fn fnCollectBmpDirs = 
	(
		local thisUserIni = getMaxIniFile ()
		local iniDirs = #()
		local dirInt = 1
		while (getIniSetting thisUserIni "BitmapDirs" ("Dir" + dirInt as string)) != "" do 
		(
			append iniDirs (getIniSetting thisUserIni "BitmapDirs" ("Dir" + dirInt as string))
			dirInt += 1
		)
		return #(iniDirs, dirInt)
	)
	
	--When the Add texture paths button is pressed, gather all of the texture paths from the build tree.  Then parse the .ini file to see if they already exist.  If they don't, then add them.
	on addTexturePaths pressed do
	(
		local thisTexDirs = #()
		local addedDirs = #()
		getSubDirs FSArt thisTexDirs "*texture*"
		local thisUserIni = getMaxIniFile ()
		local iniDirs = fnCollectBmpDirs ()
		for i in thisTexDirs do
		(
			if (findItem iniDirs[1] i == 0) then 
			(
				setINISetting thisUserIni "BitmapDirs" ("Dir" + iniDirs[2] as string) i
				append addedDirs i
				iniDirs[2] += 1
			)
		)
		if addedDirs.count > 0 then
		(
			messageBox ("The following texture directories were added:\n" + addedDirs as string + "\nIn order for this change to take effect, you must restart Max.")
		)
		else
		(
			messageBox "No new texture directories were added because you already have them all defined."
		)
	)
	
	--Define a function that returns the root doc node of the shaderProfile file
	fn getFSXProfileRoot fsxProfileFile = 
	(
		local tempXML = MaxXML ()
		tempXML.loadDocument fsxProfileFile
		local tempRoot = tempXML.getRootNode()
		local rootChilds = tempRoot.getChildNodes()
		local profilesRoot = rootChilds[2].selectNodes("/ShaderProfiles")
		local theseProfiles = profilesRoot[1].getChildNodes ()
		local thisNode
		local friendlyNames = #()
		for i = 1 to theseProfiles.count do
		(
			local friendlyNodes = theseProfiles[i].selectNodes("FriendlyName")
			append friendlyNames (friendlyNodes[1].getNodeValue ())
			if (friendlyNodes[1].getNodeValue ()) == updaterProfiles.selected then thisNode = theseProfiles[i]
		)
		return #(tempXML,thisNode,profilesRoot[1],fsxProfileFile,friendlyNames)
	)
	
	--Define a function that will load up the list of shader profiles from the myProfiles.xml file
	fn loadProfileList =
	(
		if doesFileExist (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml") then
		(
			local theseFriendlies = getFSXProfileRoot (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
			updaterProfiles.items = theseFriendlies[5]
		)
	)
	
	--Setup the function to initialize the activeX control
	fn populateMatTree =
	(
		--Create the scenematerials node
		local newNode = matTree.nodes.add "Scene Materials"
		
		--Populate all of the nodes per material
		for m in sceneMaterials do
		(
			--Add standard and FSXMaterials
			if classOf m == standardMaterial or classOf m == P3DMaterial then
			(
				local thisNode = newNode.nodes.add m.name
				thisNode.tag = dotNetMXSValue m
			)
			if classOf m == multiMaterial then
			(
				local multiNode = newNode.nodes.add m.name
				multiNode.tag = dotNetMXSValue m
				for subM in m.materialList do
				(
					local subMNode = multiNode.nodes.add subM.name
					subMNode.tag = dotNetMXSValue subM
				)
			)
		)
		matTree.indent = 35
		matTree.labelEdit = true
		matTree.backColor = matTree.backColor.FromARGB 255 192 222 249
		matTree.foreColor = matTree.foreColor.FromARGB 255 50 10 10
		matTree.hotTracking = true
		matTree.fullRowSelect = false
		newNode.expand ()
	)
	
	--Define the conditions to refresh the material activeX control
	on refreshMats pressed do
	(
		matTree.nodes.clear ()
		populateMatTree ()
	)
	
	--Compare the selected material with the sceneMaterial list and return the appropriate scene material.  Return the appropriate sub material if it's a multi-sub.
	fn fnReturnSceneMat hitNode=
	(
		if hitNode != undefined do
		try
		(
			matTree.selectedNode = hitNode
			return hitNode.tag.value
		)
		catch ()
	)
	
	--Define a function to return the bitmap from the path. If it can't find it there, search until it does find it.
	fn fnFindBitmap bmpActiveBitmap =
	(
		--First check the current max file path for the texture
		if doesFileExist (maxFilePath + (fileNameFromPath bmpActiveBitmap)) then return (maxFilePath + (fileNameFromPath bmpActiveBitmap))
		else
		(
			--Next check the main texture directory
			if usingAcesBuild then (if doesFileExist (FSArt + "texture\\" + fileNameFromPath bmpActiveBitmap) then return (FSArt + "texture\\" + fileNameFromPath bmpActiveBitmap))
			
			--If none of the other paths have turned up the tetxure, then look in a parallel texture directory
			local thisTexDir = ((getFileNamePath (subString maxFilePath 1 (maxFilePath.count - 1))) + "texture")
			if doesFileExist thisTexDir then
			(
				if doesFileExist (thisTexDir + "\\" + (fileNameFromPath bmpActiveBitmap)) then return (thisTexDir + "\\" + (fileNameFromPath bmpActiveBitmap))
				else return bmpActiveBitmap
			)
			else
			(
				return bmpActiveBitmap
			)
		)
	)
	
	--Define a function that will load up the new bitmap image to be displayed in the bitmap area of the viewer.
	fn loadNewBitmap bmpSwitchState activeMaterial = 
	(
		matTypeState = case of
		(
			(classOf activeMaterial == standardMaterial and classOf activeMaterial.diffuseMap == bitmapTexture) : (bmpSwitchState as string + "a")
			(classOf activeMaterial == P3DMaterial and classOf activeMaterial.standard.diffuseMap == bitmapTexture): (bmpSwitchState as string + "b")
		)
		
		thisShaderGlobals.newBmpLoad = case matTypeState of
		(
			"1a" : try (fnFindBitmap activeMaterial.diffuseMap.fileName) catch ("No Bitmap Texture")
			"1b" : try (fnFindBitmap activeMaterial.standard.diffuseMap.fileName) catch ("No Bitmap Texture")
			"2a" : try (fnFindBitmap activeMaterial.diffuseMap.fileName) catch ("No Bitmap Texture")
			"2b" : try (fnFindBitmap activeMaterial.standard.diffuseMap.fileName) catch ("No Bitmap Texture")
			"3a" : try (fnFindBitmap activeMaterial.selfIllumMap.fileName) catch ("No Bitmap Texture")
			"3b" : try (fnFindBitmap activeMaterial.standard.selfIllumMap.fileName) catch ("No Bitmap Texture")
			"4a" : try (fnFindBitmap activeMaterial.selfIllumMap.fileName) catch ("No Bitmap Texture")
			"4b" : try (fnFindBitmap activeMaterial.standard.selfIllumMap.fileName) catch ("No Bitmap Texture")
			"5a" : try (fnFindBitmap activeMaterial.specularMap.fileName) catch ("No Bitmap Texture")
			"5b" : try (fnFindBitmap activeMaterial.standard.specularMap.fileName) catch ("No Bitmap Texture")
			"6a" : try (fnFindBitmap activeMaterial.specularMap.fileName) catch ("No Bitmap Texture")
			"6b" : try (fnFindBitmap activeMaterial.standard.specularMap.fileName) catch ("No Bitmap Texture")
			"7a" : try (fnFindBitmap activeMaterial.reflectionMap.fileName) catch ("No Bitmap Texture")
			"7b" : try (fnFindBitmap activeMaterial.standard.reflectionMap.fileName) catch ("No Bitmap Texture")
			"8a" : try (fnFindBitmap activeMaterial.bumpMap.fileName) catch ("No Bitmap Texture")
			"8b" : try (fnFindBitmap activeMaterial.standard.bumpMap.fileName) catch ("No Bitmap Texture")
			"9a" : "No Bitmap Texture"
			"9b" : try (fnFindBitmap activeMaterial.texFresnelRamp.fileName) catch ("No Bitmap Texture")
			"10a" : "No Bitmap Texture"
			"10b" : try (fnFindBitmap activeMaterial.texDetailMap.fileName) catch ("No Bitmap Texture")
			default : "No Bitmap Texture"
		)
	)
	
	--When the selection on the material activeX control is changed, update all of the parameters
	on matTree BeforeLabelEdit cancelState do
	(
		enableAccelerators = false
	)
	on matTree afterLabelEdit newLabel do
	(
		if newLabel.label != undefined then
		(
			local matToEdit = fnReturnSceneMat matTree.selectedNode
			matToEdit.name = newLabel.label
		)
		enableAccelerators = true
	)
	
	--Define the function that fills in the bitmap and the bitmap text information
	fn loadBmpInfo =
	(
		local bmpToShrink
		finalBmpLoad = bitmap 384 384
		if thisShaderGlobals.newBmpLoad == "No Bitmap Texture" then thisShaderGlobals.newBmpLoad = (acesArtTools + "Images\\NoImage.jpg")

		if bitmapSwitcher.state == 2 or bitmapSwitcher.state == 4 or bitmapSwitcher.state == 6 then
		(
			try
			(
				for i = 1 to (BitmapLayerManager.getLayerCount thisShaderGlobals.newBmpLoad) do
				(
					if (BitmapLayerManager.getLayerName thisShaderGlobals.newBmpLoad (i - 1)) == "Alpha 1" then
					(
						bmpToShrink = (BitmapLayerManager.LoadLayer thisShaderGlobals.newBmpLoad (i - 1) true)
						exit
					)
				)
			) catch ()
		)
		else
		(
			try bmpToShrink = openBitmap thisShaderGlobals.newBmpLoad catch bmpToShrink = openBitmap (acesArtTools + "Images\\CantFindImage.jpg")
		)
		
		copy bmpToShrink finalBmpLoad
		currentBitmap.bitmap = finalBmpLoad
		try (currentBitMapName.text = filenameFromPath thisShaderGlobals.newBmpLoad) catch ()
		try (currentBitmapPath.text = getFilenamePath thisShaderGlobals.newBmpLoad) catch ()
		try
		(
			currentBitMapSize.text = (bmpToShrink.width as string + "x" + bmpToShrink.height as string)
		)
		catch
		(
			if bmpToShrink == undefined then copy (openBitmap (acesArtTools + "Images\\CantFindImage.jpg")) finalBmpLoad
			currentBitmap.bitmap = finalBmpLoad
			if bitmapSwitcher.state == 2 or bitmapSwitcher.state == 4 or bitmapSwitcher.state == 6 then currentBitMapSize.text = "Can't find alpha channel"
				else currentBitMapSize.text = "Can't Open"
		)
	)
	
	--Define a function that fills in the bitmaps and other properties whether your selecting the first material or a submaterial
	fn fillInBmpSlot thisMat =
	(
		if classOf thisMat == multiMaterial or classOf thisMat == undefinedClass then
		(
			thisShaderGlobals.newBmpLoad = "No Bitmap Texture"
			loadBmpInfo ()
		)
		if classOf thisMat == standardMaterial or classOf thisMat == P3DMaterial then
		(
			loadNewBitmap bitmapSwitcher.state thisMat
			loadBmpInfo ()
		)
	)
	
	--When you select a new bitmap type from the bitmap type radio buttons, update the thisShaderGlobals.thisMaterial value.
	on bitmapSwitcher changed newBmpState do
	(
		local thisMat = fnReturnSceneMat matTree.selectedNode
		fillInBmpSlot thisMat
	)
	
	--Define a function to change the values based on type of material
	fn fnChangeMatValue =
	(
		local thisMat = fnReturnSceneMat matTree.selectedNode
		if classOf thisMat == standardMaterial then
		(
			thisMat.diffuseColor = difColor.color
			thisMat.specularColor = specColor.color
			thisMat.specularLevel = spnSpecValue.value
		)
		if classOf thisMat == P3DMaterial then
		(
			thisMat.standard.diffuseColor = difColor.color
			thisMat.standard.specularColor = specColor.color
			thisMat.standard.specularLevel = spnSpecValue.value
		)
	)
	
	--Change the diffuse color, spec color and spec value when changed in the tool
	on difColor changed newDifColor do
	(
		fnChangeMatValue ()
	)
	on specColor changed newSpecColor do
	(
		fnChangeMatValue ()
	)
	on spnSpecValue changed newSpecValue do
	(
		fnChangeMatValue ()
	)
	
	--Define a function that tells what to do with the bitmap of the selected material (fill it or clear it)
	fn fnAffectBitmap bmpClassBitmapTex boolHasFresnel boolHasDetail mapType =
	(
		thisShaderGlobals.matToChange = fnReturnSceneMat matTree.selectedNode
				
		local changeTypeState = case of
		(
			(classOf thisShaderGlobals.matToChange == standardMaterial) : (mapType)
			(classOf thisShaderGlobals.matToChange == P3DMaterial): ("standard." + mapType)
		)
		
		matSlotToLoad = case mapType of
		(
			"texFresnelRamp" : (
									try
									(
										thisShaderGlobals.matToChange.bHasFresnelRamp = boolHasFresnel
										execute ("thisShaderGlobals.matToChange.texFresnelRamp = " + bmpClassBitmapTex)
										if (MatEditor.isOpen()) == true then (MatEditor.Close(); MatEditor.Open())
									) catch (messageBox "You can't put a fresnel ramp in a standard material!")
								)
			"texDetailMap" : 	(
									try
									(
										thisShaderGlobals.matToChange.bHasDetailMap = boolHasDetail
										execute ("thisShaderGlobals.matToChange.texDetailMap = " + bmpClassBitmapTex)
										if (MatEditor.isOpen()) == true then (MatEditor.Close(); MatEditor.Open())
									) catch (messageBox "You can't put a detail map in a standard material!")
								)
			default : try (execute ("thisShaderGlobals.matToChange." + changeTypeState + " = " + bmpClassBitmapTex)) catch ()
		)
		fillInBmpSlot thisShaderGlobals.matToChange
	)
	
	--Define a function that will update the various texture maps
	fn updateAcesTextureMaps btnName mapType = 
	(
		if btnName.map != undefined and classOf btnName.map == Bitmaptexture then
		(
			if btnName.map.filename != "" then
			(
				local texFileName = ("bitmapTexture filename:\"" + filenameFromPath btnName.map.filename + "\"")
				fnAffectBitmap texFileName true true mapType
				btnName.text = "M"
			)
		)
		else
		(
			fnAffectBitmap "undefined" false false mapType
			btnName.text = ""
		)
	)
	
	--Allow user to change bitmap associated with given channel
	on difBtn picked newDif do
	(
		updateAcesTextureMaps difBtn "diffuseMap"
	)
	on nightBtn picked newNight do
	(
		updateAcesTextureMaps nightBtn "selfillumMap"
	)
	on specBtn picked newSpec do
	(
		updateAcesTextureMaps specBtn "specularMap"
	)
	on reflectBtn picked newReflect do
	(
		updateAcesTextureMaps reflectBtn "reflectionMap"
	)
	on bumpBtn picked newBump do
	(
		updateAcesTextureMaps bumpBtn "bumpMap"
	)
	on fresnelBtn picked newFresnel do
	(
		updateAcesTextureMaps fresnelBtn "texFresnelRamp"
	)
	on detailBtn picked newDetail do
	(
		updateAcesTextureMaps detailBtn "texDetailMap"
	)
	
	--This button will assign the material selected in the matList listbox (the main material selector) to all selected items
	on assignMatToSel pressed do
	(
		local thisMat = fnReturnSceneMat matTree.selectedNode
		try
		(
			$.material = thisMat
		)
		catch ()
	)
	
	--If the show material button is pressed, toggle the material showing in viewport
	on showMatInView pressed do
	(
		local thisMat = fnReturnSceneMat matTree.selectedNode
		try
		(
			showTextureMap thisMat on
		)
		catch ()
	)
	
	--Define the rollout that contains all of the shader params wedges into it.
	Rollout shadeParams "Shader Parameters:"
	(
		--Define detail, bump and fresnel properties
		groupBox gbDetBumpFres "Detail/Bump/Fresnel/Emissive" height:220 width:190 pos:[10,10]
		label fresnelAffects "Fresnel ramp affects:" pos:[20,80]
		checkbox affectDiffuse "Diffuse" pos:[20,100]
		checkbox affectReflection "Reflection" pos:[20,120]
		checkbox affectSpecular "Specular" pos:[20,140]
		label detScaleText "Detail Scale: " align:#left pos:[20,30]
		spinner detScale type:#integer align:#left range:[0,64,1] width:70 pos:[20,45]
		label bmpScaleText "Bump Scale: " align:#left pos:[120,30]
		spinner bmpScale type:#integer align:#left range:[0,64,1] width:70 pos:[120,45]
		--Emissive information
		dropdownlist setEmissiveMode "Emissive Mode:" width:150 items:#("AdditiveNightOnly", "Blend", "MultiplyBlend", "Additive", "AdditiveNightOnlyUserControlled", "BlendUserControlled", "MultiplyBlendUserControlled", "AdditiveUserControlled") pos:[20,170]
		
		--Specular and reflection functionality
		groupBox gbSpecialFunc "Specular and reflection" height:220 width:200 pos:[210,10]
		checkbox specialEnvInvBaseAlpha "Blend Env by inv Dif A" pos:[220,125]
		checkbox specialEnvInvSpecAlpha "Blend Env by Spec A" pos:[220,145]
		checkbox specialDifDifAlpha "Blend Dif by Dif A" pos:[220,165]
		checkbox specialDifInvSpecAlpha "Blend Dif by inv Spec A" pos:[220,185]
		checkbox blendUseGlobal "Use global environment map" pos:[220,205]
		label refScaleText "Reflection Scale: "  align:#left pos:[220,30]
		spinner reflctScale type:#integer width:80 range:[0,100,100] pos:[220,45]
		label specMapPwrScaleText "Specular Map Power Scale:" align:#left pos:[220,75]
		spinner specMapPwrScale type:#integer width:80 range:[0,256,64] pos:[220,90]
		
		--Bloom and Blend info
		groupBox gbBloomBlend "Bloom and Blend info" height:220 width:280 pos:[420,10]
		label specBloomFloorText "Specular Bloom Floor: " align:#left pos:[430,30] 
		spinner specBloomFloor type:#float range:[0,1.0,0.9] width:80 pos:[430,45]
		checkbox hasBloom	"Allow bloom" pos:[430,160]
		checkbox hasBloomCopy     "Bloom material by copy" pos:[540,160]
	    checkbox hasBloomModulate "Bloom material mod by alpha" pos:[540,180]
		checkbox hasNoSpecularBloom "No specular bloom (use emissive)" pos:[430,200]
		checkbox hasEmissiveBloom	"Emissive Bloom" pos:[430,180]
		button setDefOpaque "Default Opaque" pos:[430,120] width:100
		button setDefTransp "Default Transparent" pos:[570,120] width:110
		dropdownlist blendSrc "Source Blend:" width:100 pos:[430,70] items:#("Zero", "One", "SrcColor", "InvSrcColor", "SrcAlpha", "InvSrcAlpha", "DestAlpha", "InvDestAlpha", "DestColor", "InvDestColor")
		dropdownlist blendDest "Destination Blend:" width:110 pos:[570,70] items:#("Zero", "One", "SrcColor", "InvSrcColor", "SrcAlpha", "InvSrcAlpha", "DestAlpha", "InvDestAlpha", "DestColor", "InvDestColor")
		label ambLightScaleText "Ambient Light Scale: " align:#left pos:[570,30] 
		spinner ambLightScale type:#float range:[0.00,1.00,0.90] width:80 pos:[570,45]

		--Alpha and Z-Test
		groupBox gbAlphaZTest "Alpha and Z-Test" height:220 width:260 pos:[710,10]
		--Z Test Alpha section
		checkbox yesZTestAlpha "Z-Test Alpha" pos:[720,30]
		label ZTestLevelText "Alpha Test Level: " align:#left pos:[720,110]
		spinner ZTestLevel type:#float range:[0.0,255.0,0.0] width:100 pos:[720,125]
		dropdownlist ZTestMode "Alpha Test Mode:" width:100 items:#( "Never", "Less", "Equal", "LessEqual", "Greater", "NotEqual", "GreaterEqual", "Always" ) pos:[720,60]
		--Set Final Alpha at render time
		checkbox setAlphaRender "Set final alpha value at render time" pos:[720,165]
		label setAlphaRenderText "Final Alpha Multiply: " align:#left across:2 pos:[720,185]
		spinner setAlphaRenderVal type:#float width:100 range:[0.0,1.0,1.0] pos:[720,200]
		label setZBiasText "ZBias: " align:#left across:2 pos:[720,220]
		spinner setZBiasVal type:#float width:100 range:[-100.0,1.0,100.0] pos:[720,240]
				
		
		--All the special Prepar3D parameters
		groupBox gbFSXBools "Prepar3D Special Parameters" height:140 width:320 pos:[10,240]
		checkbox hasVerticalNormal	"Assume vertical normal" pos:[20,260]
		checkbox yesZWriteAlpha		"Z-Write Alpha" pos:[20,280]
		checkbox noZWriteAlpha	"No Z Write" pos:[20,300]
		checkbox hasNoShadow		"No shadow" pos:[20,320]
		checkbox hasPrelitVertices	"Prelit vertices" pos:[20,340]
		checkbox hasNoBaseMaterialSpecular	"No Base Material Specular" pos:[20,360]
		checkbox isSkinned		"Skinned mesh" pos:[170,260]
		checkBox cbIsTwoSided "Double Sided Material" pos:[170,280]
		checkBox cbBlendConst "Blend Constant" pos:[170,300]
		checkBox cbWrap "Force Wrap" pos:[170,320]
		checkBox cbClamp "Force Clamp" pos:[170,340]
		
		groupBox gbEmissivePrecip "Aircraft and Precipitation properties" height:140 width:270 pos:[340,240]
		--Aircraft Material info
		checkbox isPanelProps	"This material is a panel texture" pos:[350,260]
		checkbox isNNumberProps	"This material is an N-Number texture" pos:[350,280]
		--Precipitation Properties
		label precipInfo "Precipitation Info:" pos:[350,310]
		checkbox hasPrecip       "Take into account" type:#boolean pos:[350,330]
		checkbox hasPrecipOffset "Apply offset to start" type:#boolean pos:[350,350]
		label hasPrecipOffsetText "Offset: " align:#left pos:[500,330]
		spinner precipOffsetAmount type:#float align:#left range:[0,1,0] width:80 pos:[500,350]
		
		--When the rollout is opened, make sure to define what the selected material is.
		on shadeParams open do
		(
			thisShaderGlobals.matToChange = fnReturnSceneMat matTree.selectedNode
		)
		
		--Define what happens when UI from the "Detail and Fresnel Properties" group is changed.
		on detScale changed newDetScale do 
		(
			detScaleText.caption = ("Detail Scale: " + newDetScale as string)
			thisShaderGlobals.matToChange.iDetailScale = detScale.value
			thisShaderGlobals.matToChange.params.lbl_DetailScale2.text = (detScale.value as string)
		)
		on bmpScale changed newBmpScale do
		(
			bmpScaleText.caption = ("Bump Scale: " + newBmpScale as string)
			thisShaderGlobals.matToChange.iBumpScale = bmpScale.value
			thisShaderGlobals.matToChange.params.lbl_BumpScale2.text = (bmpScale.value as string)
		)
		on affectDiffuse changed newAffectDiffuse do
		(
			thisShaderGlobals.matToChange.bFresnelDiffuse = newAffectDiffuse
		)
		on affectReflection changed newAffectReflection do
		(
			thisShaderGlobals.matToChange.bFresnelReflection = newAffectReflection
		)
		on affectSpecular changed newAffectSpecular do
		(
			thisShaderGlobals.matToChange.bFresnelSpecular = newAffectSpecular
		)
		
		--Define what happens when UI from the "Special Functionality" group is changed.
		on reflctScale changed newRefScaleVal do
		(
			refScaleText.caption = ("Reflection Scale: " + newRefScaleVal as string)
			thisShaderGlobals.matToChange.iEnvLevelScale = reflctScale.value
			thisShaderGlobals.matToChange.params.lbl_ReflectScale2.text = (reflctScale.value as string)
		)
		on specMapPwrScale changed newSpecMapPwrScaleVal do
		(
			specMapPwrScaleText.caption = ("Specular Map Power Scale: " + newSpecMapPwrScaleVal as string)
			thisShaderGlobals.matToChange.fSpecMapPowerScale = specMapPwrScale.value
			thisShaderGlobals.matToChange.params.lbl_SpecMapPowScale2.text = (specMapPwrScale.value as string)
		)
		on specialEnvInvBaseAlpha changed newSpecialEnvInvBaseAlpha do
		(
			if newSpecialEnvInvBaseAlpha == true then (specialEnvInvSpecAlpha.checked = false; thisShaderGlobals.matToChange.bBlendInvSpecAlpha = false)
			thisShaderGlobals.matToChange.bBlendInvBaseAlpha = newSpecialEnvInvBaseAlpha
		)
		on specialEnvInvSpecAlpha changed newSpecialEnvInvSpecAlpha do
		(
			if newSpecialEnvInvSpecAlpha == true then (specialEnvInvBaseAlpha.checked = false; thisShaderGlobals.matToChange.bBlendInvBaseAlpha = false)
			thisShaderGlobals.matToChange.bBlendInvSpecAlpha = newSpecialEnvInvSpecAlpha
		)
		on specialDifDifAlpha changed newSpecialDifDifAlpha do
		(
			thisShaderGlobals.matToChange.bBlendDiffuseBaseAlpha = newSpecialDifDifAlpha
		)
		on specialDifInvSpecAlpha changed newSpecialDifInvSpecAlpha do
		(
			thisShaderGlobals.matToChange.bBlendDiffuseInvSpecAlpha = newSpecialDifInvSpecAlpha
		)
		on blendUseGlobal changed newBlendUseGlobal do
		(
			thisShaderGlobals.matToChange.bUseGlobalEnv = newBlendUseGlobal
		)
				
		--Define what happens when UI from the "Bloom and Blend info" group is changed.
		on specBloomFloor changed newSpecBloomFloor do
		(
			specBloomFloorText.caption = ("Specular Bloom Floor: " + newSpecBloomFloor as string)
			thisShaderGlobals.matToChange.fSpecularBloomFloor  = newSpecBloomFloor
			thisShaderGlobals.matToChange.params.lbl_SpecularBloomFloor2.text = (newSpecBloomFloor as string)
		)
		on ambLightScale changed newAmbLightScale do
		(
			ambLightScaleText.caption = ("Ambient Light Scale: " + newAmbLightScale as string)
			thisShaderGlobals.matToChange.fAmbientLightScale  = newAmbLightScale
			thisShaderGlobals.matToChange.params.lbl_AmbientLightScale2.text = (newAmbLightScale as string)
		)
		on hasBloom changed newHasBloom do
		(
			thisShaderGlobals.matToChange.bAllowBloom = newHasBloom
		)
		on hasNoSpecularBloom changed newHasNoSpecularBloom do
		(
			thisShaderGlobals.matToChange.bNoSpecularBloom = newHasNoSpecularBloom
		)
		on hasEmissiveBloom changed newHasEmissiveBloom do
		(
			thisShaderGlobals.matToChange.bAllowEmissiveBloom = newHasEmissiveBloom
		)
		on hasBloomCopy changed newHasBloomCopy do
		(
			if newHasBloomCopy == true then (hasBloomModulate.checked = false; thisShaderGlobals.matToChange.bBloomModulate = false)
			thisShaderGlobals.matToChange.bBloomCopy = newHasBloomCopy
		)
		on hasBloomModulate changed newHasBloomModulate do
		(
			if newHasBloomModulate == true then (hasBloomCopy.checked = false; thisShaderGlobals.matToChange.bBloomCopy = false)
			thisShaderGlobals.matToChange.bBloomModulate = newHasBloomModulate
		)
		on setDefOpaque pressed do
		(
			blendSrc.selection = 2; blendDest.selection = 1; thisShaderGlobals.matToChange.iSrcBlend = 2; thisShaderGlobals.matToChange.iDstBlend = 1
		)
		on setDefTransp pressed do
		(
			blendSrc.selection = 5; blendDest.selection = 6; thisShaderGlobals.matToChange.iSrcBlend = 5; thisShaderGlobals.matToChange.iDstBlend = 6
		)
		on blendSrc selected newBlendSrc do
		(
			thisShaderGlobals.matToChange.iSrcBlend = newBlendSrc
		)
		on blendDest selected newBlendDest do
		(
			thisShaderGlobals.matToChange.iDstBlend = newBlendDest
		)
			
		--Define what happens when UI from the "Enhanced Parameters and Alpha Info" group is changed.
		on ZTestLevel changed newZTestLevel do
		(
			ZTestLevelText.caption = ("Alpha Test Level: " + newZTestLevel as string)
			thisShaderGlobals.matToChange.fAlphaTestLevel = newZTestLevel
			thisShaderGlobals.matToChange.params.lbl_AlphaTestLevel2.text = (newZTestLevel as string)
		)
		on yesZTestAlpha changed newYesZTestAlpha do
		(
			thisShaderGlobals.matToChange.bZTestAlpha = newYesZTestAlpha
		)
		on ZTestMode selected newZTestMode do
		(
			thisShaderGlobals.matToChange.iAlphaTestMode = newZTestMode
		)
		on setAlphaRender changed newSetAlphaRender do
		(
			thisShaderGlobals.matToChange.bFinalAlphaWrite = newSetAlphaRender
		)
		on setAlphaRenderVal changed newAlphaRenderVal do
		(
			setAlphaRenderText.caption = ("Final Alpha Multiply: " + newAlphaRenderVal as string)
			thisShaderGlobals.matToChange.fFinalAlphaWriteValue = newAlphaRenderVal
			thisShaderGlobals.matToChange.params.lbl_FinalAlphaWrite2.text = (newAlphaRenderVal as string)
		)
		on setZBiasVal changed newZBiasVal do
		(
			setZBiasText.caption = ("ZBias: " + newZBiasVal as string)
			thisShaderGlobals.matToChange.fZBias = newZBiasVal
			thisShaderGlobals.matToChange.params.lbl_ZBias.text = (newZBiasVal as string)
		)
		on hasVerticalNormal changed newHasVerticalNormal do
		(
			thisShaderGlobals.matToChange.bAssumeVerticalNormal = newHasVerticalNormal
		)
		on yesZWriteAlpha changed newYesZWriteAlpha do
		(
			thisShaderGlobals.matToChange.bZWriteAlpha = newyesZWriteAlpha
		)
		on noZWriteAlpha changed newNoZWriteAlpha do
		(
			thisShaderGlobals.matToChange.bNoZWrite = newNoZWriteAlpha
		)
		on hasNoShadow changed newHasNoShadow do
		(
			thisShaderGlobals.matToChange.bNoShadow = newHasNoShadow
		)
		on hasPrelitVertices changed newHasPrelitVertices do
		(
			thisShaderGlobals.matToChange.bPrelitVertices = newHasPrelitVertices
		)
		on hasNoBaseMaterialSpecular changed newHasNoBaseMaterialSpecular do
		(
			thisShaderGlobals.matToChange.bNoBaseMaterialSpecular = newHasNoBaseMaterialSpecular
		)
		on isSkinned changed newIsSkinned do
		(
			thisShaderGlobals.matToChange.bSkinned = newIsSkinned
		)
		on cbIsTwoSided changed newCBIsTwoSided do
		(
			thisShaderGlobals.matToChange.bDoubleSided = newCBIsTwoSided
		)
		on cbBlendConst changed newCBBlendConst do
		(
			thisShaderGlobals.matToChange.bBlendConstant = newCBBlendConst
		)
		on cbWrap changed newCBWrap do
		(
			thisShaderGlobals.matToChange.bForceTextureAddressWrap = newCBWrap
		)
		on cbClamp changed newCBClamp do
		(
			thisShaderGlobals.matToChange.bForceTextureAddressClamp = newCBClamp
		)
		
		--Define what happens when UI from the "Emissive, Aircraft and Precipitation properties" group is changed.
		on setEmissiveMode selected newSetEmissiveMode do
		(
			thisShaderGlobals.matToChange.iEmissiveMode = newSetEmissiveMode
		)
		on isPanelProps changed newIsPanelProps do
		(
			thisShaderGlobals.matToChange.bIsPanelTexture = newIsPanelProps
		)
		on isNNumberProps changed newIsNNumberProps do
		(
			thisShaderGlobals.matToChange.bIsNNumberTexture = newIsNNumberProps
		)
		on hasPrecip changed newHasPrecip do
		(
			thisShaderGlobals.matToChange.bUsePrecip = newHasPrecip
		)
		on hasPrecipOffset changed newHasPrecipOffset do
		(
			thisShaderGlobals.matToChange.bUsePrecipOffset = newHasPrecipOffset
		)
		on precipOffsetAmount changed newPrecipOffsetAmount do
		(
			hasPrecipOffsetText.caption = ("Offset: " + newPrecipOffsetAmount as string)
			thisShaderGlobals.matToChange.fPrecipOffset = newPrecipOffsetAmount
			thisShaderGlobals.matToChange.params.lbl_Offset2.text =  (newPrecipOffsetAmount as string)
		)
	)
	
	--Define the function that populates the shader parameters with whichever material is selected from the material and bitmap section.
	fn populateShadeParams thisMat =
	(
		try
		(
			removeRollout shadeParams FSXShadersTool
			FSXShadersTool.size = [950,480]
		)
		catch ()
		if classOf thisMat == P3DMaterial then
		(
			addRollout shadeParams FSXShadersTool
			FSXShadersTool.size = [950,885]
			
			--Use selected material to set up the shader parameters block
			shadeParams.affectDiffuse.checked = thisMat.bFresnelDiffuse
			shadeParams.affectReflection.checked = thisMat.bFresnelReflection
			shadeParams.affectSpecular.checked = thisMat.bFresnelSpecular
			shadeParams.detScale.value = thisMat.iDetailScale
			shadeParams.detScaleText.caption = ("Detail Scale: " + shadeParams.detScale.value as string)
			shadeParams.bmpScale.value = thisMat.iBumpScale
			shadeParams.bmpScaleText.caption = ("Bump Scale: " + shadeParams.bmpScale.value as string)
			
			shadeParams.specialEnvInvBaseAlpha.checked = thisMat.bBlendInvBaseAlpha
			shadeParams.specialEnvInvSpecAlpha.checked = thisMat.bBlendInvSpecAlpha
			shadeParams.specialDifDifAlpha.checked = thisMat.bBlendDiffuseBaseAlpha
			shadeParams.specialDifInvSpecAlpha.checked = thisMat.bBlendDiffuseInvSpecAlpha
			shadeParams.blendUseGlobal.checked = thisMat.bUseGlobalEnv
			shadeParams.reflctScale.value = thisMat.iEnvLevelScale
			shadeParams.refScaleText.caption = ("Reflection Scale: " + shadeParams.reflctScale.value as string)
			shadeParams.specMapPwrScale.value = thisMat.fSpecMapPowerScale
			shadeParams.specMapPwrScaleText.caption = ("Specular Map Power Scale: " + shadeParams.specMapPwrScale.value as string)
			
			shadeParams.specBloomFloor.value = thisMat.fSpecularBloomFloor
			shadeParams.specBloomFloorText.caption = ("Specular Bloom Floor: " + shadeParams.specBloomFloor.value as string)
			shadeParams.ambLightScale.value = thisMat.fAmbientLightScale
			shadeParams.ambLightScaleText.caption = ("Ambient Light Scale: " + shadeParams.ambLightScale.value as string)
			shadeParams.hasBloom.checked = thisMat.bAllowBloom
			shadeParams.hasEmissiveBloom.checked = thisMat.bAllowEmissiveBloom
			shadeParams.hasBloomCopy.checked = thisMat.bBloomCopy
			shadeParams.hasBloomModulate.checked = thisMat.bBloomModulate
			shadeParams.hasNoSpecularBloom.checked = thisMat.bNoSpecularBloom
			shadeParams.blendSrc.selection = thisMat.iSrcBlend
			shadeParams.blendDest.selection = thisMat.iDstBlend
			
			shadeParams.yesZTestAlpha.checked = thisMat.bZTestAlpha
			shadeParams.ZTestLevel.value = thisMat.fAlphaTestLevel
			shadeParams.ZTestLevelText.caption = ("Alpha Test Level: " + shadeParams.ZTestLevel.value as string)
			shadeParams.ZTestMode.selection = thisMat.iAlphaTestMode
			shadeParams.setAlphaRender.checked = thisMat.bFinalAlphaWrite
			shadeParams.setAlphaRenderVal.value = thisMat.fFinalAlphaWriteValue
			shadeParams.setAlphaRenderText.caption = ("Final Alpha Multiply: " + shadeParams.setAlphaRenderVal.value as string)
			shadeParams.setZBiasVal.value = thisMat.fZBias
			shadeParams.setZBiasText.caption = ("ZBias: " + shadeParams.setZBiasVal.value as string)
						
			shadeParams.hasVerticalNormal.checked = thisMat.bAssumeVerticalNormal
			shadeParams.yesZWriteAlpha.checked = thisMat.bZWriteAlpha
			shadeParams.noZWriteAlpha.checked = thisMat.bNoZWrite
			shadeParams.hasNoShadow.checked = thisMat.bNoShadow
			shadeParams.hasPrelitVertices.checked = thisMat.bPrelitVertices
			shadeParams.hasNoBaseMaterialSpecular.checked = thisMat.bNoBaseMaterialSpecular
			shadeParams.isSkinned.checked = thisMat.bSkinned
			shadeParams.cbIsTwoSided.checked = thisMat.bDoubleSided
			shadeParams.cbBlendConst.checked = thisMat.bBlendConstant
			shadeParams.cbWrap.checked = thisMat.bForceTextureAddressWrap
			shadeParams.cbClamp.checked = thisMat.bForceTextureAddressClamp
			
			shadeParams.setEmissiveMode.selection = thisMat.iEmissiveMode
			shadeParams.isPanelProps.checked = thisMat.bIsPanelTexture
			shadeParams.isNNumberProps.checked = thisMat.bIsNNumberTexture
			
			shadeParams.hasPrecip.checked = thisMat.bUsePrecip
			shadeParams.hasPrecipOffset.checked = thisMat.bUsePrecipOffset
			shadeParams.precipOffsetAmount.value = thisMat.fPrecipOffset
			shadeParams.hasPrecipOffsetText.caption = ("Offset: " + shadeParams.precipOffsetAmount.value as string)
		)
	)
	
	--Define a function that determines which texture slots are used and adjust the mapbuttons to reflect that
	fn fnFillMapSlots matPrefix matBtn = 
	(
		if matPrefix != undefined then matBtn.text = "M"
	)
	
	--Define a function that will fill in the colors, the bmp info and the material parameters.
	fn fnFillMatInfo thisMat =
	(
		--First reset all the map button texture slots
		difBtn.text = ""
		nightBtn.text = ""
		specBtn.text = ""
		reflectBtn.text = ""
		bumpBtn.text = ""
		fresnelBtn.text = ""
		detailBtn.text = ""
		
		--Get the diffuse color, specular color and specular value from the selected material.
		try
		(
			if classOf thisMat == standardMaterial then
			(
				difColor.color = thisMat.diffuseColor
				specColor.color = thisMat.specularColor
				spnSpecValue.value = thisMat.specularLevel
				fnFillMapSlots thisMat.diffuseMap difBtn
				fnFillMapSlots thisMat.selfIllumMap nightBtn
				fnFillMapSlots thisMat.specularMap specBtn
				fnFillMapSlots thisMat.reflectionMap reflectBtn
				fnFillMapSlots thisMat.bumpMap bumpBtn
			)
			if classOf thisMat == P3DMaterial then
			(
				difColor.color = thisMat.standard.diffuseColor
				specColor.color = thisMat.standard.specularColor
				spnSpecValue.value = thisMat.standard.specularLevel
				fnFillMapSlots thisMat.standard.diffuseMap difBtn
				fnFillMapSlots thisMat.standard.selfIllumMap nightBtn
				fnFillMapSlots thisMat.standard.specularMap specBtn
				fnFillMapSlots thisMat.standard.reflectionMap reflectBtn
				fnFillMapSlots thisMat.standard.bumpMap bumpBtn
				fnFillMapSlots thisMat.texFresnelRamp fresnelBtn
				fnFillMapSlots thisMat.texDetailMap detailBtn
			)
		)
		catch
		(
			difColor.color = (color 0 0 0)
			specColor.color = (color 0 0 0)
			spnSpecValue.value = 0
		)		
		
		--Load up the parameters for the selected material
		fillInBmpSlot thisMat
		populateShadeParams thisMat
		
		--Change the currenly select label
		local thisMatName
		try thisMatName = (thisMat.name) catch (thisMatName = "")
		lblCurrentMat.text = ("Currently Selected Material: " + thisMatName)
	)
	
	--When a new node in the matTree is selected, find the associated material and then load up the properties and the bitmaps
	on matTree click arg do
	(
		--Find the node the was clicked
		hitNode = matTree.GetNodeAt (dotNetObject "System.Drawing.Point" arg.x arg.y)
		
		--Find the selected material
		local thisMat = fnReturnSceneMat hitNode
		
		--Fill in the params
		fnFillMatInfo thisMat
	)	
	
	--When you choose to update your material based on a profile, it will update the params for either an Prepar3D material or upgrade a standard material to
	--an Prepar3D material with the params from the profile as the new params for the Prepar3D material.
	
	--Define a function that creates the profile file if none can be found
	fn fnCreateFSXProfile thisFSProfile =
	(
		makeDir (thisFSProfile + "\\My Documents\\Aces Files")
		local thisFSXProfile = createFile (thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
		format "%\n%\n%\n" "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>" "<ShaderProfiles>" "</ShaderProfiles>" to:thisFSXProfile
		close thisFSXProfile
	)
	
	--Define a function that applies the serialized material to the selected slot and then repopulates the tree and reselects that slot.
	fn fnApplySerializedMat thisFSXMat selectedMat =
	(
		for i = 1 to sceneMaterials.count do
		(
			if classOf sceneMaterials[i] == multiMaterial then
			(
				for j = 1 to sceneMaterials[i].materialList.count do
				(
					if sceneMaterials[i][j] == thisFSXMat then sceneMaterials[i][j] = fnApplyFSXMaterial ()
					showTextureMap sceneMaterials[i][j] on
				)
			)
			else
			(
				if sceneMaterials[i] == thisFSXMat then
				(
					local objList = for obj in $* where obj.material == thisFSXMat collect obj
					sceneMaterials[i] = fnApplyFSXMaterial ()
					for thisObj in objList do thisObj.material = sceneMaterials[i]
					showTextureMap sceneMaterials[i] on
				)
			)
		)
		
		--repopulate the mat tree to pick up the changes
		matTree.nodes.clear ()
		populateMatTree ()
		
		--Reselect the original material using the fullpath from the originally selected node
		local thisNode = matTree.topNode
		skipToString selectedMat "\\"
		while not eof selectedMat do
		(
			local thisNodeCopy = thisNode
			local thisPath = (readDelimitedString selectedMat "\\")
			local nodeChildren = thisNodeCopy.getNodeCount(false)
			for i = 0 to (nodeChildren - 1) do
			(
				if thisNodeCopy.nodes.item[i].text == thisPath then thisNode = thisNode.nodes.item[i]
			)
		)
		matTree.selectedNode = thisNode
		
		--Get the material and then fill in all the info.
		local thisMat = fnReturnSceneMat matTree.selectedNode
		fnFillMatInfo thisMat
		
		--Reselect the node after populating the params
		matTree.selectedNode = thisNode
	)
	
	on upDateFromProfile pressed do
	(
		if doesFileExist (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml") then
		(
			local selectedMat = matTree.selectedNode.fullPath as stringStream
			local loadMat = getFSXProfileRoot (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
			fnLoadFSXMaterial loadMat[2]
			local thisFSXMat = fnReturnSceneMat matTree.selectedNode
			
			--Define a function that will repopulate the original texture into the loaded material
			if classOf thisFSXMat == standardMaterial then
			(
				--Make sure the name and all of the textures from the current material are not updated.
				fsxMatMem.friendlyName = thisFSXMat.name
				try fsxMatMem.diffuseFile = thisFSXMat.diffuseMap.filename catch ()
				try fsxMatMem.emissiveFile = thisFSXMat.selfIllumMap.filename catch ()
				try fsxMatMem.specFile = thisFSXMat.specularMap.filename catch ()
				try fsxMatMem.envFile = thisFSXMat.reflectionMap.filename catch ()
				try fsxMatMem.bumpFile = thisFSXMat.bumpMap.filename catch ()
			)
			if classOf thisFSXMat == P3DMaterial then
			(
				--Make sure the name and all of the textures from the current material are not updated.
				fsxMatMem.friendlyName = thisFSXMat.name
				try fsxMatMem.diffuseFile = thisFSXMat.standard.diffuseMap.filename catch (fsxMatMem.diffuseFile = undefined)
				try fsxMatMem.emissiveFile = thisFSXMat.standard.selfIllumMap.filename catch (fsxMatMem.emissiveFile = undefined)
				try fsxMatMem.specFile = thisFSXMat.standard.specularMap.filename catch (fsxMatMem.specFile = undefined)
				try fsxMatMem.envFile = thisFSXMat.standard.reflectionMap.filename catch (fsxMatMem.envFile = undefined)
				try fsxMatMem.bumpFile = thisFSXMat.standard.bumpMap.filename catch (fsxMatMem.bumpFile = undefined)
				try fsxMatMem.fresnelFile = thisFSXMat.texFresnelRamp.filename catch (fsxMatMem.fresnelFile = undefined)
				try fsxMatMem.detailFile = thisFSXMat.texDetailMap.filename catch (fsxMatMem.detailFile = undefined)
			)
			--Reapply and reselect the slot
			fnApplySerializedMat thisFSXMat selectedMat
		)
		else
		(
			messageBox "I couldn't find a user profile to update, please use the save profile functionality to create one."
		)
	)
	
	--Define a function that will save a profile and create a new file if it can't find the MyP3DProfiles.xml file
	fn fnSaveNewFSXProfile doesExist tempFSXMat =
	(
		if doesFileExist (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml") then
		(
			local thisProfile = getFSXProfileRoot (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
			fnSaveFSXMaterial tempFSXMat thisProfile[1] thisProfile[2] thisProfile[3] thisProfile[4] doesExist
		)
		else
		(
			if queryBox "I couldn't find a user profile to update, do you want me to create one?" then
			(
				fnCreateFSXProfile thisShaderGlobals.thisFSProfile
				local thisProfile = getFSXProfileRoot (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
				fnSaveFSXMaterial tempFSXMat thisProfile[1] thisProfile[2] thisProfile[3] thisProfile[4] doesExist
			)
		)
	)
	
	--When create new profile is pressed, take the existing settings and add them to the MyP3DProfiles.XMl file
	on btnCreateNewProfile pressed do
	(
		local thisFSXMat = fnReturnSceneMat matTree.selectedNode
		if classOf thisFSXMat == P3DMaterial then
		(
			Rollout rollNewProfileName "New profile name"
			(
				editText strNewProfileName "New Profile Name:" fieldwidth:280 pos:[10,10]
				button btnGoProfileName "Go!" width:180 pos:[10,40]
				button btnCancelProfile "Cancel" width:180 pos:[200,40]
				
				on btnGoProfileName pressed do
				(
					local tempFSXMat = fnReturnSceneMat matTree.selectedNode
					local currentName = tempFSXMat.name
					tempFSXMat.name = strNewProfileName.text
					if strNewProfileName.text != "" then
					(
						fnSaveNewFSXProfile false tempFSXMat
						loadProfileList ()
						destroyDialog rollNewProfileName
					)
					else
					(
						messageBox "Please enter a name for the new Prepar3D shader profile!"
					)
					tempFSXMat.name = currentName
				)
				
				on btnCancelProfile pressed do
				(
					destroyDialog rollNewProfileName
				)
			)
			createDialog rollNewProfileName 400 80
		)
		else
		(
			messageBox "Please select a Prepar3D material to create a profile from!"
		)
	)
	
	--When the save existing profile button is pressed, save over the existing profile
	on btnSaveExistingProfile pressed do
	(
		local thisFSXMat = fnReturnSceneMat matTree.selectedNode
		if classOf thisFSXMat == P3DMaterial then
		(
			local thisName = thisFSXMat.name
			thisFSXMat.name = updaterProfiles.selected
			fnSaveNewFSXProfile true thisFSXMat
			thisFSXMat.name = thisName
			loadProfileList ()
		)
		else
		(
			messageBox "Please select a Prepar3D material to create a profile from!"
		)		
	)
	
	--When the delete button is pressed, remove the profile from the profiles document
	on btnDeleteProfile pressed do
	(
		if updaterProfiles.selected != undefined then
		(
			if doesFileExist (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml") then
			(
				local thisProfile = getFSXProfileRoot (thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
				thisProfile[3].removeChild(thisProfile[2])
				thisProfile[1].saveDocument(thisShaderGlobals.thisFSProfile + "\\My Documents\\Prepar3D v4 Files\\MyP3DProfiles.xml")
				loadProfileList ()
			)
			else
			(
				messageBox "I couldn't find a user profile to update, please use the save profile functionality to create one."
			)
		)
	)
	
	--Create a method for visualizing the shader using the default DirectX shader in Max 9.
	checkbutton visDXShader "Visualize using Direct X!" width:140 pos:[370,420]
	
	--Define a function that removes the shader visualization and resets the materials to the way they were.
	fn fnResetShaderVis thisMatToReapply =
	(
		--If the visualization is on, reset the shader back on the model and delete the omni light
		if visDXShader.state == true then
		(
			local selectedMat = matTree.selectedNode.fullPath as stringStream
			try (fnApplySerializedMat thisMatToReapply selectedMat) catch ()
			try (delete $AcesDXOmni) catch ()
		)
	)
	
	on visDXShader changed state do
	(
		local thisFSXMat = fnReturnSceneMat matTree.selectedNode
		if thisFSXMat != undefined then
		(
			if state == on then
			(
				--First disable the selection window so the user can't change which material they are viewing
				matTree.enabled = false
				
				--Create an omni light for sun visualization
				omniLight pos:[-20,-20,20] name:"AcesDXOmni" color:(color 128 128 128)
				
				--Now serialize the selected material for application
				fnSerializeFSXMaterial thisFSXMat
				global acesMatToReapply = fnAcesMatToDXShader fsxMatMem thisFSXMat
				
				--Minimize the window so you can see the viewport
				FSXShadersTool.placement = #minimized
				
				--Select the omni light for manipulation
				select $AcesDXOmni
				
				--Setup a call back so that if the user tries to save with the Dx visualizer open, it resets the shader first
				callbacks.addScript #filePreSave ("matBitRollout.fnResetShaderVis acesMatToReapply; matBitRollout.visDXShader.state = false; matBitRollout.matTree.enabled = true; callbacks.removescripts id:#removeVisDXSave") id:#removeVisDXSave
				callbacks.addScript #preExport ("matBitRollout.fnResetShaderVis acesMatToReapply; matBitRollout.visDXShader.state = false; matBitRollout.matTree.enabled = true; callbacks.removescripts id:#removeVisDXExport") id:#removeVisDXExport
			)
			else
			(
				--Re-enable the matTree control and then re-apply the Prepar3D material stored in memory to the original slot.
				matTree.enabled = true
				
				local selectedMat = matTree.selectedNode.fullPath as stringStream
				fnApplySerializedMat acesMatToReapply selectedMat
				
				--delete the omni light for visualization
				try (delete $AcesDXOmni) catch ()
				
				--remove the callbacks
				callbacks.removescripts id:#removeVisDXSave
				callbacks.removescripts id:#removeVisDXExport
			)
		)
	)
	
	--Populate stuff when you first open the tool
	on matBitRollout open do
	(
		--Populate the sceneMaterials into the material tree control
		populateMatTree ()
		
		--Populate the list of profiles
		loadProfileList ()
	)
	
	--Make sure if the user closes the tool that everything gets reset correctly.
	on matBitRollout close do
	(
		fnResetShaderVis acesMatToReapply
		
		--remove the callbacks
		callbacks.removescripts id:#removeVisDXSave
		callbacks.removescripts id:#removeVisDXExport
	)
)
addRollout matBitRollout FSXShadersTool