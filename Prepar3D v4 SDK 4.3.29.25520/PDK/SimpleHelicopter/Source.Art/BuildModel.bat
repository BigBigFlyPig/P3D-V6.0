

SET XtoMdlPath=..\..\..\..\..\Modeling SDK\3DSM2012\Plugins
SET ModelSrcPath=
SET ModelSrc=SimpleHelicopter.x
SET ModelSrcInterior=SimpleHelicopter.x
SET ModelDest=..\Content\SimObjects\SimpleHelicopter\model\SimpleHelicopter.mdl
SET ModelDestInterior=..\Content\SimObjects\SimpleHelicopter\model\SimpleHelicopter Interior.mdl
SET ModelDefPath=ModelDef.xml

"%XtoMdlPath%\xToMdl.exe" /DICT:"%ModelDefPath%" /KEEP /XANIM /OUT:"%ModelDest%" "%ModelSrc%"
"%XtoMdlPath%\xToMdl.exe" /DICT:"%ModelDefPath%" /KEEP /XANIM /OUT:"%ModelDestInterior%" "%ModelSrcInterior%"