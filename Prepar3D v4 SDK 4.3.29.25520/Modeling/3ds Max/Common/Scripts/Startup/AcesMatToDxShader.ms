

--Include the FSXMaterialIO.
fileIn (acesScriptsFolder + "\\Startup\\FSXMaterialIO.ms")

--This function will take an existing material (whether it is Prepar3D or not) and create a DX shader based on the given paramaters of the material.
fn fnAcesMatToDXShader acesMatToShade sceneMatToApply = 
(
	--Ger the directory of the user profile.
	local userDir = (fnGetFSUserProfile() + "\\My Documents\\Aces Files\\")
	
	--Make the directory for the files if it doesn't exist.
	makeDir (userDir + "maxHLSL")
	
	local newEffectFileName = userDir + "maxHLSL\\general.fx"
	local originalEffectFileName = acesArtTools + "maxHLSL\\General.fx"
	
	--Delete the old effect file to create the new one.
	--If we ever want to visualize more than one, we can generate them based
	--on the name of the underlining standard material set in max.
	if (getfiles originalEffectFileName).count != 0 then
	(
		if (getfiles newEffectFileName).count != 0 then
		(
			deleteFile newEffectFileName;
		)
	)
	
	--Open the original effect file.
	local generalShaderFile = openFile originalEffectFileName mode:"r"
	local generalShaderFileText = ""
	
	--Make sure we were able to open the file for read.
	if (generalShaderFile != undefined) then
	(
		--Loop through the original file and copy everything out
		seek generalShaderFile #eof
		local maxlen=filepos generalShaderFile
		seek generalShaderFile 0
		generalShaderFileText = readChars generalShaderFile maxlen errorAtEOF:false
	
		--Close the original effect file.
		close generalShaderFile;
	)

	--First ouput the appropriate AcesMatValues.fxh file for the General.fx shder to include and consume
	--local acesMatValues = createFile (userDir + "maxHLSL\\AcesMatValues.fxh")
	local newEffectFile = createFile (newEffectFileName) 
	--Output the diffuse color 
	local difAsFloat = acesMatToShade.diffuseRGBA/255
	format "%\n" ("float4 MatDiffuse < string UIName = \"Diffuse\"; string UIWidget = \"Color\"; > = float4( " + difAsFloat.r as string + "f, " + difAsFloat.g as string + "f, " + difAsFloat.b as string + "f, " + difAsFloat.a as string + "f );") to:newEffectFile
	--Output the specular color
	if acesMatToShade.noBaseMatSpec == false then format "%\n" "#define SHD_SPECULAR true;" to:newEffectFile
	local specAsFloat = acesMatToShade.baseSpecColor/255
	format "%\n" ("float4 MatSpecular < string UIName = \"Specular\"; string UIWidget = \"Color\"; > = float4( " + specAsFloat.r as string + "f, " + specAsFloat.g as string + "f, " + specAsFloat.b as string + "f, " + acesMatToShade.baseSpecScale as string + "f );") to:newEffectFile
	
	format "%\n" ("float EnvLevelScale < string UIName = \"Reflection Scale\"; string UIWidget = \"slider\"; float UIMin = 0.0f; float UIMax = 100.0f; > = " + acesMatToShade.envScale as string + ";") to:newEffectFile
	format "%\n" ("float MatSpecMapPowerScale < string UIName = \"Specular Map Power Scale\"; string UIWidget = \"slider\"; float UIMin = 0.0f; float UIMax = 100.0f; > = " + acesMatToShade.specScale as string + ";") to:newEffectFile
	
	--Handle supporting alpha testing if enabled
	if acesMatToShade.useAlphaTest then
	(
		format "%\n" "#define SHD_ALPHA_TEST true;" to:newEffectFile
		format "%\n" ("float AlphaTestThresHold < string UIName = \"Alpha Test Level\"; string UIWidget = \"slider\"; float UIMin = 0.0f; float UIMax = 255.0f; > = " + acesMatToShade.alphaTestThreshold as string + ";") to:newEffectFile
	)
	
	--Define all of the textures that are used and turn on the appropriate flags associated with each one
	
	--#define the Diffuse variable if there is a diffuse texture
	if acesMatToShade.diffuseFile != undefined then (format "%\n" "#define SHD_BASE true;" to:newEffectFile)
	format "%\n" ("Texture2D BaseTexture < string UIName = \"Diffuse Texture\"; >;") to:newEffectFile
	
	--#define the Detail variable if there is a detail texture
	if acesMatToShade.detailFile != undefined then (format "%\n" "#define SHD_DETAIL true;" to:newEffectFile)
	format "%\n" ("Texture2D DetailTexture < string UIName = \"Detail Texture\"; >;") to:newEffectFile
	
	--#define the SpecularMap variable if there is a specular texture
	if acesMatToShade.specFile != undefined then (format "%\n" "#define SHD_SPECULARMAP true;" to:newEffectFile)
	format "%\n" ("Texture2D SpecularTexture < string UIName = \"Specular Texture\"; >;") to:newEffectFile
	
	--#define the Bump variable if there is a bump texture
	if acesMatToShade.bumpFile != undefined then (format "%\n" "#define SHD_BUMP true;" to:newEffectFile)
	format "%\n" ("Texture2D BumpTexture < string UIName = \"Bump Texture\"; >;") to:newEffectFile
	
	--#define the Env variable if there is a environment texture or if globalEnv is checked
	if (acesMatToShade.envFile != undefined) or (acesMatToShade.useGlobalEnv) then (format "%\n" "#define SHD_ENV true;" to:newEffectFile	)
	format "%\n" ("TextureCube EnvTexture < string UIName = \"Environment Texture\"; >;") to:newEffectFile
	
	--#define the Env variable if there is a environment texture or if globalEnv is checked
	if (acesMatToShade.fresnelFile != undefined) then 
	(
		if (acesMatToShade.affectDiffuse) then (format "%\n" "#define SHD_FRESNEL_DIFFUSE true;" to:newEffectFile)
		if (acesMatToShade.affectEnv) then (format "%\n" "#define SHD_FRESNEL_ENV true;" to:newEffectFile)
		if (acesMatToShade.affectSpec) then (format "%\n" "#define SHD_FRESNEL_SPECULAR true;" to:newEffectFile)
	)
	format "%\n" ("Texture2D FresnelRamp < string UIName = \"Fresnel Ramp\"; >;") to:newEffectFile
	
	--emissiveBlend (1:ADDITIVE, 2:DAY_NIGHT_BLEND, 3:MULTIPLY, 4:ADDITIVE_ALWAYS, 5:ADDITIVE_USER_CONTROLLED, 6:DAY_NIGHT_BLEND_USER_CONTROLLED, 7:MULTIPLY_USER_CONTROLLED, 8:ADDITIVE_ALWAYS_USER_CONTROLLED)
	--If we have emissive handle supporting it.
	if (acesMatToShade.emissiveFile != undefined) then
	(
		if(acesMatToShade.emissiveBlend == 1 or acesMatToShade.emissiveBlend == 5) then
		(
			format "%\n" "#define SHD_ADDITIVE_NIGHT_EMISSIVEMAP true;" to:newEffectFile
		)
		
		if(acesMatToShade.emissiveBlend == 2 or acesMatToShade.emissiveBlend == 6) then
		(
			format "%\n" "#define SHD_BLENDED_EMISSIVEMAP true;" to:newEffectFile
		)
		
		if(acesMatToShade.emissiveBlend == 3 or acesMatToShade.emissiveBlend == 7) then
		(
			format "%\n" "#define SHD_MULTIPLY_EMISSIVEMAP true;" to:newEffectFile
		)
		
		if(acesMatToShade.emissiveBlend == 4 or acesMatToShade.emissiveBlend == 8) then
		(
			format "%\n" "#define SHD_ADDITIVE_EMISSIVEMAP true;" to:newEffectFile
		)
	)
	
	--define the Emissive and ShadowMap textures, though it won't be used at all in the shader since we don't have the lighting system in Max
	format "%\n" ("Texture2D EmissiveTexture < string UIName = \"Emissive Texture\"; >;") to:newEffectFile
	format "%\n" ("Texture2D ShadowMap <>;") to:newEffectFile
	
	--Define the rest of the global variables based on the stored material.
	format "%\n" ("float4  MatScales = { " + acesMatToShade.bumpScale as string + ", " + acesMatToShade.detailScale as string + ", " + (acesMatToShade.envScale) as string + ", 0.0 };") to:newEffectFile
	format "%\n" ("float4  MatConstants = { 0.0, " + acesMatToShade.precipOffset as string + ", " + acesMatToShade.specScale as string + ", " + acesMatToShade.specBloomFloor as string + " };") to:newEffectFile
	
	--Now go throught the constants and set them accordingly
	if acesMatToShade.prelit then format "%\n" "#define SHD_PRELIT true;" to:newEffectFile
	if acesMatToShade.blendConstant then format "%\n" "#define SHD_PRELIT_CONSTANT true;" to:newEffectFile 
	if acesMatToShade.envBlend == 1 then format "%\n" "#define SHD_BLENDENVBYINVBASEALPHA true;" to:newEffectFile
	if acesMatToShade.envBlend == 2 then format "%\n" "#define SHD_BLENDENVBYSPECULARMAPALPHA true;" to:newEffectFile
	if acesMatToShade.diffuseBlend == 1 then format "%\n" "#define SHD_BLEND_DIFFUSE_BY_DIFFUSE_ALPHA true;" to:newEffectFile
	if acesMatToShade.diffuseBlend == 2 then format "%\n" "#define SHD_BLEND_DIFFUSE_BY_INV_SPEC_ALPHA true;" to:newEffectFile
	if acesMatToShade.assumeVertNormal then format "%\n" "#define SHD_VERTICAL_NORMAL true;" to:newEffectFile
	if acesMatToShade.noSpecBloom then format "%\n" "#define SHD_NOSPECULARBLOOM true;" to:newEffectFile
	if acesMatToShade.doubleSided then format "%\n" "#define SHD_DOUBLE_SIDED true;" to:newEffectFile
	if acesMatToShade.useFinalAlphaBlend then format "%\n" "#define SHD_FINAL_ALPHA_BLEND true;" to:newEffectFile
	
	local alphaEnabled = "FALSE"
	local depthEnabled = "TRUE"
	
	--Determine if we need alpha enabled or not.
	if acesMatToShade.sourceBlend == 5 and acesMatToShade.destBlend == 6 then
	(
		alphaEnabled = "TRUE"
		depthEnabled = "FALSE"
	)
	
	format "%\n" ("float State_FinalAlphaBlendFactor = " + acesMatToShade.finalAlphaBlend as string + ";") to:newEffectFile
	format "%\n" ("#define SHD_ALPHA_BLEND_ENABLED " + alphaEnabled as string +";") to:newEffectFile
	format "%\n" ("#define SHD_SOURCE_BLEND " + acesMatToShade.sourceBlend as string + ";") to:newEffectFile
	format "%\n" ("#define SHD_DEST_BLEND " + acesMatToShade.destBlend as string + ";") to:newEffectFile
	format "%\n" ("#define SHD_DEPTH_ENABLED " + depthEnabled as string + ";") to:newEffectFile
	
	--Copy the contents of the original effect file into the new effect file so it's at the end.
	--This way the defines and values are where we need them.
	format "%\n" (generalShaderFileText) to:newEffectFile
	
	close newEffectFile
		
	--Now that the include files have been updated, force the update of the timestamp on general.fx so Max will realod the file instead of using the cached version
	--if usingAcesBuild then
	--(
		--DOSCommand ("pushd " + userDir + "maxHLSL\\&call " + FSRoot + "\\public\\ext\\tools\\x86\\touch.exe /f General.fx")
	--)
	
	--Just in case something goes wrong, apply the current material to slot 2 so it can be recovered
	meditMaterials[2] = fnApplyFSXMaterial ()
	--Now setup the new DX material and load the general.fx file
	local thisDXMat = DirectX_9_Shader ()
	thisDXMat.effectFile = newEffectFileName
	
	--Define a function that finds the associated .psd file and then imagetools it and returns the newly created .tga file.
	fn findDXMats matToFind =
	(		
		--First check the local path, then check the build
		if doesFileExist (maxFilePath  + (getFilenameFile matToFind) + ".dds") then
		(
			return (maxFilePath + (getFilenameFile matToFind) + ".dds")
		)
		else
		(
			local dxTexDirs = #()
			if usingAcesBuild then
			(
				getSubDirs (FSRoot + "\\public\\distrib\\") dxTexDirs "*texture*"
				for i = 1 to dxTexDirs.count do
				(
					--Find the texture and convert it to a .dds
					if doesFileExist (dxTexDirs[i] + (getFilenameFile matToFind) + ".dds") then
					(
						return (dxTexDirs[i] + (getFilenameFile matToFind) + ".dds")
					)
				)
			)
			else
			(
				local fsInstallDir = (maxUtils.getRegKey "HKEY_LOCAL_MACHINE" "SOFTWARE\Lockheed Martin\Prepar3D v4" "SetupPath")
				getSubDirs fsInstallDir dxTexDirs "*texture*"
				for i = 1 to dxTexDirs.count do
				(
					--Find the texture and convert it to a .dds
					if doesFileExist (dxTexDirs[i] + (getFilenameFile matToFind) + ".dds") then
					(
						return (dxTexDirs[i] + (getFilenameFile matToFind) + ".dds")
					)
				)
			)
		)
	)
	
	--If there is a texture in the material, find the corresponding .dds file and put it in the appropriate slot
	if acesMatToShade.diffuseFile != undefined then try (thisDXMat.BaseTexture = openBitmap (findDXMats acesMatToShade.diffuseFile)) catch ()
	if acesMatToShade.detailFile != undefined then try (thisDXMat.DetailTexture = openBitmap (findDXMats acesMatToShade.detailFile)) catch ()
	if acesMatToShade.specFile != undefined then try (thisDXMat.SpecularTexture = openBitmap (findDXMats acesMatToShade.specFile)) catch ()
	if acesMatToShade.bumpFile != undefined then try (thisDXMat.BumpTexture = openBitmap (findDXMats acesMatToShade.bumpFile)) catch ()
	if acesMatToShade.envFile != undefined then try (thisDXMat.EnvTexture = openBitmap (findDXMats acesMatToShade.envFile)) catch ()
	if acesMatToShade.useGlobalEnv then try (thisDXMat.EnvTexture = openBitmap (FSArt + "\\texture\\GlobalEnvTest.dds")) catch ()
	if acesMatToShade.fresnelFile != undefined then try (thisDXMat.FresnelRamp = openBitmap (findDXMats acesMatToShade.fresnelFile)) catch ()
	if acesMatToShade.emissiveFile != undefined then try (thisDXMat.EmissiveTexture = openBitmap (findDXMats acesMatToShade.emissiveFile)) catch ()
	
	--Use the omni light that was created
	try (thisDXMat.vPointLightWorldSpace = 1) catch ()
	
	--Now apply the new DX Material to the material in the scene that has been changed.
	local matToReturn
	for i = 1 to sceneMaterials.count do
	(
		if classOf sceneMaterials[i] == multiMaterial then
		(
			for j = 1 to sceneMaterials[i].materialList.count do
			(
				if sceneMaterials[i][j] == sceneMatToApply then 
				(
					sceneMaterials[i][j] = thisDXMat
					matToReturn = sceneMaterials[i][j]
				)
				showTextureMap sceneMaterials[i][j] on
			)
		)
		else
		(
			if sceneMaterials[i] == sceneMatToApply then
			(
				local objList = for obj in $* where obj.material == sceneMatToApply collect obj
				sceneMaterials[i] = thisDXMat
				for thisObj in objList do thisObj.material = sceneMaterials[i]
				showTextureMap sceneMaterials[i] on
				matToReturn = sceneMaterials[i]
			)
		)
	)
	
	--Return the current material so that the original material can be re-applied
	return matToReturn
)