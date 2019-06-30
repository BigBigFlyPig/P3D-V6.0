To use the Multi-FunctionDisplay files in an aircraft SimObject:

1. Add the TargetingPodSample add-on to Prepar3D - see Learning Center for details.

2. Add a camera definition to the aircraft.cfg file with the following required parameters:

Title = "TargetingPodView"
TextureSizeX=915
TextureSizeY=597
RenderToTexture=TRUE

Example:

[CameraDefinition.009]
Title = "TargetingPodView"
Guid = {C70411BD-66CC-435C-9C75-7DA613B9A016}
Description = Sample sensor view
Origin = Cockpit
SnapPbhAdjust = Ordinal
SnapPbhReturn = True
PanPbhAdjust = Ordinal
PanPbhReturn = True
Track = None
ShowAxis = FrontOnly
AllowZoom = TRUE
InitialZoom = 1.0
SmoothZoomTime = 2.0
RenderDesignators=TRUE 
ShowWeather=FALSE 
XyzAdjust = TRUE
ShowLensFlare=TRUE
Category = Cockpit
InitialXyz = 0.66, -1.53, -4.36
HideUserAircraft=1
HideShadows = 1
HideReflection = 1
ShowLabels = 0
UseGlobalTerrainView=1
TextureSizeX=915
TextureSizeY=597
RenderToTexture=TRUE


3. Add two VCockpit entries to the panel.cfg for the map displays.  The 'texture' paramameter must match the examples below, and the gauge00 entry must match the locations of the tsdMap.xml and twdMap.xml files (located in the xmlPanels subfolder).

Example:

//--------------------------------------------------------
[Vcockpit04]
background_color		= 0,0,0
size_mm				= 1024,1024
visible				= 0
pixel_size			= 1024,1024
texture				= $MFD_Map_1
no_luminous			= 1
gauge00				= MFD!tsdMap,  0,0,885,590


//--------------------------------------------------------
[Vcockpit05]
background_color		= 0,0,0
size_mm				= 1024,1024
visible				= 0
pixel_size			= 1024,1024
texture				= $MFD_Map_2
no_luminous			= 1
gauge00				= MFD!twdMap,  0,0,476,476



4. To add the Multi-FunctionDisplay.swf and Multi-FunctionDisplay_2D.swf Scaleform panels to the panel.cfg as Windows, add the Window entries as normal referencing the correct SCALEFORM_FILE name and add the following to each entry:

SCALEFORM_RTT_ELEMENT_00 = $MFD_Map_1
SCALEFORM_RTT_ELEMENT_01 = $MFD_Map_2
SCALEFORM_RTT_ELEMENT_02 = TargetingPodView





