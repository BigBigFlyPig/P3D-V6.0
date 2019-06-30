

SET XtoMdlPath=..\..\..\..\..\Modeling SDK\3DSM2012\Plugins
SET ModelSrcPath=
SET ModelSrc=F-16.x
SET ModelSrcInterior=F-16_Interior.x
SET ModelDest=..\Content\SimObjects\SimpleAirplane\model\F16.mdl
SET ModelDestInterior=..\Content\SimObjects\SimpleAirplane\model\F16_Interior.mdl
SET ModelDefPath=ModelDef.xml

"%XtoMdlPath%"\xToMdl.exe /DICT:"%ModelDefPath%" /KEEP /XANIM /OUT:"%ModelDest%" "%ModelSrc%"
"%XtoMdlPath%"\xToMdl.exe /DICT:"%ModelDefPath%" /KEEP /XANIM /OUT:"%ModelDestInterior%" "%ModelSrcInterior%"