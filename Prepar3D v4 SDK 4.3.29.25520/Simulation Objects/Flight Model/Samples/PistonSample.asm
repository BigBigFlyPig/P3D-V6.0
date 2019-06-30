; Copyright (c) Lockheed Martin Corporation.  All rights reserved.
; Sample Piston Engine Airplane Flight Dynamics File 
;
;
;
        include asm.inc         ; include this in ALL .asm files!
        include airtoken.inc    ; definitions of token macros
        include sim1.inc        ; definitions of token values
.data

sim_data        label   byte

	;******START OF AERODYNAMIC DATA ************************************************************************************
	;********************************************************************************************************************


	
	;**** BEGINNING OF REQUIRED AERODYNAMICS ************************

	;The following 6 blocks define the base stability and control derivatives
	;Lift, Drag, Pitch, Side Force, Roll, and Yaw.
        TOKEN_BEGIN     AIR_80_LIFT_PARAMS
                REAL8   0.000000        ; CL spoiler
                REAL8   1.2             ; CL flaps
                REAL8   0.000000        ; UNUSED
                REAL8   0.715988        ; CLih
                REAL8   -0.273018       ; CLde
                REAL8   0.000000        ; UNUSED
        TOKEN_END

        TOKEN_BEGIN     AIR_80_DRAG_PARAMS
                REAL8   0.065           ; CDo
                REAL8   0.07            ; CD flaps
                REAL8   0.007000        ; CD gear
                REAL8   0.000000        ; CD spoiler
        TOKEN_END

        TOKEN_BEGIN     AIR_80_PITCH_PARAMS
                REAL8   2.133893        ; CMih
                REAL8   -1.6            ; CMde
                REAL8   -0.051200       ; CMde due to propwash
                REAL8   -2.601277       ; CLq
                REAL8   0.988016        ; CL adot
                REAL8   2.0             ; CM adot
                REAL8   -28.770823      ; CMq
                REAL8   -10.000000      ; CMq due to propwash
                REAL8   0.0             ; CMo
                REAL8   -0.07           ; CM flap
                REAL8   -1.066947       ; CM delta trim
                REAL8   0.020000        ; CM gear
                REAL8   0.000000        ; CM spoiler
        TOKEN_END

        TOKEN_BEGIN     AIR_80_SIDE_FORCE_PARAMS
                REAL8   -0.569647       ; CyB
                REAL8   0.038363        ; CyP
                REAL8   0.018805        ; CyR
                REAL8   -0.153025       ; Cy Delta Rudder
        TOKEN_END

        TOKEN_BEGIN     AIR_80_ROLL_PARAMS
                REAL8   0.045           ; ClB
                REAL8   -0.9            ; ClP
                REAL8   -0.061076       ; ClR
                REAL8   0.000000        ; Cl Delta Spoiler
                REAL8   -0.269225       ; Cl Delta Aileron
                REAL8   0.010           ; Cl Delta Rudder
        TOKEN_END

        TOKEN_BEGIN     AIR_80_YAW_PARAMS
                REAL8   0.099757        ; CnB
                REAL8   0.034553        ; CnP
                REAL8   -0.524805       ; CnR
                REAL8   -3.000000       ; CnR due to propwash
                REAL8   0.000000        ; UNUSED
                REAL8   0.000000        ; UNUSED
                REAL8   -0.002345       ; Cn Delta Aileron
                REAL8   0.063277        ; Cn Delta Rudder
                REAL8   0.002048        ; Cn Delta Rudder due to propwash
        TOKEN_END

	;CL vs. Alpha
	;The first entry defines the number of data points (maximum 47 entries)
        TOKEN_BEGIN     AIR_CL_ALPHA
                dd      13      ; Number of Entries

                REAL8     -3.142,	 0.000
                REAL8     -2.356,	 0.500
                REAL8     -1.571,	 0.000
                REAL8     -0.366,	-1.528
                REAL8     -0.078,	 0.000
                REAL8      0.017,	 0.590
                REAL8      0.262,	 2.096
                REAL8      0.288,	 2.183
                REAL8      0.314,	 2.096
                REAL8      0.340,	 1.528
                REAL8      1.571,	 0.000
                REAL8      2.356,	-0.500
                REAL8      3.142,	 0.000
                
        TOKEN_END

	;CM vs. Alpha
	;The first entry defines the number of data points (maximum 47 entries)
        TOKEN_BEGIN     AIR_CM_ALPHA
                dd      13       ; Number of Entries

                REAL8   -3.142,   0.000 
                REAL8   -0.550,  -0.545
                REAL8   -0.375,  -0.545
                REAL8   -0.305,  -0.273
                REAL8   -0.288,  -0.227
                REAL8   -0.271,  -0.189
                REAL8    0.000,   0.000 
                REAL8    0.271,   0.189 
                REAL8    0.288,   0.227 
                REAL8    0.305,   0.273 
                REAL8    0.375,   0.545 
                REAL8    0.550,   0.545 
                REAL8    3.142,   0.000 
                
        TOKEN_END

	;**** END OF REQUIRED AERODYNAMICS ************************************************

	;**** GROUND EFFECT ************************
	;Scalar on Lift due to ground proximity  (max 11 entries)
	;IN:  Ratio of wingspan / height above ground
	;OUT: Scalar on CL
	TOKEN_BEGIN     AIR_GROUND_EFFECT
                dd      11      ; Number of Entries
                REAL8           0.054000,       1.250000        ;
                REAL8           0.100000,       1.160000        ;
                REAL8           0.200000,       1.096100        ;
                REAL8           0.300000,       1.060000        ;
                REAL8           0.400000,       1.040000        ;
                REAL8           0.500000,       1.030000        ;
                REAL8           0.600000,       1.024200        ;
                REAL8           0.700000,       1.021300        ;
                REAL8           0.800000,       1.016100        ;
                REAL8           0.900000,       1.010000        ;
                REAL8           1.000000,       1.000000        ;       
        TOKEN_END

	;**** END OF GROUND EFFECT ************************

	;**** BEGINNING OF ADDITIONAL CONTROL EFFECTS *************

	;Scalar affect of aileron and rudder trim
	;0 implies no trim
        TOKEN_BEGIN    AIR_61S_AIL_RUD_TRIM_CONSTANTS
               REAL8   0.000000        ; aileron_trim_scale
               REAL8   0.314159        ; rudder_trim_scale
        TOKEN_END
        

	;Elevator effectiveness scaling table (max 7 entries)
	; IN:  Elevator angle  (radians)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_ELEVATOR_SCALING
                dd      7       ; Number of Entries
                REAL8           -0.349000,      1.000000        ;
                REAL8           -0.262000,      0.800000        ;
                REAL8           -0.175000,      0.700000        ;
                REAL8           0.000000,       0.500000        ;
                REAL8           0.175000,       0.700000        ;
                REAL8           0.349000,       0.800000        ;
                REAL8           0.524000,       1.000000        ;       
        TOKEN_END

        ;Aileron effectiveness scaling table  (max 7 entries)
	; IN:  Aileron angle  (radians)
	; OUT: Effectiveness scalar
	TOKEN_BEGIN     AIR_AILERON_SCALING
                dd      7       ; Number of Entries
                REAL8           -0.209000,      1.000000        ;
                REAL8           -0.140000,      0.800000        ;
                REAL8           -0.070000,      0.600000        ;
                REAL8           0.000000,       0.400000        ;
                REAL8           0.105000,       0.600000        ;
                REAL8           0.209000,       0.800000        ;
                REAL8           0.279000,       1.000000        ;       
        TOKEN_END

	;Rudder effectiveness scaling table  (max 7 entries)
	; IN:  Rudder angle  (radians)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_RUDDER_SCALING
                dd      7       ; Number of Entries
                REAL8           -0.489000,      1.000000        ;
                REAL8           -0.349000,      0.800000        ;
                REAL8           -0.175000,      0.600000        ;
                REAL8           0.000000,       0.400000        ;
                REAL8           0.175000,       0.600000        ;
                REAL8           0.349000,       0.800000        ;
                REAL8           0.489000,       1.000000        ;       
        TOKEN_END

	;Elevator elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2) (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_ELEVATOR_ELASTICITY
                dd      5       ; Number of Entries
                REAL8           0.000000,       1.000000        ;
                REAL8           135.000000,     1.000000        ;
                REAL8           300.000000,     0.800000        ;
                REAL8           500.000000,     0.500000        ;
                REAL8           700.000000,     0.200000        ;       
        TOKEN_END

	;Elevator trim elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2)  (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_70_ELEVATOR_TRIM_ELASTICITY
                dd      5       ; Number of Entries
                REAL8           0.000000,       1.000000        ;
                REAL8           135.000000,     0.800000        ;
                REAL8           300.000000,     0.500000        ;
                REAL8           500.000000,     0.200000        ;
                REAL8           700.000000,     0.100000        ;       
        TOKEN_END

	;Aileron elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2)  (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_AILERON_ELASTICITY
                dd      5       ; Number of Entries
                REAL8           0.000000,       1.000000        ;
                REAL8           300.000000,     0.500000        ;
                REAL8           600.000000,     0.200000        ;
                REAL8           1482.000000,    0.000000        ;
                REAL8           1500.000000,    -0.100000       ;       
        TOKEN_END

	;Rudder elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2)  (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_RUDDER_ELASTICITY
                dd      5       ; Number of Entries
                REAL8           0.000000,       1.000000        ;
                REAL8           300.000000,     1.000000        ;
                REAL8           800.000000,     0.500000        ;
                REAL8           1000.000000,    0.300000        ;
                REAL8           1500.000000,    0.200000        ;       
        TOKEN_END

	;Load factor on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft load factor 
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_AILERON_LOAD_FACTOR_EFF
                dd      5       ; Number of Entries
                REAL8           0.000000,       1.000000        ;
                REAL8           3.000000,       1.000000        ;
                REAL8           4.000000,       0.800000        ;
                REAL8           6.000000,       0.650000        ;
                REAL8           8.000000,       0.200000        ;       
        TOKEN_END

	;**** END OF ADDITIONAL CONTROL EFFECTS *************

	;**** START OF ANGLE OF ATTACK TABLES ***************

	;Cl (roll) induced by angle of attack  (max 7 entries)
	; IN: Angle of attack (degrees)
	; OUT: Delta Cl
        TOKEN_BEGIN     AIR_70S_Cl_ALPHA_ROLL
                dd      7       ; Number of Entries
                REAL8           -17.000000,     0.020000        ;
                REAL8           -13.000000,     0.010000        ;
                REAL8           -10.000000,     0.000000        ;
                REAL8           16.000000,      0.000000        ;
                REAL8           17.000000,      -0.005000       ;
                REAL8           18.000000,      0.012000        ;
                REAL8           19.000000,      0.020000        ;       
        TOKEN_END

	;Cn (yaw) induced by angle of attack  (max 7 entries)
	; IN: Angle of attack (degrees)
	; OUT: Delta Cn
        TOKEN_BEGIN     AIR_70S_CN_ALPHA_YAW
                dd      7       ; Number of Entries
                REAL8           -16.000000,     0.020000        ;
                REAL8           -13.000000,     0.015000        ;
                REAL8           -10.000000,     0.000000        ;
                REAL8           16.000000,      0.000000        ;
                REAL8           17.000000,      -0.007000       ;
                REAL8           18.000000,      0.006000        ;
                REAL8           19.000000,      0.016000        ;       
        TOKEN_END
	

	;Scalar on Cmde due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmde
        TOKEN_BEGIN     AIR_61S_ALPHA_ON_CMDE
                dd      5       ; Number of Entries
                REAL8           -15.000000,     0.800000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           8.000000,       1.000000        ;
                REAL8           15.000000,      0.400000        ;
                REAL8           20.000000,      0.200000        ;       
        TOKEN_END

	;Scalar on Cmih due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmih
        TOKEN_BEGIN     AIR_61S_ALPHA_ON_CMIH
                dd      5       ; Number of Entries
                REAL8           -15.000000,     0.000000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           13.000000,      0.300000        ;
                REAL8           15.000000,      0.000000        ;
                REAL8           25.000000,      1.000000        ;       
        TOKEN_END

	;Scalar on Cmadot due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmadot
        TOKEN_BEGIN     AIR_61S_ALPHA_ON_CMADOT
                dd      5       ; Number of Entries
                REAL8           -15.000000,     1.000000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           5.000000,       1.000000        ;
                REAL8           15.000000,      1.000000        ;
                REAL8           25.000000,      1.000000        ;       
        TOKEN_END

	;Scalar on Cmq due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmq
        TOKEN_BEGIN     AIR_61S_ALPHA_ON_CMQ
                dd      5       ; Number of Entries
                REAL8           -15.000000,     1.000000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           11.000000,      1.000000        ;
                REAL8           15.000000,      1.000000        ;
                REAL8           25.000000,      1.000000        ;       
        TOKEN_END

	;Scalar on Cndr due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cndr
        TOKEN_BEGIN     AIR_70S_ALPHA_ON_CNDR
                dd      5       ; Number of Entries
                REAL8           -15.000000,     1.000000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           11.000000,      1.000000        ;
                REAL8           15.000000,      1.000000        ;
                REAL8           25.000000,      1.000000        ;       
        TOKEN_END

	;Scalar on Clda due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Clda
        TOKEN_BEGIN     AIR_70S_ALPHA_ON_CLDA
                dd      5       ; Number of Entries
                REAL8           -15.000000,     1.000000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           11.000000,      1.000000        ;
                REAL8           15.000000,      1.000000        ;
                REAL8           25.000000,      1.000000        ;       
        TOKEN_END

	;Scalar on ClBeta due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on ClBeta
	TOKEN_BEGIN     AIR_ALPHA_ON_CL_BETA
                dd      9       ; Number of Entries
                REAL8           -180.000000,    1.000000        ;
                REAL8           -90.000000,     0.200000        ;
                REAL8           -15.000000,     0.100000        ;
                REAL8           -10.000000,     0.600000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           10.000000,      0.600000        ;
                REAL8           15.000000,      0.100000        ;
                REAL8           90.000000,      0.200000        ;
                REAL8           180.000000,     1.000000        ;       
        TOKEN_END        
        
	;Scalar on Clp due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmp
	TOKEN_BEGIN     AIR_ALPHA_ON_CLP
                dd      9       ; Number of Entries
                REAL8           -180.000000,    1.000000        ;
                REAL8           -90.000000,     0.500000        ;
                REAL8           -15.000000,     0.100000        ;
                REAL8           -10.000000,     0.400000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           10.000000,      0.400000        ;
                REAL8           15.000000,      0.100000        ;
                REAL8           90.000000,      0.500000        ;
                REAL8           180.000000,     1.000000        ;       
        TOKEN_END

	;Scalar on CnBeta due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on CnBeta
	TOKEN_BEGIN     AIR_ALPHA_ON_CN_BETA
                dd      9       ; Number of Entries
                REAL8           -180.000000,    1.000000        ;
                REAL8           -90.000000,     0.500000        ;
                REAL8           -15.000000,     0.100000        ;
                REAL8           -10.000000,     0.600000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           10.000000,      0.600000        ;
                REAL8           15.000000,      0.100000        ;
                REAL8           90.000000,      0.500000        ;
                REAL8           180.000000,     1.000000        ;       
        TOKEN_END

	;Scalar on Cnr due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cnr
	TOKEN_BEGIN     AIR_ALPHA_ON_CNR
                dd      9       ; Number of Entries
                REAL8           -180.000000,    1.000000        ;
                REAL8           -90.000000,     0.400000        ;
                REAL8           -15.000000,     0.100000        ;
                REAL8           -10.000000,     0.600000        ;
                REAL8           0.000000,       1.000000        ;
                REAL8           10.000000,      0.600000        ;
                REAL8           15.000000,      0.100000        ;
                REAL8           90.000000,      0.400000        ;
                REAL8           180.000000,     1.000000        ;       
        TOKEN_END

	;**** END OF ANGLE OF ATTACK TABLES ***************


	;**** START OF MACH TABLES ***************        

	;Scalar on Lift due to mach (max 17 entries)
	;IN:  Mach
	;OUT: Scalar on CL0
        TOKEN_BEGIN     AIR_CL_MACH
                dd      17      ; Number of Entries
                REAL8           0.000000,       1.000000        ;
                REAL8           0.200000,       1.000000        ;
                REAL8           0.400000,       1.000000        ;
                REAL8           0.600000,       1.000000        ;
                REAL8           0.800000,       1.000000        ;
                REAL8           1.000000,       1.000000        ;
                REAL8           1.200000,       1.000000        ;
                REAL8           1.400000,       1.000000        ;
                REAL8           1.600000,       1.000000        ;
                REAL8           1.800000,       1.000000        ;
                REAL8           2.000000,       1.000000        ;
                REAL8           2.200000,       1.000000        ;
                REAL8           2.400000,       1.000000        ;
                REAL8           2.600000,       1.000000        ;
                REAL8           2.800000,       1.000000        ;
                REAL8           3.000000,       1.000000        ;
                REAL8           3.200000,       1.000000        ;       
        TOKEN_END


	;Scalar on Drag due to mach (max 17 entries)
	;IN:  Mach
	;OUT: Delta CD0 due to mach
	TOKEN_BEGIN  AIR_10XPACK_CD0_MACH
	
		UINT32	17		;NUMBER OF ENTRIES
	               ;Mach   ;Delta CD0

	        REAL8   0.00,	0.0000
	        REAL8   0.20,	0.0000
	        REAL8   0.54,	0.0020
	        REAL8   0.66,	0.0034
	        REAL8   0.77,	0.0177
	        REAL8   0.84,	0.0203
	        REAL8   0.91,	0.0226
	        REAL8   0.97,	0.0243
	        REAL8   1.09,	0.0254
	        REAL8   1.32,	0.0251
	        REAL8   1.48,	0.0238
	        REAL8   1.73,	0.0170
	        REAL8   2.40,	0.0114
	        REAL8   2.60,	0.0101
	        REAL8   2.80,	0.0098
	        REAL8   3.00,	0.0098
	        REAL8   3.20,	0.0098

		TOKEN_END

	;**************************************************************************************
	; The following mach data tables are fixed size (17 elements) and are assumed to have
	; inputs of Mach from 0.0 to 3.2, where each table step is 0.2 Mach.  The output is 
	; an integer which is the scalar multiplied by 2^11, or 2048.  E.g. a desired about  of 
	; 0.25 would be configured in the table as 512.  All outputs are additive to the base
	; aerodynamic coefficient. 
	;**************************************************************************************
	
	;Integer mach table on Clde
	;IN:  Mach index (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clde due to mach
        TOKEN_BEGIN     AIR_CL_DELTAE
                dw          0           ; Mach = 0.0 CL_DELTAE
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cladot
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cladot due to mach
        TOKEN_BEGIN     AIR_CL_ADOT
                dw          0           ; Mach = 0.0 CL_ADOT
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on CLq
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta CLq due to mach
        TOKEN_BEGIN     AIR_CL_Q
                dw          0           ; Mach = 0.0 CL_Q
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Clih
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clih due to mach
        TOKEN_BEGIN     AIR_CL_IH
                dw          0           ; Mach = 0.0 CL_IH
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cmde
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmde due to mach
        TOKEN_BEGIN     AIR_CM_DELTAE
                dw          0           ; Mach = 0.0 CM_DELTAE
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cmadot
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmadot due to mach
        TOKEN_BEGIN     AIR_CM_ADOT
                dw          0           ; Mach = 0.0 CM_ALPHADOT
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cmq
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmq due to mach
        TOKEN_BEGIN     AIR_CM_Q
                dw          0           ; Mach = 0.0 CM_Q
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cmih
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmih due to mach
        TOKEN_BEGIN     AIR_CM_IH
                dw          0           ; Mach = 0.0 CM_IH
                dw          0           ; Mach = 0.2 table output scale => 1.0 = 2048
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END
            

	;Integer mach table on Cm0
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cm0 due to mach
        TOKEN_BEGIN     AIR_CMO
                dw          0           ; Mach = 0.0 CM0
                dw          0           ; Mach = 0.2
                dw          8           ; Mach = 0.4
                dw         45           ; Mach = 0.6
                dw        150           ; Mach = 0.8
                dw        240           ; Mach = 1.0
                dw         50           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cyb
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cyb due to mach
        TOKEN_BEGIN     AIR_CY_BETA
                dw          0           ; Mach = 0.0 CY_BETA
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cydr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cydr due to mach
        TOKEN_BEGIN     AIR_CY_DELTAR
                dw          0           ; Mach = 0.0 CY_DELTAR
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cyr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cyr due to mach
        TOKEN_BEGIN     AIR_CY_R
                dw          0           ; Mach = 0.0 CY_R
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cyp
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cyp due to mach
        TOKEN_BEGIN     AIR_CY_P
                dw          0           ; Mach = 0.0 CY_P
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Clb
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clb due to mach
        TOKEN_BEGIN     AIR_CL_BETA
                dw          0           ; Mach = 0.0 CL_BETA
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

        
	;Integer mach table on Cldr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cldr due to mach
        TOKEN_BEGIN     AIR_CL_DELTAR
                dw          0           ; Mach = 0.0 CL_DELTAR
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Clda
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clda due to mach
        TOKEN_BEGIN     AIR_CL_DELTAA
                dw          0           ; Mach = 0.0 CL_DELTAA
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Clr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clr due to mach
        TOKEN_BEGIN     AIR_CL_R
                dw          0           ; Mach = 0.0 CL_R
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Clp
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clp due to mach
        TOKEN_BEGIN     AIR_CL_P
                dw          0           ; Mach = 0.0 CL_P
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

        
	;Integer mach table on Cnb
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnb due to mach
        TOKEN_BEGIN     AIR_CN_BETA
                dw          0           ; Mach = 0.0 CN_BETA
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

        
	;Integer mach table on Cndr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cndr due to mach
        TOKEN_BEGIN     AIR_CN_DELTAR
                dw          0           ; Mach = 0.0 CN_DELTAR
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cnda
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnda due to mach
        TOKEN_BEGIN     AIR_CN_DELTAA
                dw          0           ; Mach = 0.0 CN_DELTAA
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;Integer mach table on Cnr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnr due to mach
        TOKEN_BEGIN     AIR_CN_R
                dw          0           ; Mach = 0.0 CN_R
                dw          0           ; Mach = 0.2
                dw       1000           ; Mach = 0.4
                dw       5000           ; Mach = 0.6
                dw       6000           ; Mach = 0.8
                dw       7000           ; Mach = 1.0
                dw       6000           ; Mach = 1.2
                dw       3000           ; Mach = 1.4
                dw       3000           ; Mach = 1.6
                dw       3000           ; Mach = 1.8
                dw       3000           ; Mach = 2.0
                dw       3000           ; Mach = 2.2
                dw       3000           ; Mach = 2.4
                dw       3000           ; Mach = 2.6
                dw       3000           ; Mach = 2.8
                dw       3000           ; Mach = 3.0
                dw       3000           ; Mach = 3.2
        TOKEN_END

        
	;Integer mach table on Cnp 
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnp due to mach
        TOKEN_BEGIN     AIR_CN_P
                dw          0           ; Mach = 0.0 CN_P
                dw          0           ; Mach = 0.2
                dw          0           ; Mach = 0.4
                dw          0           ; Mach = 0.6
                dw          0           ; Mach = 0.8
                dw          0           ; Mach = 1.0
                dw          0           ; Mach = 1.2
                dw          0           ; Mach = 1.4
                dw          0           ; Mach = 1.6
                dw          0           ; Mach = 1.8
                dw          0           ; Mach = 2.0
                dw          0           ; Mach = 2.2
                dw          0           ; Mach = 2.4
                dw          0           ; Mach = 2.6
                dw          0           ; Mach = 2.8
                dw          0           ; Mach = 3.0
                dw          0           ; Mach = 3.2
        TOKEN_END

	;**** END OF MACH TABLES *********************************


	;***END OF AERODYNAMICS**********************************************************************************************
	;********************************************************************************************************************






	;********************************************************************************************************************
	;****START OF ENGINE DATA *******************************************************************************************


	;Piston engine mechanical efficiency table (max 5 entries)
	;IN:  RPM
	;OUT: Power Efficiency scalar
        TOKEN_BEGIN     AIR_61S_ENG_MECHANICAL_EFFICIENCY
                dd      5       ; Number of Entries
                REAL8           400.000000,     0.9000        ;
                REAL8           750.000000,     0.67200        ;
                REAL8           1725.000000,    0.672000        ;
                REAL8           1840.000000,    0.640000        ;
                REAL8           2300.000000,    0.640000        ;       
        TOKEN_END

	;Piston engine friction table (max 4 entries)
	;IN:  RPM
	;OUT: Torque due to Friction applied in resistance to RPM (ft-lbs)
        TOKEN_BEGIN     AIR_61S_ENGINE_FRICTION
                dd      4       ; Number of Entries
                REAL8          -300.000000,    -41.00     ;
                REAL8           300.000000,     41.00     ;
                REAL8           900.000000,     40.00     ;
                REAL8           2300.000000,    95.0      ;       
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
                REAL8   660.000000      ; Max Oil Temp (deg Rankine)
                REAL8   0.030000        ; Oil temp tc
        TOKEN_END

	;Oil Pressure
        TOKEN_BEGIN     AIR_61S_OIL_PRESSURE
                REAL8   1.000000        ; Oil pressure tuning constant
                REAL8   10800.000000    ; Oil pressure max (PSF)
                REAL8   0.800000        ; Oil pressure tc
        TOKEN_END

	;Fuel Pressure
        TOKEN_BEGIN     AIR_61S_FUEL_PRESSURE
                REAL8   1.000000        ; Fuel pressure tuning constant
                REAL8   720.000000     ; Fuel pressure max (PSF)
                REAL8   2.000000        ; Fuel pressure tc
        TOKEN_END

	;Radiator Temperature
        TOKEN_BEGIN     AIR_61S_RADIATOR_TEMPERATURE
                REAL8   1.000000        ; Radiator temp tuning constant
                REAL8   0.015000        ; Radiator temp cooling constant
                REAL8   670.000000      ; Radiator temp max (deg Rankine)
                REAL8   0.020000        ; Radiator temp tc
        TOKEN_END

	;*****END ENGINE DATA *********************************************************

	;*****START PROPELLER DATA ****************************************************

	;Propeller efficiency 2D input table (max 12 x 14 entries)
	;relates engine power input to thrust produced by prop
	;
	;First row is propeller advance ratio
	;First column is propeller blade angle
	;
	;IN:  Y: Blade Angle (degrees)
	;IN:  X: Advance Ratio,J where J = Vel (Ft/sec) / (Diameter (Ft) * Rev/Sec)
	;OUT: Efficiency, where Thrust = (EngPower / Vel) * Efficiency 
	TOKEN_BEGIN	AIR_61S_PROP_EFFICIENCY	

	UINT32	7,13	;rows,cols
	;--------------------
	REAL8	0.0,	0.00,	 0.20,	 0.40,	 0.60,	 0.80,	1.00,	 1.20,	  1.40,   1.60,   1.80,   2.00,   2.20
	REAL8	15.0,	0.15,	 0.40,	 0.71,	 0.86,	 0.72,	 0.50,	  0.34,   0.23,   0.15,   0.11,   0.08,   0.06
	REAL8	20.0,	0.10,	 0.30,	 0.62,	 0.79,	 0.86,	 0.80,	  0.55,   0.42,   0.30,   0.19,   0.12,   0.09
	REAL8	25.0,	0.08,	 0.23,	 0.49,	 0.72,	 0.82,	 0.87,	  0.82,   0.60,   0.41,   0.28,   0.18,   0.13
	REAL8	30.0,	0.07,	 0.18,	 0.33,	 0.50,	 0.72,	 0.82,	  0.87,   0.85,   0.56,   0.42,   0.26,   0.19
	REAL8	35.0,	0.06,	 0.16,	 0.26,	 0.40,	 0.55,	 0.72,	  0.82,   0.86,   0.87,   0.70,   0.40,   0.30
	REAL8	40.0,	0.05,	 0.12,	 0.23,	 0.33,	 0.45,	 0.57,	  0.70,   0.81,   0.86,   0.87,   0.85,   0.50

	TOKEN_END


	;Propeller Power Required Coefficient (max 12 x 14 entries)
	;Relates propeller speed and angle and aircraft speed to power required to turn the propeller
	;
	;First row is propeller advance ratio
	;First column is propeller blade angle
	;
	;IN:  Y: Blade Angle (degrees)
	;IN:  X: Advance Ratio,J where J = Vel (Ft/sec) / (Diameter (Ft) * Rev/Sec)
	;OUT: Power Required Coefficient, Cp
	TOKEN_BEGIN	AIR_61S_PROP_PWR_CF
	
	UINT32	7,14	;rows,cols
	;--------------------
	REAL8	 0.0,	 0.000,  0.200,  0.400,  0.600,  0.800,  1.000,  1.200,  1.400,  1.600,  1.800,  2.000,  2.200,  2.400
	REAL8	15.0,	 0.025,  0.025,  0.025,  0.022,  0.009, -0.057, -0.188, -0.338, -0.522, -0.705, -0.915, -1.092, -1.220
	REAL8	20.0,	 0.062,  0.052,  0.049,  0.042,  0.037,  0.010, -0.074, -0.188, -0.338, -0.525, -0.726, -0.942, -1.120
	REAL8	25.0,	 0.098,  0.094,  0.088,  0.080,  0.070,  0.050,  0.020, -0.040, -0.134, -0.272, -0.468, -0.717, -0.933
	REAL8	30.0,	 0.138,  0.132,  0.128,  0.120,  0.110,  0.099,  0.078,  0.040, -0.017, -0.110, -0.248, -0.468, -0.741
	REAL8	35.0,	 0.206,  0.198,  0.188,  0.178,  0.163,  0.150,  0.130,  0.105,  0.070,  0.023, -0.074, -0.254, -0.510
	REAL8	40.0,	 0.250,  0.242,  0.233,  0.223,  0.214,  0.204,  0.190,  0.175,  0.150,  0.118,  0.072,  0.019, -0.059
	TOKEN_END

	;*****END PROPELLER DATA ****************************************************



	;**** PID CONTROLLERS *******************************************************
	;The following PID controllers are utilized only by computer controlled (AI) aircraft system for heading and airspeed control
        TOKEN_BEGIN     AIR_AP_PID_CONTROLLERSF
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <2.000000, 0.000000, 0.000000, 0.000763, 2.000000, 0.000000, 20.000000> ; head hold pid: p, i, i2, d, i_boundary, i2_boundary, d_boundary
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <2.400000, 0.000840, 0.000000, 10.00000, 50.00000, 0.000000, 50.00000>  ; airspeed hold pid: p, i, i2, d, i_boundary, i2_boundary, d_boundary
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
        TOKEN_END

sim_size        equ     $ - sim_data

        end
