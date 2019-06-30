This example illustrates using Shp2Vec to generate terrain vector data for the area surrounding VHHH.

The SourceData folder contains:
- Input Shapefiles that define the geometry and attribution for the vector data. Note the xml file, which defines how Shp2Vec should process the attributes for the objects within the Shapefiles.

The Output folder contains:
	- cvx7824.BGL (the output file).
	- ConsoleOutput.txt contains the console output from running Shp2Vec for this example.

Please notice the command line within ConsoleOutput.txt uses the default REPLACECELLS behavior. This will completely remove all of the original scenery within the bounds of the created BGL.