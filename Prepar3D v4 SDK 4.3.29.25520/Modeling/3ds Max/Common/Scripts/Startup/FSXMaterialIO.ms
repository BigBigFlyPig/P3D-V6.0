---This script provides function calls that allow you to load and save XML based materials settings from within Max.

--Define the structure of the material in memory
struct fsxMaterialMemory
(
        matID,                                                                                          --String GUID
        friendlyName,                                                                           --String
        diffuseRGBA = color 128 128 128 255,                            --Color RGBA
        diffuseFile,                                                                            --String filename
        baseSpecColor = color 255 255 255 255,                          --Color RGBA
        baseSpecScale = 40.0,                                                           --Float
        specFile,                                                                                       --String filename
        specScale = 1.0,                                                                        --Float
        hasDetail = false,                                                                      --Bool
        detailFile,                                                                                     --String filename
        detailScale = 1.0,                                                                      --Float
        bumpFile,                                                                                       --String filename
        bumpScale = 1.0,                                                                        --Float
        useGlobalEnv = false,                                                           --Bool
        hasEnvMap = false,                                                                      --Bool
        envFile,                                                                                        --String filename
        envScale = 1.0,                                                                         --Float
        envBlend = 3,                                                                           --(1:BY_INVERSE_DIFFUSE_ALPHA, 2:BY_INVERSE_SPECULAR_ALPHA, 3:NEITHER)
        diffuseBlend = 3,                                                                       --(1:BY_DIFFUSE_ALPHA, 2:BY_SPECULAR_ALPHA, 3:NEITHER)
        hasFresnel = false,                                                                     --Bool
        fresnelFile,                                                                            --String filename
        affectDiffuse = false,                                                          --Bool
        affectEnv = false,                                                                      --Bool
        affectSpec = false,                                                                     --Bool
        emissiveFile,                                                                           --String filename
        emissiveBlend = 1,                                                                      --(1:ADDITIVE, 2:DAY_NIGHT_BLEND, 3:MULTIPLY, 4:ADDITIVE_ALWAYS, 5:ADDITIVE_USER_CONTROLLED, 6:DAY_NIGHT_BLEND_USER_CONTROLLED, 7:MULTIPLY_USER_CONTROLLED, 8:ADDITIVE_ALWAYS_USER_CONTROLLED)
        usePrecip = false,                                                                      --Bool
        usePrecipOffset = false,                                                        --Bool
        precipOffset = 0.0,                                                                     --Float (time)
        useAlphaTest = false,                                                           --Bool
        alphaTestCompare = 1,                                                           --(1:NEVER, 2:LESS, 3:EQUAL, 4:LESS_EQUAL, 5:GREATER, 6:NOT_EQUAL, 7:GREATER_EQUAL, 8:ALWAYS)
        alphaTestThreshold = 0.0,                                                       --Float (0 - 1.0)
        useFinalAlphaBlend = false,                                                     --Bool
        finalAlphaBlend = 1.0,                                                          --Float (0 - 1.0)
        bloomMode = 4,                                                                          --(1:BY_COPY, 2:MODULATE_BY_ALPHA, 3:NONE, 4:NOBLOOM)
        emissiveBloom = false,                                                          --Bool
        noSpecBloom = false,                                                            --Bool
        specBloomFloor = 0.9,                                                           --Float (0 - 1.0)
        ambientLightScale = 1.0,                                                        --Float (0 - 1.0)
        sourceBlend = 2,                                                                        --(1:ZERO, 2:ONE, 3:SOURCE_COLOR, 4:INVERSE_SOURCE_COLOR, 5:SOURCE_ALPHA, 6:INVERSE_SOURCE_ALPHA,
        destBlend = 1,                                                                          --7:DESTINATION_ALPHA, 8:INVERSE_DESTINATION_ALPHA, 9:DESTINATION_COLOR, 10:INVERSE_DESTINATION_COLOR)
        assumeVertNormal = false,                                                       --Bool
        zWriteAlpha = false,                                                            --Bool
        noZWrite = false,                                                                       --Bool
        noShadow = false,                                                                       --Bool
        prelit = false,                                                                         --Bool
        noBaseMatSpec = false,                                                          --Bool
        skinned = false,                                                                        --Bool
        doubleSided = false,                                                            --Bool
        blendConstant = false,                                                          --Bool
        textureAddressMode = 3                                                          --(1:TEXTURE_ADDRESS_CLAMP, 2:TEXTURE_ADDRESS_WRAP, 3:NEITHER)
)

--Make the material global so it can be used in several functions
global fsxMatMem = fsxMaterialMemory ()

