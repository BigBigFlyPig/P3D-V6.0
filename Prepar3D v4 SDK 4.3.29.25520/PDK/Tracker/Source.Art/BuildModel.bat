

SET XtoMdlPath=..\..\..\..\..\Modeling SDK\3DSM2012\Plugins
SET ModelSrcPath=
SET ModelSrc=Tracker.x
SET ModelSrcInterior=Tracker.x
SET ModelDest=..\Content\SimObjects\Tracker\model\Tracker.mdl
SET ModelDestInterior=..\Content\SimObjects\Tracker\model\Tracker Interior.mdl
SET ModelDefPath=..\..\..\..\..\Modeling SDK\bin\ModelDef.xml

"%XtoMdlPath%"\xToMdl.exe /DICT:"%ModelDefPath%" /KEEP /OUT:"%ModelDest%" "%ModelSrc%"
"%XtoMdlPath%"\xToMdl.exe /DICT:"%ModelDefPath%" /KEEP /OUT:"%ModelDestInterior%" "%ModelSrcInterior%"