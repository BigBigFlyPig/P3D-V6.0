; Copyright (c) Lockheed Martin Corporation.  All rights reserved.
; Sample Piston Engine Helicopter Dynamics File 
;
;
;
	include asm.inc 	; include this in ALL .asm files!
	include airtoken.inc	; definitions of token macros
	include sim1.inc	; definitions of token values

.data

sim_data	label	byte

	;********************************************************************************************************************
	;****START OF ENGINE DATA *******************************************************************************************


	;Piston engine mechanical efficiency table (max 5 entries)
	;IN:  RPM
	;OUT: Power Efficiency scalar
	TOKEN_BEGIN	AIR_61S_ENG_MECHANICAL_EFFICIENCY
	UINT32	5
	REAL8	  0.0,		0.77
	REAL8	  700.0,	0.77
	REAL8	  2000.0,	0.67
	REAL8	  2200.0,	0.54
	REAL8	  2700.0,	0.54

	TOKEN_END


	;Piston engine friction table (max 4 entries)
	;IN:  RPM
	;OUT: Torque due to Friction applied in resistance to RPM (ft-lbs)
	TOKEN_BEGIN	AIR_61S_ENGINE_FRICTION
	UINT32	4
	REAL8	-300.0,-38.00
	REAL8	 300.0, 38.00
	REAL8	 500.0, 25.00
	REAL8	2700.0, 31.00

	TOKEN_END


	;**ENGINE TEMPERATURE AND PRESSURE CONSTANTS


	;Exhaust Gas Temperature
        TOKEN_BEGIN     AIR_61S_EGT
                REAL8   1.000000        ; EGT tuning constant
                REAL8   1660.000000     ; EGT peak temperature (typical peak: 1200 degF + 460)
                REAL8   2.000000        ; EGT tc
        TOKEN_END


	;Cylinder Head Temperature
        TOKEN_BEGIN     AIR_61S_CHT
                REAL8   1.000000        ; CHT tuning constant
                REAL8   0.650000        ; CHT cooling constant
                REAL8   960.000000      ; CHT Max Temp (deg Rankine)
                REAL8   0.015000        ; CHT tc
        TOKEN_END

	;Oil Temperature
        TOKEN_BEGIN     AIR_61S_OIL_TEMPERATURE
                REAL8   1.000000        ; Oil temp tuning constant
                REAL8   0.210000        ; Oil temp cooling constant
                REAL8   700.000000      ; Max Oil Temp (deg Rankine)
                REAL8   0.030000        ; Oil temp tc
        TOKEN_END

	;Oil Pressure
        TOKEN_BEGIN     AIR_61S_OIL_PRESSURE
                REAL8   1.000000        ; Oil pressure tuning constant
                REAL8   8640.000        ; Oil pressure max (PSF)
                REAL8   0.800000        ; Oil pressure tc
        TOKEN_END


	;Fuel Pressure
        TOKEN_BEGIN     AIR_61S_FUEL_PRESSURE
                REAL8   1.000000        ; Fuel pressure tuning constant
                REAL8   2376.000000     ; Fuel pressure max (PSF)
                REAL8   2.000000        ; Fuel pressure tc
        TOKEN_END

	;Radiator Temperature
        TOKEN_BEGIN     AIR_61S_RADIATOR_TEMPERATURE
                REAL8   1.000000        ; Radiator temp tuning constant
                REAL8   0.015000        ; Radiator temp cooling constant
                REAL8   670.000000      ; Radiator temp max (deg Rankine)
                REAL8   0.020000        ; Radiator temp tc
        TOKEN_END


sim_size	equ	$ - sim_data

	end
