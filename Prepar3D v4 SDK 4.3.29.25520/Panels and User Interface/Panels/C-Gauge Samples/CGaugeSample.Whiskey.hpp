//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Whiskey.cpp

//  Set up gauge header
char whiskey_gauge_name[]        = GAUGE_NAME;
extern PELEMENT_HEADER      whiskey_list;
extern MOUSERECT                whiskey_mouse_rect[];

GAUGE_HEADER_FS700(GAUGE_W, whiskey_gauge_name, &whiskey_list, \
                        whiskey_mouse_rect, 0, 0, 0, 0);


/////////////////////////////////////////////////////////////////////////////
FAILURE_RECORD  whiskey_fail[] =
{
    {FAIL_GAUGE_MAGNETIC_COMPASS, FAIL_ACTION_FREEZE},
    {FAIL_NONE, FAIL_ACTION_NONE}
};


/////////////////////////////////////////////////////////////////////////////
FLOAT64 FSAPI   whiskey_moving_card_x_cb( PELEMENT_MOVING_IMAGE pelement )
{
    FLOAT64     val = 360.0 - pelement->source_var_x.var_value.n;

    if( val >= 0 && val <= 360 )
        return val;
    else
        return 0;
}

FLOAT64 FSAPI   whiskey_moving_card_y_cb( PELEMENT_MOVING_IMAGE pelement )
{
    return 1.0;
}

MAKE_MOVING
(
    whiskey_moving_card,
    BMP_COMPASS_SMALL_CARD,
    NULL,
    whiskey_fail,
    IMAGE_USE_ERASE | IMAGE_USE_TRANSPARENCY,
    0,
    22,41,

    WHISKEY_COMPASS_DEGREES, whiskey_moving_card_x_cb,
    0, 360,

    MODULE_VAR_NONE, whiskey_moving_card_y_cb,
    0, 0
)


PELEMENT_HEADER     whiskey_moving_card_list[2] =
{
    &whiskey_moving_card.header,
    NULL
};

MAKE_STATIC
(
    whiskey_background,
    BMP_COMPASS_SMALL_BACKGROUND,
    &whiskey_moving_card_list,
    NULL,
    IMAGE_USE_TRANSPARENCY,
    0,
    0,0
)

PELEMENT_HEADER     whiskey_list = &whiskey_background.header;


/////////////////////////////////////////////////////////////////////////////
MOUSE_BEGIN( whiskey_mouse_rect, HELPID_GAUGE_WHISKEY_COMPASS, 0, 0 )
MOUSE_END


/////////////////////////////////////////////////////////////////////////////
#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

