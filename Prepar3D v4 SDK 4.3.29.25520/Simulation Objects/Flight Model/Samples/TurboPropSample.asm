; Copyright (c) Lockheed Martin Corporation.  All rights reserved.
; Sample Turbo Prop flight dynamics file
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

        TOKEN_BEGIN	AIR_80_LIFT_PARAMS
		REAL8	0.000000	; CL spoiler
		REAL8	0.673300	; CL flaps
		REAL8	0.000000	; UNUSED
		REAL8	-0.94800	; CLih
		REAL8	-0.38900	; CLde
		REAL8	0.000000	; UNUSED
	TOKEN_END

	TOKEN_BEGIN	AIR_80_DRAG_PARAMS
		REAL8	0.023         	; CDo
		REAL8	0.080000	; CD flaps
		REAL8	0.017000	; CD gear
		REAL8	0.000000	; CD spoiler
	TOKEN_END

	TOKEN_BEGIN	AIR_80_PITCH_PARAMS
		REAL8	-3.21631	; CMih
		REAL8	-1.46480     	; CMde
		REAL8	0.000000	; CMde due to propwash
		REAL8	-7.81250	; CLq
		REAL8	4.327600	; CL adot
		REAL8	9.765600	; CM adot
		REAL8	-35.1560        ; CMq
		REAL8	0.000000	; CMq due to propwash
		REAL8	-0.02930     	; CMo
		REAL8	-0.10000	; CM flap
		REAL8	-0.70000	; CM delta trim
		REAL8	0.000000 	; CM gear
		REAL8	0.000000	; CM spoiler
	TOKEN_END

	TOKEN_BEGIN	AIR_80_SIDE_FORCE_PARAMS
		REAL8	-0.57226	; CyB
		REAL8	0.000000	; CyP
		REAL8	0.346190	; CyR
		REAL8	-0.09131	; Cy Delta Rudder
	TOKEN_END

	TOKEN_BEGIN	AIR_80_ROLL_PARAMS
		REAL8	0.0976000	; ClB
		REAL8	-1.465000	; ClP
		REAL8	-0.146480	; ClR
		REAL8	0.0000000	; Cl Delta Spoiler
		REAL8	-0.200200	; Cl Delta Aileron
		REAL8	0.0151360	; Cl Delta Rudder
	TOKEN_END

	TOKEN_BEGIN	AIR_80_YAW_PARAMS
		REAL8	0.146480	; CnB
		REAL8	0.063476	; CnP
		REAL8	-0.97650	; CnR
		REAL8	0.000000	; CnR due to propwash
		REAL8	0.000000	; UNUSED
		REAL8	0.000000	; UNUSED
		REAL8	-0.03027	; Cn Delta Aileron
		REAL8	0.050290	; Cn Delta Rudder
		REAL8	0.000000	; Cn Delta Rudder due to propwash
	TOKEN_END

	
	;CL vs. Alpha
	;The first entry defines the number of data points (maximum 47 entries)
        TOKEN_BEGIN     AIR_CL_ALPHA
			
		UINT32	13		;NUMBER OF ENTRIES

	        REAL8    -3.142,   0.000
	        REAL8    -2.356,   0.500
	        REAL8    -1.571,   0.000
	        REAL8    -0.308,  -1.085
	        REAL8    -0.029,   0.000
	        REAL8     0.000,   0.159
	        REAL8     0.244,   1.488
	        REAL8     0.279,   1.550
	        REAL8     0.314,   1.488
	        REAL8     0.349,   1.085
	        REAL8     1.571,   0.000
	        REAL8     2.356,  -0.500
	        REAL8     3.142,   0.000
                  
        TOKEN_END

	;CM vs. Alpha
	;The first entry defines the number of data points (maximum 47 entries)
        TOKEN_BEGIN     AIR_CM_ALPHA
	               
		UINT32	13		;NUMBER OF ENTRIES

	        REAL8   -3.142,	 0.000
	        REAL8   -0.541,	-0.603
	        REAL8   -0.367,	-0.603
	        REAL8   -0.297,	-0.302
	        REAL8   -0.279,	-0.251
	        REAL8   -0.262,	-0.209
	        REAL8    0.000,	 0.000
	        REAL8    0.262,	 0.209
	        REAL8    0.279,	 0.251
	        REAL8    0.297,	 0.302
	        REAL8    0.367,	 0.603
	        REAL8    0.541,	 0.603
	        REAL8    3.142,	 0.000

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
        TOKEN_BEGIN     AIR_61S_AIL_RUD_TRIM_CONSTANTS
      	      REAL8  -0.2 ;cl_delta_ailertrim
	      REAL8   0.2 ;cn_delta_rudtrim
        TOKEN_END

	;Elevator effectiveness scaling table (max 7 entries)
	; IN:  Elevator angle  (radians)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_ELEVATOR_SCALING       
        UINT32  5               ;NUMBER OF ENTRIES

        REAL8	-0.349, 1.0	;-20 DEG
	REAL8	-0.175, 1.0	;-10 DEG
	REAL8	0.0,	1.0	; 0
	REAL8	0.175,	1.0	;10 DEG
	REAL8	0.349,	1.0	;20 DEG

        TOKEN_END

	
	TOKEN_END
        ;Aileron effectiveness scaling table  (max 7 entries)
	; IN:  Aileron angle  (radians)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_AILERON_SCALING        
        UINT32  7               ;NUMBER OF ENTRIES

        REAL8   -0.785, 1.0     ;-45 DEG
        REAL8   -0.524, 1.0     ;-30 DEG
        REAL8   -0.175, 1.0     ;-10 DEG
        REAL8   0.0,    1.0
        REAL8   0.175,  1.0     ;10 DEG
        REAL8   0.524,  1.0     ;30 DEG
        REAL8   0.785,  1.0     ;45 DEG

        TOKEN_END


	;Rudder effectiveness scaling table  (max 7 entries)
	; IN:  Rudder angle  (radians)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_RUDDER_SCALING         
        UINT32  7               ;NUMBER OF ENTRIES

	REAL8	-0.785, 0.5	;-45 DEG
	REAL8	-0.524, 0.6	;-30 DEG
	REAL8	-0.175, 0.9	;-10 DEG
	REAL8	0.0,	1.0
	REAL8	0.175,	0.9	;10 DEG
	REAL8	0.524,	0.6	;30 DEG
	REAL8	0.785,	0.5	;45 DEG

        TOKEN_END

	;Elevator elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2) (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_ELEVATOR_ELASTICITY
        UINT32  5               ;NUMBER OF ENTRIES

                ;q=.5rhoV^2
	REAL8	 0.0,		1.0	;Ve = 0    keas
	REAL8	 135.0, 	1.0	;Ve = 200  keas
	REAL8	 300.0, 	1.0	;Ve = 297  keas
	REAL8	 1000.0,	0.6	;Ve = 543  keas
	REAL8	 1500.0,	0.5	;Ve = 665  keas

        TOKEN_END


	;Elevator trim elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2)  (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_70_ELEVATOR_TRIM_ELASTICITY      
        UINT32  5               ;NUMBER OF ENTRIES

                ;q=.5rhoV^2
	REAL8	 0.0,		1.0	;Ve = 0    keas
	REAL8	 300.0, 	0.8	;Ve = 297  keas
	REAL8	 800.0, 	0.2	;Ve = 485  keas
	REAL8	 1000.0,	0.2	;Ve = 543  keas
	REAL8	 1500.0,	0.2	;Ve = 665  keas

        TOKEN_END


	;Aileron elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2)  (psf)
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_AILERON_ELASTICITY 
        UINT32  5               ;NUMBER OF ENTRIES

                ;q=.5rhoV^2
	REAL8	 0.0,		1.0	;Ve = 0    keas
	REAL8	 300.0, 	0.7	;Ve = 297  keas
	REAL8	 600.0, 	0.2	;Ve = 420  keas
	REAL8	 1482.0,	0.1	;Ve = 760 keas
	REAL8	 1500.0,        0.1	;Ve = 665  keas

        TOKEN_END

        ;Rudder elasticity on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft dynamic pressure ( 1/2 Rho * V^2)  (psf)
	; OUT: Effectiveness scalar
	TOKEN_BEGIN     AIR_61S_RUDDER_ELASTICITY            
        UINT32  5               ;NUMBER OF ENTRIES

                ;q=.5rhoV^2
	REAL8	 0.0,		1.0	;Ve = 0    keas
	REAL8	 300.0, 	1.0	;Ve = 297  keas
	REAL8	 800.0, 	0.5	;Ve = 485  keas
	REAL8	 1000.0,	0.3	;Ve = 543  keas
	REAL8	 1500.0,	0.2	;Ve = 665  keas

        TOKEN_END

       	;Load factor on effectiveness scaling table  (max 5 entries)
	; IN:  Aircraft load factor 
	; OUT: Effectiveness scalar
        TOKEN_BEGIN     AIR_61S_AILERON_LOAD_FACTOR_EFF
                dd      5       ; Number of Entries
                REAL8           0.000000,       1.0        ;
                REAL8           3.000000,       1.0        ;
                REAL8           4.000000,       1.0        ;
                REAL8           6.000000,       1.0        ;
                REAL8           8.000000,       1.0        ;       
        TOKEN_END

       	;**** END OF ADDITIONAL CONTROL EFFECTS *************


	;**** START OF ANGLE OF ATTACK TABLES ***************

	;Cl (roll) induced by angle of attack  (max 7 entries)
	;IN: Angle of attack (degrees)
	;OUT: Delta Cl
	TOKEN_BEGIN	AIR_70S_Cl_ALPHA_ROLL

	UINT32	7		;NUMBER OF ENTRIES

        REAL8    -17.0,	 0.00
        REAL8    -10.0,	 0.00
        REAL8     -6.0,	 0.00
        REAL8     10.0,	 0.00
        REAL8     12.0,	 0.00
        REAL8     13.0,	 0.00
        REAL8     17.0,	 0.00

	TOKEN_END

	;Cn (yaw) induced by angle of attack  (max 7 entries)
	; IN: Angle of attack (degrees)
	; OUT: Delta Cn
	TOKEN_BEGIN	AIR_70S_CN_ALPHA_YAW

	UINT32	7		;NUMBER OF ENTRIES

        REAL8    -16.0,	 0.00
        REAL8    -13.0,	 0.00
        REAL8    -10.0,	 0.00
        REAL8     10.0,	 0.00
        REAL8     11.0,	 0.00
        REAL8     13.0,	 0.00
        REAL8     15.0,	 0.00

	TOKEN_END
        
      	;Scalar on Cmde due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmde
	TOKEN_BEGIN	AIR_61S_ALPHA_ON_CMDE
	dd      5       ; Number of Entries
	REAL8  -25.0,	1.0
	REAL8  -12.0,	1.0
	REAL8	12.0,	1.0
	REAL8	16.0,	1.0
	REAL8	25.0,	1.0
	TOKEN_END

	;Scalar on Cmih due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmih
	TOKEN_BEGIN	AIR_61S_ALPHA_ON_CMIH
        dd      5       ; Number of Entries
	REAL8  -15.0,	1.0
	REAL8	 0.0,	1.0
	REAL8	13.0,	1.0
	REAL8	14.0,	1.0
	REAL8	25.0,	1.0
	TOKEN_END

	;Scalar on Cmadot due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmadot
        TOKEN_BEGIN     AIR_61S_ALPHA_ON_CMADOT
        dd      5       ; Number of Entries
	REAL8  -15.0,	1.0
	REAL8	 0.0,	1.0
	REAL8	 5.0,	1.0
	REAL8	16.0,	1.0
	REAL8	25.0,	1.0
	TOKEN_END

	;Scalar on Cmq due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmq
        TOKEN_BEGIN     AIR_61S_ALPHA_ON_CMQ
        dd      5       ; Number of Entries
	REAL8  -15.0,	1.0
	REAL8	 0.0,	1.0
	REAL8	11.0,	1.0
	REAL8	16.0,	0.7
	REAL8	25.0,	0.7
	TOKEN_END


	;Scalar on Cndr due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cndr
        TOKEN_BEGIN     AIR_70S_ALPHA_ON_CNDR
        dd      5       ; Number of Entries
	REAL8  -15.0,	1.0
	REAL8	 0.0,	1.0
	REAL8	13.0,	1.0
	REAL8	16.0,	1.0
	REAL8	25.0,	1.0

	TOKEN_END

	;Scalar on Clda due to angle of attack  (max 5 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Clda
        TOKEN_BEGIN     AIR_70S_ALPHA_ON_CLDA
        dd      5       ; Number of Entries
	REAL8  -15.0,	1.0
	REAL8	 0.0,	1.0
	REAL8	13.0,	1.0
	REAL8	16.0,	1.0
	REAL8	25.0,	1.0

	TOKEN_END

	;Scalar on ClBeta due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on ClBeta
	TOKEN_BEGIN     AIR_ALPHA_ON_CL_BETA
        dd      9       ; Number of Entries
	REAL8  -180.0,	1.0
	REAL8	-90.0,	0.2
	REAL8	-16.0,	0.2
	REAL8	-10.0,	0.8
	REAL8	  0.0,	1.0
	REAL8	 10.0,	0.8
	REAL8	 16.0,	0.2
	REAL8	 90.0,	0.2
	REAL8	180.0,	1.0

	TOKEN_END

	;Scalar on Clp due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cmp
	TOKEN_BEGIN     AIR_ALPHA_ON_CLP
        dd      9       ; Number of Entries
	REAL8  -180.0,	1.0
	REAL8	-90.0,	0.3
	REAL8	-16.0,	0.2
	REAL8	-10.0,	0.6
	REAL8	  0.0,	1.0
	REAL8	 10.0,	0.6
	REAL8	 16.0,	0.2
	REAL8	 90.0,	0.3
	REAL8	180.0,	1.0

	TOKEN_END

	;Scalar on CnBeta due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on CnBeta
	TOKEN_BEGIN     AIR_ALPHA_ON_CN_BETA
        dd      9       ; Number of Entries
	REAL8  -180.0,	1.0
	REAL8	-90.0,	0.0
	REAL8	-16.0,	0.2
	REAL8	-10.0,	0.3
	REAL8	  0.0,	1.0
	REAL8	 10.0,	0.3
	REAL8	 16.0,	0.2
	REAL8	 90.0,	0.0
	REAL8	180.0,	1.0

	TOKEN_END

	;Scalar on Cnr due to angle of attack  (max 9 entries)
	; IN:  Angle of attack (degrees)
	; OUT: Scalar on Cnr
	TOKEN_BEGIN     AIR_ALPHA_ON_CNR
        dd      9       ; Number of Entries
	REAL8  -180.0,	1.0
	REAL8	-90.0,	0.1
	REAL8	-16.0,	0.2
	REAL8	-10.0,	0.4
	REAL8	  0.0,	1.0
	REAL8	 10.0,	0.4
	REAL8	 16.0,	0.2
	REAL8	 90.0,	0.1
	REAL8	180.0,	1.0

	TOKEN_END

	;**** END OF ANGLE OF ATTACK TABLES ***************


	;**** START OF MACH TABLES ***************        
 		

        	;Scalar on Lift due to mach (max 17 entries)
	;IN:  Mach
	;OUT: Scalar on CL0
        TOKEN_BEGIN     AIR_CL_MACH
        	UINT32  17

	        REAL8         0.0,    1.0000
	        REAL8         0.2,    1.0000
	        REAL8         0.4,    1.0369
	        REAL8         0.6,    1.1018
	        REAL8         0.8,    1.2687
	        REAL8         1.0,    0.6000
	        REAL8         1.2,    0.5000
	        REAL8         1.4,    0.4000
	        REAL8         1.6,    0.4000
	        REAL8         1.8,    0.4000
	        REAL8         2.0,    0.4000
	        REAL8         2.2,    0.3968
	        REAL8         2.4,    0.3571
	        REAL8         2.6,    0.3373
	        REAL8         2.8,    0.3274
	        REAL8         3.0,    0.3274
	        REAL8         3.2,    0.3274

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
	TOKEN_BEGIN	AIR_CL_DELTAE
	 	dw	    0		;0.0		
		dw	    0		;0.2		
		dw	  -21		;0.4		
		dw	  -82		;0.6		
		dw	 -246		;0.8		
		dw	 -246		;1.0		
		dw	 -205		;1.2		
		dw	 -164		;1.4		
		dw	 -123		;1.6		
		dw	  -82		;1.8		
		dw	  -41		;2.0		
		dw	  -41		;2.2		
		dw	  -41		;2.4		
		dw	  -41		;2.6		
		dw	  -41		;2.8		
		dw	  -41		;3.0		
		dw	  -41		;3.2		
	TOKEN_END

	;Integer mach table on Cladot
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cladot due to mach
	TOKEN_BEGIN	AIR_CL_ADOT
	 	dw	    0		;0.0		 
		dw	    0		;0.2		
		dw	  410		;0.4		
		dw	 1024		;0.6		
		dw	 2253		;0.8		
		dw	 3482		;1.0		
		dw	 2253		;1.2		
		dw	 1024		;1.4		
		dw	  410		;1.6		
		dw	  410		;1.8		
		dw	  410		;2.0		
		dw	  410		;2.2		
		dw	  410		;2.4		
		dw	  410		;2.6		
		dw	  410		;2.8		
		dw	  410		;3.0		
		dw	  410		;3.2		
	TOKEN_END

       	;Integer mach table on CLq
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta CLq due to mach
	TOKEN_BEGIN	AIR_CL_Q
	 	dw	    0		;0.0		 
		dw	    0		;0.2		
		dw	 -410		;0.4		
		dw	-1024		;0.6		
		dw	-2458		;0.8		
		dw	-2458		;1.0		
		dw	-1024		;1.2		
		dw	 -410		;1.4		
		dw	    0		;1.6		
		dw	    0		;1.8		
		dw	    0		;2.0		
		dw	    0		;2.2		
		dw	    0		;2.4		
		dw	    0		;2.6		
		dw	    0		;2.8		
		dw	    0		;3.0		
		dw	    0		;3.2		
	TOKEN_END

        ;Integer mach table on Clih
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clih due to mach
        TOKEN_BEGIN     AIR_CL_IH
		dw	    0		;0.0		 
		dw	    0		;0.2		
		dw	   20		;0.4		
		dw	   82		;0.6		
		dw	  328		;0.8		
		dw	  328		;1.0		
		dw	  328		;1.2		
		dw	  328		;1.4		
		dw	  328		;1.6		
		dw	  328		;1.8		
		dw	  328		;2.0		
		dw	  328		;2.2		
		dw	  328		;2.4		
		dw	  328		;2.6		
		dw	  328		;2.8		
		dw	  328		;3.0		
		dw	  328		;3.2		
	TOKEN_END

        ;Integer mach table on Cmde
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmde due to mach
	TOKEN_BEGIN     AIR_CM_DELTAE
		dw	    0		;0.0	        
		dw	    0		;0.2	       
		dw	    0		;0.4	       
		dw	  -33		;0.6	       
		dw	  -99		;0.8	       
		dw	 -207		;1.0	       
		dw	 -205		;1.2	       
		dw	 -102		;1.4	       
		dw	    0		;1.6	       
		dw	   51		;1.8	       
		dw	  102		;2.0	       
		dw	  102		;2.2	       
		dw	  102		;2.4	       
		dw	  102		;2.6	       
		dw	  102		;2.8	       
		dw	  102		;3.0	       
		dw	  102		;3.2	       
	TOKEN_END

	;Integer mach table on Cmadot
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmadot due to mach
        TOKEN_BEGIN     AIR_CM_ADOT
		dw	    0		;0.0	        
		dw	   51		;0.2	       
		dw	  205		;0.4	       
		dw	  563		;0.6	       
		dw	 1536		;0.8	       
		dw	 2048		;1.0	       
		dw	    0		;1.2	       
		dw	-6144		;1.4	       
		dw	-8192		;1.6	       
		dw	-9216		;1.8	       
		dw     -10240		;2.0		
		dw     -10240		;2.2		
		dw     -10240		;2.4		
		dw     -10240		;2.6		
		dw     -10240		;2.8		
		dw     -10240		;3.0		
		dw     -10240		;3.2		
	TOKEN_END

 	;Integer mach table on Cmq
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmq due to mach
        TOKEN_BEGIN     AIR_CM_Q
		dw	    0		;0.0	       
		dw	-1229		;0.2	      
		dw	-2458		;0.4	      
		dw	-3686		;0.6	      
		dw	-5222		;0.8	      
		dw	-6758		;1.0	      
		dw	-5068		;1.2	      
		dw	-3379		;1.4	      
		dw	-1690		;1.6	      
		dw	    0		;1.8	      
		dw	 1024		;2.0	      
		dw	 2048		;2.2	      
		dw	 2048		;2.4	      
		dw	 2048		;2.6	      
		dw	 2048		;2.8	      
		dw	 2048		;3.0	      
		dw	 2048		;3.2	      
	TOKEN_END

        ;Integer mach table on Cmih
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cmih due to mach
	TOKEN_BEGIN     AIR_CM_IH
		dw	    0		;0.0		 
		dw	  102		;0.2		
		dw	  205		;0.4		
		dw	  410		;0.6		
		dw	 1024		;0.8		
		dw	  717		;1.0		
		dw	  717		;1.2		
		dw	  717		;1.4		
		dw	  717		;1.6		
		dw	  717		;1.8		
		dw	  717		;2.0		
		dw	  717		;2.2		
		dw	  717		;2.4		
		dw	  717		;2.6		
		dw	  717		;2.8		
		dw	  717		;3.0		
		dw	  717		;3.2		
	TOKEN_END


 
       	;Integer mach table on Cm0
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cm0 due to mach
	TOKEN_BEGIN     AIR_CMO
		dw	    0		;0.0
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    2		;0.6
		dw	    6		;0.8
		dw	    6		;1.0
		dw	    6		;1.2
		dw	    6		;1.4
		dw	    6		;1.6
		dw	    6		;1.8
		dw	    6		;2.0
		dw	    6		;2.2
		dw	    6		;2.4
		dw	    6		;2.6
		dw	    6		;2.8
		dw	    6		;3.0
		dw	    6		;3.2
	TOKEN_END

        ;Integer mach table on Cyb
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cyb due to mach
	TOKEN_BEGIN     AIR_CY_BETA
		dw	    0		;0.0		
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

	;Integer mach table on Cydr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cydr due to mach
        TOKEN_BEGIN     AIR_CY_DELTAR
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	  -10		;0.8
		dw	  -10		;1.0
		dw	  -10		;1.2
		dw	  -10		;1.4
		dw	  -10		;1.6
		dw	  -10		;1.8
		dw	  -10		;2.0
		dw	  -10		;2.2
		dw	  -10		;2.4
		dw	  -10		;2.6
		dw	  -10		;2.8
		dw	  -10		;3.0
		dw	  -10		;3.2
	TOKEN_END

	;Integer mach table on Cyr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cyr due to mach
        TOKEN_BEGIN     AIR_CY_R
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

	;Integer mach table on Cyp
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cyp due to mach
        TOKEN_BEGIN     AIR_CY_P
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

	;Integer mach table on Clb
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clb due to mach
        TOKEN_BEGIN     AIR_CL_BETA
		dw	    0		;0.0		
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END



	;Integer mach table on Cldr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cldr due to mach
        TOKEN_BEGIN     AIR_CL_DELTAR
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

	;Integer mach table on Clda
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clda due to mach
        TOKEN_BEGIN     AIR_CL_DELTAA
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	  -10		;0.4
		dw	  -21		;0.6
		dw	  -41		;0.8
		dw	  -51		;1.0
		dw	  -51		;1.2
		dw	  -51		;1.4
		dw	  -51		;1.6
		dw	  -51		;1.8
		dw	  -51		;2.0
		dw	  -51		;2.2
		dw	  -51		;2.4
		dw	  -51		;2.6
		dw	  -51		;2.8
		dw	  -51		;3.0
		dw	  -51		;3.2
	TOKEN_END

	;Integer mach table on Clr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clr due to mach
        TOKEN_BEGIN     AIR_CL_R
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

	;Integer mach table on Clp
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Clp due to mach
        TOKEN_BEGIN     AIR_CL_P
		dw	    0		;0.0		
		dw	  -10		;0.2
		dw	  -41		;0.4
		dw	 -102		;0.6
		dw	 -205		;0.8
		dw	 -225		;1.0
		dw	 -225		;1.2
		dw	 -225		;1.4
		dw	 -225		;1.6
		dw	 -225		;1.8
		dw	 -225		;2.0
		dw	 -225		;2.2
		dw	 -225		;2.4
		dw	 -225		;2.6
		dw	 -225		;2.8
		dw	 -225		;3.0
		dw	 -225		;3.2
	TOKEN_END


	;Integer mach table on Cnb
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnb due to mach
        TOKEN_BEGIN     AIR_CN_BETA
	 	dw	    0		;0.0		
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END


	;Integer mach table on Cndr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cndr due to mach
        TOKEN_BEGIN     AIR_CN_DELTAR
		dw	    0		;0.0		
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

	;Integer mach table on Cnda
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnda due to mach
        TOKEN_BEGIN     AIR_CN_DELTAA
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	   -2		;0.4
		dw	   -4		;0.6
		dw	   -6		;0.8
		dw	   -6		;1.0
		dw	   -6		;1.2
		dw	   -6		;1.4
		dw	   -6		;1.6
		dw	   -6		;1.8
		dw	   -6		;2.0
		dw	   -6		;2.2
		dw	   -6		;2.4
		dw	   -6		;2.6
		dw	   -6		;2.8
		dw	   -6		;3.0
		dw	   -6		;3.2
	TOKEN_END

	;Integer mach table on Cnr
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnr due to mach
        TOKEN_BEGIN     AIR_CN_R
		dw	    0		;0.0		
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END

 
	;Integer mach table on Cnp 
	;IN:  Mach index  (see Guidlines on Mach Integer Tables)
	;OUT: Delta Cnp due to mach
        TOKEN_BEGIN     AIR_CN_P
		dw	    0		;0.0		 
		dw	    0		;0.2
		dw	    0		;0.4
		dw	    0		;0.6
		dw	    0		;0.8
		dw	    0		;1.0
		dw	    0		;1.2
		dw	    0		;1.4
		dw	    0		;1.6
		dw	    0		;1.8
		dw	    0		;2.0
		dw	    0		;2.2
		dw	    0		;2.4
		dw	    0		;2.6
		dw	    0		;2.8
		dw	    0		;3.0
		dw	    0		;3.2
	TOKEN_END


	;**** END OF MACH TABLES *********************************


	;***END OF AERODYNAMICS**********************************************************************************************
	;********************************************************************************************************************




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
	REAL8	0.0,	  1.0, 	 	 4.237  ; Pressure Ratio (Psl/P)
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
	REAL8	40.0,	0.031
	REAL8  101.5,	1.9

	TOKEN_END

	;Atmospheric density on shaft torque (max 10 entries))
	;IN:  Ambient density (slug/ft^3)
	;OUT: Scalar on corrected shaft torque
	TOKEN_BEGIN	AIR_80_DENSITY_ON_TP_TORQUE
        UINT32 10
        
        REAL8   0.000225,   0.000  ;60k feet
        REAL8   0.000587,   0.800  ;40k feet
        REAL8   0.000739,   0.830  ;35k feet
        REAL8   0.000891,   0.890  ;30k feet
        REAL8   0.001144,   0.930  ;25k feet
        REAL8   0.001268,   0.970  ;20k feet
        REAL8   0.001496,   1.000  ;15k feet
        REAL8   0.001756,   1.025  ;10k feet
        REAL8   0.002049,   1.050  ;5k feet
        REAL8   0.002377,   1.000  ;0k feet
        
	TOKEN_END

	;Engine RPM vs. Friction Torque table (max 4 entries)
	;IN:  Percent engine RPM
	;OUT: Torque due to Friction expressed as a percentage of max torque
	TOKEN_BEGIN	AIR_61S_ENGINE_FRICTION

	UINT32	4

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


	UINT32	12,13	;rows,cols
	;--------------------
	REAL8	0.0 ,	0.0  ,	0.2  ,	0.4  ,	0.6  ,	0.8  ,	1.0  ,	1.2  ,	1.4  ,	1.6  ,	1.8  ,	2.0  ,	2.2
	REAL8	10.0,   0.000,  0.021,  0.046,  0.155,  0.619,  0.700,  0.598,  0.456,  0.400,  0.343,  0.271,  0.215
	REAL8	15.0,	0.380,	0.542,	0.670,	0.790,	0.500,	0.403,	0.360,	0.320,	0.290,	0.253,	0.210,	0.164
	REAL8	20.0,	0.081,	0.280,	0.560,	0.760,	0.860,	0.625,	0.552,	0.498,	0.425,	0.361,	0.307,	0.255
	REAL8	25.0,	0.050,	0.200,	0.425,	0.650,	0.800,	0.883,	0.750,	0.674,	0.595,	0.509,	0.425,	0.332
	REAL8	30.0,	0.040,	0.160,	0.325,	0.505,	0.680,	0.820,	0.890,	0.870,	0.748,	0.636,	0.542,	0.449
	REAL8	35.0,	0.030,	0.130,	0.270,	0.400,	0.550,	0.690,	0.810,	0.885,	0.909,	0.700,	0.606,	0.523
	REAL8	40.0,	0.020,	0.100,	0.210,	0.330,	0.440,	0.560,	0.690,	0.790,	0.865,	0.904,	0.900,	0.712
	REAL8	45.0,	0.010,	0.109,	0.179,	0.269,	0.371,	0.478,	0.573,	0.660,	0.760,	0.831,	0.881,	0.906
	REAL8	50.0,	0.000,	0.081,	0.155,	0.240,	0.313,	0.397,	0.471,	0.552,	0.629,	0.720,	0.785,	0.840
	REAL8	55.0,	0.000,	0.067,	0.135,	0.206,	0.283,	0.353,	0.427,	0.506,	0.571,	0.640,	0.710,	0.743
	REAL8	70.0,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000
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

	UINT32 12,14	;rows,cols
	;--------------------

        REAL8    0.0,	0.0000,	0.2000,	0.4000,	 0.6000,   0.8000,   1.0000,	1.2000,	 1.4000,  1.6000,  1.8000,  2.00  ,  2.2000,  2.4000
        REAL8   10.0,	0.0600,	0.0450,	0.0250,	-0.0600,  -0.2360,  -0.7500,   -1.5800, -2.6000, -3.5700, -4.8200, -6.0500, -7.1700, -8.3900
        REAL8   15.0,	0.1200,	0.0950,	0.0700,	 0.0350,  -0.0500,  -0.3780,   -1.0800, -1.7800, -2.6100, -3.4500, -4.3900, -5.4500, -6.6000
        REAL8   20.0,	0.2480,	0.2250,	0.1800,	 0.1420,   0.0890,  -0.0320,   -0.2500, -0.8060, -1.6300, -2.3900, -3.2000, -4.0600, -5.0000
        REAL8   25.0,	0.3540,	0.3430,	0.3090,	 0.2630,   0.2220,   0.1460,	0.0000,	-0.1670, -0.5670, -1.2600, -2.0200, -2.9400, -3.7800
        REAL8   30.0,   0.4720, 0.4680, 0.4610,  0.4300,   0.3920,   0.3470,    0.2900,  0.1650,  0.0000, -0.3560, -0.7600, -1.3500, -2.2800
        REAL8   35.0,   0.6050, 0.6080, 0.5860,  0.5590,   0.5550,   0.5360,    0.5060,  0.4610,  0.4040,  0.3050,  0.1270, -0.2340, -1.6300
        REAL8   40.0,   0.7680, 0.7530, 0.7370,  0.7220,   0.7220,   0.7000,    0.6770,  0.6540,  0.6120,  0.5590,  0.4830,  0.3850,  0.0770
        REAL8   45.0,   0.9120, 0.9080, 0.8890,  0.8890,   0.8890,   0.8890,    0.8930,  0.8780,  0.8510,  0.8100,  0.7340,  0.6500,  0.5290
        REAL8   50.0,	1.0300,	1.0300,	1.0300,	 1.0300,   1.0300,   1.0300,	1.0300,	 1.0300,  1.0070,  0.9610,  0.9000,  0.8170,  0.6840
        REAL8   55.0,	1.1170,	1.1170,	1.1170,	 1.1170,   1.1170,   1.1170,	1.1170,	 1.1170,  1.1170,  1.1170,  1.1170,  1.1170,  1.1170
        REAL8   60.0,	1.2610,	1.2610,	1.2610,	 1.2610,   1.2610,   1.2610,	1.2610,	 1.2610,  1.2610,  1.2610,  1.2610,  1.2610,  1.2610

	TOKEN_END

	;*****END PROPELLER DATA ****************************************************



	;**** PID CONTROLLERS *******************************************************
	;The following PID controllers are utilized only by computer controlled (AI) aircraft system for heading and airspeed control

	TOKEN_BEGIN     AIR_AP_PID_CONTROLLERSF
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
		AIRFILE_PID	<1.8,      0.0,      0.0,      1.5,	 2.0,      0.0,      20.0    >	; head_hold pid_p, i, i2, d, i_boundary, i2_boundary, d_boundary
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
		AIRFILE_PID	<0.35,     0.002,    0.0,      7.0,	 10.0,     0.0,      80.0    >	; airspeed_hold pid_p, i, i2, d, i_boundary, i2_boundary, d_boundary
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
                AIRFILE_PID     <0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000>  ; UNUSED
        TOKEN_END



	TOKEN_END


sim_size	equ	$ - sim_data

	end
