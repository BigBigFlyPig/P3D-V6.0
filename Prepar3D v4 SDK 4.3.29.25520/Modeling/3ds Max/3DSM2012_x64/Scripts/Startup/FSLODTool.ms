--This populates an LOD list based on the appropriate LOD names and then allows you to switch them quickly and see
--polygon counts.
rollout FSLODTool "Switch LODs!"
(
	button populate "Populate the LODs!" pos:[10,10] width:145
	button interactLOD "Interactive LOD Window" highlightColor:(color 228 200 255) pos:[160,10] width:145
	label pCountVal "Polygon Count:" align:#left pos:[10,35]
	dotNetControl allLOD "treeView" width:295 height:150 pos:[10,55]
	label excludeLabel "Exclude from List:" pos:[10,210] align:#left
	checkBox excludeBones "Bones" pos:[100,210] checked:true
	checkBox excludeAttach "AttachPts" pos:[155,210] checked:true
	dotNetControl mouseOverText "textBox" pos:[10,10]
	timer hoverTimer "Timer" active:false
	
	--Set up a function to recursively add child nodes
	fn addLODChildren theNode theChildren =
	(
		for c in theChildren do
		(
			if not (((excludeBones.checked) and (classOf c == boneGeometry)) or ((excludeBones.checked) and (classOf c == Biped_Object))) then
			(
				if not (((excludeAttach.checked) and (matchPattern c.name pattern:"*attachpt*")) or ((excludeAttach.checked) and (matchPattern c.name pattern:"*attachpt*"))) then
				(
					newNode = theNode.Nodes.add c.name --add to the parent!
					newNode.tag = dotNetMXSValue c --.tag can contain a MXS value
					addLODChildren newNode c.children --recursive call for new node
				)
			)
		)
	)

	
	--Set up the function to repopulate the LOD list based on the object's LODs.
	fn fillInTreeView allLOD =
	(
		theRoot = allLOD
		rootNodes = for o in objects where o.parent == undefined collect o
		addLODChildren theRoot rootNodes --no need to pass the TreeView
	)

	
	fn populateLODList allLOD =
	(
		--First clear the old tree view
		allLOD.nodes.clear()
		
		--Now populate with all the nodes in the scene based on the checkboxes
		fillInTreeView allLOD
		
		--Add in the "Unhide all node"
		local unhideNode = allLOD.nodes.add "Unhide All"
		unhideNode.tag = dotNetMXSValue (max unhide all)
		
		try (allLOD.nodes.item[0].expand ()) catch ()
	)
	
	--When the rollout is open, populate the LOD list
	on FSLODTool open do
	(
		populateLODList allLOD
		
		allLOD.indent = 10
		allLOD.backColor = allLOD.backColor.FromARGB 255 192 222 249
		allLOD.foreColor = allLOD.foreColor.FromARGB 255 50 10 10
		allLOD.hotTracking = true
		allLOD.fullRowSelect = false
		
		--Set the mouse over pic to be invisible
		mouseOverText.size = (dotNetObject "System.drawing.size" 0 0)
		mouseOverText.backColor = mouseOverText.backColor.FromARGB 255 192 222 249
	)
	
	--When the populate button is pressed, repopulate the LODs based on the current scene.
	on populate pressed do
	(
		populateLODList allLOD
	)
	
	--When any of the checkboxes are changed, repopulate the list with the appropriate settings
	on excludeBones changed newExcludeBones do populateLODList allLOD
	on excludeAttach changed newExcludeAttach do populateLODList allLOD
	
	--When an LOD item is selected, unhide that LOD item
	on allLOD mouseDown arg do
	(
		local dummyString = 0
		local hitNode = allLOD.GetNodeAt (dotNetObject "System.Drawing.Point" arg.x arg.y)
		allLOD.selectedNode = hitNode
		
		--If one of the nodes is selected, hide everything but that node and its children and update the polygon counter with that nodes and its children's face count
		if hitNode != undefined then
		(
			try
			(
				if hitNode.text == "Unhide All" then
				(
					max unhide all
					pCountVal.text = "Polygon Count: 0"
					clearSelection ()
				)
				else
				(
					hide$*
					select (execute ("$" + (hitNode.tag.value).name as string + "...*"))
					unhide $
					for obj in $ do
					(
						try
						(
							dummyString += obj.faces.count
						)
						catch ()
					)
					pCountVal.text = ("Polygon Count: " + dummyString as string)
				)
			)
			catch ()
		)
		else
		(
			max unhide all
		)
	)
		
	--Reparent the selected node when a drag and drop operation is performed.  If the node is collapsed and the mouse hovers, then expand the node or vice versa.
	on allLOD itemDrag arg do
	(
		global acesDraggedLOD = arg.item.tag.value
		global acesDraggedNode = allLOD.SelectedNode
		hoverTimer.ticks = 0
	)
	on hoverTimer tick do
	(
		if hoverTimer.ticks > 0 then
		(
			if hoverNode != undefined then
			(
				try
				(
					if hoverNode.isExpanded == false then 
					(
						hoverNode.expand ()
						hoverTimer.ticks = 0
						hoverTimer.active = false
					)
					else 
					(
						if hoverNode.isExpanded == true then 
						(
							hoverNode.collapse ()
							hoverTimer.ticks = 0
							hoverTimer.active = false
						)
					)
				)
				catch()
			)
		)
	)
	on allLOD mouseMove arg do
	(
		--If you are hovering over a node and it is expandable, expand or close it.
		hoverTimer.active = true
		
		if acesDraggedLOD != undefined and acesDraggedNode != undefined then
		(
			local hitNode = allLOD.GetNodeAt (dotNetObject "System.Drawing.Point" arg.x arg.y)
			allLOD.selectedNode = hitNode
			global hoverNode = hitNode
			mouseOverText.text = acesDraggedNode.text
			mouseOverText.borderStyle = mouseOverText.borderStyle.none
			mouseOverText.size = dotNetObject "System.Drawing.size" (acesDraggedNode.bounds.right - acesDraggedNode.bounds.left) (acesDraggedNode.bounds.bottom - acesDraggedNode.bounds.top)
			if dragOffset == undefined then
			(
				global dragOffset = [acesDraggedNode.bounds.left - arg.x + allLOD.pos.x,acesDraggedNode.bounds.top - arg.y + allLOD.pos.y]
			)
			else
			(
				mouseOverText.pos = [arg.x,arg.y] + dragOffset
			)
		)
		--Reset the timer to 0 each time the mouse is moved
		hoverTimer.ticks = 0
	)
	on allLOD mouseUp arg do
	(
		if acesDraggedLOD != undefined then
		(
			try
			(
				local hitNode = allLOD.GetNodeAt (dotNetObject "System.Drawing.Point" arg.x arg.y)
				acesDraggedLOD.parent = hitNode.tag.value
				populateLODList allLOD
				allLOD.selectedNode = hitNode
			)
			catch ()
		)
		--Void the mouse over image, the drag offset and the drag value.  Also reset the hover timer and turn it off.
		mouseOverText.size = dotNetObject "System.Drawing.size" 0 0
		dragOffset = undefined
		acesDraggedLOD = undefined
		hoverTimer.active = false
		hoverNode = undefined
	)
	
	--When the interactiveLOD button is pressed, setup a situation similar to the game engine for viewing.
	on interactLOD pressed do
	(
		hide $*
		unhide (execute ("$" + (getFilenameFile maxFilename) + "_LOD_100" + "...*"))
		
		--Get the viewport transform matrix so we can draw our text
		local coordSysTM = Inverse(viewport.getTM())
		local viewDir = -coordSysTM.row3
		local thisViewSize = getViewSize()
		local textPos = mapScreenToCP [thisViewSize.x / 4,thisViewSize.y / 5]
		local cpTransformMatrix = (getCPTM())
		local newTextPos = textPos * cpTransformMatrix
		local textSize = (getScreenScaleFactor textPos) / 8.000
		local textFacing = -viewDir
		
		--Create the text
		thisText = text text:"LOD : 100" align:#center size:textSize pos:newTextPos name:"LODText" dir:textFacing wireColor:red
		
		tool checkLODFactor
		(
			local p2ThisViewPoint
			local objBottomLODNum = 1
			on mousePoint clickno do
			(
				if clickno == 1 then () else #stop
				/*(
					local viewTM = inverse(getViewTM())
					freecamera name:"LODCam" pos:viewTM[4] dir:viewTM[3]
					viewport.setCamera $LODCam
				)*/
				p2ThisViewPoint = viewPoint
			)
			
			on mouseMove mouseMove do
			(
				if viewPoint.y < p2ThisViewPoint.y then (max izoom in) else (max izoom out)
				--Gather the LOD numbers into an array and sort them from lowest to highest
				local objLODArray = #()
				for obj in $*_LOD_* do 
				(
					LODNumber = fnDefineLODNum obj.name
					append objLODArray LODNumber
				)
				sort objLODArray
				--The first LOD in the array is the lowest LOD.
				objBottomLOD = objLODArray[objBottomLODNum] as string
				--find the top LOD then find it's radius.
				objBottomLODObj = (execute ("$" + (getFilenameFile maxFilename) + "_LOD_" + objBottomLOD + "...*"))
				p3LODRadius = objBottomLODObj.max - objBottomLODObj.min
				P3LODRadius = ((p3LODRadius.x * 0.5) + (p3LODRadius.y * 0.5) + (p3LODRadius.z * 0.5)) / 3
				--Move the viewport coordinates into world space.
				p3ScreenMin = (mapScreenToWorldRay [0,0]).pos
				p3ScreenMax = (mapScreenToWorldRay (getViewSize ())).pos
				p3ScreenSize = p3ScreenMax - p3ScreenMin
				p3ScreenSize.x = abs(p3ScreenSize.x)
				p3ScreenSize.y = abs(p3ScreenSize.y)
				--Determine the screen scaling factor based on the in game code
				p3ScreenFactor = ((0.25 * p3ScreenSize.x) / 2) / 16
				--Run the LOD code from the game.  If the LOD says to swap up, then swap up.  If it says stay the same, then stay the same.
				fnLODCode = (((2.5 * p3LODRadius)) * (1 / p3ScreenFactor))
				if fnLODCode > (objLODArray[objBottomLODNum] as float) then
				(
					hide (execute ("$" + (getFilenameFile maxFilename) + "_LOD_" + objBottomLOD + "...*"))
					objBottomLODNum += 1
					if objBottomLODNum > objLODArray.count then objBottomLODNum = objLODArray.count
					objBottomLOD = objLODArray[objBottomLODNum] as string
					unhide (execute ("$" + (getFilenameFile maxFilename) + "_LOD_" + objBottomLOD + "...*"))
				)
				try
				(
					if fnLODCode < (objLODArray[objBottomLODNum - 1] as float) then
					(
						hide (execute ("$" + (getFilenameFile maxFilename) + "_LOD_" + objBottomLOD + "...*"))
						objBottomLODNum -= 1
						if objBottomLODNum < 1 then objBottomLODNum = 1
						objBottomLOD = objLODArray[objBottomLODNum] as string
						unhide (execute ("$" + (getFilenameFile maxFilename) + "_LOD_" + objBottomLOD + "...*"))
					)
				)
				catch ()
				p2ThisViewPoint = viewPoint
				
				pushPrompt (objBottomLOD as string)
				
				--Find the screen location in world space
				local thisViewSize = getViewSize()
				local textPos = mapScreenToCP [thisViewSize.x / 4,thisViewSize.y / 5]
				local cpTransformMatrix = (getCPTM())
				local newTextPos = textPos * cpTransformMatrix
				local textSize = (getScreenScaleFactor textPos) / 8.000
				$LODText.pos = newTextPos
				$LODText.size = (textSize as float)
				$LODText.text = ("LOD:" + objBottomLOD as string)
			)
			
			on stop do
			(
				delete $LODText
				--delete $LODCam
				unhide $*
			)
		)
		startTool checkLODFactor
	)
	--An easy way to update the names of a file.  Rename the Max file name, then press this button.  It will rename the
	--LODs for you.
	button renBuild "Match LODs to filename!" pos:[10,230] width:295
	on renBuild pressed do
	(
		local LODname = getfilenamefile maxFileName
		for obj in $*_LOD_* do
		(
			LODNumber = fnDefineLODNum obj.name
			obj.name = (LODName + "_LOD_" + LODNumber)
		)
		if rootnode.children.count == 1 then rootNode.children.name = LODName
		populateLODList allLOD
	)
)