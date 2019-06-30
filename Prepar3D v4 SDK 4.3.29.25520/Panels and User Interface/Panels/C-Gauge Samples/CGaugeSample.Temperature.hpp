//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Temperature.c

//	Set up gauge header
char temperature_gauge_name[]		= GAUGE_NAME;
extern PELEMENT_HEADER		temperature_list;
extern MOUSERECT			temperature_mouse_rect[];

GAUGE_HEADER_FS700(GAUGE_W, temperature_gauge_name, &temperature_list, \
						temperature_mouse_rect, 0, 0, 0, 0);


#define GAUGE_CHARSET				DEFAULT_CHARSET
#define GAUGE_FONT_DEFAULT			"Courier New"
#define GAUGE_WEIGHT_DEFAULT		FW_NORMAL


/////////////////////////////////////////////////////////////////////////////
FAILURE_RECORD	temperature_fail[] =
{
	{FAIL_SYSTEM_ELECTRICAL_PANELS, FAIL_ACTION_NO_DRAW},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


/////////////////////////////////////////////////////////////////////////////
FLOAT64 FSAPI	temperature_string_cb( PELEMENT_STRING pelement )
{
	FLOAT64		val = pelement->source_var[0].var_value.n;

	val *= 256;

	if( pelement->source_var[1].var_value.n == ENGLISH_UNITS )
		val = val*9/5 +32;

	// limit var
	if ( val > 999 )
		val = 999.0;
	else if( val < -99 )
		val = -99;

	wsprintf(pelement->string, "%3d", (UINT32)val);

	return val;
}

MAKE_STRING
(
	temperature_string,
	NULL,
	temperature_fail,
	IMAGE_USE_ERASE | IMAGE_USE_BRIGHT | IMAGE_USE_TRANSPARENCY,
	0,
	28, 9,
	60, 29,
	3,

	TOTAL_AIR_TEMP,
	DISPLAY_UNITS,
	MODULE_VAR_NONE,
	RGB(255,0,0),
	RGB(0,0,0),
	RGB(92,92,92),
	GAUGE_FONT_DEFAULT,
	GAUGE_WEIGHT_DEFAULT,
	GAUGE_CHARSET,
	0,
	DT_CENTER | DT_VCENTER | DT_SINGLELINE,
	NULL,
	temperature_string_cb
)


FLOAT64 FSAPI	temperature_cf_cb( PELEMENT_ICON pelement )
{
	if( pelement->source_var.var_value.n == ENGLISH_UNITS )
		return 0;
	else
		return 1;
}

MAKE_ICON
(
	temperature_cf,
	BMP_TEMPERATURE_SMALL_F,
	NULL,
	temperature_fail,
	0,
	0,
	99, 9,
	DISPLAY_UNITS, temperature_cf_cb,
	ICON_SWITCH_TYPE_SET_CUR_ICON,
	2,
	0,
	0
)

PELEMENT_HEADER		temperature_icon_list[] =
{
	&temperature_string.header,
	&temperature_cf.header,
	NULL
};


/////////////////////////////////////////////////////////////////////////////
MAKE_STATIC
(
	temperature_background,
	BMP_TEMPERATURE_SMALL_BACKGROUND,
	&temperature_icon_list,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		temperature_list	= &temperature_background.header;


MOUSE_BEGIN( temperature_mouse_rect, HELPID_CONCORDE_TEMPERATURE_TOTAL, 0, 0 )
MOUSE_END


/////////////////////////////////////////////////////////////////////////////
#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

