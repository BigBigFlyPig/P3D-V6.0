
--Include the funxtoin files so that the functions are available
fileIn (acesScriptsFolder + "\\Startup\\ModelVerifyFunction.ms")

--Define a global array used to store the broken part information
global acesBrokenParts = #(#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),"")

Rollout brokenObjectsRollout "This model has the following problems:"
(
	editText brokenLODInfo "LOD Info:" text:"" pos:[10,10]
	
	listBox brokenParts "The following parts of this model are broken:" pos:[10,40] width:670 height:8 items:#()
	listBox brokenItems "These are the broken items:" pos:[10,200] width:670 height:24 items:#()
	button capHolesGo "Fix selected open edges!" width:220 pos:[10,540] enabled:false
	button btnFixScale "Fix selected model scale!" width:220 pos:[235,540] enabled:false
	button btnWeldVerts "Weld selected vertices!" width:220 pos:[460,540] enabled:false
	button undoLastFix "Revert last fix!" width:670 pos:[10,565] enabled:false

	--Define a function to populate the broken bits
	fn fnPopulateBrokenParts =
	(
		local brokenObjsArray = #()
		if acesBrokenParts[1].count > 0 then append brokenObjsArray "Broken GUID or Friendly name"
		if acesBrokenParts[2].count > 0 then append brokenObjsArray "Broken Hierarchy"
		if acesBrokenParts[3].count > 0 then append brokenObjsArray "Multiple root nodes"
		if acesBrokenParts[4].count > 0 then append brokenObjsArray "Broken scale on parts"
		if acesBrokenParts[5].count > 0 then append brokenObjsArray "Duplicate part names"
		if acesBrokenParts[6].count > 0 then append brokenObjsArray "Broken attach points"
		if acesBrokenParts[7].count > 0 then append brokenObjsArray "Missing Prepar3D Shaders"
		if acesBrokenParts[8].count > 0 then append brokenObjsArray "Incorrect Prepar3D Shader Settings"
		if acesBrokenParts[9].count > 0 then append brokenObjsArray "Colocated vertices"
		if acesBrokenParts[11].count > 0 then append brokenObjsArray "Open edges (non-manifold geometry)"
		if acesBrokenParts[12].count > 0 then append brokenObjsArray "Insane texture vertices"
		if acesBrokenParts[13].count > 0 then append brokenObjsArray "Parts with no UV coordinates"
		brokenParts.selection = 0
		return brokenObjsArray
	)
	
	--When the broken parts are selected, populate the items list with appropriate information
	on brokenParts selected newBrokenPart do
	(
		if brokenParts.selected == "Broken GUID or Friendly name" then (brokenItems.items = acesBrokenParts[1]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Broken Hierarchy" then (brokenItems.items = acesBrokenParts[2]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Multiple root nodes" then (brokenItems.items = acesBrokenParts[3]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Broken scale on parts" then (brokenItems.items = acesBrokenParts[4]; btnFixScale.enabled = true; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Duplicate part names" then (brokenItems.items = acesBrokenParts[5]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Broken attach points" then (brokenItems.items = acesBrokenParts[6]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Missing Prepar3D Shaders" then (brokenItems.items = acesBrokenParts[7]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Incorrect Prepar3D Shader Settings" then (brokenItems.items = acesBrokenParts[8]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Colocated vertices" then (brokenItems.items = acesBrokenParts[9]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = true)
		if brokenParts.selected == "Open edges (non-manifold geometry)" then (brokenItems.items = acesBrokenParts[11]; btnFixScale.enabled = false; capHolesGo.enabled = true; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Insane texture vertices" then (brokenItems.items = acesBrokenParts[12]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
		if brokenParts.selected == "Parts with no UV coordinates" then (brokenItems.items = acesBrokenParts[13]; btnFixScale.enabled = false; capHolesGo.enabled = false; btnWeldVerts.enabled = false)
	)
	
	--When the rollout is opened, populate the list of broken stuff.
	on brokenObjectsRollout open do
	(
		brokenparts.items = fnPopulateBrokenParts ()
	)
	
	on brokenItems selected newBrokenItem do
	(
		--When selecting an object from the colocated verts list, select the object in the viewport and highlight the selected vertex.
		if brokenParts.selected == "Colocated vertices" then
		(
			try
			(
				local thisItemArr = execute brokenItems.selected
				hide $*
				unhide (execute ("$" + thisItemArr[1]))
				select (execute ("$" + thisItemArr[1]))
				subObjectLevel = 1
				select $.verts[#{thisItemArr[2], thisItemArr[3]}]
				max zoomExt sel all
			)
			catch
			(
				messageBox "I can't access the vertices.  Make sure your model is a mesh and that the stack is collapsed."
			)
		)
		
		--When selecting an object from the open edges list, select the object in the viewport and show its open edges.
		if brokenParts.selected == "Open edges (non-manifold geometry)" then
		(
			try
			(
				max unhide all
				openEdgeObj = execute ("$" + brokenItems.selected)
				hide $*
				unhide openEdgeObj
				select openEdgeObj
				try
				(
					max modify mode
					subobjectLevel = 2
					local allOpenEdges
					if classOf $ == editable_mesh then
					(
						allOpenEdges = meshop.getOpenEdges openEdgeObj
					)
					if classOf $ == editable_Poly then
					(
						allOpenEdges = polyOp.getOpenEdges openEdgeObj
					)
					select openEdgeObj.edges[allOpenEdges]
				)
				catch ()
				max zoomExt sel all
			)
			catch
			(
				messageBox "This object has a space in its name.  Please remove that."
			)
		)
		
		--When selecting an object from the insane vertex list, select the object in the viewport and highlight the selected vertex.
		if brokenParts.selected == "Insane texture vertices" then
		(
			try
			(
				insaneVertString = brokenItems.selected as stringStream
				seek insaneVertString 0
				insaneVertObj = readDelimitedString insaneVertString ":"
				insaneVertIndex = readDelimitedString insaneVertString ":"
				hide $*
				unhide (execute ("$" + insaneVertObj))
				select (execute ("$" + insaneVertObj))
				subObjectLevel = 1
				select $.verts[(insaneVertIndex as integer)]
				max zoomExt sel all
			)
			catch (messageBox "I can't access this texture vert.  Make sure your model is a mesh and the stack is collapsed and that there are no spaces in the name.")
		)
		
		--When selecting an object from the no UV coords list, select the object in the viewport and highlight the selected object.
		if brokenParts.selected == "Parts with no UV coordinates" then
		(
			try
			(
				noUVObj = execute ("$" + brokenItems.selected)
				hide $*
				unhide noUVObj
				select noUVObj
				max zoomExt sel all
			)
			catch
			(
				messageBox "There is a space in this object's name.  Please fix that."
			)
		)
	)
	--When the cap holes button is pressed, do a file>>hold on the max file, then cap holes on selected objects.
	on capHolesGo pressed do
	(
		holdMaxFile ()
		try
		(
			oldPolyCount = 0
			local objSelection = #()
			for obj in $ do
			(
				oldPolyCount += obj.faces.count
				append objSelection obj
			)
			newPolyCount = 0
			for i = 1 to objSelection.count do
			(
				--Cap up the open edges in each object				
				select objSelection[i]
				modPanel.addModToSelection (Cap_Holes ())
				$.modifiers[#Cap_Holes].Smooth_With_Old_Faces = 1
				convertTo $ triMeshGeometry
				
				--Update the new poly count so the comparison between the old and new poly counts can be made.
				newPolyCount += $.faces.count
			)
			if queryBox ("The object you capped had " + (oldPolyCount as string) + " faces and now has " + (newPolyCount as string) + " faces.  Do you wish to continue?") beep:true then 
			(
				local theseBrokenParts = verifyAcesOpenEdges ()
				if theseBrokenParts[1] == true then 
				(
					acesBrokenParts[11] = theseBrokenParts[2]
					brokenparts.items = fnPopulateBrokenParts ()
					brokenItems.items = #()
				)
				else
				(
					acesBrokenParts[11] = #()
					brokenparts.items = fnPopulateBrokenParts ()
					brokenItems.items = #()
				)
				undoLastFix.enabled = true
			)
			else
			(
				undoLastFix.enabled = false
				fetchMaxFile quiet:true
			)
		)
		catch
		(
			messageBox "Please select an object to cap!"
		)
	)
	
	--Define a function that scales all of the children of an object down the tree
	fn fnScaleChildren thisScale children = 
	(
		for c in children do
		(
			local childScale = thisScale * c.Scale
			c.scale = [1,1,1]
			c.objectOffsetScale *= childScale
			c.objectOffsetPos *= childScale
			local newScale = childScale
			fnScaleChildren newScale c.children
		)
	)
	
	--Define a function that checks the scale and correctly populates the various list boxes if the scale is broken
	fn fnCheckAcesScale = 
	(
		local theseBrokenParts = verifyAcesModelScale ()
		if theseBrokenParts[1] == true then (acesBrokenParts[4] = theseBrokenParts[2]; brokenObject = true) else (acesBrokenParts[4] = #())
		brokenParts.items = fnPopulateBrokenParts ()
		brokenItems.items = #()
	)
	
	--When the fix scale button is pressed, reset all of the objects scale to be 1.0
	on btnFixScale pressed do
	(
		holdMaxFile ()
		for obj in $* where obj.parent == undefined do
		(
			local thisScale = obj.scale
			obj.scale = [1,1,1]
			obj.objectOffsetScale *= thisScale
			fnScaleChildren thisScale obj.children
		)
		
		--Recheck the scale and repopulate the broken objects
		fnCheckAcesScale ()
		
		undoLastFix.enabled = true
	)
	
	--When the weld vertices button is pressed, weld the two selected vertices and then renumber all of the vertices in the array so the rest are correct.
	on btnWeldVerts pressed do
	(
		--Hold the file so it can be reverted if necessary
		holdMaxFile ()
		
		--Define the current broken parts selection so it can be reselected later.
		local curBPSel = brokenParts.selection
		
		--First try to collapse the selected verts
		try ($.EditablePoly.collapse #Vertex) catch ()
		try (meshop.weldVertSet $ $.selectedVerts) catch ()
		
		--Get the highest and lowest vert index from the two verts that were welded.
		local thisItemArr = (execute brokenItems.selected), highVert, lowVert
		if thisItemArr[2] > thisItemArr[3] then 
		(
			highVert = thisItemArr[2]
			lowVert = thisItemArr[3]
		)
		else
		(
			highVert = thisItemArr[3]
			lowVert = thisItemArr[2]
		)
				
		--Remove the vertex entry from the list
		deleteItem brokenItems.items brokenItems.selection
		
		--Now go back and reorder all of the vertices in the list that match the name of the object that was just merged
		for i = 1 to brokenItems.items.count do
		(
			if ((execute brokenItems.items[i])[1]) == thisItemArr[1] then
			(
				local thisItem = (execute brokenItems.items[i])
				--Any vert with a larger index than the highest vert that just got collapsed will have its index bumped down by one.
				if thisItem[2] > highVert then (thisItem[2] -= 1)
				if thisItem[3] > highVert then (thisItem[3] -= 1)
				
				--Any vert that was colocated with the highest vert is now colocated with the newly collapsed vert
				if thisItem[2] == highVert then (thisItem[2] = lowVert)
				if thisItem[3] == highVert then (thisItem[3] = lowVert)
				
				brokenItems.items[i] = thisItem as string
			)
		)
		
		--Now push the items back into the global broken objects list and then update everything
		acesBrokenParts[9] = brokenItems.items
		brokenparts.items = fnPopulateBrokenParts ()
		brokenItems.items = acesBrokenParts[9]
		brokenParts.selection = curBPSel
		
		--Enable the revert button and disable to weld button if there are no verts left
		if acesBrokenParts[9].count == 0 then btnWeldVerts.enabled = false
		undoLastFix.enabled = true
	)
	
	--When the undo last fix button is pressed, bring back the held file.
	on undoLastFix pressed do
	(
		fetchMaxFile quiet:true
		
		--Recheck the scale and repopulate the broken objects
		fnCheckAcesScale ()
		
		undoLastFix.enabled = false
	)
)

--Define a UI set for verifying your model data.
Rollout verifyRollout "Model verification tools"
(
	group "Verify Model Data"
	(
		checkbox cbVerifyScale "Verify scale?" pos:[10,25] checked:true
		checkbox cbVerifyLOD "Verify LODs?" pos:[160,25] checked:true
		checkbox cbVerifyGUID "Verify GUID and friendly?" pos:[10,45] checked:true
		checkbox cbVerifyEdges "Verify open edges?" pos:[160,45] checked:true
		checkbox cbVerifyCoVerts "Verify colocated verts?" pos:[10,65] checked:true
		checkbox cbVerifyDupes "Verify duplicate parts?" pos:[10,85] checked:true
		checkbox cbVerifyTexVerts "Verify texture verts?" pos:[160,65] checked:true
		checkbox cbVerifyMats "Verify materials?" pos:[10,105] checked:true
		checkbox cbVerifyRoot "Verify root node?" pos:[160,85] checked:true
		button verifyModelButton "Verify!" width:290 pos:[10,125]
	)
	
	--Verify the model data when the button is pressed
	on verifyModelButton pressed do
	(
		--Reset the array of broken parts
		acesBrokenParts = #(#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),#(),#())
			
		--Based on the checked verifiers, verify the model appropriately and populate the fields accordingly		
		--Define an object is broken boolean
		local brokenObject = false
		
		if cbVerifyScale.checked then (local brokenParts = verifyAcesModelScale (); if brokenParts[1] == true then (acesBrokenParts[4] = brokenParts[2]; brokenObject = true))
		if cbVerifyLOD.checked then 
		(
			local brokenParts = verifyAcesModelLOD ()
			if brokenParts[3].count > 0 then (acesBrokenParts[14] = brokenParts[3] as string) else (acesBrokenParts[14] = brokenParts[4] as string)
			if brokenParts[1] == true then (acesBrokenParts[2] = brokenParts[2]; brokenObject = true)
		)
		else
		(
			acesBrokenParts[14] = "LODs were not verified this time."
		)
		if cbVerifyGUID.checked then (local brokenParts = verifyAcesGUID (); if brokenParts[1] == true then (acesBrokenParts[1] = brokenParts[2]; brokenObject = true))
		if cbVerifyEdges.checked then (local brokenParts = verifyAcesOpenEdges (); if brokenParts[1] == true then (acesBrokenParts[11] = brokenParts[2]; brokenObject = true))
		if cbVerifyCoVerts.checked then (local brokenParts = verifyAcesCoVerts (); if brokenParts[1] == true then (acesBrokenParts[9] = brokenParts[2]; brokenObject = true))
		if cbVerifyDupes.checked then (local brokenParts = verifyAcesDupes (); if brokenParts[1] == true then (acesBrokenParts[5] = brokenParts[2]; acesBrokenParts[6] = brokenParts[3]; brokenObject = true))
		if cbVerifyTexVerts.checked then (local brokenParts = verifyAcesTexVerts (); if brokenParts[1] == true then (acesBrokenParts[12] = brokenParts[2]; acesBrokenParts[13] = brokenParts[3]; brokenObject = true))
		if cbVerifyMats.checked then (local brokenParts = verifyAcesMats (); if brokenParts[1] == true then (acesBrokenParts[7] = brokenParts[2]; acesBrokenParts[8] = brokenParts[3]; brokenObject = true))
		if cbVerifyRoot.checked then (local brokenParts = verifyAcesRoot (); if brokenParts[1] == true then (acesBrokenParts[3] = brokenParts[2]; brokenObject = true))		
		
		if brokenObject == true then
		(
			try (destroyDialog brokenObjectsRollout) catch ()
			createDialog brokenObjectsRollout 700 640 bgcolor:(color 168 172 170) fgcolor:(color 100 0 0)
			brokenObjectsRollout.brokenLODInfo.text = acesBrokenParts[14]
		)
		else
		(
			try (destroyDialog brokenObjectsRollout) catch ()
			messageBox "As far as I can tell, this model is perfect!"
		)
	)
)