--Define the function calls that access either XML or the material in memory
--This function will take an XML blob that contains a Material and serialize that material into memory
fn fnLoadFSXMaterial xmlBlob =
(
        --Define a function that takes a color string value and turns it into RGBA color
        fn fntxtToColor colorString =
        (
                local R = (readDelimitedString colorString ",") as integer
                local G = (readDelimitedString colorString ",") as integer
                local B = (readDelimitedString colorString ",") as integer
                local A = (readLine colorString) as integer
                return (color R G B A)
        )

        --Reset the global fsxMatMem struct
        fsxMatMem = fsxMaterialMemory ()

        --Search for all the information in the XML file and load anything that it finds
        local thisFriendly = xmlBlob.selectNodes("FriendlyName")
        fsxMatMem.friendlyName = (thisFriendly[1].getNodeValue())

        --Drill into the actual material definition now
        local thisMat = xmlBlob.selectNodes("MaterialDefinition")
        local thisMat = thisMat[1]

        try
        (
                local thisNode = thisMat.selectNodes("DiffuseColor")
                try (fsxMatMem.diffuseRGBA = (fnTxtToColor ((thisNode[1].selectNodes("RGBA"))[1].getNodeValue() as stringStream))) catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("DiffuseMap")
                try (fsxMatMem.diffuseFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue()) catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("BaseSpecular")
                try (fsxMatMem.baseSpecColor = fnTxtToColor ((thisNode[1].selectNodes("RGBA"))[1].getNodeValue() as stringStream)) catch ()
                try (fsxMatMem.baseSpecScale = (thisNode[1].selectNodes("Scale"))[1].getNodeValue() as float) catch ()
        ) catch ()

        try (fsxMatMem.noBaseMatSpec = (thisMat.selectNodes("NoBaseMaterialSpecular"))[1].getNodeValue() as booleanClass) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("SpecularMap")
                try (fsxMatMem.specFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue()) catch ()
                try (fsxMatMem.specScale = (thisNode[1].selectNodes("Scale"))[1].getNodeValue() as float) catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("DetailMap")
                try (fsxMatMem.detailFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue(); fsxMatMem.hasDetail = true) catch ()
                try (fsxMatMem.detailScale = (thisNode[1].selectNodes("Scale"))[1].getNodeValue() as float) catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("BumpMap")
                try (fsxMatMem.bumpFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue()) catch ()
                try (fsxMatMem.bumpScale = (thisNode[1].selectNodes("Scale"))[1].getNodeValue() as float) catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("EnvironmentMap")
                if thisNode.count == 0 then
                (
                        fsxMatMem.useGlobalEnv = false
                )
                else
                (
                        try (fsxMatMem.envFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue(); fsxMatMem.hasEnvMap = true; fsxMatMem.useGlobalEnv = false) catch (fsxMatMem.useGlobalEnv = true)
                        try (fsxMatMem.envScale = (thisNode[1].selectNodes("Scale"))[1].getNodeValue() as float) catch ()
                        try
                        (
                                fsxMatMem.envBlend = case of
                                (
                                        (((thisNode[1].selectNodes("EnvironmentBlend"))[1].getNodeValue()) == "BY_INVERSE_DIFFUSE_ALPHA") : 1
                                        (((thisNode[1].selectNodes("EnvironmentBlend"))[1].getNodeValue()) == "BY_INVERSE_SPECULAR_ALPHA") : 2
                                )
                        )
                        catch ()
                )
        ) catch ()

        try
        (
                try (fsxMatMem.diffuseBlend = case of
                        (
                                (((thisMat.selectNodes("DiffuseBlend"))[1].getNodeValue()) == "BY_DIFFUSE_ALPHA") : 1
                                (((thisMat.selectNodes("DiffuseBlend"))[1].getNodeValue()) == "BY_SPECULAR_ALPHA") : 2
                        )
                )
                catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("Fresnel")
                try (fsxMatMem.fresnelFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue(); fsxMatMem.hasFresnel = true) catch ()
                try (fsxMatMem.affectDiffuse = (thisNode[1].selectNodes("AffectDiffuse"))[1].getNodeValue() as booleanClass) catch ()
                try (fsxMatMem.affectEnv = (thisNode[1].selectNodes("AffectEnvironment"))[1].getNodeValue() as booleanClass) catch ()
                try (fsxMatMem.affectSpec = (thisNode[1].selectNodes("AffectSpecular"))[1].getNodeValue() as booleanClass) catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("EmissiveMap")
                try (fsxMatMem.emissiveFile = (thisNode[1].selectNodes("Filename"))[1].getNodeValue()) catch ()
                try
                (
                        fsxMatMem.emissiveBlend = case of
                        (
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "ADDITIVE") : 1
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "DAY_NIGHT_BLEND") : 2
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "MULTIPLY") : 3
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "ADDITIVE_ALWAYS") : 4
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "ADDITIVE_USER_CONTROLLED") : 5
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "DAY_NIGHT_BLEND_USER_CONTROLLED") : 6
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "MULTIPLY_USER_CONTROLLED") : 7
                                (((thisNode[1].selectNodes("EmissiveBlend"))[1].getNodeValue()) == "ADDITIVE_ALWAYS_USER_CONTROLLED") : 8
                        )
                )
                catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("PrecipitationEffect")
                try (fsxMatMem.precipOffset = (thisNode[1].selectNodes("PrecipitationEffectOffset"))[1].getNodeValue() as float; fsxMatMem.usePrecip = true; fsxMatMem.usePrecipOffset = true) catch ()
        ) catch ()

        local thisNode
        try (thisNode = thisMat.selectNodes("AlphaTestZWrite")) catch (thisNode = #())
        if thisNode.count == 1 then
        (
                fsxMatMem.useAlphaTest = true
                try (fsxMatMem.alphaTestThreshold = ((thisNode[1].selectNodes("Threshold"))[1].getNodeValue() as float) * 255.00) catch ()
                try
                (
                        fsxMatMem.alphaTestCompare = case of
                        (
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "NEVER") : 1
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "LESS") : 2
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "EQUAL") : 3
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "LESS_EQUAL") : 4
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "GREATER") : 5
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "NOT_EQUAL") : 6
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "GREATER_EQUAL") : 7
                                (((thisNode[1].selectNodes("CompareFunction"))[1].getNodeValue()) == "ALWAYS") : 8
                        )
                )
                catch ()
        )

        try (fsxMatMem.finalAlphaBlend = (thisMat.selectNodes("FinalAlphaBlend"))[1].getNodeValue() as float; fsxMatMem.useFinalAlphaBlend = true) catch ()

        try (fsxMatMem.emissiveBloom = (thisMat.selectNodes("EmissiveBloom"))[1].getNodeValue() as booleanClass) catch ()
        try (fsxMatMem.noSpecBloom = (thisMat.selectNodes("NoSpecularBloom"))[1].getNodeValue() as booleanClass) catch ()
        try (fsxMatMem.specBloomFloor = (thisMat.selectNodes("SpecularBloomFloor"))[1].getNodeValue() as float) catch ()
        try
        (
                fsxMatMem.bloomMode = case of
                (
                        (((thisMat.selectNodes("BloomMode"))[1].getNodeValue()) == "BY_COPY") : 1
                        (((thisMat.selectNodes("BloomMode"))[1].getNodeValue()) == "MODULATE_BY_ALPHA") : 2
                        (((thisMat.selectNodes("BloomMode"))[1].getNodeValue()) == "NONE") : 3
                )
        )
        catch
        (
                fsxMatMem.bloomMode = 4
        )

        try (fsxMatMem.ambientLightScale = (thisMat.selectNodes("AmbientLightScale"))[1].getNodeValue() as float) catch ()

        --Define a function that returns the blend mode based on the string value
        fn fnGetBlendMode thisNode =
        (
                return case of
                (
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "ZERO") : 1
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "ONE") : 2
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "SOURCE_COLOR") : 3
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "INVERSE_SOURCE_COLOR") : 4
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "SOURCE_ALPHA") : 5
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "INVERSE_SOURCE_ALPHA") : 6
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "DESTINATION_ALPHA") : 7
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "INVERSE_DESTINATION_ALPHA") : 8
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "DESTINATION_COLOR") : 9
                        (((thisNode[1].selectNodes("BlendMode"))[1].getNodeValue()) == "INVERSE_DESTINATION_COLOR") : 10
                )
        )

        try
        (
                local thisNode = thisMat.selectNodes("SourceBlend")
                try
                (
                        fsxMatMem.sourceBlend = fnGetBlendMode thisNode
                )
                catch ()
        ) catch ()

        try
        (
                local thisNode = thisMat.selectNodes("DestinationBlend")
                try
                (
                        fsxMatMem.destBlend = fnGetBlendMode thisNode
                )
                catch ()
        ) catch ()

        try (fsxMatMem.assumeVertNormal = (thisMat.selectNodes("AssumeVerticalNormal"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.zWriteAlpha = (thisMat.selectNodes("ZWriteAlpha"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.noZWrite = (thisMat.selectNodes("NoZWrite"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.noShadow = (thisMat.selectNodes("NoShadow"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.prelit = (thisMat.selectNodes("Prelit"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.noBaseMatSpec = (thisMat.selectNodes(""))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.skinned = (thisMat.selectNodes("Skinned"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.doubleSided = (thisMat.selectNodes("DoubleSided"))[1].getNodeValue() as booleanClass) catch ()

        try (fsxMatMem.blendConstant = (thisMat.selectNodes("BlendConstant"))[1].getNodeValue() as booleanClass) catch ()

        try
        (
                try (fsxMatMem.textureAddressMode = case of
                        (
                                (((thisMat.selectNodes("TextureAddressMode"))[1].getNodeValue()) == "CLAMP") : 1
                                (((thisMat.selectNodes("TextureAddressMode"))[1].getNodeValue()) == "WRAP") : 2
                        )
                )
                catch ()
        ) catch ()

        return fsxMatMem
)

--This function will serialize a material from the given material slot into memory
fn fnSerializeFSXMaterial thisFSXMaterial =
(
        --Reinitialize the struct to clear it out and give it the friendly name of the material
        fsxMatMem = fsxMaterialMemory ()
        fsxMatMem.friendlyName = thisFSXMaterial.name

        --If the Material is an Prepar3D Material, serialize it and fill in all values
        if classOf thisFSXMaterial == P3DMaterial then
        (
                fsxMatMem.diffuseRGBA = (color (thisFSXMaterial.standard.diffuseColor.r as integer) (thisFSXMaterial.standard.diffuseColor.g as integer) (thisFSXMaterial.standard.diffuseColor.b as integer) ((thisFSXMaterial.standard.opacity * 2.55) as integer))
                try (fsxMatMem.diffuseFile = thisFSXMaterial.standard.diffuseMap.fileName) catch ()

                fsxMatMem.baseSpecColor = thisFSXMaterial.standard.specularColor
                fsxMatMem.baseSpecScale = thisFSXMaterial.standard.specularLevel
                try (fsxMatMem.specFile = thisFSXMaterial.standard.specularMap.fileName) catch ()
                fsxMatMem.specScale = thisFSXMaterial.fSpecMapPowerScale

                try (fsxMatMem.detailFile = thisFSXMaterial.texDetailMap.filename; fsxMatMem.hasDetail = true) catch ()
                fsxMatMem.useGlobalEnv = thisFSXMaterial.bUseGlobalEnv
                fsxMatMem.detailScale = thisFSXMaterial.iDetailScale

                try (fsxMatMem.bumpFile = thisFSXMaterial.standard.bumpMap.fileName) catch ()
                fsxMatMem.bumpScale = thisFSXMaterial.iBumpScale

                try (fsxMatMem.envFile = thisFSXMaterial.standard.reflectionMap.fileName; fsxMatMem.hasEnvMap = true) catch (fsxMatMem.hasEnvMap = false)
                fsxMatMem.envScale = thisFSXMaterial.iEnvLevelScale

                if thisFSXMaterial.bBlendInvBaseAlpha == true then fsxMatMem.envBlend = 1
                if thisFSXMaterial.bBlendInvSpecAlpha == true then fsxMatMem.envBlend = 2

                if thisFSXMaterial.bBlendDiffuseBaseAlpha == true then fsxMatMem.diffuseBlend = 1
                if thisFSXMaterial.bBlendDiffuseInvSpecAlpha == true then fsxMatMem.diffuseBlend = 2

                try (fsxMatMem.fresnelFile = thisFSXMaterial.texFresnelRamp.filename; fsxMatMem.hasFresnel = true) catch ()

                fsxMatMem.affectDiffuse = thisFSXMaterial.bFresnelDiffuse
                fsxMatMem.affectEnv = thisFSXMaterial.bFresnelReflection
                fsxMatMem.affectSpec = thisFSXMaterial.bFresnelSpecular

                try (fsxMatMem.emissiveFile = thisFSXMaterial.standard.selfIllumMap.fileName) catch ()
                fsxMatMem.emissiveBlend = thisFSXMaterial.iEmissiveMode

                fsxMatMem.usePrecip = thisFSXMaterial.bUsePrecip
                fsxMatMem.usePrecipOffset = thisFSXMaterial.bUsePrecipOffset
                fsxMatMem.precipOffset = thisFSXMaterial.fPrecipOffset

                fsxMatMem.useAlphaTest = thisFSXMaterial.bZTestAlpha
                fsxMatMem.alphaTestCompare = thisFSXMaterial.iAlphaTestMode
                fsxMatMem.alphaTestThreshold = thisFSXMaterial.fAlphaTestLevel

                fsxMatMem.useFinalAlphaBlend = thisFSXMaterial.bFinalAlphaWrite
                fsxMatMem.finalAlphaBlend = thisFSXMaterial.fFinalAlphaWriteValue

                if (thisFSXMaterial.bAllowBloom == true and thisFSXMaterial.bBloomCopy == true) then fsxMatMem.bloomMode = 1
                if (thisFSXMaterial.bAllowBloom == true and thisFSXMaterial.bBloomModulate == true) then fsxMatMem.bloomMode = 2
                if (thisFSXMaterial.bAllowBloom == true and thisFSXMaterial.bBloomModulate == false and thisFSXMaterial.bBloomCopy == false) then fsxMatMem.bloomMode = 3
                if (thisFSXMaterial.bAllowBloom == false) then fsxMatMem.bloomMode = 4

                fsxMatMem.emissiveBloom = thisFSXMaterial.bAllowEmissiveBloom

                fsxMatMem.noSpecBloom = thisFSXMaterial.bNoSpecularBloom

                fsxMatMem.specBloomFloor = thisFSXMaterial.fSpecularBloomFloor

                fsxMatMem.ambientLightScale = thisFSXMaterial.fAmbientLightScale

                fsxMatMem.sourceBlend = thisFSXMaterial.iSrcBlend

                fsxMatMem.destBlend = thisFSXMaterial.iDstBlend

                fsxMatMem.assumeVertNormal = thisFSXMaterial.bAssumeVerticalNormal

                fsxMatMem.zWriteAlpha = thisFSXMaterial.bZWriteAlpha

                fsxMatMem.noZWrite = thisFSXMaterial.bNoZWrite

                fsxMatMem.noShadow = thisFSXMaterial.bNoShadow

                fsxMatMem.prelit = thisFSXMaterial.bPrelitVertices

                fsxMatMem.noBaseMatSpec = thisFSXMaterial.bNoBaseMaterialSpecular

                fsxMatMem.skinned = thisFSXMaterial.bSkinned

                fsxMatMem.doubleSided = thisFSXMaterial.bDoubleSided

                fsxMatMem.blendConstant = thisFSXMaterial.bBlendConstant

                if thisFSXMaterial.bForceTextureAddressClamp == true then fsxMatMem.textureAddressMode = 1
                if thisFSXMaterial.bForceTextureAddressWrap == true then fsxMatMem.textureAddressMode = 2
        )

        --If it is a standard material, return all of the pertinent information in the standard material
        if classOf thisFSXMaterial == standardMaterial then
        (
                fsxMatMem.diffuseRGBA = (color (thisFSXMaterial.diffuseColor.r as integer) (thisFSXMaterial.diffuseColor.g as integer) (thisFSXMaterial.diffuseColor.b as integer) ((thisFSXMaterial.opacity * 2.55) as integer))
                try (fsxMatMem.diffuseFile = thisFSXMaterial.diffuseMap.fileName) catch ()

                fsxMatMem.baseSpecColor = thisFSXMaterial.specularColor
                fsxMatMem.baseSpecScale = thisFSXMaterial.specularLevel
                try (fsxMatMem.specFile = thisFSXMaterial.specularMap.fileName) catch ()

                try (fsxMatMem.detailFile = thisFSXMaterial.texDetailMap.filename; fsxMatMem.hasDetail = true) catch ()

                try (fsxMatMem.bumpFile = thisFSXMaterial.bumpMap.fileName) catch ()

                try (fsxMatMem.envFile = thisFSXMaterial.reflectionMap.fileName) catch (fsxMatMem.hasEnvMap = false)

                try (fsxMatMem.emissiveFile = thisFSXMaterial.selfIllumMap.fileName) catch ()
        )

        return fsxMatMem
)

--This function will apply a material in memory to the given material slot
fn fnApplyFSXMaterial =
(
        --If the materials is not an Prepar3D Material, make it so
        thisFSXMaterial = P3DMaterial ()

        thisFSXMaterial.name = fsxMatMem.friendlyName

        thisFSXMaterial.standard.diffuse = fsxMatMem.diffuseRGBA
        thisFSXMaterial.standard.opacity = (fsxMatMem.diffuseRGBA.a / 2.55)
        local thisDiffuseFile = fsxMatMem.diffuseFile
        if thisDiffuseFile != undefined then thisFSXMaterial.standard.diffuseMap = bitmapTexture filename:fsxMatMem.diffuseFile

        thisFSXMaterial.standard.specularColor = fsxMatMem.baseSpecColor
        thisFSXMaterial.standard.specularLevel = fsxMatMem.baseSpecScale
        local thisSpecFile = fsxMatMem.specFile
        if thisSpecFile != undefined then thisFSXMaterial.standard.specularMap = bitmapTexture filename:fsxMatMem.specFile
        thisFSXMaterial.fSpecMapPowerScale = fsxMatMem.specScale

        local thisDetailFile = fsxMatMem.detailFile
        if thisDetailFile != undefined then (thisFSXMaterial.bHasDetailMap = true; thisFSXMaterial.texDetailMap = bitmapTexture filename:fsxMatMem.detailFile)
        thisFSXMaterial.iDetailScale = fsxMatMem.detailScale

        local thisBumpFile = fsxMatMem.bumpFile
        if thisBumpFile != undefined then thisFSXMaterial.standard.bumpMap = bitmapTexture filename:fsxMatMem.bumpFile
        thisFSXMaterial.iBumpScale = fsxMatMem.bumpScale

        local thisEnvFile = fsxMatMem.envFile
        if thisEnvFile != undefined then thisFSXMaterial.standard.reflectionMap = bitmapTexture  filename:fsxMatMem.envFile else thisFSXMaterial.bUseGlobalEnv = fsxMatMem.useGlobalEnv
        thisFSXMaterial.bUseGlobalEnv = fsxMatMem.useGlobalEnv
        thisFSXMaterial.iEnvLevelScale = fsxMatMem.envScale

        if fsxMatMem.envBlend == 1 then (thisFSXMaterial.bBlendInvBaseAlpha = true; thisFSXMaterial.bBlendInvSpecAlpha = false)
        if fsxMatMem.envBlend == 2 then (thisFSXMaterial.bBlendInvSpecAlpha = true; thisFSXMaterial.bBlendInvBaseAlpha = false)
        if fsxMatMem.envBlend == 3 then (thisFSXMaterial.bBlendInvBaseAlpha = false; thisFSXMaterial.bBlendInvSpecAlpha = false)

        if fsxMatMem.diffuseBlend == 1 then (thisFSXMaterial.bBlendDiffuseBaseAlpha = true; thisFSXMaterial.bBlendDiffuseInvSpecAlpha = false)
        if fsxMatMem.diffuseBlend == 2 then (thisFSXMaterial.bBlendDiffuseInvSpecAlpha = true; thisFSXMaterial.bBlendDiffuseBaseAlpha = false)
        if fsxMatMem.diffuseBlend == 3 then (thisFSXMaterial.bBlendDiffuseBaseAlpha = false; thisFSXMaterial.bBlendDiffuseInvSpecAlpha = false)

        local thisFresnelFile = fsxMatMem.fresnelFile
        if thisFresnelFile != undefined then (thisFSXMaterial.bHasFresnelRamp = true; thisFSXMaterial.texFresnelRamp = bitmapTexture filename:fsxMatMem.fresnelFile)

        thisFSXMaterial.bFresnelDiffuse = fsxMatMem.affectDiffuse
        thisFSXMaterial.bFresnelReflection = fsxMatMem.affectEnv
        thisFSXMaterial.bFresnelSpecular = fsxMatMem.affectSpec

        local thisEmmissiveFile = fsxMatMem.emissiveFile
        if thisEmmissiveFile != undefined then thisFSXMaterial.standard.selfIllumMap = bitmapTexture filename:fsxMatMem.emissiveFile

        thisFSXMaterial.iEmissiveMode = fsxMatMem.emissiveBlend

        thisFSXMaterial.bUsePrecip = fsxMatMem.usePrecip
        if fsxMatMem.usePrecipOffset == true then (thisFSXMaterial.bUsePrecipOffset = true; thisFSXMaterial.fPrecipOffset = fsxMatMem.precipOffset)

        thisFSXMaterial.bZTestAlpha = fsxMatMem.useAlphaTest
        thisFSXMaterial.iAlphaTestMode = fsxMatMem.alphaTestCompare
        thisFSXMaterial.fAlphaTestLevel = fsxMatMem.alphaTestThreshold

        thisFSXMaterial.bFinalAlphaWrite = fsxMatMem.useFinalAlphaBlend
        thisFSXMaterial.fFinalAlphaWriteValue = fsxMatMem.finalAlphaBlend

        if fsxMatMem.bloomMode == 1 then (thisFSXMaterial.bAllowBloom = true; thisFSXMaterial.bBloomCopy = true; thisFSXMaterial.bBloomModulate = false)
        if fsxMatMem.bloomMode == 2 then (thisFSXMaterial.bAllowBloom = true; thisFSXMaterial.bBloomModulate = true; thisFSXMaterial.bBloomCopy = false)
        if fsxMatMem.bloomMode == 3 then (thisFSXMaterial.bAllowBloom = true; thisFSXMaterial.bBloomCopy = false; thisFSXMaterial.bBloomModulate = false)
        if fsxMatMem.bloomMode == 4 then (thisFSXMaterial.bAllowBloom = false; thisFSXMaterial.bBloomCopy = false; thisFSXMaterial.bBloomModulate = false)

        thisFSXMaterial.bAllowEmissiveBloom = fsxMatMem.emissiveBloom

        thisFSXMaterial.bNoSpecularBloom = fsxMatMem.noSpecBloom

        thisFSXMaterial.fSpecularBloomFloor = fsxMatMem.specBloomFloor

        thisFSXMaterial.fAmbientLightScale = fsxMatMem.ambientLightScale

        thisFSXMaterial.iSrcBlend = fsxMatMem.sourceBlend

        thisFSXMaterial.iDstBlend = fsxMatMem.destBlend

        thisFSXMaterial.bAssumeVerticalNormal = fsxMatMem.assumeVertNormal

        thisFSXMaterial.bZWriteAlpha = fsxMatMem.zWriteAlpha

        thisFSXMaterial.bNoZWrite = fsxMatMem.noZWrite

        thisFSXMaterial.bNoShadow = fsxMatMem.noShadow

        thisFSXMaterial.bPrelitVertices = fsxMatMem.prelit

        thisFSXMaterial.bNoBaseMaterialSpecular = fsxMatMem.noBaseMatSpec

        thisFSXMaterial.bSkinned = fsxMatMem.skinned

        thisFSXMaterial.bDoubleSided = fsxMatMem.doubleSided

        thisFSXMaterial.bBlendConstant = fsxMatMem.blendConstant

        if fsxMatMem.textureAddressMode == 1 then (thisFSXMaterial.bForceTextureAddressClamp = true; thisFSXMaterial.bForceTextureAddressWrap = false)
        if fsxMatMem.textureAddressMode == 2 then (thisFSXMaterial.bForceTextureAddressWrap = true; thisFSXMaterial.bForceTextureAddressClamp = false)
        if fsxMatMem.textureAddressMode == 3 then (thisFSXMaterial.bForceTextureAddressClamp = false; thisFSXMaterial.bForceTextureAddressWrap = false)

        --Put the material created into the first slot
        return thisFSXMaterial
)

--This function will save a material from memory to the given XML location
fn fnSaveFSXMaterial thisFSXMaterial thisMaxXML xmlBlob matRootnode thisXMlFile doesExist =
(
        --Load the material into memory so it can be written out quickly.
        fnSerializeFSXMaterial thisFSXMaterial

        --Create a function that adds a single value to the XML file
        fn fnAddMatNode xmlBlob nodeName thisMaxXML =
        (
                local nodeAdd = thisMaxXML.createNode nodeName
                xmlBlob.addChild nodeAdd
        )

        --Create a function that adds a text node to a node or to the XML file
        fn fnAddMatValue xmlBlob matValue nodeName thisMaxXML =
        (
                local nodeAdd = thisMaxXML.createNode nodeName
                nodeAdd.addChild (thisMaxXML.createTextNode matValue)
                xmlBlob.addChild nodeAdd
        )

        --If you are not updating an existing node in the XML file, create the new. give it a GUID node and add it to the XML file.
        if doesExist != true then
        (
                local newGUID = allCaps (maxUtils.getGUID ())
                local nodeAdd = thisMaxXML.createNode "Material"
                local thisModelGUID = thisMaxXML.createAttribute "id" (newGUID)
                nodeAdd.addAttribute(thisModelGUID)
                matRootnode.addChild nodeAdd
                xmlBlob = matRootnode.selectSingleNode("Material[@id=\"" + newGUID + "\"]")
        )
        else
        (
                --if the node exists, first make sure the GUID is all caps, then remove it's friendlyName and Material definition from the file.
                local thisGUID = xmlBlob.getAttributeValue("id")
                thisGUID = allcaps thisGUID
                local thisAttr = xmlBlob.getAttributes()
                xmlBlob.removeAttribute(thisAttr[1])
                local thisGUIDAdd = thisMaxXML.createAttribute "id" (thisGUID)
                xmlBlob.addAttribute(thisGUIDAdd)
                try (xmlBlob.removeChild((xmlBlob.getChildNodes())[2])) catch ()
                try (xmlBlob.removeChild((xmlBlob.getChildNodes())[1])) catch ()
        )

        --Using the functions above, add all of the properties for this Material entry
        fnAddMatValue xmlBlob fsxMatMem.friendlyName "FriendlyName" thisMaxXML
        fnAddMatNode xmlBlob "MaterialDefinition" thisMaxXML
        local matDef = ((xmlBlob.selectNodes("MaterialDefinition"))[1])
        fnAddMatNode matDef "DiffuseColor" thisMaxXML
        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/DiffuseColor"))[1]) ((fsxMatMem.diffuseRGBA.r as integer) as string + "," + (fsxMatMem.diffuseRGBA.g as integer) as string + "," + (fsxMatMem.diffuseRGBA.b as integer) as string + "," + (fsxMatMem.diffuseRGBA.a as integer) as string) "RGBA" thisMaxXML
        if fsxMatMem.diffuseFile != undefined then
        (
                fnAddMatNode matDef "DiffuseMap" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/DiffuseMap"))[1]) ((getFilenameFile fsxMatMem.diffuseFile) + ".dds") "Filename" thisMaxXML
        )
        fnAddMatNode matDef "BaseSpecular" thisMaxXML
        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/BaseSpecular"))[1]) ((fsxMatMem.baseSpecColor.r as integer) as string + "," + (fsxMatMem.baseSpecColor.g as integer) as string + "," + (fsxMatMem.baseSpecColor.b as integer) as string + "," + (fsxMatMem.baseSpecColor.a as integer) as string) "RGBA" thisMaxXML
        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/BaseSpecular"))[1]) (fsxMatMem.baseSpecScale as string) "Scale" thisMaxXML
        if fsxMatMem.noBaseMatSpec == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.noBaseMatSpec as string)) "NoBaseMaterialSpecular" thisMaxXML)
        if fsxMatMem.specFile != undefined then
        (
                fnAddMatNode matDef "SpecularMap" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/SpecularMap"))[1]) ((getFilenameFile fsxMatMem.specFile) + ".dds") "Filename" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/SpecularMap"))[1]) (fsxMatMem.specScale as string) "Scale" thisMaxXML
        )
        if fsxMatMem.hasDetail == true then
        (
                fnAddMatNode matDef "DetailMap" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/DetailMap"))[1]) ((getFilenameFile fsxMatMem.detailFile) + ".dds") "Filename" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/DetailMap"))[1]) (fsxMatMem.detailScale as string) "Scale" thisMaxXML
        )
        if fsxMatMem.bumpFile != undefined then
        (
                fnAddMatNode matDef "BumpMap" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/BumpMap"))[1]) ((getFilenameFile fsxMatMem.bumpFile) + ".dds") "Filename" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/BumpMap"))[1]) (fsxMatMem.bumpScale as string) "Scale" thisMaxXML
        )

        if fsxMatMem.useGlobalEnv or fsxMatMem.hasEnvMap then
        (
                fnAddMatNode matDef "EnvironmentMap" thisMaxXML
                if fsxMatMem.hasEnvMap == true then
                (
                        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/EnvironmentMap"))[1]) ((getFilenameFile fsxMatMem.envFile) + ".dds") "Filename" thisMaxXML
                )
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/EnvironmentMap"))[1]) (fsxMatMem.envScale as string) "Scale" thisMaxXML
                local envBlendText = case of
                (
                        (fsxMatMem.envBlend == 1) : "BY_INVERSE_DIFFUSE_ALPHA"
                        (fsxMatMem.envBlend == 2) : "BY_INVERSE_SPECULAR_ALPHA"
                        default : "NEITHER"
                )
                if (envBlendText != "NEITHER") and (envBlendText != undefined) then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/EnvironmentMap"))[1]) envBlendText "EnvironmentBlend" thisMaxXML)
        )

        local diffBlendText = case of
        (
                (fsxMatMem.diffuseBlend == 1) : "BY_DIFFUSE_ALPHA"
                (fsxMatMem.diffuseBlend == 2) : "BY_SPECULAR_ALPHA"
                default : "NEITHER"
        )
        if (diffBlendText != "NEITHER") and (diffBlendText != undefined) then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) diffBlendText "DiffuseBlend" thisMaxXML)
        if fsxMatMem.hasFresnel == true then
        (
                fnAddMatNode matDef "Fresnel" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/Fresnel"))[1]) ((getFilenameFile fsxMatMem.fresnelFile) + ".dds") "Filename" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/Fresnel"))[1]) (allcaps (fsxMatMem.affectDiffuse as string)) "AffectDiffuse" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/Fresnel"))[1]) (allcaps (fsxMatMem.affectEnv as string)) "AffectEnvironment" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/Fresnel"))[1]) (allcaps (fsxMatMem.affectSpec as string)) "AffectSpecular" thisMaxXML
        )
        if fsxMatMem.emissiveFile != undefined then
        (
                fnAddMatNode matDef "EmissiveMap" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/EmissiveMap"))[1]) ((getFilenameFile fsxMatMem.emissiveFile) + ".dds") "Filename" thisMaxXML
                local emissiveBlendText = case of
                (
                        (fsxMatMem.emissiveBlend == 1) : "ADDITIVE"
                        (fsxMatMem.emissiveBlend == 2) : "DAY_NIGHT_BLEND"
                        (fsxMatMem.emissiveBlend == 3) : "MULTIPLY"
                        (fsxMatMem.emissiveBlend == 4) : "ADDITIVE_ALWAYS"
                        (fsxMatMem.emissiveBlend == 5) : "ADDITIVE_USER_CONTROLLED"
                        (fsxMatMem.emissiveBlend == 6) : "DAY_NIGHT_BLEND_USER_CONTROLLED"
                        (fsxMatMem.emissiveBlend == 7) : "MULTIPLY_USER_CONTROLLED"
                        (fsxMatMem.emissiveBlend == 8) : "ADDITIVE_ALWAYS_USER_CONTROLLED"
                )
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/EmissiveMap"))[1]) emissiveBlendText "EmissiveBlend" thisMaxXML
        )
        if fsxMatMem.usePrecip == true then
        (
                fnAddMatNode matDef "PrecipitationEffect" thisMaxXML
                if fsxMatMem.usePrecipOffset == true then
                (
                        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/PrecipitationEffect"))[1]) (fsxMatMem.precipOffset as string) "PrecipitationEffectOffset" thisMaxXML
                )
        )
        if fsxMatMem.useAlphaTest == true then
        (
                fnAddMatNode matDef "AlphaTestZWrite" thisMaxXML
                local alphaCompareText = case of
                (
                        (fsxMatMem.alphaTestCompare == 1) : "NEVER"
                        (fsxMatMem.alphaTestCompare == 2) : "LESS"
                        (fsxMatMem.alphaTestCompare == 3) : "EQUAL"
                        (fsxMatMem.alphaTestCompare == 4) : "LESS_EQUAL"
                        (fsxMatMem.alphaTestCompare == 5) : "GREATER"
                        (fsxMatMem.alphaTestCompare == 6) : "NOT_EQUAL"
                        (fsxMatMem.alphaTestCompare == 7) : "GREATER_EQUAL"
                        (fsxMatMem.alphaTestCompare == 8) : "ALWAYS"
                )
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/AlphaTestZWrite"))[1]) alphaCompareText "CompareFunction" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/AlphaTestZWrite"))[1]) (((fsxMatMem.alphaTestThreshold) / 255.00) as string) "Threshold" thisMaxXML
        )
        if fsxMatMem.useFinalAlphaBlend == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.finalAlphaBlend as string)) "FinalAlphaBlend" thisMaxXML)

        local bloomModeText = case of
        (
                (fsxMatMem.bloomMode == 1) : "BY_COPY"
                (fsxMatMem.bloomMode == 2) : "MODULATE_BY_ALPHA"
                (fsxMatMem.bloomMode == 3) : "NONE"
        )
        if (bloomModeText != undefined) then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) bloomModeText "BloomMode" thisMaxXML)
        if fsxMatMem.emissiveBloom == true then fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.emissiveBloom as string)) "EmissiveBloom" thisMaxXML
        if fsxMatMem.noSpecBloom == true then fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.noSpecBloom as string)) "NoSpecularBloom" thisMaxXML
        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.specBloomFloor as string)) "SpecularBloomFloor" thisMaxXML

        fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (fsxMatMem.ambientLightScale as string) "AmbientLightScale" thisMaxXML
        fn intToBlendMode int =
        (
                return case of
                (
                        (int == 1) : "ZERO"
                        (int == 2) : "ONE"
                        (int == 3) : "SOURCE_COLOR"
                        (int == 4) : "INVERSE_SOURCE_COLOR"
                        (int == 5) : "SOURCE_ALPHA"
                        (int == 6) : "INVERSE_SOURCE_ALPHA"
                        (int == 7) : "DESTINATION_ALPHA"
                        (int == 8) : "INVERSE_DESTINATION_ALPHA"
                        (int == 9) : "DESTINATION_COLOR"
                        (int == 10) : "INVERSE_DESTINATION_COLOR"
                )
        )
        if fsxMatMem.sourceBlend != undefined then
        (
                fnAddMatNode matDef "SourceBlend" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/SourceBlend"))[1]) (intToBlendMode (fsxMatMem.sourceBlend)) "BlendMode" thisMaxXML
        )
        if fsxMatMem.destBlend != undefined then
        (
                fnAddMatNode matDef "DestinationBlend" thisMaxXML
                fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition/DestinationBlend"))[1]) (intToBlendMode (fsxMatMem.destBlend)) "BlendMode" thisMaxXML
        )
        if fsxMatMem.assumeVertNormal == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.assumeVertNormal as string)) "AssumeVerticalNormal" thisMaxXML)
        if fsxMatMem.zWriteAlpha == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.zWriteAlpha as string)) "ZWriteAlpha" thisMaxXML)
        if fsxMatMem.noZWrite == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.noZWrite as string)) "NoZWrite" thisMaxXML)
        if fsxMatMem.noShadow == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.noShadow as string)) "NoShadow" thisMaxXML)
        if fsxMatMem.prelit == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.prelit as string)) "Prelit" thisMaxXML)
        if fsxMatMem.skinned == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.skinned as string)) "Skinned" thisMaxXML)
        if fsxMatMem.doubleSided == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.doubleSided as string)) "DoubleSided" thisMaxXML)
        if fsxMatMem.blendConstant == true then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) (allCaps (fsxMatMem.blendConstant as string)) "BlendConstant" thisMaxXML)
        local texAddressMode = case of
        (
                (fsxMatMem.textureAddressMode == 1) : "CLAMP"
                (fsxMatMem.textureAddressMode == 2) : "WRAP"
                default : "NEITHER"
        )
        if texAddressMode == "CLAMP" then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) texAddressMode "TextureAddressMode" thisMaxXML)
        if texAddressMode == "WRAP" then (fnAddMatValue ((xmlBlob.selectNodes("MaterialDefinition"))[1]) texAddressMode "TextureAddressMode" thisMaxXML)

        --Save the changes
        thisMaxXML.saveDocument(thisXMlFile)
)

