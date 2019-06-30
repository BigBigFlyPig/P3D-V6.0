
--Set up all the local paths to use this will leave a file called "set.txt" in the directory with the max script
userDir = (fnGetFSUserProfile() + "\\My Documents\\CloudScript")

if doesFileExist userDir == false then
(
	DOSCommand ("mkdir " + "\"" + userDir + "\"")
)
if doesFileExist (userDir + "\\CreationSettings") == false then
(
	DOSCommand ("mkdir " + "\"" + userDir + "\\CreationSettings\"")
)
global createDir = (userDir + "\\CreationSettings\\")
if doesFileExist (userDir + "\\ColorPresets") == false then
(
	DOSCommand ("mkdir " + "\"" + userDir + "\\ColorPresets\"")
)
global colorDir = (userDir + "\\ColorPresets")
if doesFileExist (userDir + "\\Export") == false then
(
	DOSCommand ("mkdir " + "\"" + userDir + "\\Export\"")
)
global exportDir = (userDir + "\\Export\\")
Rollout clearscene "Setup Scene"
(
	--This button sets up the materials to match the cumulus01.bmp
	button selectAll "Select all sprites!" offset:[-58,0]
	on selectAll pressed do
	(
		select $plane*
	)
	button ClearSprite "Clear Old Sprites!" offset:[55,-26]
	on ClearSprite pressed do
	(
		deselect $*
		select $plane*
		for obj in $ do obj.lookat = obj
		max delete
	)
	button hideAll "Hide the Boxes!" offset:[-56,0]
	on hideAll pressed do
	(
		hide $box*
		hide $stratus*
		hide $solid*
		hide $wispy*
		hide $base*
	)
	button UnhideBoxes "Unhide the Boxes!" offset:[58,-26]
	on UnhideBoxes pressed do
	(
		unhide $box*
		unhide $stratus*
		unhide $solid*
		unhide $wispy*
		unhide $base*
	)
)
--Define some variables for the function to use
boxTotArea = 0
numSprites = 0
matNum = 1
UVW1 = 1
UVW2 = 1
Rollout Clouds "Create Cloud"
(
	--Set up the number, size, type and cull distance of all sprites
	editText sprite_num "Number of Sprites:" fieldWidth:59
	editText sprite_size "Size of Sprites:" fieldWidth:76
	editText cull_dist "Cull Distance:" fieldWidth:81
	editText ran_Value "Random Size Value:" fieldWidth:50
	radioButtons cloud_type "Cloud Coverage Type:" labels:#("Low Coverage", "Stratus", "Solid Cumulus", "Wispy Cumulus") offset:[80,-85]
	checkBox SelectCheck "Fill only selected boxes!" offset:[0,6]
	checkBox spritePerBox "Make each box have a sprite!"
	button Cloud "Create Cloud!!" offset:[-55,6]
	button LoadSettings "Load Cloud Settings!!" offset:[55,-26]
	editText sprite_count "Sprite Count:" fieldWidth:30 text:"0" offset:[0,6]
	button updateSpriteCount "Update Sprite Count!" offset:[60,-26]
	editText overdraw_amt "Overdraw Amount:" fieldwidth:80 text:"0" offset:[0,6]
	button overdraw_calc "Calculate!" offset:[80,-26]
	--Define a function that creates texture verts UVW coordinates for each of the 16 texture positions in the cumulus01.bmp
	fn texturePos matNum =
	(
		if matNum < 5 then
		(
			global tVert1 = [((matNum * .25) - .25),0.75,0]
			global tvert2 = [(matNum * .25),0.75,0]
			global tVert3 = [((matNum * .25) - .25),0.75,0]
			global tvert4 = [(matNum * .25),0.75,0]
			global tVert5 = [((matNum * .25) - .25),0.75,0]
			global tVert6 = [(matNum * .25),0.75,0]
			global tVert7 = [((matNum * .25) - .25),1.00,0]
			global tVert8 = [(matNum * .25),1.00,0]
		)
		if (matNum > 4 and matnum < 9) then
		(
			global tVert1 = [(((matNum - 4) * .25) - .25),0.50,0]
			global tvert2 = [((matNum - 4) * .25),0.50,0]
			global tVert3 = [(((matNum - 4) * .25) - .25),0.50,0]
			global tvert4 = [((matNum - 4) * .25),0.50,0]
			global tVert5 = [(((matNum - 4) * .25) - .25),0.50,0]
			global tVert6 = [((matNum - 4) * .25),0.50,0]
			global tVert7 = [(((matNum - 4) * .25) - .25),0.75,0]
			global tVert8 = [((matNum - 4) * .25),0.75,0]
		)
		if (matNum > 8 and matnum < 13) then
		(
			global tVert1 = [(((matNum - 8) * .25) - .25),0.25,0]
			global tvert2 = [((matNum - 8) * .25),0.25,0]
			global tVert3 = [(((matNum - 8) * .25) - .25),0.25,0]
			global tvert4 = [((matNum - 8) * .25),0.25,0]
			global tVert5 = [(((matNum - 8) * .25) - .25),0.25,0]
			global tvert6 = [((matNum - 8) * .25),0.25,0]
			global tvert7 = [(((matNum - 8) * .25) - .25),0.50,0]
			global tvert8 = [((matNum - 8) * .25),0.50,0]
		)
		if (matNum > 12 and matnum < 17) then
		(
			global tVert1 = [(((matNum - 12) * .25) - .25),0,0]
			global tvert2 = [((matNum - 12) * .25),0,0]
			global tVert3 = [(((matNum - 12) * .25) - .25),0,0]
			global tvert4 = [((matNum - 12) * .25),0,0]
			global tVert5 = [(((matNum - 12) * .25) - .25),0,0]
			global tvert6 = [((matNum - 12) * .25),0,0]
			global tvert7 = [(((matNum - 12) * .25) - .25),0.25,0]
			global tvert8 = [((matNum - 12) * .25),0.25,0]
		)
	)
	--Define the function that will be called to fill all the different types of boxes
	fn FillBoxes boxes =
	(
		boxVolume = ((boxes.max.x - boxes.min.x) * (boxes.max.y - boxes.min.y) * (boxes.max.z - boxes.min.z))
		numSprites = ((boxVolume / boxTotArea) * (sprite_num.text as float)) as integer
		if spritePerBox.checked == true then
		(
			if numSprites == 0 then numSprites = 1
		)
		--Fill box with a ratio of total sprites equivalent to volume ratio of box to all boxes
		for sprite_list = 1 to numSprites do
		(
			--Find the bounds of the box to fill
			Xval = (random boxes.min.X boxes.max.X)
			Yval = (random boxes.min.Y boxes.max.Y)
			Zval = (random boxes.min.Z boxes.max.Z)
			--Set wire color to same color as that of the box it's in
			newColor = boxes.wirecolor
			--define UVW values based on cloud type
			if cloud_type.state == 1 then
			(
				UVW1 = 5
				UVW2 = 10
			)
			if cloud_type.state == 2 then
			(
				UVW1 = 1
				UVW2 = 3
			)
			if cloud_type.state == 3 then
			(
				UVW1 = 11
				UVW2 = 16
			)
			if cloud_type.state == 4 then
			(
				UVW1 = 5
				UVW2 = 16
			)
			RotBool = false
			RotProp = getUserProp boxes "Rotation"
			if RotProp != undefined then RotBool = true
			if boxes.name == "base" then
			(
				RotBool = true
				RotProp = "5"
				UVW1 = 4
				UVW2 = 4
			)
			if boxes.name == "stratus" then
			(
				UVW1 = 1
				UVW2 = 3
			)
			if boxes.name == "solid" then
			(
				UVW1 = 11
				UVW2 = 16
			)
			if boxes.name == "wispy" then
			(
				UVW1 = 5
				UVW2 = 10
			)
			--Pick a random texture to assign (based on cloud type below)
			matNum = random UVW1 UVW2
			--Call function that determines the UVW coordinates based on cloud texture type
			texturePos matNum
			--Randomize the size of the sprites by the amount in the edit text
			ranSize = random -(ran_Value.text as float) (ran_Value.text as float)
			if ran_Value.text != "" then
			(
				ranSpriteSize = (sprite_size.text as float) + ranSize 
			)
			else
			(
				ranSpriteSize = (sprite_size.text as float)
			)
			--Create the plane with all of the appropriate values
			CreatePlane = plane wirecolor:newColor length:ranSpriteSize width:ranSpriteSize pos:[Xval,Yval,Zval] lengthsegs:1 widthsegs:1 material:meditmaterials[1] mapcoords:on
			if RotBool == true then 
			(
				select CreatePlane
				setUserProp $ "Rotation" RotProp
				convertTo $ TriMeshGeometry
				setTVert $ 1 tVert1
				setTVert $ 2 tVert2
				setTVert $ 3 tVert3
				setTVert $ 4 tVert4
				setTVert $ 5 tVert5
				setTVert $ 6 tVert6
				setTVert $ 7 tVert7
				setTVert $ 8 tVert8
			)
			if RotBool == false then 
			(
				select CreatePlane
				convertTo $ TriMeshGeometry
				setTVert $ 1 tVert1
				setTVert $ 2 tVert2
				setTVert $ 3 tVert3
				setTVert $ 4 tVert4
				setTVert $ 5 tVert5
				setTVert $ 6 tVert6
				setTVert $ 7 tVert7
				setTVert $ 8 tVert8
			)
		)
	)
	on Cloud pressed do
	(
		--Setup the progress bar
		progressStart "Creating a cloud"
		progressNum = 0.0
		--Define the total cubic volume of all the boxes put together
		boxTotArea = 0
		boxArray = #($box*, $solid*, $stratus*, $wispy*, $base*)
		for eachBox in boxArray do
		(
			for eachBoxes in eachBox do
			(
				boxTotArea = boxTotArea + ((eachBoxes.max.x - eachBoxes.min.x) * (eachBoxes.max.y - eachBoxes.min.y) * (eachBoxes.max.z - eachBoxes.min.z))
			)
		)
		--Update progress bar 5%
		progressNum = progressNum + 5.0
		progressUpdate progressNum
		--Export the settings for this particular file into a text file
		fileOut = createFile (createDir + (getfilenamefile maxFileName) + ".txt")
		format "%\n%\n%\n%\n%\n%\n%\n" sprite_num.text sprite_size.text cull_dist.text cloud_type.state ran_Value.text selectCheck.state spritePerBox.state to:fileOut
		if sprite_num.text == "" then
		(
			messageBox "Please enter the number of sprites."
		)
		if sprite_size.text == "" then
		(
			messageBox "Please enter the sprite size."
		)
		if cull_dist.text == "" then
		(
			messageBox "Please enter the cull distance."
		)
		else
		(
			All_Boxes = #($box*, $solid*, $stratus*, $wispy*, $base*)
			if selectCheck.checked == true then
			(
				for selectedBoxes in $selection do
				(
					FillBoxes selectedBoxes
				)
			)
			else
			(
				for boxTypes in All_boxes do
				(
					for boxes in boxTypes do
					(
						FillBoxes boxes
					)
				)
			)
			--Update progress bar to 10%
			progressNum = progressNum + 5.0
			progressUpdate progressNum
			for allObj in All_boxes do
			(
				hide allObj
			)
			deselect $*
			--Check all planes...
			global All_Planes = $plane* as array
			global Del_Planes = #()
			--Set up sprite count to adjust progress bar
			progressSprite = 0.0
			for i = 1 to All_Planes.count do
			(
				--...against the distance of all other planes...
				for j = (i + 1) to All_Planes.count do
				(
					if (distance All_Planes[i] All_Planes[j])<=((sprite_size.text as float)/(cull_dist.text as float)) do
					(
						if finditem Del_Planes All_Planes[i] == 0 then
						(
							append Del_Planes All_Planes[i]
						)
					)
				)
				--Update the progress bar
				progressSprite = progressSprite + 1.0
				progressUpdate (progressNum + ((progressSprite / $plane*.count) * 90.0))
			)
			--Delete all that are within clipping radius
			for delP in Del_Planes do
			(
				delP.lookat = delP
				delete delP
			)
			--Make all the planes face the camera
			for obj3 in $plane* do
			(
				--turn off back face cull so you can select the sprites from any angle
				obj3.backfacecull = false
				$camera01.lookat = obj3
			)
			sprite_count.text = $plane*.count as string
			progressEnd ()
		)
	)
	on loadSettings pressed do
	(
		fileIn = openFile (createDir + (getfilenamefile maxFileName) + ".txt")
		if fileIn == undefined then
		(
			messageBox "There are no saved settings for this Max file."
		)
		else
		(
			sprite_num.text = readline fileIn
			sprite_size.text = readline fileIn
			cull_dist.text = readline fileIn
			cloud_type.state = readline fileIn as integer
			ran_Value.text = readline fileIn
			if readLine fileIn == "true" then selectCheck.state = true else selectCheck.state = false
			if readLine fileIn == "true" then spritePerBox.state = true else spritePerBox.state = false
		)
	)
	on updateSpriteCount pressed do
	(
		sprite_count.text = $plane*.count as string
	)
	on overdraw_calc pressed do
	(
		totalArea = 0
		for overPlanes in $plane* do
		(
			--Set up vertices so that we can measure the width and length of each sprite
			try
			(
				Vert1 = overPlanes.verts[1].pos
				Vert2 = overPlanes.verts[2].pos
				Vert3 = overPlanes.verts[3].pos
				Plane_Width = distance Vert1 Vert2
				Plane_Length = distance Vert1 Vert3
			)
			--Or if it is still a plane for some reason export width and length
			catch
			(
				Plane_Width = (overPlanes.width * overPlanes.scale.x)
				Plane_Length = (overPlanes.length * overPlanes.scale.y)
			)
			totalArea = totalArea + ((plane_Width * plane_Length) / 256000000)
		)
		overdraw_amt.text = totalArea as string
	)
)
Rollout GroupUtil "Setup cloud groups"
(
	edittext CloudGroupNum "Cloud Group Number:" fieldwidth:20 text:"0"
	button SetCloudGroups "Assign to Selected!" offset:[54,-25]
	on SetCloudGroups pressed do
	(
		for objSprite in $selection do
		(
			setuserprop objSprite "Cloudgroup" (CloudGroupNum.text as integer)
		)
		CloudGroupNum.text = (((CloudGroupNum.text as integer) + 1) as string)
	)
	edittext CloudRotNum "Cloud Rotation:" fieldwidth:20 text:"0" offset:[0,6]
	button SetCloudRot "Assign to Selected!" offset:[28,-25]
	on SetCloudRot pressed do
	(
		for objSprite in $selection do
		(
			setuserprop objSprite "Rotation" (CloudRotNum.text as integer)
		)
	)
)
Rollout Export "Export Cloud"
(
	button SaveColors "Save Color Settings!" offset:[-60,0]
	button LoadColors "Load Color Settings!" offset:[60,-26]
	colorpicker Color1 "Color 1:" color:[104,105,110] 
	spinner Alpha1 "Alpha 1:" range:[0,255,255] offset:[-80,-25] fieldwidth:40
	spinner Color1Perc "Color 1 Height:" range:[0,100,0] offset:[0,5]
	colorpicker Color2 "Color 2:" color:[155,156,163]
	spinner Alpha2 "Alpha 2:" range:[0,255,255] offset:[-80,-25] fieldwidth:40
	spinner Color2Perc "Color 2 Height:" range:[0,100,30] offset:[0,5]
	colorpicker Color3 "Color 3:" color:[193,196,203]
	spinner Alpha3 "Alpha 3:" range:[0,255,255] offset:[-80,-25] fieldwidth:40
	spinner Color3Perc "Color 3 Height:" range:[0,100,50] offset:[0,5]
	colorpicker Color4 "Color 4:" color:[224,230,235]
	spinner Alpha4 "Alpha 4:" range:[0,255,255] offset:[-80,-25] fieldwidth:40
	spinner Color4Perc "Color 4 Height:" range:[0,100,70] offset:[0,5]
	colorpicker Color5 "Color 5:" color:[250,250,250]
	spinner Alpha5 "Alpha 5:" range:[0,255,255] offset:[-80,-25] fieldwidth:40
	spinner Color5Perc "Color 5 Height:" range:[0,100,100] offset:[0,5]
	on SaveColors pressed do
	(
		saveName = getMaxSaveFileName filename:(colorDir + "\\*.txt")
		if saveName != undefined then
		(
			fileOut = createFile saveName
			format "%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n%\n" Color1.color.r Color1.color.g Color1.color.b Color1Perc.value Color2.color.r  Color2.color.g Color2.color.b Color2Perc.value Color3.color.r  Color3.color.g Color3.color.b Color3Perc.value Color4.color.r  Color4.color.g Color4.color.b Color4Perc.value Color5.color.r  Color5.color.g Color5.color.b Color5Perc.value to:fileOut
		)
	)
	on LoadColors pressed do
	(
		loadName = getMaxOpenFileName filename:(colorDir + "\\*.txt")
		if loadName != undefined then
		(
			fileIn = openFile loadName
			color1.color.r = readline fileIn as float
			color1.color.g = readline fileIn as float
			color1.color.b = readline fileIn as float
			Color1Perc.value = readline fileIn as float
			color2.color.r = readline fileIn as float
			color2.color.g = readline fileIn as float
			color2.color.b = readline fileIn as float
			Color2Perc.value = readline fileIn as float
			color3.color.r = readline fileIn as float
			color3.color.g = readline fileIn as float
			color3.color.b = readline fileIn as float
			Color3Perc.value = readline fileIn as float
			color4.color.r = readline fileIn as float
			color4.color.g = readline fileIn as float
			color4.color.b = readline fileIn as float
			Color4Perc.value = readline fileIn as float
			color5.color.r = readline fileIn as float
			color5.color.g = readline fileIn as float
			color5.color.b = readline fileIn as float
			Color5Perc.value = readline fileIn as float
		)
	)
	on Alpha1 changed val do
	(
		Alpha2.value = val
		Alpha3.value = val
		Alpha4.value = val
		Alpha5.value = val
	)
	button VisCloud "Visualize Color Settings!!"
	on VisCloud pressed do
	(
		select $plane*
		Cloud_Height = $.max.z - $.min.z
		Cloud_Lowpoint = $.min.z
		Level0 = Cloud_Lowpoint + (Cloud_Height * (.01 * Color1Perc.value))
		Level1 = Cloud_Lowpoint + (Cloud_Height * (.01 * Color2Perc.value))
		Level2 = Cloud_Lowpoint + (Cloud_Height * (.01 * Color3Perc.value))
		Level3 = Cloud_Lowpoint + (Cloud_Height * (.01 * Color4Perc.value))
		Level4 = Cloud_Lowpoint + (Cloud_Height * (.01 * Color5Perc.value))
		for planes in $plane* do
		(
			convertToMesh planes
			setNumCPVVerts planes 4
			defaultVCfaces planes
			for i = 1 to 4 do
			(
				if planes.verts[i].pos.z > Level3 then
				(
					setvertcolor planes i [(Color5.color.r as float),(Color5.color.g as float),(Color5.color.b as float)]
				)
				if planes.verts[i].pos.z <= Level3 then
				(
					setvertcolor planes i [(Color4.color.r as float),(Color4.color.g as float),(Color4.color.b as float)]
				)
				if planes.verts[i].pos.z <= Level2 then
				(
					setvertcolor planes i [(Color3.color.r as float),(Color3.color.g as float),(Color3.color.b as float)]
				)
				if planes.verts[i].pos.z <= Level1 then
				(
					setvertcolor planes i [(Color2.color.r as float),(Color2.color.g as float),(Color2.color.b as float)]
				)
				if planes.verts[i].pos.z <= Level0 then
				(
					setvertcolor planes i [(Color1.color.r as float),(Color1.color.g as float),(Color1.color.b as float)]
				)
			)
			planes.showVertexColors = true
		)
	)
	fn ExportCloud fileOut =
	(
		--Write the version number, plane count, texture and shading levels to binary file
		WriteLong fileOut 9100
		WriteLong fileOut $plane*.count
		WriteString fileOut "Cumulus01.bmp"
		WriteLong fileOut 5
		WriteFloat fileOut (Color1Perc.value as integer)
		WriteByte fileOut (Color1.color.r as integer)
		WriteByte fileOut (Color1.color.g as integer)
		WriteByte fileOut (Color1.color.b as integer)
		WriteByte fileOut (Alpha1.value as integer)
		WriteFloat fileOut (Color2Perc.value as integer)
		WriteByte fileOut (Color2.color.r as integer)
		WriteByte fileOut (Color2.color.g as integer)
		WriteByte fileOut (Color2.color.b as integer)
		WriteByte fileOut (Alpha2.value as integer)
		WriteFloat fileOut (Color3Perc.value as integer)
		WriteByte fileOut (Color3.color.r as integer)
		WriteByte fileOut (Color3.color.g as integer)
		WriteByte fileOut (Color3.color.b as integer)
		WriteByte fileOut (Alpha3.value as integer)
		WriteFloat fileOut (Color4Perc.value as integer)
		WriteByte fileOut (Color4.color.r as integer)
		WriteByte fileOut (Color4.color.g as integer)
		WriteByte fileOut (Color4.color.b as integer)
		WriteByte fileOut (Alpha4.value as integer)
		WriteFloat fileOut (Color5Perc.value as integer)
		WriteByte fileOut (Color5.color.r as integer)
		WriteByte fileOut (Color5.color.g as integer)
		WriteByte fileOut (Color5.color.b as integer)
		WriteByte fileOut (Alpha5.value as integer)
		rotmin = 0.0
		rotmax = 360.0
		--Set up stringStream for information on shading groups
		groupString = stringStream ""
		groupCount = 0
		for obj in $plane* do
		(
			--Define the UVW values of the sprite
			Vert1Val = getTVert obj 1
			UVW_Val1 = Vert1Val.x
			UVW_Val2 = Vert1Val.y
			Vert2Val = getTVert obj 8
			UVW_Val3 = Vert2Val.x
			UVW_Val4 = Vert2Val.y
			--format "%,%,%,%\n" UVW_Val1 UVW_Val2 UVW_Val3 UVW_Val4
			--Set up vertices so that we can measure the width and length of each sprite
			try
			(
				Vert1 = obj.verts[1].pos
				Vert2 = obj.verts[2].pos
				Vert3 = obj.verts[3].pos
				Plane_Width = distance Vert1 Vert2
				Plane_Length = distance Vert1 Vert3
			)
			--Or if it is still a plane for some reason export width and length
			catch
			(
				Plane_Width = (obj.width * obj.scale.x)
				Plane_Length = (obj.length * obj.scale.y)
			)
			--Will set Rotation of sprite to User Defined Properties number (-# #). Otherwise will default to 0.0 360.0 (full random rotation).
			Rot_Props = getUserProp obj "Rotation"
			try
			(
				if Rot_Props != undefined then rotmin = -(Rot_Props as float) else rotmin = 0.0
				if Rot_Props != undefined then rotmax = (Rot_Props as float) else rotmax = 360.0
			)
			catch
			(
				messageBox "Bad user defined properties!"
				rotmin = 0.0
				rotmax = 360.0
			)
			Cloud_Props = getUserProp obj "Cloudgroup"
			if Cloud_Props == undefined then
			(
				messageBox (obj.name + " has no cloud group, please assign a cloud group!")
				exit
			)
			seek groupString 0
			PlaneColor = obj.wirecolor as string
			if skipToString groupString PlaneColor == undefined then
			(
				format "%@%$\n" PlaneColor groupCount to:groupString
				groupCount = groupCount + 1
			)
			seek groupString 0
			skipToString groupString PlaneColor
			skipToString groupString "@"
			groupNum = readDelimitedString groupString "$"
			--Write the values for each plane to the binary file
			WriteFloat fileOut obj.Center.x
			WriteFloat fileOut obj.Center.z
			WriteFloat fileOut -(obj.Center.y)
			WriteShort fileOut (groupNum as integer)
			WriteShort fileOut (Cloud_Props as integer)
			WriteShort fileOut (Plane_Width as integer)
			WriteShort fileOut (Plane_Width as integer)
			WriteShort fileOut (Plane_Length as integer)
			WriteShort fileOut (Plane_Length as integer)
			WriteShort fileOut (rotmin as integer) #signed
			WriteShort fileOut (rotmax as integer)
			WriteFloat fileOut UVW_Val1
			WriteFloat fileOut UVW_Val2
			WriteFloat fileOut UVW_Val3
			WriteFloat fileOut UVW_Val4
		)
		fclose fileOut
	)
	button ExCloud "Export Cloud!!" offset:[0,10]
	on ExCloud pressed do
	(
		scriptExit = 0
		global groupTestArray = #()
		groupTestCheck = 0
		deselect $*
		for groupTest in $plane* do
		(
			if getUserProp groupTest "Cloudgroup" == undefined do
			(
				selectmore groupTest
				append groupTestArray groupTest.name
				groupTestCheck = 1
			)
		)
		if groupTestCheck == 1 then 
		(
			groupError = createFile (exportDir + "GroupError.txt")
			messageBox ((groupTestArray as string) + " do not have a cloud group number.  Please assign one!")
			for i = 1 to groupTestArray.count do
			(
				format "%\n" (groupTestArray[i] as string) to:groupError
			)
			close groupError
			scriptExit = 1 
		)
		--Set up binary file to export to
		if scriptExit == 0 then
		(
			textName = getMaxSaveFileName filename:(exportDir + "*.cld")
			if textName != undefined then 
			(
				fileOut = fopen textName "wb"
				ExportCloud fileOut
			)
		)
	)
	--editText cldWild "Cloud Wildcard:" FieldWidth:100
	--button ExAllCloud "Export All Clouds!!"
	--on ExAllCloud pressed do
	--(
	--	for AllClouds in getFiles (exportDir + cldWild.text + ".max") do 
	--	(
	--		loadMaxFile AllClouds;
	--		--Set up .txt file to export to
	--		AllCloudName = getFileNameFile AllClouds
	--		fileOut = fopen (exportDir + AllCloudName + ".cld") "wb"
	--		ExportCloud fileOut
	--	)
	--)
)
CloudFinal = newRolloutFloater "Cloud Tool" 300 820 
addRollout clearscene CloudFinal
addRollout Clouds CloudFinal
addRollout GroupUtil CloudFinal
addRollout Export CloudFinal
