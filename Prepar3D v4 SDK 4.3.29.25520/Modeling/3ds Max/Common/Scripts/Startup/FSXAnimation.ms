---------------------------------------------------------------------------
-- Prepar3D New Animation Manager
---------------------------------------------------------------------------

rollout summaryRollout "Summary" width:450 height:400
(
	listBox lbxSummary "Animation Summary" pos:[5,5] width:440 height:28
	button btnSummaryDone "Close Summary" pos:[455,20]
	
	on btnSummaryDone pressed do
	(
		summaryRollout.open = false
	)
)

rollout AnimationDLG "Animation Dialog" width:600 height:398
(
	---- Local variables ----
	global rootAnimDef,curSelNodeList,curSelNodeRoot,curAnimInfo,curAnimName,curSelGroup;
	global state_animPresent, state_markerPresent, selectedMarker;
		
	---- Dialog definitions ----
	listbox animGroup "Groups" pos:[5,8] width:150 height:14 
	listbox animList "Animation List" pos:[160,8] width:200 height:14 
	button objectSelect "Update Sel" pos:[370,20] width:80 height:20 message:"" toolTip:"Click to select which objects to edit animations for."
	edittext selectedObj "" pos:[460,20] width:110 height:18
	listbox objectAnimList "Object Anims" pos:[370,45] width:200 height:7

	button createAnim "Create" pos:[370,165] width:90 height:20 toolTip:"Creates a new animation for the selected part without keyframe offsets."
	button deleteAnim "Delete" pos:[480,165] width:90 height:20
	edittext newFramesStart "Start" pos:[370,190] width:88 height:20
	edittext newFramesEnd "End" pos:[480,190] width:88 height:20


	GroupBox grp4 "Auto Rotate" pos:[400,218] width:170 height:119
	checkbutton RotateX "X" pos:[412,235] width:39 height:20
	edittext rotateStartX "" pos:[455,235] width:50 height:20
	edittext rotateEndX "" pos:[510,235] width:50 height:20
	checkbutton RotateY "Y" pos:[412,260] width:39 height:20
	edittext rotateStartY "" pos:[455,260] width:50 height:20
	edittext rotateEndY "" pos:[510,260] width:50 height:20
	checkbutton RotateZ "Z" pos:[412,285] width:39 height:20
	edittext rotateStartZ "" pos:[455,285] width:50 height:20
	edittext rotateEndZ "" pos:[510,285] width:50 height:20
	button rotateCreate "Create" pos:[410,310] width:150 height:20

	GroupBox grp3 "Markers And Events" pos:[6,218] width:388 height:150
	listbox markerList "" pos:[16,237] width:100 height:9
	button addMarker "Add Marker" pos:[127,237] width:80 height:20 toolTip:"Adds a marker to the list for this animation"
	button addEvent "Add Event" pos:[212,237] width:80 height:20 toolTip:"Adds an event to ths list for this animation"
	button deleteMarker "Delete" pos:[298,237] width:80 height:20 toolTip:"Deletes the currently selected marker/event."
	edittext markerIDEdit "ID" pos:[230,267] width:140 height:18
	spinner frameSpinner "Frame" pos:[148,268] width:75 height:16 range:[0,1000,0] type:#integer scale:1
	edittext edt5 "" pos:[158,321] width:0 height:0
	edittext markerName "Name/Command" pos:[127,294] width:250 height:20
	edittext markerParam "Parameter" pos:[127,321] width:250 height:20
	
	button convertAmbient "Tick18 -> Ambient" pos:[15,374] width:100 height:20 toolTip:"Auto-annotates tick 18 parts for ambient animation."
	button convertFS9 "FS9 -> FS10" pos:[115,374] width:100 height:20 toolTip:"Auto-annotates old animations based on animation script."
    button btnReloadModelDef "Reload ModelDef.xml" pos:[360,374] width:120 height:20
	button btnSummary "Summary" pos:[490,374] width:80 height:20

	
	---- Local functions ----

	-- Loads and parses the XML file. Will return the real root node of
	-- the animation definition file.
	global rootDocumentNodeList;
	fn LoadAnimationDef =
	(
		local xmlParser,filePath,res;
		
		-- Create XML Parser
		xmlParser = MaxXML();
		
		-- Determine where the document is located (need plugin directory)
		-- TODO : Need easy way to do this!!!
		if (MaxUtils == undefined) then
		(
			filePath = "e:\fsx\public\core\bin\art\Max6_Plugins";
			--print "Failed finding plugin path!!";
		)
		else 
		(
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

			--print ("Plugin path = " + filePath);
		)
		
		-- Load up the XML file		
		if filePath != undefined then
		(
			res = xmlParser.LoadDocument(filePath + "\\modeldef.xml");
		)

		
		-- Validate that the document has loaded and get the real
		-- root node for it.
		if res == false then
		(
			--print ("Failed loading XML file = " + filePath + "\modeldef.xml");
		  	-- TODO Error
		)
		else
		(
			-- Take the document root and extract the real XML root node. This is essentially
			-- the second child node since the first one is the <?xml...> tag.
			--print ("Loaded XML file = " + filePath + "\modeldef.xml");
			local rootNode = xmlParser.GetRootNode();
			rootDocumentNodeList = rootNode.GetChildNodes();
			
			return rootDocumentNodeList[2];
		)

		-- Null out the parser so it can safely be destroyed		
		xmlParser = undefined;
	  	return undefined;
	)

	fn Marker_ExtractName markerToken =
	(
		return markerToken[3][2];
	)

	fn Marker_ExtractParam markerToken =
	(
		return markerToken[4][2];
	)

	fn Marker_ExtractID markerToken =
	(
		return markerToken[2][2];
	)

	fn Marker_ExtractType markerToken =
	(
		return markerToken[1][2];
	)

	-- Determines and returns the animation notrack for the supplied node. If none exists,
	-- a new one will be created.		
	global GetNoteTrackInfo_Result;
	fn GetNoteTrackInfo curNode =
	(
		GetNoteTrackInfo_Result = undefined;
		
		-- First thing is to get the note track from the node, or create
		-- one if it isn't there
		if (curNode != undefined) and (curNode.controller != undefined) do
		(
			if (numNoteTracks curNode.controller) < 1 then
			(
				GetNoteTrackInfo_Result = NoteTrack "Anim";
				addNoteTrack curNode.controller GetNoteTrackInfo_Result;
			)
			else 
				GetNoteTrackInfo_Result = getNoteTrack curNode.controller 1;
		)
		return GetNoteTrackInfo_Result;
	)

	-- Because all parameters to markers and events are in the form PRAM = "VALUE", this function will
	-- parse a note key and extract all the PARAM/VALUE pairs then store them into an array
	fn TokenizeNoteKey noteKey = 
	(
		local cmdArray = #("","");
		local tokenArray = #(#("",""),#("",""),#("",""),#("",""));  -- Hardcoded order for Type,ID,Name,Param
		local cmdPos = 1;
		local str = noteKey.value as stringstream;
		while (peekToken str) != undefined do
		(
			local token = readToken str;
			if  cmdPos == 1 then
			(
				if token != "=" then
				(
					cmdArray[1] = token;
				)
				else
				(				
					cmdPos = 2;
				)
				
			)
			else
			(
				cmdArray[2] = token;

				if (cmdArray[1] == "MARKER_EVENT_TYPE") then
					(tokenArray[1] = cmdArray;)
				else
				if (cmdArray[1] == "MARKER_EVENT_ID") then
					(tokenArray[2] = cmdArray;)
				else
				if (cmdArray[1] == "MARKER_EVENT_NAME") then
					(tokenArray[3] = cmdArray;)
				else
				if (cmdArray[1] == "MARKER_EVENT_PARAM") then
					(tokenArray[4] = cmdArray;)
				else append tokenArray cmdArray;

				cmdPos = 1;
				cmdArray = #("","");
			)
		)
		return tokenArray;
	)

	fn EnumerateObjectAnimationList curNode =
	(
		local animStrList,ntrack,nkeys;
		animStrList = #();

		ntrack = GetNoteTrackInfo curNode;
		nkeys = ntrack.keys;
		
		-- Go through all notes and find the start and end frames
		-- for the specified animation
		local i = 1;
		while nkeys[i] != undefined do
		(
			local nkey = nkeys[i];
			local keyTokens = TokenizeNoteKey nkey;
			if (keyTokens[5] != undefined) do
			(
				if (keyTokens[5][1] == "ANIM_START") do
				(
					append animStrList keyTokens[5][2];
				)
			)
			i = i + 1;
		)
		return animStrList;
	)


	-- Populates the animation list and dependant dialogs
	fn PopulateObjectAnimationList =
	(
		local animStrList,animGroupNode,animNodeList,ntrack,nkeys;
		animStrList = #();
		animNodeList = #();
		if (curSelNodeList != undefined) do
		(
			for listNode in curSelNodeList do
			(
				--print listNode;
				--print "foo";
				local res = EnumerateObjectAnimationList(listNode);
				--print res;
				--print "bar";
				local merged = for name in res
					where (finditem animStrList name) == 0 collect name
				--print merged;
				--print "baz";
				join animStrList merged
			)
		)

		objectAnimList.items = animStrList;
	)

	fn SetSelection curNode =
	(
	    curSelNodeList = curNode;

		-- Find the selection's parent (well the first selection's)
		curSelNodeRoot = undefined;
		if (curSelNodeList != undefined) and (curSelNodeList.count > 0) do
		(
			curSelNodeRoot = curSelNodeList[1];
			while curSelNodeRoot.parent != undefined do
			(
				curSelNodeRoot = curSelNodeRoot.parent;
			)
		)
		
		-- Populate the current animation list
		PopulateObjectAnimationList();
	)

	fn FindKeyTimeSlot keys time = (
		local lastKey,i;
		while (getNoteKeyIndex keys time) != undefined do 
		(
			time += 1t;
			--print ("*Time Adjust = " + (time as string));
		)
		return time;
	)

	-- This function creates a new key, ensuring there is no overlap
	-- with existing keys by incrementing the time's tick count	
	fn CreateKey keys time = (
		return addNewNoteKey keys (FindKeyTimeSlot keys time);
	)

	-- This function is used to set up anim start/end tags. It also takes care of reserving
	-- the slots for unselected nodes
	fn CreateKeyRecursivePrefix namePrefix nameMatch bAllowNuke rootNode curNode time value = 
	(
		-- Get the note track for this guy
		local ntrack = GetNoteTrackInfo rootNode;

		
		-- Check prefix if needed
		local bCreateKey = true;
		local bNukeKeys = false;
		if namePrefix != undefined then
		(
		    local prefix = substring rootNode.name 1 namePrefix.count
		    -- print ("Name prefix = " + namePrefix + "; Prefix = " + prefix + "\n");
		    local cmpRes = stricmp prefix namePrefix
		    if cmpRes != 0 then
		    (
		        bCreateKey = false;
		    )
		    else
		    (
		        bNukeKeys = true;
                --print ("  Found: "+rootNode.name+"  For: "+value);
		    )
        )
        else
        (
		    if nameMatch != undefined do
		    (
		        local cmpRes = stricmp rootNode.name nameMatch
		        if cmpRes != 0 then
		        (
		            bCreateKey = false;
		        )
		        else
		        (
		            bNukeKeys = true;
                    --print ("  Found: "+rootNode.name+"  For: "+value);
		        )
		        
            )
        )

		-- Delete all keys if requested
		if bAllowNuke == true do
		(
		    if bNukeKeys == true do
		    (
		        while ntrack.keys[1] != undefined do
		        (
			        ntrack.keys[1].selected = true;
			        deleteNoteKeys ntrack.keys #selection
		        )
		    )
		)

		-- Create a key for this node
		if bCreateKey == true do
		(
		    local newKey = CreateKey ntrack.keys time;
			newKey.value = value;
	    )
	
		-- process all children
		local child = rootNode.children;
		for i = 1 to child.count do
			CreateKeyRecursivePrefix namePrefix nameMatch bAllowNuke child[i] curNode time value;
	)
	
	fn CreateKeyRecursive rootNode curNode time value = 
	(
	    CreateKeyRecursivePrefix undefined undefined false rootNode curNode time value;
	)
	-- Determine the category we have selected, i.e: All = none.
	fn DetermineAnimationGroup curSel rootNode =
	(
		if curSelGroup != undefined do
		(
			if curSelGroup != "All" do
			(
				animGroupNode = rootAnimDef.SelectSingleNode("/ModelInfo/AnimationGroup[@name=\"" + curSelGroup + "\"]");
				return animGroupNode;
			)
		)
		return undefined;
	)

	-- Populates the animation list and dependant dialogs
	fn PopulateAnimationList =
	(
		local animStrList,animGroupNode,animNodeList;
		animStrList = #();
		animNodeList = #();

		-- Check which category (or group) we are in.
		-- Populate the listbox apropriately
		animGroupNode = DetermineAnimationGroup curSelNodeList[1] curSelNodeRoot;
		if animGroupNode == undefined then
		(
			-- Select all animations
			animNodeList = rootAnimDef.SelectNodes("/ModelInfo/Animation");
		)
		else
		(
			local animRefList;

			-- Get all the references for the group
			animRefList = animGroupNode.SelectNodes("AnimationRef");
			
			-- Go through this list and extract all nodes
			for i = 1 to animRefList.count do
			(
				local guid,animRef,refedList;
				local query = "/ModelInfo/Animation[@guid=\"";
				
				animRef = animRefList[i];
				guid = animRef.GetAttributeValue("guid");
				
				query = query + guid;
				query = query + "\"]";
				refedList = rootAnimDef.SelectNodes(query)
				
				join animNodeList refedList
			)
		)		
		
		-- Take our explicit node list and turn it into a string list
		for i = 1 to animNodeList.count do
		(
			local name,animNode;
			animNode = animNodeList[i];
			name = animNode.GetAttributeValue("name");
			append animStrList name;
		)
		sort animStrList;
		animList.items = animStrList;
	)


	-- Populates the animation list and dependant dialogs
	fn PopulateGroupList =
	(
		local animStrList,animNodeList;
		animStrList = #();
		animNodeList = #();

		animNodeList = rootAnimDef.SelectNodes("/ModelInfo/AnimationGroup");
		
		-- Take our explicit node list and turn it into a string list
		append animStrList "All";
		for i = 1 to animNodeList.count do
		(
			local name,animNode;
			animNode = animNodeList[i];
			name = animNode.GetAttributeValue("name");
			append animStrList name;
		)
		animGroup.items = animStrList;
	)

 	fn ReselectMarker =
	(
		for i = 1 to markerList.items.count do
		(
			if markerList.items[i] == selectedMarker do
				markerList.selection = i;
		)
	)

	fn PopulateMarkerInfo marker =
	(
		if (marker != undefined) and (curAnimInfo != undefined) then
		(
			local markerToken = TokenizeNoteKey marker;
			markerName.text = Marker_ExtractName(markerToken);
			markerParam.text = Marker_ExtractParam(markerToken);
			--print ("MT : " + ((floor marker.time.frame) as string));
			--print ("MV : " + ((((floor marker.time.frame) as integer)) as string));
			frameSpinner.range = [curAnimInfo[2],curAnimInfo[3],1];
			frameSpinner.value = (((floor marker.time.frame) as integer)) as float;
			markerIDEdit.text = Marker_ExtractID(markerToken);
		)
		else
		(
			markerName.text = "N/A";
			markerParam.text = "";
			frameSpinner.value = 0;
			markerIDEdit.text = "";
		)
	)

	fn FindMarkerByID markerID =
	(
		if (curAnimInfo != undefined) and (markerID != "") do
		(
			local ntrack = GetNoteTrackInfo curSelNodeList[1]
			local nkeys = ntrack.keys;
			
			-- Go through all keys and validate the marker against the supplied ID
			local i = 1;
			while nkeys[i] != undefined do
			(
				local nkey = nkeys[i];
				if  (nkey.time>=curAnimInfo[2]) and (nkey.time<=curAnimInfo[3]) do
				(
					local keyTokens = TokenizeNoteKey nkey;
					if Marker_ExtractID(keyTokens) == markerID do
						return nkey;
				)
				i = i + 1;
			)
		)
		return undefined;
	)


	-- This function updates the global state of the dialog based on a set of local variables
	-- which keep track of the state of the tool
	fn UpdateDialogState animInfo =
	(
		-- Copy the selected marker ID to keep track of the selection
		selectedMarker = markerList.selected;

		-- Find the marker/event node based on its ID
		local marker = FindMarkerByID(selectedMarker);
		PopulateMarkerInfo(marker);

		-- Update the general dialog state as needed
		if marker != undefined then 
			state_markerPresent = true;
		else
			state_markerPresent = false;

		convertAmbient.enabled = true;
		convertFS9.enabled = true;

		-- Update the enabled/disabled states as needed
		createAnim.enabled = not state_animPresent;
		--deleteAnim.enabled = state_animPresent;
		
	    -- Update the enabled/disabled states as needed for marker/event section
		markerList.enabled = state_animPresent;
		addMarker.enabled = state_animPresent;
		addEvent.enabled = state_animPresent;
		deleteMarker.enabled = state_animPresent and state_markerPresent;
		markerIDEdit.enabled = state_animPresent and state_markerPresent;
		frameSpinner.enabled = state_animPresent and state_markerPresent;
		edt5.enabled = state_animPresent and state_markerPresent;
		markerName.enabled = state_animPresent and state_markerPresent;
		markerParam.enabled = state_animPresent and state_markerPresent;
			
		-- Update current selection name
		if curSelNodeList != undefined then
		(
			if curSelNodeList.count == 1 then
				selectedObj.text = curSelNodeList[1].name;
			else
				selectedObj.text = "<multiple>";
		)
		else
			selectedObj.text = "N/A";
		
		
    )



	fn PopulateMarkerList animInfo =
	(
		-- Flush out the current marker list
		local markerStrList = #();
		
		-- Populate with all enties
		if animInfo != undefined do
		(
			local ntrack = GetNoteTrackInfo curSelNodeList[1]
			
			-- Go through all keys and validate the marker against the supplied ID
			nkeys = ntrack.keys;
			local i = 1;
			while nkeys[i] != undefined do
			(
				local nkey = nkeys[i];
				if  (nkey.time >= animInfo[2]) and (nkey.time <= animInfo[3]) do
				(
					local markerToken = TokenizeNoteKey nkey;
					local markerID = Marker_ExtractID(markerToken);
					if markerID != "" do
					(
						append markerStrList markerID;
					)
				)
				
				i = i + 1;
			)
		)
		markerList.items = markerStrList;
	)

		
	-- This function goes through a note track and extracts all relevant information to
	-- a specific animtion, including its name, start/end times and markers/events
	global GetAnimationInformation_Result;
	fn GetAnimationInformation curNode anim  =
	(
		local ntrack,sframe,eframe,nkeys;
		
		if (curNode != undefined) do
		(
			start_key = "ANIM_START = \"" + anim + "\"";
			end_key = "ANIM_END = \"" + anim + "\"";

			
			ntrack = GetNoteTrackInfo curNode
			nkeys = ntrack.keys;
			
			-- Go through all notes and find the start and end frames
			-- for the specified animation
			local i = 1;
			while nkeys[i] != undefined do
			(
				local nkey = nkeys[i];
				local keyTokens = TokenizeNoteKey nkey;
				if (keyTokens[5] != undefined) do
				(
					if (keyTokens[5][1] == "ANIM_START") and (keyTokens[5][2] == anim) do
						(sframe = nkey.time;/*print "Found Start";print keyTokens;*/)
					if (keyTokens[5][1] == "ANIM_END") and (keyTokens[5][2] == anim) do
						(eframe = nkey.time;/*print "Found End";print keyTokens;*/)
				)
				i = i + 1;
			)
			
			-- If we have a defined animation, find all marker and event keys
			-- located in this anim
			if (sframe != undefined) and (eframe != undefined) do
			(
				local markerList = #();
				
				-- Scan markers
				i = 1;
				while nkeys[i] != undefined do
				(
					local nkey = nkeys[i];
					local markerInfo = #("","","","");
					if  (nkey.time>=sframe) and (nkey.time<=eframe) do
					(
						local keyTokens = TokenizeNoteKey nkey;
						for j = 1 to keyTokens.count do
						(
							markerInfo[1] = keyTokens[1][2];
							markerInfo[2] = keyTokens[2][2];
							markerInfo[3] = keyTokens[3][2];
							markerInfo[4] = keyTokens[4][2];
					)

						if (markerInfo[1]!="") and (markerInfo[2]!="") do
							append markerList markerInfo;
					)
					i = i + 1;
				)

				-- Start building the anim info result array
				GetAnimationInformation_Result = #(animName,sframe,eframe,markerList);
	
				return GetAnimationInformation_Result;			
			)
		)
		

		return undefined;
	)
	
	fn DeleteAnimation curNode anim =
	(
		local ntrack,sframe,eframe,nkeys;
		local animInfo,i;
		
		ntrack = GetNoteTrackInfo curNode;
		nkeys = ntrack.keys;
		
		-- Go through all notes and find the start and end frames
		i = 1;
		while nkeys[i] != undefined do
		(
			local nkey = nkeys[i];
			local keyTokens = TokenizeNoteKey nkey;
			if (keyTokens[1] != undefined) do
			(
				try
				(
					if (keyTokens[5][1] == "ANIM_START") and (keyTokens[5][2] == anim) do
						nkey.selected = true;
					if (keyTokens[5][1] == "ANIM_END") and (keyTokens[5][2] == anim) do
						nkey.selected = true;
				)
				catch
				(
					if (keyTokens[2][1] == "MARKER_EVENT_ID") do
						nkey.selected = true;
				)
			)
			i = i + 1;
		)
		deleteNoteKeys ntrack.keys #selection

		return undefined;
	)
	
	fn UpdateAnimationSelection animName = 
	(
			animInfo = GetAnimationInformation curSelNodeList[1] animName;
	
			if animInfo != undefined then
				state_animPresent = true;
			else
				state_animPresent = false;
			curAnimInfo = animInfo

			PopulateMarkerList(curAnimInfo);
			UpdateDialogState(curAnimInfo);

			-- Pre populate the auto rotate boxes if possible
			animNode = rootAnimDef.SelectSingleNode("/ModelInfo/Animation[@name=\""+curAnimName+"\"]");
			if animNode != undefined do 
			(
				local startX = animNode.GetAttributeValue("autoRotateXStart");
				local startY = animNode.GetAttributeValue("autoRotateYStart");
				local startZ = animNode.GetAttributeValue("autoRotateZStart");
				local endX = animNode.GetAttributeValue("autoRotateXEnd");
				local endY = animNode.GetAttributeValue("autoRotateYEnd");
				local endZ = animNode.GetAttributeValue("autoRotateZEnd");
				if (startX != "") and (endX != "") then
				(
					RotateX.state = true;
					rotateStartX.text = startX;
					rotateEndX.text = endX;
				)
				else
				(
					RotateX.state = false;
					rotateStartX.text = "0";
					rotateEndX.text = "0";
				)
		
				if (startY != "") and (endY != "") then
				(
					RotateY.state = true;
					rotateStartY.text = startY;
					rotateEndY.text = endY;
				)
				else
				(
					RotateY.state = false;
					rotateStartY.text = "0";
					rotateEndY.text = "0";
				)
		
				if (startZ != "") and (endZ != "") then
				(
					RotateZ.state = true;
					rotateStartZ.text = startZ;
					rotateEndZ.text = endZ;
				)
				else
				(
					RotateZ.state = false;
					rotateStartZ.text = "0";
					rotateEndZ.text = "0";
				)
			)

	)
	
	fn CreateMarkerValue ID Type name param =
	(
		local keyValue = "MARKER_EVENT_ID = \"" + ID + "\"  ";
		keyValue = keyValue + "MARKER_EVENT_TYPE = \"" + Type + "\"  ";
		keyValue = keyValue + "MARKER_EVENT_NAME = \"" + name + "\"  ";
		keyValue = keyValue + "MARKER_EVENT_PARAM = \"" + param + "\"  ";
		return keyValue;
	)


	fn CreateMarker time ID name =
	(
		if (curAnimInfo != undefined) do
		(
			local ntrack = GetNoteTrackInfo curSelNodeList[1]
			local newKey = CreateKey ntrack.keys (curAnimInfo[2] + time);
			newKey.value = CreateMarkerValue ID "Marker" name "";
		)
	)

	fn CreateEvent time ID name param =
	(
		if (curAnimInfo != undefined) do
		(
			local ntrack = GetNoteTrackInfo curSelNodeList[1]
			local newKey = CreateKey ntrack.keys (curAnimInfo[2] + time);
			newKey.value = CreateMarkerValue ID "Event" name param;
		)
	)

	fn DeleteMarkerByID ID =
	(
		local animInfo = curAnimInfo;

		-- Locate our marker
		if animInfo != undefined do
		(
			local ntrack = GetNoteTrackInfo curSelNodeList[1]
			
			-- Go through all keys and validate the marker against the supplied ID
			local nkeys = ntrack.keys;
			local i = 1;local done = false;
			while (nkeys[i] != undefined) and (done == false) do
			(
				local nkey = nkeys[i];
				if (nkey.time >= animInfo[2]) and (nkey.time <= animInfo[3]) do
				(
					local markerToken = TokenizeNoteKey nkey;
					local marker = Marker_ExtractID(markerToken);
					if marker == ID do
					(
						deleteNoteKey nkeys i;
						done = true;
					)
				)
				i += 1;
			)
		)
	)

	
	-- Event handlers
	




	fn CreateNewAnimation =
	(
		local nk_start,nk_end;

		local CurSelNode;
		local i = 1;
		for i = 1 to curSelNodeList.count do
		(
			CurSelNode = curSelNodeList[i];
			
			-- Determine the end of the note tracks
			local ntrack;
			ntrack = GetNoteTrackInfo CurSelNode;
			local nkeys = ntrack.keys;
			
			-- Determine if this animation already exists or is reserved somewhere
			local animStartTime = 0f;
			local animEndTime = 0f;
			animStartTime.frame = (newFramesStart.text) as integer;
			animEndTime.frame = (newFramesEnd.text) as integer;
			
			-- Create new animation at the end
		    nk_start = CreateKey ntrack.keys animStartTime;
			nk_start.value = ("ANIM_START = \"" + curAnimName + "\"");
	
		    nk_end = CreateKey ntrack.keys animEndTime;
			nk_end.value = ("ANIM_END = \"" + curAnimName + "\"");
		)
	
		-- Update animation display
		UpdateAnimationSelection(curAnimName);
	)

	fn CreateRotation curNode curAnim startX endX startY endY startZ endZ =
	(
		--print ("New rot start = " + (startX as string) + ", " + (startY as string) + ", " + (startZ as string));
		--print ("New rot end = " + (endX as string) + ", " + (endY as string) + ", " + (endZ as string));
	
		-- Gather the animation information
		local animInfo = GetAnimationInformation curNode curAnim;
		
		if animInfo != undefined do (in coordsys local
		(
			-- Grab the controler
			in coordsys local (
			local RX = curNode.rotation.x_rotation;
			local RY = curNode.rotation.y_rotation;
			local RZ = curNode.rotation.z_rotation;
			--print ("Rot Init = " + (curNode.rotation.x_rotation as string) + ", " + (curNode.rotation.y_rotation as string) + ", " + (curNode.rotation.z_rotation as string));
			--print ("Rot Init2 = " + (RX as string) + ", " + (RY as string) + ", " + (RZ as string));
			)
			
			-- Delete all keys within the range
	        deleteTime curNode.rotation.controller animInfo[2] animInfo[3] #incLeft #incRight #noSlide 
			
			-- Create keys
	         with animate on
			(
				at time animInfo[2]
				( 
					curNode.rotation.x_rotation = startX
					curNode.rotation.y_rotation = startY
					curNode.rotation.z_rotation = startZ
--					curNode.rotation.x_rotation = RX + startX
--					curNode.rotation.y_rotation = RY + startY
--					curNode.rotation.z_rotation = RZ + startZ
					--print ("Rot A = " + (curNode.rotation.x_rotation as string) + ", " + (curNode.rotation.y_rotation as string) + ", " + (curNode.rotation.z_rotation as string));
				)

				at time (animInfo[2]+(1*(animInfo[3]-animInfo[2]))/4) 
				(
					curNode.rotation.x_rotation = - (startX / 2) 
					curNode.rotation.y_rotation = - (startY / 2) 
					curNode.rotation.z_rotation = - (startZ / 2) 
--					curNode.rotation.x_rotation = RX + (startX / 2) 
--					curNode.rotation.y_rotation = RY + (startY / 2) 
--					curNode.rotation.z_rotation = RZ + (startZ / 2) 
					--print ("Rot B = " + (curNode.rotation.x_rotation as string) + ", " + (curNode.rotation.y_rotation as string) + ", " + (curNode.rotation.z_rotation as string));
				)

				at time (animInfo[2]+(2*(animInfo[3]-animInfo[2]))/4) 
				(
					curNode.rotation.x_rotation = - (startX / 2) 
					curNode.rotation.y_rotation = - (startY / 2) 
					curNode.rotation.z_rotation = - (startZ / 2) 
--					curNode.rotation.x_rotation = RX 
--					curNode.rotation.y_rotation = RY 
--					curNode.rotation.z_rotation = RZ 
					--print ("Rot C = " + (curNode.rotation.x_rotation as string) + ", " + (curNode.rotation.y_rotation as string) + ", " + (curNode.rotation.z_rotation as string));
				)


				at time (animInfo[2]+(3*(animInfo[3]-animInfo[2]))/4) 
				(
					curNode.rotation.x_rotation = (endX / 2) 
					curNode.rotation.y_rotation = (endY / 2) 
					curNode.rotation.z_rotation = (endZ / 2) 
--					curNode.rotation.x_rotation = RX + (endX / 2) 
--					curNode.rotation.y_rotation = RY + (endY / 2) 
--					curNode.rotation.z_rotation = RZ + (endZ / 2) 
					--print ("Rot D = " + (curNode.rotation.x_rotation as string) + ", " + (curNode.rotation.y_rotation as string) + ", " + (curNode.rotation.z_rotation as string));
				)

				at time animInfo[3] 
				(
					curNode.rotation.x_rotation = (endX / 2) 
					curNode.rotation.y_rotation = (endY / 2) 
					curNode.rotation.z_rotation = (endZ / 2) 
--					curNode.rotation.x_rotation = RX + endX
--					curNode.rotation.y_rotation = RY + endY
--					curNode.rotation.z_rotation = RZ + endZ
					--print ("Rot E = " + (curNode.rotation.x_rotation as string) + ", " + (curNode.rotation.y_rotation as string) + ", " + (curNode.rotation.z_rotation as string));
				)

			) 
		))
	)

	on AnimationDLG open do
	(
		local curSelList;
		
		-- Open and parse the XML file
		rootAnimDef = LoadAnimationDef();
		
		if rootAnimDef != undefined then
		(
			-- Assume the current node is the first selected node
			curSelNodeList = getCurrentSelection();
			curSelNodeRoot = undefined;
			--curSelList = getCurrentSelection();
			curSelGroup = "All";
			curAnimName = "";
			selectedMarker = "";
			state_animPresent = false;
			state_markerPresent = false;
			if curSelNodeList.count != 0 do 
			(
			    --SetSelection(curSelList[1]);
			    SetSelection(curSelNodeList);
			)
		
			-- Populate the category list
			PopulateGroupList();
			
			-- Populate the animation list dialog and dependant components
			PopulateAnimationList();
			
			-- Update the dialog settings
			UpdateDialogState(curAnimInfo);
		
			-- Determine if animation is defined for the selected object
			-- and populate the apropriate fields
			animName = animList.selected;
			curAnimName = animName;
			UpdateAnimationSelection(curAnimName);
		)
		else
		(
			messageBox "Failed to load modeldef.xml.  Please close the tool and try again."
		)

	)
	on animGroup selected sel do
	(
		local selGroup;
		selGroup = animGroup.items[sel];
		curSelGroup = selGroup;
		PopulateAnimationList();
	)
	on animList selected sel do
	(
		local animInfo,animName;

		animGroup.items[sel]
		
		-- Determine if animation is defined for the selected object
		-- and populate the apropriate fields
		animName = animList.items[sel];
		curAnimName = animName;
		UpdateAnimationSelection(curAnimName);
		
	)

	on objectAnimList selected sel do
	(
		local animName = objectAnimList.items[sel];
		local animIdx = finditem animList.items animName;
		animList.selection = animIdx;
		curAnimName = animName;
		UpdateAnimationSelection(curAnimName);
	)
	
	on objectSelect pressed do
	(
		curSelNodeList = getCurrentSelection();
		curSelNodeRoot = undefined;
		if curSelNodeList.count != 0 do 
		(
		    SetSelection(curSelNodeList);
		)
	
		-- Populate the animation list dialog and dependant components
		PopulateAnimationList();
		UpdateAnimationSelection(curAnimName);
		PopulateObjectAnimationList();
		
		-- Update the dialog settings
		UpdateDialogState(curAnimInfo);
	)
	
	on createAnim pressed do
	(
	    CreateNewAnimation();
		PopulateObjectAnimationList();
	)
	on deleteAnim pressed do
	(
		local CurSelNode;
		local i = 1;
		for i = 1 to curSelNodeList.count do
		(
			CurSelNode = curSelNodeList[i];
			DeleteAnimation CurSelNode curAnimName;
		)
		UpdateAnimationSelection(curAnimName);
		PopulateObjectAnimationList();
	)
	on rotateCreate pressed do
	(
		local rotXStart = (rotateStartX.text) as integer;
		local rotYStart = (rotateStartY.text) as integer;
		local rotZStart = (rotateStartZ.text) as integer;
		local rotXEnd = (rotateEndX.text) as integer;
		local rotYEnd = (rotateEndY.text) as integer;
		local rotZEnd = (rotateEndZ.text) as integer;
	
		if RotateX.state == false do (rotateXStart = 0;rotateXEnd = 0;)
		if RotateY.state == false do (rotateYStart = 0;rotateYEnd = 0;)
		if RotateZ.state == false do (rotateZStart = 0;rotateZEnd = 0;)
		
		local CurSelNode;
		local i = 1;
		for i = 1 to curSelNodeList.count do
		(
			CurSelNode = curSelNodeList[i];
			CreateRotation CurSelNode curAnimName rotXStart rotXEnd rotYStart rotYEnd rotZStart rotZEnd;
		)
	)
	on markerList selected sel do
	(
		UpdateDialogState(curAnimInfo);
	)
	on addMarker pressed do
	(
		-- Creare new marker
		CreateMarker 0 ("NEW_MARKER_" + ((random 1 1000) as string)) "MARKER";
		
		-- Repopulate the marker list
		PopulateMarkerList(curAnimInfo);
		ReselectMarker();
	)
	on addEvent pressed do
	(
		-- Creare new marker
		CreateEvent 0 ("NEW_EVENT_" + ((random 1 1000) as string)) "VFX0" "Param"
		
		-- Repopulate the marker list
		PopulateMarkerList(curAnimInfo);
		ReselectMarker();
	)
	on deleteMarker pressed do
	(
		DeleteMarkerByID(selectedMarker);
		PopulateMarkerList(curAnimInfo);
		UpdateDialogState(curAnimInfo);
	)
	on markerIDEdit entered text do
	(
		-- Find the marker and update its time
		local marker = FindMarkerByID(selectedMarker);
		if marker != undefined do
		(
			local markerToken = TokenizeNoteKey marker;
			local newValue = CreateMarkerValue text (Marker_ExtractType(markerToken)) (Marker_ExtractName(markerToken)) (Marker_ExtractParam(markerToken));
			marker.value = newValue;
		)
			
		-- Repopulate the marker list
		PopulateMarkerList(curAnimInfo);
		
		-- Move the selection to match the change
		selectedMarker = text;
		ReselectMarker();
		PopulateMarkerInfo(marker);
	)
	on frameSpinner changed val do
	(
		if curAnimInfo != undefined do
		(
			--print ("New Val = " + (val as string));
			-- Determine the requested time
			local markerTime = 0f;
			markerTime.frame = (val as integer);
			--print markerTime;
			
			-- Need to go through and find the first free time slot there
			local ntrack = GetNoteTrackInfo curSelNodeList[1];
			markerTime = FindKeyTimeSlot ntrack.keys markerTime;
			--print markerTime;
			
			-- Get the actual marker and update its time value
			local marker = FindMarkerByID(selectedMarker);
			if marker != undefined do
				marker.time = markerTime;
				
			-- Update the dialog...
			marker = FindMarkerByID(selectedMarker);
			PopulateMarkerList(curAnimInfo);
			ReselectMarker();
			PopulateMarkerInfo(marker);
			--print ntrack.keys;
		)
	)
	on markerName entered text do
	(
		-- Find the marker and update its time
		local marker = FindMarkerByID(selectedMarker);
		if marker != undefined do
		(
			local markerToken = TokenizeNoteKey marker;
			local newValue = CreateMarkerValue (Marker_ExtractID(markerToken)) (Marker_ExtractType(markerToken)) text (Marker_ExtractParam(markerToken));
			marker.value = newValue;
		)
			
		PopulateMarkerInfo(marker);
	)
	on markerParam entered text do
	(
		-- Find the marker and update its time
		local marker = FindMarkerByID(selectedMarker);
		if marker != undefined do
		(
			local markerToken = TokenizeNoteKey marker;
			local newValue = CreateMarkerValue (Marker_ExtractID(markerToken)) (Marker_ExtractType(markerToken)) (Marker_ExtractName(markerToken)) text;
			marker.value = newValue;
		)
			
		PopulateMarkerInfo(marker);
	)
	on convertAmbient pressed do
	(
		CreateKeyRecursivePrefix "tick18_" undefined true curSelNodeRoot curSelNodeList[1] animationrange.start ("ANIM_START = \"Ambient\"");
		CreateKeyRecursivePrefix "tick18_" undefined false curSelNodeRoot curSelNodeList[1] animationrange.end ("ANIM_END = \"Ambient\"");
	)
	on convertFS9 pressed do
	(
		local nodeList;
		nodeList = rootAnimDef.SelectNodes("/ModelInfo/Animation[@type=\"Sim\"]");
		if nodeList != undefined do 
		(
			for i = 1 to nodeList.count do
			(
				local name,animNode,animLengthStr;
				local animLength = 100f;
				animNode = nodeList[i];
				name = animNode.GetAttributeValue("name");
				
				animLengthStr = animNode.GetAttributeValue("length");
				if animLengthStr != "" do
				(
				    animLength = ((animLengthStr as float) as time);
				)
	
	            --print ("Converting: " + name + "  Length=" + (animLength as string));
			    CreateKeyRecursivePrefix name undefined true curSelNodeRoot curSelNodeList[1] 0f ("ANIM_START = \""+name+"\"");
			    CreateKeyRecursivePrefix name undefined false curSelNodeRoot curSelNodeList[1] animLength ("ANIM_END = \""+name+"\"");
			)
		)
	)
	fn EnumerateRecursive curNode animStrList =
	(
		if curNode != undefined do
		(
			local animStrList;
			animStrList = #();
			append animStrList curNode.name;
	
			-- Spew all related animations
			local animInfo;
			local ntrack = GetNoteTrackInfo curNode;
			if (ntrack != undefined) do
			(
				local nkeys = ntrack.keys;
				local i = 1;
				while nkeys[i] != undefined do
				(
					local nkey = nkeys[i];
					local keyTokens = TokenizeNoteKey nkey;
					if (keyTokens[5] != undefined) do
					(
						if (keyTokens[5][1] == "ANIM_START") do
						(
							local anim =  keyTokens[5][2];
							--print anim;
							animInfo = GetAnimationInformation curNode anim;
							append animStrList ("    ANIM: "+anim+" (Start = "+((animInfo[2].frame) as string) + 
												"  End = "+((animInfo[3].frame) as string)+")");
						)
					)
					i = i + 1;
				)
			)
	
			-- Recurse
			local child = curNode.children;
			for i = 1 to child.count do
				join animStrList (EnumerateRecursive child[i] animStrList);
				
			return animStrList;
		)
	)

	fn updateSummary =
	(
		local animStrList;
		animStrList = #();
		if (curSelNodeRoot != undefined) then
		(
			summaryRollout.lbxSummary.items = (EnumerateRecursive curSelNodeRoot animStrList);
		)
		else
		(
			summaryRollout.lbxSummary.items = (EnumerateRecursive rootNode animStrList);
		)
	)
	
    on btnReloadModelDef pressed do
    (
        
        rootAnimDef = LoadAnimationDef();
        
        if rootAnimDef != undefined then
        (        
            -- Populate the category list
            PopulateGroupList();
                
            -- Populate the animation list dialog and dependant components
            PopulateAnimationList();
        )
    )
    
	on btnSummary pressed  do
	(
		summaryRollout.open = true
		updateSummary();
	)
)

fn StartFSXAnimationEditor = 
(
	AnimationRollout = newRolloutFloater "Animation Manager" 600 455
	addRollout AnimationDLG AnimationRollout
	-- CreateDialog AnimationDLG width:580 height:400
	
	addRollout summaryRollout AnimationRollout rolledUp:true
	AnimationDLG.updateSummary();
)
