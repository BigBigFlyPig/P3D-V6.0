
---This tool will separate wheels and apply the correct animation and stuff.
Rollout rollDetachWhls "Detach and setup wheels"
(
	radioButtons rbWheelType labels:#("Left Front", "Right Front","RearWheels") align:#left pos:[10,10] columns:1
	spinner spnTurnRadius "Turn Radius:" range:[-360,360,-360] width:80 type:#integer pos:[120,10] align:#left
	--radioButtons rbRotAxis "Rotation Axis:" labels:#("X","Y","Z") pos:[120,10] align:#left
	button btnSetupWheels "Go!" width:265 pos:[10,65]
	button btnUndoWheelSetup "Undo last wheel!" width:265 pos:[10,90]
	
	--When the rearwheels are selected, get rid of the turn radius field.
	on rbWheelType changed newRbWheelType do
	(
		if newRbWheelType == 3 then
		(
			spnTurnRadius.enabled = false
		)
		else (spnTurnRadius.enabled = true)
	)
	
	--Define a function that will return the LOD number of a given object
	fn fnGetLodNum obj = 
	(
		local objName, LODNum
		objName = obj.name as stringStream
		skipToString objName "_LOD_"
		LODNum = readLine objName
		if LODNum != "" then
		(
			return LODNum
		)
	)
	
	--Define a function that sets up rotational animation
	fn fnSetupRotAnim rotAxis obj rotAmount animLength = 
	(
		rotAxis = case rotAxis of
		(
			"X" : [1,0,0]
			"Y" : [0,1,0]
			"Z" : [0,0,1]
		)
		animationRange = interval 0 animLength
		with animate on
		(
			at time 0f (obj.pos = obj.pos; obj.rotation = obj.rotation)
			--at time 0f (obj.pos = obj.pos; (rotate obj (angleAxis 0 rotAxis)))
			at time (0.25 * animLength) (rotate obj (angleAxis (rotAmount * 0.25) rotAxis))
			at time (0.50 * animLength) (rotate obj (angleAxis (rotAmount * 0.25) rotAxis))
			at time (0.75 * animLength) (rotate obj (angleAxis (rotAmount * 0.25) rotAxis))
			at time (animLength) (rotate obj (angleAxis (rotAmount * 0.25) rotAxis))
		)
		--Add in the note tracks for the wheels
		local newNote = noteTrack "Anim"
		addNoteTrack obj.controller newNote
	)
	
	--Setup a function that adds in the note track information to the animated part
	fn fnAddNoteKeys thisObj keyValue animLength = 
	(
		local thisNoteTrack = getNoteTrack thisObj.controller 1
		addNewNoteKey thisNoteTrack.keys 0
		addNewNoteKey thisNoteTrack.keys animLength
		local thisNoteKeys = thisNoteTrack.keys
		thisNoteKeys[1].value = ("ANIM_START = \"" + keyValue + "\"")
		thisNoteKeys[2].value = ("ANIM_END = \"" + keyValue + "\"")
	)
	
	--Define a function that creates the wheel animations based on whether it is a static or turning wheel
	fn fnSetupWhlAnim parentObj thisObj objNamePrefix frontWheel LODNum = 
	(
		fnSetupRotAnim "X" thisObj -360 100
		--Add the c_tire_still_key note tracks to the spinning wheels
		fnAddNoteKeys thisObj "c_tire_still_key" 100
		
		if frontWheel == true then
		(
			local newDummy = dummy pos:thisObj.pivot name:(objNamePrefix + "turn_" + LODNum) boxsize:[.1,.1,.1] parent:parentObj
			thisObj.parent = newDummy
			fnSetupRotAnim "Z" newDummy spnTurnRadius.value 200
			--Add the c_wheel keys to the note track
			fnAddNoteKeys newDummy "c_wheel" 200
		)
		else
		(
			thisObj.parent = parentObj
		)
	)
	
	--Define a function that detaches the faces and creates a new mesh with the proper name
	fn fnDetachMesh obj objNamePrefix frontWheel = 
	(
		LODNum = fnGetLodNum obj
		local newMesh = meshop.detachFaces obj obj.selectedFaces asMesh:true delete:true
		update obj
		local eMesh = editable_Mesh ()
		eMesh.mesh = newMesh
		eMesh.name = (objNamePrefix + LODNum)
		eMesh.pivot = eMesh.center
		eMesh.material = obj.material
		update eMesh
		fnSetupWhlAnim obj eMesh objNamePrefix frontWheel LODNum
	)
	
	fn fnSetupWheels wheelName turnState =
	(
		for obj in $ do
		(
			if (matchPattern obj.name pattern:"*_LOD_*") then
			(
				if classOf obj == editable_Mesh then
				(
					if obj.selectedFaces.count > 0 then
					(
						fnDetachMesh obj wheelName turnState
					)
				)
			)
			else
			(
				--TODO:  Solve case where wheel is already detached.
			)
		)
	)
	
	--When the go button is pressed, setup the selected wheel parts based on the given parameters
	on btnSetupWheels pressed do
	(
		if $ != undefined then
		(
			--First hold the file so it can be undone if necessary
			holdMaxFile ()
			
			--Do the rearwheels
			if rbWheelType.state == 3 then
			(
				fnSetupWheels "RearWheels_" false
			)
			
			--Do the Left Front wheel
			if rbWheelType.state == 1 then
			(
				fnSetupWheels "LFrontWheel_" true
			)
			
			--Do the Right Front wheel
			if rbWheelType.state == 2 then
			(
				fnSetupWheels "RFrontWheel_" true
			)

		)
	)
	
	--If the undo is pressed, fetch the last scene
	on btnUndoWheelSetup pressed do
	(
		fetchMaxFile quiet:true
	)
)

--Rollout for selecting wheels so you can easily annotate the animations
rollout wheelSelector "Wheel selection"
(
	radioButtons rbWheelType "Animation Type:" labels:#("Turn Helper","Wheels") pos:[10,10]
	button btnSelectWhl "Select Wheels!" width:270 pos:[10,50]
	
	--Select the wheels when the button is pressed
	on btnSelectWhl pressed do
	(
		if rbWheelType.state == 1 then
		(
			select $*_turn_*
		)
		if rbWheelType.state == 2 then
		(
			select $*Wheel*
			deselect $*_turn_*
		)		
	)
)
detachWhlsTool = newRolloutFloater "Wheel Animation Tool" 300 250
addRollout rollDetachWhls detachWhlsTool
addRollout wheelSelector detachWhlsTool