--This function will compare two materials in memory and determine if they are similar or different
fn fnCompareMaterials mat1 mat2 ignoreList =
(
        --Load both materials into memory
        local mat1Mem = fnSerializeFSXMaterial Mat1
        local mat2Mem = fnSerializeFSXMaterial Mat2

        if mat1Mem != false and mat2Mem != false then
        (
                local matchArray = #(), notMatching = false
                for i = 1 to (getPropNames mat1Mem).count do
                (
                        for j = 1 to ignoreList.count do
                        (
                                if (getPropNames mat1Mem)[i] != ignoreList[j] then
                                (
                                        if (getProperty mat1Mem (getPropNames mat1Mem)[i]) != (getProperty mat2Mem (getPropNames mat2Mem)[i]) then
                                        (
                                                append matchArray (((getPropNames mat1Mem)[i] as string) + " : " + (getProperty mat1Mem (getPropNames mat1Mem)[i] as string) + " != " + (getProperty mat2Mem (getPropNames mat2Mem)[i] as string))
                                                notMatching = true
                                        )
                                )
                        )
                )

                --Compare them both and return based on the results
                if notMatching then return matchArray else return true
        )
        else return "One of your materials is not an Prepar3D Material!"
)

--Add a function that will return the Materials.xml file and the material section of it.
fn fnReturnMatXML tempXML =
(
        tempXML.loadDocument(FSRoot + "\\tools\\simProp\\Autogen\\Materials.xml")
        tempRoot = tempXML.getRootNode()
        rootChilds = tempRoot.getChildNodes()
        local materialsRoot = rootChilds[2].selectNodes("Materials.Materials/Material")
        local matRootNode = rootChilds[2].selectNodes("Materials.Materials")
        return #(materialsRoot,matRootNode[1])
)


