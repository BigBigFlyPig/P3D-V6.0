//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Fuel.c

//	Set up gauge header
char fuel_gauge_name[]		= GAUGE_NAME;
extern PELEMENT_HEADER		fuel_list;
extern MOUSERECT			fuel_mouse_rect[];

GAUGE_HEADER_FS700( GAUGE_W, fuel_gauge_name, &fuel_list, \
						fuel_mouse_rect, 0, 0, 0, 0 );


/////////////////////////////////////////////////////////////////////////////
FAILURE_RECORD	fuel_fail[] =
{
	{FAIL_SYSTEM_ELECTRICAL_PANELS, FAIL_ACTION_ZERO},
	{FAIL_GAUGE_FUEL_INDICATORS, FAIL_ACTION_FREEZE},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


/////////////////////////////////////////////////////////////////////////////
#define	GAUGE_MIN_FUEL	0
#define	GAUGE_MAX_FUEL	75

NONLINEARITY  fuel_nonlinearity[] =
{
	{{30,	182},	 0, 0},
	{{119,	 47},	25, 0},
	{{246,	 93},	50, 0},
	{{241,	221},	75, 0}
};

FLOAT64 FSAPI fuel_needle_cb( PELEMENT_NEEDLE pelement )
{
	FLOAT64	val = pelement->source_var.var_value.n;

	if (val > GAUGE_MAX_FUEL)
		val = GAUGE_MAX_FUEL;
	else if (val < GAUGE_MIN_FUEL)
		val = GAUGE_MIN_FUEL;

	return val;
}

MAKE_NEEDLE
(
	fuel_needle,
	BMP_FUEL_SMALL_NEEDLE,
	NULL,
	fuel_fail,
	IMAGE_USE_TRANSPARENCY|IMAGE_USE_ERASE|IMAGE_BILINEAR_COLOR,
	0,
	150, 150,
	6, 12,
	TOTAL_FUEL_QUANTITY_GALLONS,fuel_needle_cb,
	fuel_nonlinearity,
	6
)


PELEMENT_HEADER		fuel_needle_list[] =
{
	(PELEMENT_HEADER)&fuel_needle,
	NULL
};


/////////////////////////////////////////////////////////////////////////////
MAKE_STATIC
(
	fuel_background,
	BMP_FUEL_SMALL_BACKGROUND,
	&fuel_needle_list,
	fuel_fail,
	IMAGE_USE_TRANSPARENCY,
	0,
	9, 0
)

PELEMENT_HEADER		fuel_list	= &fuel_background.header;

/////////////////////////////////////////////////////////////////////////////
MOUSE_BEGIN( fuel_mouse_rect, HELPID_GAUGE_FUEL, 0, 0 )
MOUSE_END


/////////////////////////////////////////////////////////////////////////////
#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

