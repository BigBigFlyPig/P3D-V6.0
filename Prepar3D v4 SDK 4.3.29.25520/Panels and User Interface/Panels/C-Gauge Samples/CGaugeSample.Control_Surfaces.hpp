//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Control_Surfaces.c

//	Set up gauge header
char cs_gauge_name[]		= GAUGE_NAME;
extern PELEMENT_HEADER		cs_list;
extern MOUSERECT			cs_mouse_rect[];

GAUGE_HEADER_FS700(GAUGE_W, cs_gauge_name, &cs_list,	\
						cs_mouse_rect, 0, 0, 0, 0);


/////////////////////////////////////////////////////////////////////////////
MAKE_SLIDER
(
	cs_slider_trim,
	BMP_CS_SMALL_TRIM,
	NULL,
	0,
	IMAGE_USE_ERASE | IMAGE_USE_TRANSPARENCY,
	0,
	20,44,

	MODULE_VAR_NONE, NULL, 0,
	ELEVATOR_TRIM, NULL, -100
)

MAKE_SLIDER
(
	cs_slider_ailerons,
	BMP_CS_SMALL_AILERONS,
	NULL,
	0,
	IMAGE_USE_ERASE | IMAGE_USE_TRANSPARENCY,
	0,
	95, 6,

	AILERON_DEFLECTION, NULL, 100,
	MODULE_VAR_NONE, NULL, 0
)

MAKE_SLIDER
(
	cs_slider_elevator,
	BMP_CS_SMALL_ELEVATOR,
	NULL,
	0,
	IMAGE_USE_ERASE | IMAGE_USE_TRANSPARENCY,
	0,
	98, 46,

	MODULE_VAR_NONE, NULL, 0,
	ELEVATOR_DEFLECTION, NULL, -70
)

MAKE_SLIDER
(
	cs_slider_rudder,
	BMP_CS_SMALL_RUDDER,
	NULL,
	0,
	IMAGE_USE_ERASE | IMAGE_USE_TRANSPARENCY,
	0,
	94, 83,

	RUDDER_DEFLECTION, NULL, 80,
	MODULE_VAR_NONE, NULL, 0
)

PELEMENT_HEADER		cs_sliders_list[] =
{
	&cs_slider_trim.header,
	&cs_slider_ailerons.header,
	&cs_slider_elevator.header,
	&cs_slider_rudder.header,
	NULL
};

/////////////////////////////////////////////////////////////////////////////
MAKE_STATIC
(
	cs_background,
	BMP_CS_SMALL_BACKGROUND,
	&cs_sliders_list,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		cs_list	= &cs_background.header;


/////////////////////////////////////////////////////////////////////////////
MOUSE_BEGIN( cs_mouse_rect, HELP_NONE, 0, 0 )
	MOUSE_PARENT( 0,0,44,94,	HELPID_GAUGE_PITCH_TRIM )
	MOUSE_PARENT( 60,0,76,16,	HELPID_GAUGE_ROLL_TRIM )
	MOUSE_PARENT( 82,16,32,62,	HELPID_GAUGE_PITCH_TRIM )
	MOUSE_PARENT( 60,78,76,16,	HELPID_GAUGE_YAW_TRIM )
MOUSE_END


/////////////////////////////////////////////////////////////////////////////
#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

