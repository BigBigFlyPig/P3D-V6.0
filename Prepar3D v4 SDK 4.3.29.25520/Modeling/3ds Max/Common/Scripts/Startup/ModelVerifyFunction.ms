
--Define the functions for verifying Aces models.

--Verify model scale

fn verifyAcesModelScale = 
(		
	local isBroken = false, modelScale = #()
	--Determine if the scale is okay
	for obj in $* do
	(
		if classOf obj.controller == prs then
		(
			if obj.scale != ([1,1,1] as point3) then
			(
				isBroken = true
				append modelScale (obj.name + " has a scale of " + obj.scale as string)
				--format "% / " (obj.name + " has a scale of " + obj.scale as string) to:strBrokenScale
			)
		)
	)
	return #(isBroken,modelScale)
)
	
--Determine if the LOD structure is valid or not
fn verifyAcesModelLOD =
(
	local hasLODS = false, isBroken = false, brokenHierarchy = #(), strLODInfo = "" as stringStream, strLODIsBroken = #()
	for obj in rootnode.children do
	(
		if classOf obj == dummy then
		(
			if obj.name != (getFilenameFile maxFileName) then
			(
				isBroken = true
				append brokenHierarchy ("The root node \"" + obj.name + "\" is not named correctly!")
				--format "%" ("The root node \"" + obj.name + "\" is not named correctly!") to:brokenHierarchy
			)
			for objChildren in obj.children do
			(
				if (matchPattern objChildren.name pattern:"*_LOD_*") == true then
				(
					if (matchPattern objChildren.name pattern:("*" + (getFilenameFile maxFileName) + "*")) == false then
					(
						isBroken = true
						append brokenHierarchy ("The LOD name \"" + objChildren.name + "\" does not match the filename!")
						--format "%" ("The LOD name \"" + objChildren.name + "\" does not match the filename!  ") to:brokenHierarchy
					)
					hasLODs = true
				)
			)
			if hasLODS == false then
			(
				isBroken == true
				append brokenHierarchy ("This file has no properly named LODS!")
				--format "%" "This file has no properly named LODS!  " to:brokenHierarchy
			)
		)
	)
	--Verify the LOD polygon counts
	try
	(
		for obj in $*_LOD_* do
		(
			LODObj = execute ("$" + obj.name + "...*")
			select LODObj
			LODPolyCount = 0
			for LODPoly in $ do
			(
				try (LODPolyCount += LODPoly.faces.count)
				catch ()
			)
			format "% PolyCount:% / " obj.name LODPolyCount to:strLODInfo
			deselect obj
			if $ != undefined then
			(
				for thisObj in $ do
				(
					if (matchPattern thisObj.name pattern:"*_LOD_*") then
					(
						isbroken = true
						append brokenHierarchy (thisObj.name + " should not have \"_LOD_\" in its name!")
						--format (thisObj.name + " should not have \"_LOD_\" in its name! ") to:brokenHierarchy
					)
				)
			)
		)
		arrayLODPolyCounts = #()
		seek strLODInfo 0
		while ((skipToString strLODInfo "PolyCount:") == OK) do
		(
			append arrayLODPolyCounts ((readDelimitedString strLODInfo " ") as integer)
		)
		sort arrayLODPolyCounts
		if arrayLODPolyCounts.count == 0 then
		(
			isbroken = true
			append strLODIsBroken "This model has no correctly named LODs!"
			--format "This model has no correctly named LODs!" to:strLODIsBroken 
		)
		if arrayLODPolyCounts.count == 1 then
		(
			if arrayLODPolyCounts[1] > 300 then
			(
				isbroken = true
				append strLODIsBroken "This model has only one LOD with too many polygons!"
				--format "This model has only one LOD with too many polygons!" to:strLODIsBroken 
			)
		)
		if arrayLODPolyCounts.count > 1 then
		(
			intLastLODInArray = arrayLODPolyCounts.count
			if ((arrayLODPolyCounts[intLastLODInArray] as float) / (arrayLODPolyCounts[1] as float)) < 2.0 then
			(
				isbroken = true
				append strLODIsBroken "The lowest LOD is more than half the polygons of the highest LOD!"
				--format "The lowest LOD is more than half the polygons of the highest LOD!" to:strLODIsBroken 
			)
		)
	)
	catch
	(
		format "This file's LODs are so messed up I can't even gather their information!" to:strLODInfo
	)
	
	return #(isBroken, brokenHierarchy, strLODIsBroken, strLODInfo)
)

