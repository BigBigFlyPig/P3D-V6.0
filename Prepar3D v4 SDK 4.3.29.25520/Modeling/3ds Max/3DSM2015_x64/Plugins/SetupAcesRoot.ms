
--By default, the using Aces build variable is false
global usingAcesBuild = false

try
(
	--Define the aces plugin directory and the aces root directory for the plugins
	global acesPluginDir = maxUtils.getPluginPath() as stringStream
	global acesPluginRoot = getfilenamepath (acesPluginDir as string)
	
	--If their is a folder called "MaxScripts" then you are working in the Aces build.  If there is a folder called "scripts" then you are working in the SDK.  If there is neither,
	--then you need to make sure you have one or the other on you machine.
	if doesFileExist (acesPluginRoot + "MaxScripts\\Tools") then
	(
		--Set the in house build variable to be true.
		usingAcesBuild = true
		
		--Define the FSRoot variable and the scripts folder variable
		skipToString acesPluginDir "public"
		dirMarker = filePos acesPluginDir
		seek acesPluginDir 0
		global FSRoot = substring (acesPluginDir as string) 1 (dirMarker - 7)
		global acesScriptsFolder = (acesPluginRoot + "MaxScripts\\Tools")
		global acesMax9ScriptsFolder = "Max9Scripts"
		global acesArtTools = acesPluginRoot 
	)
	if doesFileExist (acesPluginRoot + "Scripts") then
	(
		global acesScriptsFolder = ((maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath") + "Modeling\\3ds Max\\Common\\Scripts")
		global acesMax9ScriptsFolder = "Scripts"
		global acesArtTools = ((maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4 SDK" "SetupPath") + "Modeling\\3ds Max\\bin\\")
	)
)
catch 
(
	messageBox "If you want to work with the Prepar3D toolset, you need to point a path to the plugins directory.  That should be C:\Program Files\Lockheed Martin\Prepar3D SDK *\Modeling\3ds Max\3DSM *\Plugins, or wherever you have chosen to install your Prepar3D SDK."
)

--Define some XML globals that are used often
global FSXMLVersionLine = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
global FSXMLSchemaLine = "<FSData version=\"9.0\" xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:noNamespaceSchemaLocation=\"bglcomp.xsd\">"

--Define a function that sets up all of the global variables for the in house toolset
fn fnSetupAcesGlobals = 
(
	global FSObjectPath = (FSRoot + "\\art\\objects\\")
	global FSArt = (FSRoot + "\\art\\")
	global FSContent = (FSRoot + "\\content\\")
)

--If the in house variable is true, then run the setup globals function and set up a bunch of other functions only used in house
if usingAcesBuild then 
(
	--run the globals setup for in house tools
	fnSetupAcesGlobals ()
	
	--Parse the object directories and populate the directory list with them.
	global fsObjectDirsList = #()
	for objDirs in getDirectories (FSObjectPath + "*") do
	(
		local strObjDirsName = (objDirs as stringStream)
		skipToString strObjDirsName "objects\\"
		local strObjDirsShortName = readDelimitedString strObjDirsName "\\"
		if strObjDirsShortName != "obj" and strObjDirsShortName != "Textures_DO_NOT_BUILD" and strObjDirsShortName != "Weather" and strObjDirsShortName != "world" then
		(
			append fsObjectDirsList strObjDirsShortName
		)
	)
)


--Setup a bunch of functions that are used in other tools.
--This function will pull a GUID from an .x file
fn fnFindGUIDFromXFile FSXFileIn =
(
	skipToString FSXFileIn "GuidToName {"
	skipToString FSXFileIn "GuidToName {"
	skipToString FSXFileIn "\""
	return (readDelimitedString FSXFileIn "\"")
	close FSXFileIn
)

--Define a function that will create a unique flightsim GUID and return it
fn fnDefineFSGUID =
(
	local newGUIDX = (maxUtils.getGUID()) as stringStream
	skipToString newGUIDX "{"
	local thisFSGUID = readDelimitedString newGUIDX "}"
	return thisFSGUID
)

--Define a function to recursively walk through all of the directories below the specified directory (dir) and append an array (dirsArray) with each directory.
fn getSubDirs dir dirsArray wildCardPattern =
(
	for dirs in (getDirectories (dir + "*")) do
	(
		if (matchPattern dirs pattern:wildCardPattern ignoreCase:true) then append dirsArray dirs
		getSubDirs dirs dirsArray wildCardPattern
	)
)

--Define a function that creates an artProj file
fn createArtProjFile fileToCreate artProjBuildType = 
(
	local newArtProj = createFile fileToCreate
	format "<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n  %\n  <Import Project=\"$(INETROOT)\art\build\art.targets\" />\n</Project>" artProjBuildType to:newArtProj
	close newArtProj
)

--Define a function that returns the correct artProj info depending on whether the file is a container or not
fn fnReturnArtProjInfo buildDir = 
(
	local buildFolder = (getFilenameFile (substring buildDir 1 (buildDir.count - 1)))
	
	--There are different artProj files for containers and not containers.  Place the appropriate .artProjs based on this.
	if (matchPattern buildFolder pattern:"model") then
	(
		local buildDirRoot = (getFilenamePath (substring buildDir 1 (buildDir.count - 1)))
		local containerClassPath = (getFilenamePath (substring buildDirRoot 1 (buildDirRoot.count - 1)))
		local containerClass = (getFilenameFile (substring containerClassPath 1 (containerClassPath.count - 1)))
		local containerArtProj = ((getFilenameFile (substring buildDirRoot 1 (buildDirRoot.count - 1))) + ".artProj")
		return #(true,("<Import Project=\"$(INETROOT)\art\objects\\" + containerClass + "\\" + containerClass + ".sources\" />"),(buildDirRoot + containerArtProj))
	)
	else
	(
		if matchPattern buildDir pattern:"*global*" then
		(
			return #(false,"<Import Project=\"$(INETROOT)\art\objects\global\Global.sources\" />","global")
		)
		else
		(
			return #(false,"<Import Project=\"..\..\Cities.sources\" />","cities")
		)
	)
)

--Define a function that will call the appropriate CMD line stuff to build a local city directory
fn buildLocalContent buildDir pauseState buildType texBuild =
(
	local buildFolder = (getFilenameFile (substring buildDir 1 (buildDir.count - 1)))
	
	--If there is no .artProj file in this build folder, then create one.
	if not (doesFileExist (buildDir + buildFolder + ".artProj")) then
	(
		messageBox ("There is no .artProj for " + buildDir + buildFolder + ".  I will create one for you.  Make sure to sd add the .artProj file and check it in with this model.")
		
		--There are different artProj files for containers and not containers.  Place the appropriate .artProjs based on this.
		local isContainerBuild = fnReturnArtProjInfo buildDir
		if isContainerBuild[1] then
		(
			createArtProjFile (buildDir + buildFolder + ".artProj") isContainerBuild[2]
			
			--Now check for the .artProj file in the main container itself and add that if it doesn't exist
			if not (doesFileExist isContainerBuild[3]) then
			(
				createArtProjFile isContainerBuild[3] "<Import Project=\"$(INETROOT)\art\build\art.sources\" />"
			)
		)
		else
		(
			if isContainerBuild[3] == "global" then
			(
				createArtProjFile (buildDir + buildFolder + ".artProj") isContainerBuild[2]
			)
			else
			(
				createArtProjFile (buildDir + buildFolder + ".artProj") isContainerBuild[2]
			)
		)
	)
	--DOSCommand (acesCmdLine + "&pushd " + buildDir + "&build " + buildType + " content&" + texBuild + pauseState)
    messageBox "Aces command line no longer supported"
)

--Define a function that determines whether or not to build the texture directory parallel to the buildDir
fn buildAcesTexDir buildType buildDir texDir =
(
	local thisTextureDir = ((getFilenamePath (substring buildDir 1 (buildDir.count - 1))) + texDir)
	if doesFileExist thisTextureDir then 
	(
		--If there is no .artPtroj file in the folder, then create one before building
		if not (doesFileExist (thisTextureDir + "\\" + texDir + ".artProj")) then
		(
			messageBox ("There is no .artProj for " + thisTextureDir + "\\" + texDir + ".  I will create one for you.  Make sure to sd add the .artProj file and check it in with this model.")
			
			local isContainerBuild = fnReturnArtProjInfo buildDir
			if isContainerBuild[1] then
			(
				createArtProjFile (thisTextureDir + "\\" + texDir + ".artProj") isContainerBuild[2]
			)
			else
			(
				if isContainerBuild[3] == "global" then
				(
					createArtProjFile (thisTextureDir + "\\" + texDir + ".artProj") "<Import Project=\"..\..\Global_Texture.sources\" />"
				)
				else
				(
					--Gather the correct placement location from the city .artProj file
					local texDirRoot = (getFilenamePath (substring thisTextureDir 1 (thisTextureDir.count - 1)))
					local texCityName = (getFilenameFile (substring texDirRoot 1 (texDirRoot.count - 1)))
					if doesFileExist (texDirRoot + texCityName + ".artProj") then
					(
						local thisXML = MaxXML ()
						thisXML.loadDocument (texDirRoot + texCityName + ".artProj")
						local thisArtProjRoot = thisXML.getRootNode ()
						local thisArtProjChild = (thisArtProjRoot.getChildNodes ())[1]
						local thisImportNode = (thisArtProjChild.getChildNodes())[2]
						local thisImportValue = (thisImportNode.getAttributeValue("Project")) as stringStream
						skipToString thisImportValue "\\"
						local thisRegion = readDelimitedString thisImportValue "_"
						createArtProjFile (thisTextureDir + "\\" + texDir + ".artProj") ("<Import Project=\"..\..\\" + thisRegion + "_Texture.sources\" />")
					)
					else
					(
						messageBox "There is no .artProj for this city folder.  This is too complicated for me to create here.  Please create it manually and try again."
					)
				)
			)
		)
		
		return ("&cd " + (getFilenamePath (substring buildDir 1 (buildDir.count - 1))) + texDir + "&build " + buildType + " content&")
	)
	else
	(
		return ""
	)
)

--Create a functions that parses a string and makes all the letter caps
fn allCaps inputStr =
(
	for i = 1 to inputStr.count do
	(
		inputStr[i] = case inputStr[i] of
		(
			"a" : "A"
			"b" : "B"
			"c" : "C"
			"d" : "D"
			"e" : "E"
			"f" : "F"
			"g" : "G"
			"h" : "H"
			"i" : "I"
			"j" : "J"
			"k" : "K"
			"l" : "L"
			"m" : "M"
			"n" : "N"
			"o" : "O"
			"p" : "P"
			"q" : "Q"
			"r" : "R"
			"s" : "S"
			"t" : "T"
			"u" : "U"
			"v" : "V"
			"w" : "W"
			"x" : "X"
			"y" : "Y"
			"z" : "Z"
			default : inputStr[i]
		)
	)
	return inputStr
)	

--Define a function that returns the user's USERPROFILE as defined by the OS
fn fnGetFSUserProfile =
(
    local filePathTempDir = (sysInfo.tempdir + "FSSet.txt")
    
    DOSCommand ("pushd &set>" + filePathTempDir)
    
    local setFile = openFile (filePathTempDir)

    if setFile != undefined then
    (
        skipToString setFile "USERPROFILE="
        local fsUserProfile = "" as stringStream
        format "%" (readLine setFile) to:fsUserProfile
        close setFile
        deleteFile (filePathTempDir)
    )
    
	return (fsUserProfile as string)
)

--Define a function that returns the objects class name and object name and returns undefined if the object is not checked into the build tree.
fn fnGetFSClassName =
(
	local thisFilePath = maxFilePath as stringStream
	local thisObjPlace = findString (thisFilePath as string) "objects"
	if thisObjPlace != undefined then
	(
		seek thisFilePath (thisObjPlace + 7)
		local thisClass = (readDelimitedString thisFilePath "\\")
		local thisObject = (readDelimitedString thisFilePath "\\")
		return #(thisClass,thisObject)
	)
	else return undefined
)

--Define a function that cleans out the material editor window so that bogus textures aren't trying to be loaded or exported.
fn fnFSCleanMatEditorWindow = 
(
	for i = 1 to 24 do meditMaterials[i] = standard ()
	try (for i = 1 to sceneMaterials.count do meditMaterials[i] = sceneMaterials[i]) catch ()
)

fn LoadScripts =
(
    --First try to unregister all of the Prepar3D tools menus
    --Using a while loop to remove any duplicate entries
    fsToolsMenu = menuMan.findMenu "FS Tools"
    while fsToolsMenu != undefined do
    (
        try (menuMan.unregisterMenu fsToolsMenu) catch()
        fsToolsMenu = menuMan.findMenu "FS Tools"
    )

    acesToolsMenu = menuMan.findMenu "Aces Tools"
    while acesToolsMenu != undefined do
    (
        try (menuMan.unregisterMenu acesToolsMenu) catch()
        acesToolsMenu = menuMan.findMenu "Aces Tools"
    )
    
    p3dToolsMenu = menuMan.findMenu "Prepar3D Tools"
    while p3dToolsMenu != undefined do
    (
        try (menuMan.unregisterMenu p3dToolsMenu) catch()
        p3dToolsMenu = menuMan.findMenu "Prepar3D Tools"
    )

    global sdToolsMenu = menuMan.findMenu "SD Tools"
    while sdToolsMenu != undefined do
    (
        try (menuMan.unregisterMenu sdToolsMenu) catch()
        sdToolsMenu = menuMan.findMenu "SD Tools"
    )

    global acesDocsMenu = menuMan.findMenu "ACES Documentation"
    while acesDocsMenu != undefined do
    (
        try (menuMan.unregisterMenu acesDocsMenu) catch()
        acesDocsMenu = menuMan.findMenu "ACES Documentation"
    )
    
    global p3dDocsMenu = menuMan.findMenu "Prepar3D Documentation"
    while p3dDocsMenu != undefined do
    (
        try (menuMan.unregisterMenu p3dDocsMenu) catch()
        p3dDocsMenu = menuMan.findMenu "Prepar3D Documentation"
    )

    --Call all of the other plug-ins in the correct order so that they load properly regardless of how you have your drives or directories setup.
    try (fileIn (acesScriptsFolder + "\\Startup\\FSXMaterial.ms")) catch ()
    try (fileIn (acesScriptsFolder + "\\Startup\\FSXAnimation.ms")) catch ()
    try (fileIn (acesScriptsFolder + "\\Startup\\FSPolyLineTool.ms")) catch ()
    try (fileIn (acesPluginRoot + acesMax9ScriptsFolder + "\\Startup\\AcesTools.ms")) catch ()
    try (fileIn (acesScriptsFolder + "\\Startup\\SDTools.ms")) catch ()
    try (fileIn (acesScriptsFolder + "\\Startup\\AcesDocumentation.ms")) catch ( messageBox "Failed to load AcesDocumentation.ms" )
)

callbacks.addScript #postSystemStartup "LoadScripts()"
