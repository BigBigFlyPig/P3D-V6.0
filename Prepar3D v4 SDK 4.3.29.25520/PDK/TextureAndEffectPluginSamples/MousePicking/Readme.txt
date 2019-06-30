This example will demonstrate how to make picking requests on a window similar to those
used by the core input system to map mouse input into the virtual cockpit.  The result of 
these pick requests is then used to determine if an interactive element was hit and how 
far away from the camera it is.  This hit data is then visualized using custom D3D11 
rendering in an effect plugin.

The plugin consists of two files:
MousePicking.dll - The DLL library that does the picking and rendering via the PDK

add-on.xml will be copied into the output folder.  This folder can 
be copied into your Prepar3D V4 Add-ons directory.

After enabling the addon, it can be used  in Prepar3D as follows:
1. Switch to a virtual cockpit view
2. Right click, select Custom Camera -> Save Location
3. In the Manage Camera Views menu, enter a name such as "PickTest" in the Name field.
4. In the Camera Effects section, click on the Effects drop-down
5. Select MousePickingTest from the list
6. Clack Add and then Save
7. Right click, select Custom Camera -> PickTest (or whatever name was given to the custom camera)

There should be 4 white spheres that draw whenever the point they represent passes a 
VC picking hit test.