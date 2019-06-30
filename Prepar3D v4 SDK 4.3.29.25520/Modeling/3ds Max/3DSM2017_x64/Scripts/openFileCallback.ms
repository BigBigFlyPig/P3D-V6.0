
--rebuild rollout floater with new info
try (FSLODTool.populateLODList (FSLODTool.allLOD)) catch()
try (GUIDAndFriendlyRoll.fnPopulateGF ()) catch ()

--Try to update the sceneMaterials in the Shader Tool if that tool happens to be open
try
(
	matBitRollout.matTree.nodes.clear ()
	matBitRollout.populateMatTree ()
) catch ()