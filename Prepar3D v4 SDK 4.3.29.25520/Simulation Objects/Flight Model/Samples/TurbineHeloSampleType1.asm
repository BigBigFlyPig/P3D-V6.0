; Copyright (c) Lockheed Martin Corporation.  All rights reserved.
; Sample Turbine Engine Helicopter Dynamics File 
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


	;Turboprop Commanded N1 (corrected) vs. Throttle table at 0 Mach (max 9 rows, 11 columns)
	;NOTE:  The specific (low) Mach value is specified at the 0,0 position of the table
        ;	This table is used in conjunction with the AIR_70_MACH_HI_CORRECTED_COMMANDED_NE table

        ;IN: X: 1/atmospheric press ratio (Psl/P)(note: use inverse of delta pressure ratio so that values are in increasing order)
        ;IN: Y: %Power Lever / Throttle  
        ;OUT: Corrected N1 for MACH = 0

	TOKEN_BEGIN	AIR_70_MACH_0_CORRECTED_COMMANDED_NE

	UINT32 9,3
	;		//SSL  		 35K
	REAL8	0.0,	  1.0, 	 	 4.237
	REAL8	0.00,	 62.00,		 67.02
	REAL8	0.42,	 80.80,		 94.48
	REAL8	0.51,	 86.20,		101.52
	REAL8	0.61,	 89.70,		108.06
	REAL8	0.75,	 95.70,		108.06
	REAL8	0.80,	 97.00,		108.06
	REAL8	0.90,	 99.60,		108.06
	REAL8	1.00,	101.50,		108.06
	TOKEN_END

	;Turboprop Commanded N1 (corrected) vs. Throttle table at HI Mach (max 9 rows, 11 columns)
	;NOTE:  The specific (HI) Mach value is specified at the 0,0 position of the table
        ;	This table is used in conjunction with the AIR_70_MACH_0_CORRECTED_COMMANDED_NE table

        ;IN: X: 1/atmospheric press ratio (Psl/P)(note: use inverse of delta pressure ratio so that values are in increasing order)
        ;IN: Y: %Power Lever / Throttle  
        ;OUT: Corrected N1 for specified MACH
	TOKEN_BEGIN	AIR_70_MACH_HI_CORRECTED_COMMANDED_NE

	UINT32 9,3
	;		//SSL	      35K
	REAL8	0.9,	   1.0,	      4.237
	REAL8	0.00,	 44.29,       60.46
	REAL8	0.42,	 63.39,       75.10
	REAL8	0.51,	 68.28,       79.40
	REAL8	0.61,	 73.18,       83.80
	REAL8	0.70,	 76.61,       88.00
	REAL8	0.79,	 80.12,       90.00
	REAL8	0.93,	 82.38,       94.00
	REAL8	1.00,	 83.83,       95.00
	TOKEN_END


	;N1 to Shaft Torque table (max 3 elements)
	;IN:  N1 (corrected)
	;OUT: Percent maximum torque (corrected)
	TOKEN_BEGIN	AIR_70_N1_TO_SHAFT_TORQUE
	UINT32 3
			;Pct Max Trq
	REAL8	 0.0,	0.0
	REAL8	60.0,	0.031
	REAL8  100.0,	1.8

	TOKEN_END

	;Atmospheric density on shaft torque (max 10 entries))
	;IN:  Ambient density (slug/ft^3)
	;OUT: Scalar on corrected shaft torque
	TOKEN_BEGIN	AIR_80_DENSITY_ON_TP_TORQUE
        UINT32 10
     
        REAL8   0.000225,   0.000  ;60k feet
        REAL8   0.000587,   0.210  ;40k feet
        REAL8   0.000739,   0.269  ;35k feet
        REAL8   0.000891,   0.340  ;30k feet
        REAL8   0.001144,   0.427  ;25k feet
        REAL8   0.001268,   0.487  ;20k feet
        REAL8   0.001496,   0.596  ;15k feet
        REAL8   0.001756,   0.735  ;10k feet
        REAL8   0.002049,   0.849  ;5k feet
        REAL8   0.002377,   1.000  ;0k feet
        
	TOKEN_END

	;Engine RPM vs. Friction Torque table (max 4 entries)
	;IN:  Percent engine RPM
	;OUT: Torque due to Friction expressed as a percentage of max torque
	TOKEN_BEGIN	AIR_61S_ENGINE_FRICTION
	UINT32	4
			;Pct of Max Torque
	REAL8	 -10.0,-0.2
	REAL8	  10.0, 0.2
	REAL8	  70.0, 0.8
	REAL8	 100.0, 0.8

	TOKEN_END

       	;N1 vs Thrust table (max 21 rows, 11 columns)
      	; NOTE: While generally insignificant with turboprops,
	;       exhaust thrust can be modeled here      	        
	;IN: X: Mach
	;IN: Y: N1 (corrected)
	;OUT: Gross Thurst (corrected) / static thrust
        TOKEN_BEGIN     AIR_70_N1_AND_MACH_ON_THRUST

        UINT32 3,3     ;ROWS,COLS
	;        N1
        REAL8    0.0,      0.0,        0.9 ; Mach
        REAL8    0.0,  0.00000,    0.00000
        REAL8  100.0,  0.00000,    0.00000

        TOKEN_END

	;N1 vs AirFlow/Ram Drag   (max 21 rows, 11 columns)
      	; NOTE: While generally insignificant with turboprops,
	;       intake ram can be modeled here      	        
	;IN: X:  Mach
	;IN: Y:  N1 (corrected)
	;OUT: Normalized Airflow (Corrected) = Airflow / inlet area
        TOKEN_BEGIN     AIR_70_CORRECTED_AIRFLOW
        
        UINT32  3,3
	;	 N1
        REAL8    0.0,    0.0,         0.9    ; Mach
        REAL8    0.0,    0.00000,     0.00000
        REAL8  100.0,    0.00000,     0.00000
        TOKEN_END


	;**ENGINE TEMPERATURE AND PRESSURE CONSTANTS

	;Engine Pressure Ratio        
        TOKEN_BEGIN     AIR_70_EPR
	    REAL8 1.0		;EPR tuning constant 
	    REAL8 1.4		;Max EPR             
	    REAL8 14.0		;EPR time constant             
        TOKEN_END


	;Oil Temperature
        TOKEN_BEGIN     AIR_61S_OIL_TEMPERATURE
                REAL8   1.000000        ; Oil temp tuning constant
                REAL8   0.000000        ; Oil temp cooling constant
                REAL8   600.000000      ; Max Oil Temp (deg Rankine)
                REAL8   0.010000        ; Oil temp time constant
        TOKEN_END

	;Oil Pressure
        TOKEN_BEGIN     AIR_61S_OIL_PRESSURE
                REAL8   1.000000        ; Oil pressure tuning constant
                REAL8   19440.000000    ; Oil pressure max (PSF)
                REAL8   0.800000        ; Oil pressure time constant
        TOKEN_END

	;Interstage turbine temperature
        TOKEN_BEGIN     AIR_70_ITT
		REAL8 	1.0 		;ITT tuning constant
		REAL8 	2400.0  	;ITT peak temperature
                REAL8 	0.8 		;ITT time constant
        TOKEN_END


        ;Exhaust Gas Temperature
        TOKEN_BEGIN     AIR_61S_EGT
                REAL8   1.000000        ; EGT tuning constant
                REAL8   1285.000000     ; EGT peak temperature (typical peak: 1200 degF + 460)
                REAL8   2.000000        ; EGT time constant
        TOKEN_END

	;*****END ENGINE DATA *********************************************************


sim_size	equ	$ - sim_data

	end
