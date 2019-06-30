---------------------------------------------------------------------------
-- Prepar3D Extended Material Properties
---------------------------------------------------------------------------

plugin material P3DMaterial
    name:"Prepar3D"
    classID:#(0x7e8caded, 0xb6927fa7)
    extends:Standard replaceUI:false
(
        parameters main rollout:params
        (
                bFresnelDiffuse    type:#boolean  ui:bDiffuse         default:false
                bFresnelReflection type:#boolean  ui:bReflection      default:false
                bFresnelSpecular   type:#boolean  ui:bSpecular        default:false
                texFresnelRamp     type:#texturemap
                bHasFresnelRamp    type:#boolean                      default:false
                texDetailMap       type:#texturemap
                bHasDetailMap      type:#boolean                      default:false

                bUsePrecip         type:#boolean  ui:bUsePrecip       default:false
                bUsePrecipOffset   type:#boolean  ui:bUsePrecipOffset default:false
                fPrecipOffset      type:#float    ui:POffset          default:0

                iBumpScale         type:#integer  ui:PBumpScale       default:1
                cDetailColor       type:#point4   ui:PDetailColor     default:white
                fDetailOffsetU     type:#float    ui:PDetailOffsetU   default:0
                fDetailOffsetV     type:#float    ui:PDetailOffsetV   default:0
                fDetailRotation    type:#float    ui:PDetailRotation  default:0
                iDetailScale       type:#integer  ui:PDetailScaleU    default:1 -- keep naming convention and type for back-compat
                iDetailScaleV      type:#integer  ui:PDetailScaleV    default:1
                bConstrainScale    type:#boolean  ui:bConstrainScale  default:true
                iDetailBlendMode   type:#integer  ui:PDetailBlendMode default:1
                fDetailBlendWeight type:#float    ui:PDetailBlendWeight default:1
                bUseDetailAlphaAsBlendMask type:#boolean ui:bUseDetailAlphaAsBlendMask default:false
                iEnvLevelScale     type:#integer  ui:ReflectScale     default:100
                fSpecMapPowerScale type:#float    ui:SpecMapPowScale  default:64

                bBlendInvBaseAlpha                  type:#boolean   ui:specialEnvInvBaseAlpha       default:false
                bBlendInvSpecAlpha                  type:#boolean   ui:specialEnvInvSpecAlpha       default:false
                bBlendDiffuseBaseAlpha              type:#boolean   ui:bBlendDiffuseBaseAlpha       default:false
                bBlendDiffuseInvSpecAlpha           type:#boolean   ui:bBlendDiffuseInvSpecAlpha    default:false
                bMaskDiffuseBlendsByDetailBlendMask type:#boolean   ui:bMaskDiffuseBlendsByDetailBlendMask    default:false
                bUseGlobalEnv                       type:#boolean   ui:specialUseGlobalEnv          default:false
                
                bUseEmissiveAlphaAsHeatMap      type:#boolean   ui:specialEmissiveAlphaHeat     default:false
                fTemperatureScale               type:#float     ui:TemperatureScale             default:1.0

                iSrcBlend         type:#integer ui:SrcBlend            default:2
                iDstBlend         type:#integer ui:DstBlend            default:1

                bAllowBloom          type:#boolean  ui:bAllowBloom         default:true
                bBloomCopy           type:#boolean  ui:bBloomCopy          default:false
                bBloomModulate       type:#boolean  ui:bBloomModulate      default:false
                bNoSpecularBloom     type:#boolean  ui:bNoSpecularBloom    default:false
                bAllowEmissiveBloom  type:#boolean  ui:bAllowEmissiveBloom default:false
                fSpecularBloomFloor  type:#float    ui:SpecularBloomFloor  default:0.9
                fAmbientLightScale   type:#float    ui:AmbientLightScale   default:1.0

                iEmissiveMode     type:#integer  ui:EmissiveMode        default:1

                bZTestAlpha       type:#boolean  ui:bZTestAlpha         default:false
                iAlphaTestMode    type:#integer  ui:AlphaTestMode       default:1
                fAlphaTestLevel   type:#float    ui:AlphaTestLevel      default:0

                bFinalAlphaWrite  type:#boolean  ui:bFinalAlphaWrite    default:false
                fFinalAlphaWriteValue   type:#float     ui:FinalAlphaWriteValue default:1.0
                bMaskFinalAlphaBlendByDetailBlendMask type:#boolean ui:bMaskFinalAlphaBlendByDetailBlendMask default:false

                bAssumeVerticalNormal   type:#boolean   ui:bAssumeVerticalNormal        default:false
                bZWriteAlpha            type:#boolean   ui:bZWriteAlpha                 default:false
                bNoZWrite               type:#boolean   ui:bNoZWrite                    default:false
                bNoShadow               type:#boolean   ui:bNoShadow                    default:false
                bPrelitVertices         type:#boolean   ui:bPrelitVertices              default:false
                bNoBaseMaterialSpecular type:#boolean   ui:bNoBaseMaterialSpecular      default:false
                bSkinned                type:#boolean   ui:bSkinned                     default:false
                bDoubleSided            type:#boolean   ui:bDoubleSided                 default:false

                bBlendConstant            type:#boolean   ui:bBlendConstant             default:false
                bForceTextureAddressWrap  type:#boolean   ui:bForceTextureAddressWrap   default:false
                bForceTextureAddressClamp type:#boolean   ui:bForceTextureAddressClamp  default:false

                bIsPanelTexture         type:#boolean   ui:bIsPanelTexture              default:false
                bIsNNumberTexture       type:#boolean   ui:bIsNNumberTexture            default:false
				
				fZBias                  type:#float     ui:ZBias                        default:0
                
                szMaterialScript        type:#string    ui:txtMaterialScript            default:("")
        )

        rollout params "Prepar3D Extended Params"
        (
                group "Detail Texture Info"
                (
                        label lbl_Detail "Map:" align:#left across:3
                        mapbutton chooseDetail "<<None>>" align:#left width:110 offset:[-25,0]
                        bitmap dispDetailBitmap bitmap:(bitmap 110 20) align:#right width:110 height:20

                        label lbl_Detail_Color "Detail Color:" align:#left across:2
                        colorPicker PDetailColor "" align:#left width:100 height:40 offset:[-79,0]
                        
                        label lbl_DetailOffset1 "Offset: " align:#left across:3
                        spinner PDetailOffsetU "U:" align:#left offset:[-25,0] type:#float width:100 range:[0.0,1.0,0.0] scale:0.01
                        spinner PDetailOffsetV "V:" align:#left offset:[0,0] type:#float width:100 range:[0.0,1.0,0.0] scale:0.01
                        
                        label lbl_DetailRotation1 "Rotation: " align:#left across:2
                        spinner PDetailRotation "W:" align:#left offset:[-79,0] type:#float width:98 range:[0.0,360.0,0.0] scale:1
                        
                        label lbl_DetailScale1 "Scale: " align:#left across:3
                        spinner PDetailScaleU "U:" align:#left offset:[-25,0] type:#integer width:100 range:[0,64,1] scale:1
                        spinner PDetailScaleV "V:" align:#left offset:[0,0] type:#integer width:100 range:[0,64,1] scale:1
                        checkbox bConstrainScale "Constrain scale aspect ratio" align:#left offset:[90,0] type:#boolean
                        
                        dropdownlist PDetailBlendMode "Blend Mode:" items:#("Multiply","Blend")
                        
                        label lbl_DetailBlendWeight1 "Blend Weight: " align:#left across:2
                        spinner PDetailBlendWeight align:#left offset:[-65,0] type:#float width:83 range:[0.0,1.0,1.0] scale:0.01
                        
                        checkbox bUseDetailAlphaAsBlendMask "Use detail alpha as blend mask" type:#boolean
                )
                group "Bump Texture Info"
                (
                        label lbl_BumpScale1 "Scale: " align:#left across:3
                        label lbl_BumpScale2 "1" align:#left offset:[-55,0]
                        slider   PBumpScale type:#integer align:#left range:[0,64,1] width:230 offset:[-130,0] ticks:16
                )
                group "Fresnel Ramp"
                (
                        label lbl_Fresnel "FresnelRamp:" align:#left across:3
                        mapbutton chooseFresnel "<<None>>" align:#left width:110 offset:[-25,0]
                        bitmap dispBitmap bitmap:(bitmap 110 20) align:#right width:110 height:20
                        checkbox bDiffuse    "Diffuse"
                        checkbox bReflection "Reflection"
                        checkbox bSpecular   "Specular"
                )
                group "Precipitation"
                (
                        checkbox bUsePrecip       "Take into account" type:#boolean
                        checkbox bUsePrecipOffset "Apply offset to start" type:#boolean
                        label lbl_Offset1 "Offset: " align:#left across:3
                        label lbl_Offset2 "0.0" align:#left offset:[-50,0]
                        slider   POffset type:#float align:#left range:[0,1,0] width:200 offset:[-100,0]
                )
                group "Special Functionality"
                (
                        checkbox specialEnvInvBaseAlpha "Blend environment by inverse of diffuse alpha"
                        checkbox specialEnvInvSpecAlpha "Blend environment by specular map alpha"
                        checkbox bBlendDiffuseBaseAlpha "Blend diffuse by diffuse alpha"
                        checkbox bBlendDiffuseInvSpecAlpha "Blend diffuse by inverse of specular map alpha"
                        checkbox bMaskDiffuseBlendsByDetailBlendMask "Blend diffuse blends by detail blend mask"
                        checkbox specialUseGlobalEnv    "Use global environment map as reflection"
                        checkbox specialEmissiveAlphaHeat "Use emissive map alpha as heat map"
                        label lbl_ReflectScale1 "Reflection Scale: " align:#left across:2
                        label lbl_ReflectScale2 "100" align:#left offset:[-15,0]
                        slider   ReflectScale type:#integer range:[0,100,100]
                        label lbl_SpecMapPowScale1 "Specular Map Power Scale: " align:#left across:2
                        label lbl_SpecMapPowScale2 "64.0" align:#left offset:[-15,0]
                        slider   SpecMapPowScale type:#integer range:[0,256,64]
                        label lbl_TemperatureScale1 "Temperature Scale: " align:#left across:2
                        label lbl_TemperatureScale2 "1.0" align:#left offset:[-15,0]
                        slider   TemperatureScale type:#float range:[0,10,1]

                )
                group "Bloom"
                (
                        checkbox bAllowBloom    "Allow bloom"

                        checkbox bBloomCopy     "Bloom material by copying"
                        checkbox bBloomModulate "Bloom material modulating by alpha"

                        checkbox bNoSpecularBloom "No specular bloom"
                        checkbox bAllowEmissiveBloom "Allow emissive bloom"

                        label lbl_SpecularBloomFloor1 "Specular Bloom Floor: " align:#left across:2
                        label lbl_SpecularBloomFloor2 "0.9" align:#left offset:[-15,0]
                        slider   SpecularBloomFloor type:#float range:[0,1.0,0.9]

                        label lbl_AmbientLightScale "Ambient light scale: " align:#left across:2
                        label lbl_AmbientLightScale2 "1.0" align:#left offset:[-15,0]
                        slider   AmbientLightScale type:#float range:[0,1.0,1.0]
                )

                group "Framebuffer Blend"
                (
                        button butOpaque "Set Default Opaque" across:2
                        button butTransp "Set Default Transparent"
                        dropdownlist srcBlend "Source Blend:" items:#("Zero", "One", "SrcColor", "InvSrcColor", "SrcAlpha", "InvSrcAlpha", "DestAlpha", "InvDestAlpha", "DestColor", "InvDestColor")
                        dropdownlist dstBlend "Destination Blend:" items:#("Zero", "One", "SrcColor", "InvSrcColor", "SrcAlpha", "InvSrcAlpha", "DestAlpha", "InvDestAlpha", "DestColor", "InvDestColor")
                )

                group "Emissive Properties"
                (
                        dropdownlist emissiveMode "Emissive Mode:" items:#("AdditiveNightOnly", "Blend", "MultiplyBlend", "Additive", "AdditiveNightOnlyUserControlled", "BlendUserControlled", "MultiplyBlendUserControlled", "AdditiveUserControlled" )
                )
                
                group "Script Properties"
                (
                        editText txtMaterialScript "Script:" align:#left
                        button buttonBrowseScript "Browse..." align:#right
                )

                group "Material Alpha Test"
                (
                        checkbox bZTestAlpha "Z-Test Alpha"
						label lbl_ZBias "Z-Bias Level:" align:#left across:2
						spinner Zbias align:#left offset:[-25,0] type:#float width:50 range:[-100.0,1.0,100.0]
                        label lbl_AlphaTestLevel1 "Alpha Test Level: " align:#left across:2
                        label lbl_AlphaTestLevel2 "0.0" align:#left offset:[-15,0]
                        slider   AlphaTestLevel type:#float range:[0.0,255.0,0.0]
                        dropdownlist AlphaTestMode "Alpha Test Mode:" items:#( "Never", "Less", "Equal", "LessEqual", "Greater", "NotEqual", "GreaterEqual", "Always" )
                )

                group "Final Alpha Blend"
                (
                        checkbox bFinalAlphaWrite "Set final alpha value at render time"
                        label lbl_FinalAlphaWrite1 "Final Alpha Multiply: " align:#left across:2
                        label lbl_FinalAlphaWrite2 "1.0" align:#left offset:[-15,0]
                        slider   FinalAlphaWriteValue type:#float range:[0.0,1.0,1.0]
                        checkbox bMaskFinalAlphaBlendByDetailBlendMask "Mask by detail blend mask"
                )

                group "Enhanced Parameters"
                (
                        checkbox bAssumeVerticalNormal  "Assume vertical normal"
                        checkbox bZWriteAlpha           "Z-Write Alpha"
                        checkbox bNoZWrite              "No Z Write"
                        checkbox bNoShadow              "No shadow"
                        checkbox bPrelitVertices        "Prelit vertices"
                        checkbox bNoBaseMaterialSpecular        "No Base Material Specular"
                        checkbox bSkinned               "Skinned mesh"
                        checkbox bDoubleSided           "Double sided"
                        checkbox bBlendConstant         "Blend Constant"
                        checkbox bForceTextureAddressWrap "Force Texture Address Wrap"
                        checkbox bForceTextureAddressClamp "Force Texture Address Clamp"
                )

                group "Aircraft Material Params"
                (
                        checkbox bIsPanelTexture        "Is virtual cockpit panel texture"
                        checkbox bIsNNumberTexture      "Is N-Number texture"
                )

                on butOpaque pressed do
                (
                        srcBlend.selection = 2
                        dstBlend.selection = 1
                )

                on butTransp pressed do
                (
                        srcBlend.selection = 5
                        dstBlend.selection = 6
                )
                
                on buttonBrowseScript pressed do
                (
                    local scriptFile = getOpenFileName caption:"Please locate the material script you would like to apply." types:"LUA Script files (*.lua)|*.lua"
                    
                    if scriptFile != undefined then
                    (
                        txtMaterialScript.text = (getFileNameFile scriptFile as string) + ".lua"
                    )
                )

                on bBloomCopy changed val do
                (
                        if val == true do
                        (
                                bBloomModulate.state = false
                        )
                )

                on bBloomModulate changed val do
                (
                        if val == true do
                        (
                                bBloomCopy.state = false
                        )
                )

                on specialEnvInvBaseAlpha changed val do
                (
                        if val == true do
                        (
                                specialEnvInvSpecAlpha.state = false
                        )
                )

                on specialEnvInvSpecAlpha changed val do
                (
                        if val == true do
                        (
                                specialEnvInvBaseAlpha.state = false
                        )
                )

                on chooseFresnel picked texmap do
                (
                        if texmap != undefined then
                        (
                                texFresnelRamp = texmap
                                chooseFresnel.text = getFilenameFile texmap.filename as string
                                dispBitmap.bitmap = texmap.bitmap
                                bHasFresnelRamp = true
                        )
                        else
                        (
                                bHasFresnelRamp = false
                        )
                )
                on chooseDetail picked texmap do
                (
                        if texmap != undefined then
                        (
                                texDetailMap = texmap
                                chooseDetail.text = getFilenameFile texmap.filename as string
                                dispDetailBitmap.bitmap = texmap.bitmap
                                bHasDetailMap = true
                        )
                        else
                        (
                                bHasDetailMap = false
                        )
                )

                on POffset changed fNewVal do
                (
                        lbl_Offset2.text = fNewVal as string
                )
                
                on PDetailScaleU changed fNewVal do
                (
                    if bConstrainScale.checked != false then
                    (
                        PDetailScaleV.value = fNewVal
                    )
                )
                
                on PDetailScaleV changed fNewVal do
                (
                    if bConstrainScale.checked != false then
                    (
                        PDetailScaleU.value = fNewVal
                    )
                )
                
                on bConstrainScale changed fNewVal do
                (
                    if fNewVal != false then
                    (
                        PDetailScaleV.enabled = false
                        PDetailScaleV.value = PDetailScaleU.value
                    )
                    else
                    (
                        PDetailScaleV.enabled = true
                    )
                )

                on PBumpScale changed fNewVal do
                (
                        lbl_BumpScale2.text = fNewVal as string
                )

                on ReflectScale changed fNewVal do
                (
                        lbl_ReflectScale2.text = fNewVal as string
                )

                on SpecMapPowScale changed fNewVal do
                (
                        lbl_SpecMapPowScale2.text = fNewVal as string
                )
                
                on TemperatureScale changed fNewVal do
                (
                        lbl_TemperatureScale2.text = fNewVal as string
                )
                
                on SpecularBloomFloor changed fNewVal do
                (
                        lbl_SpecularBloomFloor2.text = fNewVal as string
                )

                on AmbientLightScale changed fNewVal do
                (
                        lbl_AmbientLightScale2.text = fNewVal as string
                )

                on AlphaTestLevel changed fNewVal do
                (
                        lbl_AlphaTestLevel2.text = fNewVal as string
                )
                
                on FinalAlphaWriteValue changed fNewVal do
                (
                        lbl_FinalAlphaWrite2.text = fNewVal as string
                )

                on params open do
                (
                        lbl_Offset2.text       = POffset.value as string
                        lbl_BumpScale2.text    = PBumpScale.value as string
                        lbl_ReflectScale2.text = ReflectScale.value as string
                        lbl_SpecMapPowScale2.text = SpecMapPowScale.value as string
                        lbl_TemperatureScale2.text = TemperatureScale.value as string
                        lbl_SpecularBloomFloor2.text = SpecularBloomFloor.value as string
                        lbl_AmbientLightScale2.text = AmbientLightScale.value as string
                        lbl_AlphaTestLevel2.text = AlphaTestLevel.value as string
                        lbl_FinalAlphaWrite2.text = FinalAlphaWriteValue.value as string
                        
                        if bConstrainScale.checked != false then
                        (
                            PDetailScaleV.enabled = false
                            PDetailScaleV.value = PDetailScaleU.value
                        )

                        if bHasFresnelRamp then
                        (
                                chooseFresnel.text = getFilenameFile texFresnelRamp.filename as string
                                try
                                (
                                        dispBitmap.bitmap = texFresnelRamp.bitmap
                                )
                                catch
                                (
                                        dispBitmap.bitmap = bitmap 110 20
                                )
                        )
                        else
                                dispBitmap.bitmap = bitmap 110 20

                        if bHasDetailMap then
                        (
                                chooseDetail.text = getFilenameFile texDetailMap.filename as string
                                try
                                                                (
                                                                        dispDetailBitmap.bitmap = texDetailMap.bitmap
                                                                )
                                                                catch
                                                                (
                                                                        dispBitmap.bitmap = bitmap 110 20
                                                                )
                        )
                        else
                                dispDetailBitmap.bitmap = bitmap 110 20
                )
        )
)
