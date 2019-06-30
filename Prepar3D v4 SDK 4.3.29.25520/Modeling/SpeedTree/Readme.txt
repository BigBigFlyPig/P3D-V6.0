- The SrtToMDL tool is used to compile SpeedTree runtime files (.srt) to Prepar3D model files (.mdl).  
- SrtToMDL is a simple commandline tool which takes only the srt filename as a parameter. 
- This can be typed in at the commandline:
	SrtToMDL.exe MyModelFileName.srt
- It can also be used by dragging an srt file into the SrtToMDL executable in Windows Explorer

Prerequisites:
The srt needs to have a model GUID and Friendly Name embedded or the conversion to MDL will fail.  To do this:
1) Open up the SpeedTree Model (.spm) in the SpeedTree Modeler (This must be purchased/licensed from IDV)
2) Select "Tree Properties" in the Properties windows
3) Find "Custom" properties section.  It should be at the bottom of the Tree Properties window.
4) Type the friendly name into the "User data 1" field
5) Type the GUID value into the "User data 2" field

To display properly in Prepar3D the following compiler settings should be used:
1) Creating an srt file requires the SpeedTree Compiler which must be purchased/licensed from IDV.
2) The setting  Compile->Shader Compilation->Material Merging should be set to "SpeedTree 5.x style"
3) In oder to ensure trees look coorect from above, all atlases should enable the horizontal billboard by checking the box that says "include" in the Horizontal section of the atlas properties

After generating mdls, bglcomp can be used to create a bgl library for a group a trees.

Any textures output by the SpeedTree compiler need to be copied into a directory in Prepar3D's texture path list. Unless they will be shared between multiple bgls, the simplest solution is to put them in the textures folder of the scenery entry containing the newly generated bgl.