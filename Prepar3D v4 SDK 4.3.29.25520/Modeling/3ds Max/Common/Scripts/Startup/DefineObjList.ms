	
--Define a struct that has the define object list and define current directory functions in it for use in other rollouts
struct FSDefineObjList
(
	--Define a function that will return the current dir and the appropriate builddir selection
	fn fnDefineCurDir =
	(
		local setBuildDir
		local currentDir = maxFilePath as stringStream
		if skipToString currentDir "objects\\" == OK then
		(
			currentDir = readDelimitedString currentDir "\\"
			setBuildDir = (findItem fsObjectDirsList currentDir)
		)
		else
		(
			currentDir = ""
			setBuildDir = 0
		)
		return #(currentDir,setBuildDir)
	),
	
	--get the max file name for all the objects in the current directory and append it to an array
	fn defineObjList FSObjectPath currentDir = 
	(
		local objList = #()
		currentDir = currentDir as string
		for globDir in (getDirectories (FSObjectPath + currentDir + "\\*")) do
		(
			globDirString = globDir as stringstream
			skipToString globDirString (currentDir + "\\")
			globDirName = readDelimitedString globDirString "\\"
			append objList globDirName
		)
		local newFilePath = maxFilePath as stringStream
		skipToString newFilePath (currentDir + "\\")
		local thisDir = readDelimitedString newFilePath "\\"
		local thisDirNum = findItem objList thisDir
		return #(objList,thisDirNum)
	),
	
	--Setup the build type based on debug, retail or hybrid
	fn fnDefineBuildType FSUIInput =
	(
		local buildType = case FSUIInput of
		(
			1 : "debug"
			2 : "retail"
			3 : "hybrid"
		)
		return buildType
	)
)