; Copyright (c) Lockheed Martin Corporation.  All rights reserved.
;  Sample Turbine Helicopter Flight Dynamics File 
;
; Sample Turbine Engine Helicopter Dynamics File (Secondary Type)
;
;
	include asm.inc 	; include this in ALL .asm files!
	include airtoken.inc	; definitions of token macros
	include sim1.inc	; definitions of token values

.data

sim_data	label	byte

 
	TOKEN_BEGIN	AIR_HELI_VERTICAL_TAIL
		FLOAT64   8.08		; vertical tail cross sectional area, ft**2
		FLOAT64   5.19		; vertical tail aspect ratio, dimensionless
		FLOAT64   0.0349	; vertical tail cant angle, radians
		FLOAT64 229.6		; vertical tail longitudinal position, inches
		FLOAT64   3.35		; vertical tail WL position, assumed 1/2 span, inches
		FLOAT64   0.2		; lift at zero angle of attack, CLo, nondim
		FLOAT64   0.004 	; horizontal tail drag at zero alpha
		FLOAT64   1.0		; vertical effectiveness factor due to rotor wash
	TOKEN_END


	TOKEN_BEGIN	AIR_HELI_HORIZONTAL_TAIL
		FLOAT64   9.65		; horizontal tail cross sectional area, ft**2
		FLOAT64   4.29		; horizontal tail aspect ratio, dimensionless
		FLOAT64   0.0		; radians, horizontal tail angle from vertical axis, assumed
		FLOAT64 146.2		; horizontal tail longitudinal position, inches
		FLOAT64   1.0		; horiz vertical position, approximate, inches
		REAL8	 -0.35		; lift at zero angle of attack, CLo, nondim
		FLOAT64   0.004 	; horizontal tail drag at zero alpha
		FLOAT64   0.75		; Horiz effectiveness factor due to rotor wash
	TOKEN_END


	TOKEN_BEGIN	AIR_HELI_MAIN_ROTOR
		FLOAT64   0.0		; (lat) main rotor is centered over center line
		REAL8	 -4.0		; (long) inches, main rotor fuselage station
		FLOAT64  57.6		; (vert) inches, measured from 3 view from water line 00 station
		REAL8	 -0.01745	; rads mr lat tilt, 1. degrees to left, actual slightly higher
		REAL8	 -0.01745	; rads mr long tilt, 1 degrees forward, actual slightly higher
		FLOAT64  16.625 	; main rotor radius
		FLOAT64   1.08		; main rotor blade chord length in feet
		FLOAT64   2.0		; Number blades in Main Rotor
		FLOAT64 267.75		; rotational mass moment of inertia, .333*m*r^2
		FLOAT64   0.75		; rotor hub loss factor
		FLOAT64   2.5		; main rotor section lift coefficient, per radian
		FLOAT64   0.009 	; section drag coefficient, the bucket value
		FLOAT64   0.0413	; Main solidity ratio, dimensionless - Nc/pi*R
		FLOAT64  868.31 	; Main rotor area
		FLOAT64  15.11		; Main rotor aspect ratio, dimensionless
		FLOAT64  41.36		; rad/sec, equates to 395 rpm for the main rotors
	TOKEN_END


	TOKEN_BEGIN	AIR_HELI_TAIL_ROTOR
		FLOAT64  24.0		; (lat) inches, measured from 3 view from water line 00 station
		FLOAT64 229.6		; (long) inches, tail rotor lateral
		REAL8	 -8.0		; (vert) inches, tail rotor fuselage station, inches
		FLOAT64   0.0		; radians. tail rotor lateral tilt, assumed
		FLOAT64   0.0		; radians. tail rotor longitudinal tilt, assumed
		FLOAT64   5.4167	; tail rotor radius
		FLOAT64   0.5		; tail rotor blade chord length in feet
		FLOAT64   2.0		; Number blades in Tail Rotor
		FLOAT64   1.004 	; estimated
		FLOAT64   0.60		; tail hub loss factor
		FLOAT64   4.3		; tail rotor section lift coefficient
		FLOAT64   0.009 	; section drag coefficient, the bucket value
		FLOAT64   0.1177	; tail solidity ratio, dimensionless - Nc/pi*R
	TOKEN_END


	TOKEN_BEGIN	AIR_HELI_MISCELLANOUS
		FLOAT64 29.01		; (front_area) fuselage frontal cross sectional area, ft**2
		FLOAT64 88.16		; (side_area ) fuselage side cross sectional area, ft**2
		FLOAT64 64.03		; (plane_area) fuselage top plane cross sectional area, ft**2
		FLOAT64 0.4		; (F_fuse_Cd) Front face fuselage drag coefficient (assumed)
		FLOAT64 1.8		; (S_fuse_Cd) Side face fuselage drag coefficient (assumed)
		FLOAT64 0.27		; (P_fuse_Cd) Plane face fuselage drag coefficient
	TOKEN_END


	


sim_size	equ	$ - sim_data

	end