--Verify the GUID and Friendly names
fn verifyAcesGUID =
(	
	--Setup local GUID and Friendly Name for this file
	local strThisFileGUID, strThisFileFriendly, isBroken = false, brokenGuidOrFriendly = #()
	--Check the GUID and Friendly name information and make sure it matches the .x file
	intThisFileGUIDPos = (fileProperties.findProperty #custom "Guid")
	inThisFileFriendlyPos = (fileProperties.findProperty #custom "FriendlyName")
	if intThisFileGUIDPos == 0 then
	(
		isBroken = true
		append brokenGuidOrFriendly "This file has no Guid associated with it."
		--format "% / " "This file has no Guid associated with it." to:brokenGuidOrFriendly
	)
	else
	(
		strThisFileGUID = (fileProperties.getPropertyValue #custom intThisFileGUIDPos)
	)
	if inThisFileFriendlyPos == 0 then
	(
		isBroken = true
		append brokenGuidOrFriendly "This file has no Friendly Name associated with it."
		--format "% / " "This file has no Friendly Name associated with it." to:brokenGuidOrFriendly
	)
	else
	(
		strThisFileFriendly = (fileProperties.getPropertyValue #custom inThisFileFriendlyPos)
	)
	--Get GUID and Friendly Name info from the .x file so we can compare.  Report it if we can't find that info in the .x file.
	local strThisFileGUIDFromX, strThisFileFriendlyFromX
	try
	(
		strThisFileX = openFile (maxFilePath + (getFilenameFile maxFilename) + ".x")
		skipToString strThisFileX "GuidToName {"
		skipToString strThisFileX "GuidToName {"
		if (skipToString strThisFileX "GuidToName {") != undefined then
		(
			strThisFileGUIDFromX = "This .X File's GUID stuff is busted."
			strThisFileFriendlyFromX = "This .X File's Friendly Name stuff is busted."
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
		close strThisFileX
	)
	catch
	(
		isBroken = true
		append brokenGuidOrFriendly "This .x file has no GUID or Friendly Name info."
		--format "% / " "This .x file has no GUID or Friendly Name info." to:brokenGuidOrFriendly
	)
	--Compare the GUID and Friendly Name from the .x file with that of the .max file.  If they are different, log it.
	if strThisFileGUID != strThisFileGUIDFromX then
	(
		isBroken = true
		append brokenGuidOrFriendly ("Max GUID = " + (strThisFileGUID as string) + ": .X GUID = " + (strThisFileGUIDFromX as string))
		--format "% / " ("Max GUID = " + (strThisFileGUID as string) + ": .X GUID = " + (strThisFileGUIDFromX as string)) to:brokenGuidOrFriendly
	)
	if strThisFileFriendly != strThisFileFriendlyFromX then
	(
		isBroken = true
		append brokenGuidOrFriendly ("Max FriendlyName = " + (strThisFileFriendly as string) + ": .X FriendlyName = " + (strThisFileFriendlyFromX as string))
		--format "% / " ("Max FriendlyName = " + (strThisFileFriendly as string) + ": .X FriendlyName = " + (strThisFileFriendlyFromX as string)) to:brokenGuidOrFriendly
	)
	try
	(
		if not (matchPattern strThisFileFriendly pattern:(getFileNameFile maxFilename) ignoreCase:true) then
		(
			isBroken = true
			append brokenGuidOrFriendly ("This file's friendly name does not match the filename!")
			--format "% / " ("This file's friendly name does not match the filename!") to:brokenGuidOrFriendly
		)
	)
	catch ()
	return #(isBroken, brokenGuidOrFriendly)
)

--Find problems with duplicate edges causing volume shadow issues
fn verifyAcesVolume =
(
	local isBroken = false, brokenVolume = #()
	--Go through each object that is not an attach point or platform and check for open edges and broken shadow volumes.
	for obj in $* where ((classOf obj) == editable_Mesh or (classOf obj) == editable_Poly) and not ((matchPattern obj.name pattern:"attachpt_*" ignoreCase:true) or (matchPattern obj.name pattern:"platform_*" ignoreCase:true)) do
	(
		--Determine if the volume is acceptable by checking the number of faces per edge.  If the number of faces is 1, then it has an open edge.  If the number of faces is greater than 2
		--then the volume is broken and will potentially cast a messed up shadow.
		local openEdges = #(), extraFaces = #()
		--Editable mesh pipeline for open edges and broken shadow volumes
		if classOf obj == editable_Mesh then 
		(			
			--Find the edges that have more than two colocated (aka broken volume shadow on a manifold model)
			local edgeArray = #()
			for i = 1 to obj.edges.count do
			(
				--Gather all of the edges using this edge.  If there are more than two of them, then test them to see if they will cause volume shadow problems.
				local thisEdge = (meshop.getEdgesReverseEdge obj i) as array
				if thisEdge.count > 0 then
				(
					append edgeArray thisEdge[1]
				)
			)
			sort edgeArray
			for i = 1 to edgeArray.count do
			(
				j = i + 1
				while edgeArray[i] == edgeArray[j] do
				(
					j += 1
				)
				if j - i > 1 then
				(
					isBroken = true
					local thisEdge = (meshop.getEdgesReverseEdge obj edgeArray[i]) as array
					if (findItem brokenVolume (obj.name + " : " + thisEdge[1] as string)) == 0 then append brokenVolume (obj.name + " : " + thisEdge[1] as string)
				)
			)
		)
	)
	return #(isBroken, brokenVolume)
)

--Find open edges in the model
fn verifyAcesOpenEdges =
(
	local isBroken = false, openEdgeObjs = #()
	for obj in $* where ((classOf obj) == editable_Mesh or (classOf obj) == editable_Poly) and not ((matchPattern obj.name pattern:"attachpt_*" ignoreCase:true) or (matchPattern obj.name pattern:"platform_*" ignoreCase:true)) do
	(
		if classOf obj == editable_Mesh then 
		(			
			--Find the open edges in the editable meshes
			openEdges = meshop.getOpenEdges obj as array
		)
		--Editable Poly pipeline for open edges (which includes broken shadow volumes because Editable poly can't have more than two faces per edge.  It automatically breaks them up on conversion.
		if classOf obj == editable_Poly then 
		(
			--Find the open edges and append them to an array
			openEdges = polyOp.getOpenEdges obj as array
		)
		if openEdges.count > 0 then
		(
			isBroken = true
			append openEdgeObjs obj.name
			--format "% / " obj.name to:openEdgeObjs
		)
	)
	return #(isBroken, openEdgeObjs)
)

--Determine where all the colocated verts are
fn verifyAcesCoVerts = 
(
	local isBroken = false, colocatedVerts = #()
	for obj in $* where ((classOf obj) == editable_Mesh or (classOf obj) == editable_Poly) and not ((matchPattern obj.name pattern:"attachpt_*" ignoreCase:true) or (matchPattern obj.name pattern:"platform_*" ignoreCase:true)) do
	(
		for i = 1 to obj.verts.count do
		(
			for j = i + 1 to obj.verts.count do
			(
				if (distance obj.verts[i].pos obj.verts[j].pos) < 0.001 then
				(
					isBroken = true
					append colocatedVerts (#(obj.name, i, j) as string)
					--format "%" (obj.name + ":[" + (allVerts.index as string) + "," + (secondVertNum as string) + "]") to:coVertString
				)
			)
		)
	)
	return #(isBroken, colocatedVerts)
)

--Check for duplicate part names and bad attach hierarchy.
fn verifyAcesDupes =
(
	local isBroken = false, hasDupes = false, badAttachPoints = #(), dupeNames = #()
	for obj in $* where (classOf obj) != dummy do
	(
		--compare objects and get duplicate part names
		for obj2 in $* where (classOf obj) != dummy do
		(
			if obj != obj2 then
			(
				if obj.name == obj2.name then
				(
					isBroken = true
					hasDupes = true
				)
			)
		)
		if hasDupes == true then append dupeNames obj.name
		--look at user defined props and check for bad attach hierarchy
		objUserProps = getUserPropBuffer obj as stringstream
		if (skipToString objUserProps "Effect" == OK) or (skipToString objUserProps "Platform" == OK) then
		(
			if skipToString (obj.name as stringStream) "_LOD_" == OK then
			(
				isBroken = true
				append badAttachPoints (obj.name + ": LOD is attach point!")
				--format "% : LOD is attach point! / " obj.name to:badAttachPoints
			)
			if obj.parent == undefined then
			(
				isBroken = true
				append badAttachPoints (obj.name + " : Attach point at same level as rootnode!")
				--format "% : Attach point at same level as rootnode! / " obj.name to:badAttachPoints
			)
		)
	)
	return #(isBroken, dupeNames, badAttachPoints)
)
	
--Check for insane texture vertices or objects that have no texture vertices.
fn verifyAcesTexVerts =
(
	local isBroken = false, badTexCoords = #(), insaneVerts = #()
	for obj in $* where (classOf obj) == editable_Mesh do
	(
		try
		(
			for allVerts in obj.verts do
			(
				Tvert = getTvert obj allVerts.index
				if Tvert.x > 10.0 or Tvert.y > 10.0 or Tvert.x < -10.0 or Tvert.y < -10.0 then 
				(
					isBroken = true
					append insaneVerts (obj.name + ":" + allVerts.index as string + ":" + Tvert as string)
				)
			)
		)
		catch
		(
			if classOf obj.material == multimaterial then
			(
				faceIDList = #()
				for allFaces in obj.faces do
				(
					faceID = getFaceMatID obj allFaces.index
					if findItem faceIDList faceID == 0 then
					(
						append faceIDList faceID
					)
				)
				if faceIDList.count > 0 then
				(
					for subMats in obj.material.materialList do
					(
						if classOf subMats == P3DMaterial then
						(
							for i = 1 to faceIDList.count do
							(
								if classOf subMats.standard.diffuseMap == Bitmaptexture then
								(
									append badTexCoords subMats.name
									--format "% / " subMats.name to:badTexCoords
								)
							)
						)
						if classOf subMats == Standardmaterial then
						(
							for i = 1 to faceIDList.count do
							(
								if classOf subMats.diffuseMap == Bitmaptexture then
								(
									append badTexCoords subMats.name
									--format "% / " subMats.name to:badTexCoords
								)
							)
						)

					)
				)
			)
			if classOf obj.material == Standardmaterial then
			(
				if classOf obj.material.diffuseMap == Bitmaptexture then
				(
					append badTexCoords obj.material.name
					--format "% / " obj.material.name to:badTexCoords
				)
			)
		)
	)
	return #(isBroken, insaneVerts, badTexCoords)
)

--Check to see if all the materials are Prepar3D Materials, if not, then flag the ones that aren't.  If they are Prepar3D
--materials, then check to make sure the material is valid
fn verifyAcesMats = 
(
	local isBroken = false, missingFSXShader = #(), strTexNotSpecial = #()
	for m in sceneMaterials do
	(
		if classOf m != P3DMaterial then
		(
			if classOf m == multiMaterial then
			(
				for subMats in m.materialList do
				(
					if classOf subMats != P3DMaterial then
					(
						isBroken = true
						append missingFSXShader subMats.name
						--format "% / " subMats.name to:missingFSXShader
					)
					else
					(
						try 
						(
							if classOf subMats.standard.bumpMap == bitmapTexture then
							(
								if (matchPattern (filenameFromPath subMats.standard.bumpMap.fileName) pattern:"*bump.psd") != true then
								(
									isBroken = true
									append strTexNotSpecial (filenameFromPath subMats.standard.bumpMap.fileName)
									--format "% / " (filenameFromPath subMats.standard.bumpMap.fileName) to:strTexNotSpecial
								)
							)
						)
						catch ()
						try 
						(
							if classOf subMats.standard.specularMap == bitmapTexture then
							(
								if (matchPattern (filenameFromPath subMats.standard.specularMap.fileName) pattern:"*specular.psd") != true then
								(
									isBroken = true
									append strTexNotSpecial (filenameFromPath subMats.standard.specularMap.fileName)
									--format "% / " (filenameFromPath subMats.standard.specularMap.fileName) to:strTexNotSpecial
								)
							)
						)
						catch ()
						try
						(
							if classOf subMats == P3DMaterial then
							(
								if subMats.standard.specularMap == undefined and subMats.bBlendInvSpecAlpha == true then
								(
									isBroken = true
									append strTexNotSpecial subMats.name
									append strTexNotSpecial "No specular map to blend by specular map alpha. BlendInvSpecAlpha. \n "
								)
								
								if subMats.standard.specularMap == undefined and subMats.bBlendDiffuseInvSpecAlpha == true then
								(
									isBroken = true
									append strTexNotSpecial subMats.name
									append strTexNotSpecial "No specular map to blend by specular map alpha. BlendDiffuseInvSpecAlpha. \n"
								)
							)
						)
						catch ()
					)
				)
			)
			else
			(
				isBroken = true
				append missingFSXShader m.name
				--format "% / " m.name to:missingFSXShader
			)
		)
		else
		(
			try 
			(
				if classOf m.standard.bumpMap == bitmapTexture then
				(
					if (matchPattern (filenameFromPath subMats.standard.bumpMap.fileName) pattern:"*bump.psd") != true then
					(
						isBroken = true
						append strTexNotSpecial (filenameFromPath m.standard.bumpMap.fileName)
						--format "% / " (filenameFromPath m.standard.bumpMap.fileName) to:strTexNotSpecial
					)
				)
			)
			catch ()
			try
			(
				if classOf m.standard.specularMap == bitmapTexture then
				(
					if (matchPattern (filenameFromPath subMats.standard.specularMap.fileName) pattern:"*specular.psd") != true then
					(
						isBroken = true
						append strTexNotSpecial (filenameFromPath m.standard.specularMap.fileName)
						--format "% / " (filenameFromPath m.standard.specularMap.fileName) to:strTexNotSpecial
					)
				)
			)
			catch ()
			try
			(
					if m.standard.specularMap == undefined and m.bBlendInvSpecAlpha == true then
					(
						isBroken = true
						append strTexNotSpecial m.name
						append strTexNotSpecial "No specular map to blend by specular map alpha. BlendInvSpecAlpha. \n"
						
					)
					
					if m.standard.specularMap == undefined and  m.bBlendDiffuseInvSpecAlpha == true then
					(
						isBroken = true
						append strTexNotSpecial m.name
						append strTexNotSpecial "No specular map to blend by specular map alpha. BlendDiffuseInvSpecAlpha. \n"
					)
			)
			catch ()	
		)
	)
	
	return #(isBroken, missingFSXShader, strTexNotSpecial)
)
	
--Check to see if there is more than one root node in the scene:
fn verifyAcesRoot = 
(
	local isBroken = false, multiRoots = #()
	if rootnode.children.count > 1 then
	(
		isBroken = true
		for obj in rootnode.children do
		(
			append multiRoots obj.name
			--format "% / " obj.name to:multiRoots
		)
	)
	return #(isBroken, multiRoots)
)