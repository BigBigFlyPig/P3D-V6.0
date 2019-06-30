
--Define a function to gather the LOD numbers from the objects
fn fnDefineLODNum fsObjName =
(
	objNameString = fsObjName as stringstream
	local thisPos = findString (objNameString as string) "_LOD_"
	thisPos += 4
	seek objNameString thisPos
	return (readLine objNameString)
)

--Define a function that, given an array of object names, returns the highest and lowest LOD name.
fn fnGetHighLowLOD objArray =
(
	local theseLODObjs = #()
	--First gather just the actual LOD roots themselves
	for objNum = 1 to objArray.count do
	(
		try
		(
			local thisLODNum = fnDefineLODNum objArray[objNum]
			append theseLODObjs objArray[objNum]
		)
		catch ()
	)
	
	--Then sort the LOD root numbers from high to low
	--define the sorting function
	fn fnCompareLOD v1 v2 = 
	(
		local thisResult = ((fnDefineLODNum v1) as integer) - ((fnDefineLODNum v2) as integer)
		case of
		(
			(thisResult < 0.0) : 1
			(thisResult > 0.0) : -1
			default : 0
		)
	)
	
	qsort theseLODObjs fnCompareLOD
	--Return the sorted list of LODs
	return theseLODObjs
)