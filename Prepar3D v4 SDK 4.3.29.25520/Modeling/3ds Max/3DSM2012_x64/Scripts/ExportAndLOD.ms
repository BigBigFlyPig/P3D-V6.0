  
--Close the Export and LOD tool if its already opened
try (closeRolloutFloater FSExportAndLOD) catch ()

--Define rollout floater to be used for the export and LOD tool
FSExportAndLOD = newRolloutFloater "Export and LOD Tool" 330 70

--Load all of the appropriate include files so the functions will work
fileIn (acesScriptsFolder + "\\Startup\\DefineObjList.ms")
fileIn (acesScriptsFolder + "\\Startup\\LODFunctions.ms")
fileIn (acesPluginRoot + acesMax9ScriptsFolder + "\\Startup\\FSLODTool.ms")
fileIn (acesPluginRoot + acesMax9ScriptsFolder + "\\Startup\\XportModelRollout.ms")
fileIn (acesScriptsFolder + "\\Startup\\VerifyModelRollout.ms")
fileIn (acesScriptsFolder + "\\Startup\\GUIDAndFriendlyRollout.ms")

--Define the Main Control rollout for all the tools
rollout mainLODExport "Export and LOD Tools"
(
	dotNetControl mainControl "toolStrip" pos:[10,10] width:295 height:20
	
	--On open, fix up the tool strip and define the controls
	on mainLODExport open do
	(
		--Define the color
		mainControl.backColor = mainControl.backColor.FromARGB 255 192 222 249
		mainControl.foreColor = mainControl.foreColor.FromARGB 255 50 10 10
		
		--Add the Menu items
		mainControl.items.add("LOD Tool")
		mainControl.items.add("File Properties")
		mainControl.items.add("Verify")
		mainControl.items.add("Export")
	)
	
	on mainControl click arg do
	(
		local hitNode = mainControl.getItemAt (dotNetObject "System.Drawing.Point" arg.x arg.y)
		if hitNode != undefined then
		(
			if hitNode.checked == false then
			(
				hitNode.checked = true
				local hitTest = case hitNode.text of 
				(
					"LOD Tool" : (addRollout FSLODTool FSExportAndLOD; FSExportAndLOD.size.y += 280)
					"File Properties" : (addRollout GUIDAndFriendlyRoll FSExportAndLOD; FSExportAndLOD.size.y += 335)
					"Verify" : (addRollout verifyRollout FSExportAndLOD; FSExportAndLOD.size.y += 180)
					"Export" : (addRollout FSBuildingExport FSExportAndLOD; FSExportAndLOD.size.y += 240)
				)
			)
			else
			(
				hitNode.checked = false
				local hitTest = case hitNode.text of 
				(
					"LOD Tool" : (removeRollout FSLODTool FSExportAndLOD; FSExportAndLOD.size.y -= 280)
					"File Properties" : (removeRollout GUIDAndFriendlyRoll FSExportAndLOD; FSExportAndLOD.size.y -= 335)
					"Verify" : (removeRollout verifyRollout FSExportAndLOD; FSExportAndLOD.size.y -= 180)
					"Export" : (removeRollout FSBuildingExport FSExportAndLOD; FSExportAndLOD.size.y -= 240)
				)
			)
		)
	)
)


addRollout mainLODExport FSExportAndLOD

--Define the call back events script so that things update when you open a new file
callbacks.removescripts #filePostOpen
callbacks.addScript #filePostOpen filename:(acesPluginRoot + acesMax9ScriptsFolder + "\\openFileCallback.ms")