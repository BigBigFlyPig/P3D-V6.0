
--Get the text for the macro scripts to be created from existing scripts
macroScript ExportLOD category:"Tools" tooltip:"Export and LOD Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\ExportAndLOD.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroScript UVToolSuite category:"Tools" tooltip:"UV Suite Deluxe"
(
	try (execute (openFile (acesScriptsFolder + "\\UVSuiteDeluxe.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroScript containerCreator category:"Tools" tooltip:"Build Container Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\BuildContainerTool.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript wheelTool category:"Tools" tooltip:"Wheel Animation Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\WheelsSetupTool.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript fileRenamer category:"Tools" tooltip:"Rename Flightsim File"
(
	try (execute (openFile (acesScriptsFolder + "\\RenameFile.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript AttachPointTool category:"Tools" tooltip:"Attach Point Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\AttachPointTool.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript FSCloudTool category:"Tools" tooltip:"Cloud Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\CloudTool.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript FSXShaderTool category:"Tools" tooltip:"Shader Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\FSXShaderTool.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript FSXMatEditorTool category:"Tools" tooltip:"Edit materials in the Materials.xml file"
(
	try (execute (openFile (acesScriptsFolder + "\\MaterialEditorTool.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript RemoveFSXMaterials category:"Tools" tooltip:"Remove Prepar3D materials from scene"
(
	try (execute (openFile (acesScriptsFolder + "\\RemoveFSXMat.ms"))) catch (messageBox "This tool is currently not supported.")
)
macroscript AnimationManager category:"Tools" tooltip:"Animation Manager"
(
	StartFSXAnimationEditor();
)

try (fileIn (acesScriptsFolder + "\\Startup\\XMakeFootprint.ms")) catch ()
macroscript MakeFootprint category:"Tools" tooltip:"Make footprints per city"
(
	if usingAcesBuild then
	(
		setupFootprintUI ()
	)
	else
	(
		messageBox "This tool is currently not supported."
	)
)

macroscript FSMobileSceneryTool category:"Tools" tooltip:"Mobile Scenery Offset Tool"
(
	try (execute (openFile (acesScriptsFolder + "\\MobileSceneryTool.ms"))) catch (messageBox "This tool is currently not supported.")
)

newMenuID = genClassID returnValue:true
if menuMan.registerMenuContext newMenuID[1] then
(
	mainMenuBar = menuMan.getMainMenuBar ()
	subMenu = menuMan.createMenu "Prepar3D Tools"
	exportItem = menuMan.createActionItem "ExportLOD" "Tools"
	if usingAcesBuild then UVScaleItem = menuMan.createActionItem "UVToolSuite" "Tools"
	if usingAcesBuild then containerItem = menuMan.createActionItem "containerCreator" "Tools"
	wheelToolItem = menuMan.createActionItem "wheelTool" "Tools"
	if usingAcesBuild then fileRenamerItem = menuMan.createActionItem "fileRenamer" "Tools"
	attachItem = menuMan.createActionItem "AttachPointTool" "Tools"
	fsxMatItem = menuMan.createActionItem "FSXShaderTool" "Tools"
	if usingAcesBuild then fsxMatEditorItem = menuMan.createActionItem "FSXMatEditorTool" "Tools"
	if usingAcesBuild then removeFSXItem = menuMan.createActionItem "RemoveFSXMaterials" "Tools"
	fsxAnimItem = menuMan.createActionItem "AnimationManager" "Tools"
	if usingAcesBuild then makeFPItem = menuMan.createActionItem "MakeFootprint" "Tools"
	cloudItem = menuMan.createActionItem "FSCloudTool" "Tools"
	if usingAcesBuild then msoTool = menuMan.createActionItem "FSMobileSceneryTool" "Tools"
	subMenu.addItem exportItem -1
	subMenu.addItem fsxMatItem -1
	subMenu.addItem fsxAnimItem -1
	subMenu.addItem attachItem -1
	if usingAcesBuild then subMenu.addItem UVScaleItem -1
	subMenu.addItem wheelToolItem -1
	subMenu.addItem cloudItem -1
	if usingAcesBuild then subMenu.addItem fsxMatEditorItem -1
	if usingAcesBuild then subMenu.addItem containerItem -1
	if usingAcesBuild then subMenu.addItem fileRenamerItem -1
	if usingAcesBuild then subMenu.addItem removeFSXItem -1
	if usingAcesBuild then subMenu.addItem makeFPItem -1
	if usingAcesBuild then subMenu.addItem msoTool -1
	subMenuItem = menuMan.createSubMenuItem "Prepar3D Tools" subMenu
	subMenuIndex = mainMenuBar.numItems() + 1
	mainMenuBar.addItem subMenuItem subMenuIndex
	menuMan.updateMenuBar()
)