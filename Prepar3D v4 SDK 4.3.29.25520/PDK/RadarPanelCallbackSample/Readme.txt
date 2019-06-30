Steps to install and test this sample:
1. Open Prepar3DPanelCallback.sln and build the solution

2. Update dll.xml to include the add-on

------------------ Example DLL.xml ------------------------

	<?xml version="1.0" encoding="Windows-1252"?>

	<SimBase.Document Type="Launch" version="1,0">
	  <Descr>Launch</Descr>
	  <Filename>dll.xml</Filename>
	  <Disabled>False</Disabled>
	  <Launch.ManualLoad>False</Launch.ManualLoad>
	  <Launch.Addon>
		  <Name>RadarSample</Name>
		  <DLLType>PDK</DLLType>
		  <Disabled>False</Disabled>
		  <ManualLoad>False</ManualLoad>
		  <Path>D:\Prepar3D\trunk\Content\Base\SDK\Utilities\PDK\RadarPanelCallbackSample\Debug\RadarPanelCallback.dll</Path>
	  </Launch.Addon>
	</SimBase.Document>

------------------ Example DLL.xml ------------------------

3. Copy and paste the P3DRadarExample folder into your
global Prepar3D\Gauges folder or into your panels folder. 
Make sure to copy the folder and not just the xml file that it contains.

4. Add a panel entry to your aircraft's panel.cfg such as this:

------------------- Example Changes to Panels.cfg ----------------

	[Window Titles]
	Window02=RadarExample

	[Window02]
	size_mm=265,310
	Background_color=1,1,1 
	visible=0
	position=8
	windowsize_ratio=0.4
	gauge00=P3DRadarExample!Radar, 5, 5, 255, 305
	
------------------- Example Changes to Panels.cfg ----------------\

5.  Load up the aircraft and select RadarExample from the instrument panels menu.