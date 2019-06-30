//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Attitude.c

//	Set up gauge header
char attitude_gauge_name[]		= GAUGE_NAME;
extern PELEMENT_HEADER		attitude_list;
extern MOUSERECT			attitude_mouse_rect[];

GAUGE_HEADER_FS700(GAUGE_W, attitude_gauge_name, &attitude_list, \
						attitude_mouse_rect, 0, 0, 0, 0);


/////////////////////////////////////////////////////////////////////////////
FAILURE_RECORD	attitude_fail[] =
{
	{FAIL_GAUGE_ATTITUDE, FAIL_ACTION_NONE},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


/////////////////////////////////////////////////////////////////////////////
MAKE_SPRITE
(
	attitude_sprite_outer,
	BMP_ATTITUDE_SMALL_CARD2,
	NULL,
	attitude_fail,
	IMAGE_USE_TRANSPARENCY|IMAGE_BILINEAR_COLOR,
	0,
	16, 13,
	137,134,
	1.05, 1.05,
	MODULE_VAR_NONE, NULL, 0,
	MODULE_VAR_NONE, NULL, 0,
	ATTITUDE_INDICATOR_BANK_DEGREES, NULL, -1
)

PELEMENT_HEADER		attitude_sprite_outer_list[] =
{
	&attitude_sprite_outer.header,
	NULL
};


/////////////////////////////////////////////////////////////////////////////
#define	PITCH_LIMIT		43.0		// stop at 43 degrees

FLOAT64	FSAPI	attitude_sprite_inner_cb( PELEMENT_SPRITE pelement )
{
	FLOAT64		val = pelement->source_var_y.var_value.n;

	if( val > PITCH_LIMIT )
		val = PITCH_LIMIT;
	else if( val < -PITCH_LIMIT )
		val = -PITCH_LIMIT;

	return val;
}

MAKE_SPRITE
(
	attitude_sprite_inner,
	BMP_ATTITUDE_SMALL_CARD1,
	attitude_sprite_outer_list,
	attitude_fail,
	IMAGE_USE_TRANSPARENCY|IMAGE_BILINEAR_COLOR,
	0,
	55, 53,
	137, 134,
	0.55, 0.55,
	MODULE_VAR_NONE, NULL, 0,
	ATTITUDE_INDICATOR_PITCH_DEGREES, attitude_sprite_inner_cb, 2.0,
	ATTITUDE_INDICATOR_BANK_DEGREES, NULL, -1
)

PELEMENT_HEADER		attitude_sprite_inner_list[] =
{
	&attitude_sprite_inner.header,
	NULL
};


/////////////////////////////////////////////////////////////////////////////
MAKE_STATIC
(
	attitude_background,
	BMP_ATTITUDE_SMALL_BACKGROUND,
	attitude_sprite_inner_list,
	attitude_fail,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		attitude_list	= &attitude_background.header;


/////////////////////////////////////////////////////////////////////////////
MOUSE_BEGIN( attitude_mouse_rect, HELPID_GAUGE_ATTITUDE_INDICATOR, 0, 0 )
MOUSE_END


/////////////////////////////////////////////////////////////////////////////
#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

