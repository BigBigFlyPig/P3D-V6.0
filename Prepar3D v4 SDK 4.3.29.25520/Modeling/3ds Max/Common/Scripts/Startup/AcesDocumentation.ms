
--Setup the various documentation files
--6/6/2011 Changed the retail paths to reference Prepar3D's registry entry
-- TODO: Change the paths for the internal build entries.
macroScript ACESMaterialDoc category:"Tools" tooltip:"Materials..."
(
	if usingAcesBuild then
	(
		shellLaunch "hh.exe" (fsroot + "\\Learning Center\\sdk\\modeling\\3ds_max\\modeling\\modeling_materials.html")
	)
	else
	(
		local FSDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4" "SetupPath"
		shellLaunch "hh.exe" (FSDir + "Learning Center.chm::/sdk/modeling/3ds_max/modeling/modeling_materials.html")
	)
)

macroScript ACESToolsDoc category:"Tools" tooltip:"Modeling Tools..."
(
	if usingAcesBuild then
	(
		shellLaunch "hh.exe" (fsroot + "\\Learning Center\\sdk\\modeling\\3ds_max\\tools\\3ds_max_tools_overview.html")
	)
	else
	(
		local FSDir = maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4" "SetupPath"
        shellLaunch "hh.exe" (FSDir + "Learning Center.chm::/sdk/modeling/3ds_max/tools/3ds_max_tools_overview.html")
	)
)


--Define the Aces Documentation menu
newMenuID = genClassID returnValue:true
if menuMan.registerMenuContext newMenuID[1] then
(
	mainMenuBar = menuMan.getMainMenuBar ()
	subMenu = menuMan.createMenu "Prepar3D Documentation"
	acesMatItem = menuMan.createActionItem "ACESMaterialDoc" "Tools"
	acesToolsItem = menuMan.createActionItem "ACESToolsDoc" "Tools"
	subMenu.addItem acesMatItem -1
	subMenu.addItem acesToolsItem -1
	subMenuItem = menuMan.createSubMenuItem "Prepar3D Documentation" subMenu
	subMenuIndex = mainMenuBar.numItems() + 1
	mainMenuBar.addItem subMenuItem subMenuIndex
	menuMan.updateMenuBar()
)