--AND HERE"S SOME SAMPLES OF HOW TO USE THESE VARIOUS FUNCTIONS::::::

--This loads material slot 2 into memory, applies it to material slot 1 and then compares the two materials
--fnSerializeFSXMaterial meditMaterials[2]
--meditMaterials[1] = fnApplyFSXMaterial ()
--print ("Compare = " + fnCompareMaterials meditMaterials[2] meditMaterials[1] as string)

/*tempXML = maxXML ()
tempXML.loadDocument(FSRoot + "\\tools\\simprop\\Autogen\\Materials.xml")
tempRoot = tempXML.getRootNode()
rootChilds = tempRoot.getChildNodes()
materialsRoot = rootChilds[2].selectNodes("Materials.Materials/Material")
thisMaterial = materialsRoot[3]
fnLoadFSXMaterial thisMaterial
meditMaterials[1] = fnApplyFSXMaterial ()

fnSaveFSXMaterial meditMaterials[1] tempXML (rootChilds[2].selectNodes("Materials.Materials"))[1] FSRoot + "\\tools\\simprop\\Autogen\\Materials.xml" false

rootChilds = tempRoot.getChildNodes()
materialsRoot = rootChilds[2].selectNodes("Materials.Materials/Material")
secondMaterial = materialsRoot[materialsRoot.count]
fnLoadFSXMaterial secondMaterial
meditMaterials[2] = fnApplyFSXMaterial ()
fnCompareMaterials meditMaterials[1] meditMaterials[2]*/