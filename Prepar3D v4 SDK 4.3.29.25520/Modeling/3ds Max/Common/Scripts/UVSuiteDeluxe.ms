
--UV wrap tool for setting consistent scale on UVs
floaterConsistentUVScale = newRolloutFloater "Deluxe UV Tools" 300 350

--Rollout with UV tool suite
Rollout UVToolsSuiteDeluxe "UV tools suite deluxe"
(
	group "Alignment tools"
	(
		checkButton pickVertToAlignTo "Align to specific vertex" width:150 pos:[120,25] enabled:false toolTip:"Select a vert in the UV Unwrap editor, then click in a viewport."
		checkBox cbXAlign "Align to X?" pos:[10,25] columns:1 enabled:false
		checkBox cbYAlign "Align to Y?" pos:[10,50] columns:1 enabled:false
		button btnAlignToAverage "Align to average" width:150 pos:[120,50] enabled:false
	)	
	
	--Check to see if the UVW editor is open, it it is, enable to UI elements.
	on UVToolsSuiteDeluxe open do
	(
		try
		(
			$.modifiers[#unwrap_uvw].edit ()
			pickVertToAlignTo.enabled = true
			cbXAlign.enabled = true
			cbYAlign.enabled = true
			btnAlignToAverage.enabled = true
		)
		catch ()
	)
	
	--When the Pick vertex align is pressed, make selected vertices use the picked vertex as their new X or Y value
	on pickVertToAlignTo changed newVertToAlign do
	(		
		--If the user closed the UVW unwrap, try to open it again.
		try
		(
			$.modifiers[#unwrap_uvw].edit ()
		)
		catch ()
		
		global currentVertSelection = ($.modifiers[#unwrap_uvw].getSelectedvertices ())
		tool getUpdatedVertInfo
		(
			on mousePoint newClick do
			(
				if (($.modifiers[#unwrap_uvw].getSelectedvertices ()) as string) != (currentVertSelection as string) then
				(
					try
					(
						local thisVertSelection = ($.modifiers[#unwrap_uvw].getSelectedvertices ()) as array
						local thisTvert = getTVert $ thisVertSelection[1]
						local thisVertX = thisTvert.x
						local thisVertY = thisTvert.y
						$.modifiers[#unwrap_uvw].selectVertices currentVertSelection
						if cbXAlign.checked == true then
						(
							$.modifiers[#unwrap_uvw].moveX thisVertX
						)
						if cbYAlign.checked == true then
						(
							$.modifiers[#unwrap_uvw].moveY thisVertY
						)
						pickVertToAlignTo.state = off
						#stop
					)
					catch
					(
						messageBox "Please select only one vertex to align to!"
					)
				)
			)
		)
		startTool getUpdatedVertInfo
	)
	
	--When the Align to average is pressed, make the UVs align to the average of the selected UVs based on the axis selection.
	on btnAlignToAverage pressed do
	(
		--If the user closed the UVW unwrap, try to open it again.
		try
		(
			$.modifiers[#unwrap_uvw].edit ()
		)
		catch ()
		
		if ($.modifiers[#unwrap_uvw].getSelectedvertices ()).count != 0 then
		(
			local eachVert
			local averagePosX = 0
			local averagePosY = 0
			local totalVerts = 0
			for eachVert in ($.modifiers[#unwrap_uvw].getSelectedvertices ()) do
			(
				averagePosX += ($.modifiers[#unwrap_uvw].getVertexPosition currentTime eachVert).x
				averagePosY += ($.modifiers[#unwrap_uvw].getVertexPosition currentTime eachVert).y
				totalVerts += 1
			)
			averagePosX /= totalVerts
			averagePosY /= totalVerts
			--Move the verts to the new position
			if cbXAlign.checked == true then
			(
				$.modifiers[#unwrap_uvw].moveX averagePosX 
			)
			if cbYAlign.checked == true then
			(
				$.modifiers[#unwrap_uvw].moveY averagePosY
			)
		)
	)
)

--Rollout to do consistent UV scaling for a selected face.
Rollout rollConsistentUVScale "Set consistent scale on UVs"
(
	editText txtSetUVScale "UV Scale:" text:"100.0" pos:[10,10]
	button btnSetupUVAlign "Setup UV alignment!" width:270 pos:[10,35]

	--When the setupUVAlign button is pressed, add a UVMap modifier in the normal align mode
	on btnSetupUVAlign pressed do
	(
		if (txtSetUvScale.text as float) != undefined then
		(
			if ($ != undefined) and ($.selectedFaces.count > 0) then
			(
				modPanel.addModToSelection (Uvwmap ()) ui:on
				$.modifiers[#UVW_Mapping].length = (txtSetUVScale.text as float)
				$.modifiers[#UVW_Mapping].width = (txtSetUVScale.text as float)
				$.modifiers[#UVW_Mapping].height = (txtSetUVScale.text as float)
				showProperties $.modifiers[#UVW_Mapping]
				
				Rollout rollUnwrapButton "Add a UVW Unwrap Modifier"
				(
					button btnGoToUnwrap "Go to unwrap!" width:270 pos:[10,10]
	
					--When the GoTOUnwrap button is pressed, set the scale at the size of the value in the txtSetUVScale.
					--then throw an unwrap modifier onto the object and edit it for viewing,
					on btnGoToUnwrap pressed do
					(				
						$.modifiers[#UVW_Mapping].length = txtSetUvScale.text as float
						$.modifiers[#UVW_Mapping].width = txtSetUvScale.text as float
						$.modifiers[#UVW_Mapping].height = txtSetUvScale.text as float
						modPanel.addModToSelection (Unwrap_UVW ()) ui:on
						$.modifiers[#unwrap_uvw].unwrap.edit ()
						
						--After your through, remove the button and cleanup the window for next time
						removeRollout rollUnwrapButton
						floaterConsistentUVScale.size = [300,350]
					)
				)
				floaterConsistentUVScale.size = [300,410]
				addRollout rollUnwrapButton floaterConsistentUVScale
			)
			else
			(
				messageBox "Please select some faces on which to perform this operation!"
			)
		)
		else
		(
			messageBox "Your UV scale cannot be converted to a float, are you sure it's a number?"
		)
	)
)

--This is a tool that will move UV or nonlinearly scale coordinates
Rollout rollMoveUVTool "Move or Scale UV Coordinates Tool"
(
	label lblMoveUVs "Move:" pos:[10,10]
	label lblScaleUVs "Scale:" pos:[150,10]
	spinner txtUMove "U:" range:[-1.00,1.00,0.00] type:#float scale:0.01 width:100 pos:[10,30]
	spinner txtVMove "V:" range:[-1.00,1.00,0.00] type:#float scale:0.01 width:100 pos:[10,55]
	spinner txtUScale "U:" range:[0.00,10.00,1.00] type:#float scale:0.01 width:100 pos:[150,30]
	spinner txtVScale "V:" range:[0.00,10.00,1.00] type:#float scale:0.01 width:100 pos:[150,55]
	checkBox cbPerFace "Selected Faces Only?" checked:false align:#center
	button btnMoveUVGo "Go!" width:270 pos:[10,100] tooltip:"If selected faces only isn't checked, this will operate on the whole object.  A scale of 1.0 has no effect on scale."
	
	
	--Define a function that will apply the UV scale and move offset to the texture verts in a given face
	fn fnScaleMoveUV thisObj faceSelection = 
	(
		local tVertArray = #()
		--Gather the list of texture verts into an array making sure not to duplicate any (or they will get operated on doubly)
		for eachFace in faceSelection do
		(
			--Get the texture vertices used by each of the selected faces
			local theseVerts = (meshop.getVertsUsingFace thisObj eachFace.index) as array
			local theseTverts = (getTVFace thisObj eachFace.index)
			for i = 1 to theseVerts.count do
			(
				if findItem tVertArray (theseTVerts[i] as integer) == 0 then append tVertArray (theseTVerts[i] as integer)
			)
		)
		for i = 1 to tVertArray.count do
		(
			--Go through each vert in the selected faces and apply the new Scale first and then move them.
			--First get the original texture vert information
			local thisTexVert = (getTvert thisObj tVertArray[i])
			
			--Now apply the offsets, scale first, then move
			thisTexVert.x *= txtUScale.value as float
			thisTexVert.y *= txtVScale.value as float
			thisTexVert.x += txtUMove.value as float
			thisTexVert.y += txtVMove.value as float
			
			--Push the new Texture vertex information back into the verts
			(setTvert thisObj tVertArray[i] thisTexVert)
		)
	)
	
	--When go button is pressed, offset the UVs by the amount specified in the spinners.  Negative values work as well.
	on btnMoveUVGo pressed do
	(
		arrSelectedObjs = #()
		if $ != undefined then
		(
			--Perform the UV move and scale functions on any object that is selected, and on any face that is selected if "Selected Faces" is specified
			for obj in $ do
			(
				--If per face is checked, only operate on selected faces.
				if cbPerFace.checked == true then
				(
					try (fnScaleMoveUV obj obj.selectedFaces) catch (messageBox "I can't get to the texture vertices.  Try collapsing the stack.")
				)
				--If per face isn't checked, operate on the whole object.
				else
				(
					try (fnScaleMoveUV obj obj.faces) catch (messageBox "I can't get to the texture vertices.  Try collapsing the stack.")
				)
			)
		)
	)
)
addRollout rollMoveUVTool floaterConsistentUVScale
addRollout UVToolsSuiteDeluxe floaterConsistentUVScale 
addRollout rollConsistentUVScale floaterConsistentUVScale