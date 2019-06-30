//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Fuel_Selector.c

//	Set up gauge header
char fuel_selector_gauge_name[]		= GAUGE_NAME;
extern PELEMENT_HEADER			fuel_selector_list;
extern MOUSERECT				fuel_selector_mouse_rect[];

GAUGE_HEADER_FS700(GAUGE_W, fuel_selector_gauge_name, &fuel_selector_list,
						fuel_selector_mouse_rect, 0, 0, 0, 0);


/////////////////////////////////////////////////////////////////////////////
FLOAT64 FSAPI	fuel_selector_icon_cb( PELEMENT_ICON pelement )
{
	switch (pelement->source_var.var_value.e)
	{
		case FUEL_TANK_SELECTOR_OFF:		
			return 0;

		case FUEL_TANK_SELECTOR_LEFT:
			return 1;

		case FUEL_TANK_SELECTOR_ALL:			
			return 2;

		case FUEL_TANK_SELECTOR_RIGHT:	
			return 3;

		default:
			return 2;
	}
}

MAKE_ICON
(		
	fuel_selector_icon,
	BMP_FUEL_SELECTOR_OFF,
	NULL,
	NULL,
	IMAGE_USE_ERASE | IMAGE_USE_TRANSPARENCY,
	0,
	0,0,

	FUEL_TANK_SELECTOR,fuel_selector_icon_cb,

	ICON_SWITCH_TYPE_STEP_TO,
	4,
	0,
	0
)

PELEMENT_HEADER		fuel_selector_icon_list[] =
{
	&fuel_selector_icon.header,
	NULL
};


/////////////////////////////////////////////////////////////////////////////
MAKE_STATIC
(
	fuel_selector_background,
	BMP_FUEL_SELECTOR_OFF,
	fuel_selector_icon_list,
	NULL,
	IMAGE_USE_TRANSPARENCY  | IMAGE_NO_STATIC_BLENDING,
	0,
	0,
	0
)

PELEMENT_HEADER		fuel_selector_list	= &fuel_selector_background.header;


/////////////////////////////////////////////////////////////////////////////
MODULE_VAR		fuel_selector_mouse_var	= {FUEL_TANK_SELECTOR};

BOOL FSAPI	fuel_selector_mouse_cb( PPIXPOINT relative_point, FLAGS32 mouse_flags )
{
	lookup_var( &fuel_selector_mouse_var);

	switch (fuel_selector_mouse_var.var_value.e)
	{
		case FUEL_TANK_SELECTOR_LEFT:
			trigger_key_event( KEY_FUEL_SELECTOR_ALL, 0 );			
			break;

		case FUEL_TANK_SELECTOR_ALL:
			trigger_key_event( KEY_FUEL_SELECTOR_RIGHT, 0 );
			break;

		case FUEL_TANK_SELECTOR_RIGHT:	
			trigger_key_event( KEY_FUEL_SELECTOR_OFF, 0 );
			break;

		case FUEL_TANK_SELECTOR_OFF:	
			trigger_key_event( KEY_FUEL_SELECTOR_LEFT, 0 );
			break;

		default:		
			trigger_key_event( KEY_FUEL_SELECTOR_ALL, 0 );
			break;
	}

	return TRUE;
}

MOUSE_BEGIN( fuel_selector_mouse_rect, HELPID_GAUGE_FUEL_SELECTOR, 0, 0 )
	MOUSE_CHILD_FUNCT( 0,0,39,48, CURSOR_HAND, MOUSE_LEFTSINGLE, fuel_selector_mouse_cb )
MOUSE_END


/////////////////////////////////////////////////////////////////////////////
#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

