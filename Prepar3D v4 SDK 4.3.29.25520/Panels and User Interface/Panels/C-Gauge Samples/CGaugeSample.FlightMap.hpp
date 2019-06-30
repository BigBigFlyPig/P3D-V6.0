//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.FlightMap.c

//	Set up gauge header
char flightmap_gauge_name[]	= GAUGE_NAME;
extern PELEMENT_HEADER flightmap_list;
extern MOUSERECT flightmap_rect[];

GAUGE_CALLBACK FlightMapCallback;

GAUGE_HEADER_FS700(GAUGE_W, flightmap_gauge_name, &flightmap_list, flightmap_rect, FlightMapCallback, 0, 0, 0);


/////////////////////////////////////////////////////////////////////////////
FAILURE_RECORD	flightmap_fail[] =
{
	{FAIL_SYSTEM_ELECTRICAL_AVIONICS, FAIL_ACTION_NO_DRAW},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


#define MAP_LEFT_BORDER 25
#define MAP_TOP_BORDER  35

MAKE_STATIC
(
	flightmap_image,
	BMP_FLIGHT_MAP_BLANK,
	NULL,
	NULL,
	IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION,
	0,
	MAP_LEFT_BORDER, MAP_TOP_BORDER
)

PELEMENT_HEADER     flightmap_next[] = { &flightmap_image.header, NULL };


MAKE_STATIC
(
	flightmap_background,
	BMP_FLIGHT_MAP_BACKGROUND,
	flightmap_next,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		flightmap_list	= &flightmap_background.header;


/////////////////////////////////////////////////////////////////////////////
static  FLOAT64		FSAPI GPSARG_Get_GroundSpeed (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge);
static  FLOAT64		FSAPI GPSARG_Get_Track (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge);
static  FLOAT64		FSAPI GPSARG_Get_WP_Distance (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge);
static  PCSTRINGZ	FSAPI GPSARG_Get_WP_ID (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge);
static  MOUSE_FUNCTION	  AddNewWaypoint;

MOUSE_TOOLTIP_ARGS (GPS_Args)
	MOUSE_TOOLTIP_ARG (MODULE_VAR_NONE, 0, NULL, NULL, NULL, GPSARG_Get_GroundSpeed, NULL, NULL)
	MOUSE_TOOLTIP_ARG (MODULE_VAR_NONE, 0, NULL, NULL, NULL, GPSARG_Get_Track, NULL, NULL)
	MOUSE_TOOLTIP_ARG (MODULE_VAR_NONE, 0, NULL, NULL, NULL, NULL, NULL, GPSARG_Get_WP_ID)
	MOUSE_TOOLTIP_ARG (MODULE_VAR_NONE, 0, NULL, NULL, NULL, GPSARG_Get_WP_Distance, NULL, NULL)
MOUSE_TOOLTIP_ARGS_END

MOUSE_BEGIN( flightmap_rect, HELP_NONE, 0, 0 )
	MOUSE_TOOLTIP_TEXT_STRING ("Ground Speed: %1!d! knots, Track: %2!d!�, Next Waypoint: %3!s!, Distance: %4!.1f! nm", GPS_Args)
	MOUSE_CHILD_FUNCT (MAP_LEFT_BORDER, MAP_TOP_BORDER, 230, 259, CURSOR_HAND, MOUSE_LEFTSINGLE, AddNewWaypoint)
MOUSE_END

#include <math.h>
#include "gps_info.h"

static GPS_INFO* gpsinfo = NULL;
static GPS_INFO* gpsrequest = NULL;

static POINT ScalePoint (PLATLONALT llaRef, PLATLONALT llaPoint, FLOAT64 fRefDirection, FLOAT64 dx, FLOAT64 dy);


void FSAPI FlightMapCallback ( PGAUGEHDR pgauge, SINT32 service_id, UINT_PTR extra_data )
{
	switch (service_id)
	{
	case PANEL_SERVICE_PRE_DRAW:
		{
			PELEMENT_STATIC_IMAGE pelement = (PELEMENT_STATIC_IMAGE)(pgauge->elements_list[0]->next_element[0]);

			if (!gpsinfo)
			{
				MODULE_VAR var;
				initialize_var_by_name (&var, GPS_INFO_PANEL_VARIABLE_NAME);
				gpsinfo = (GPS_INFO*)var.var_ptr;
			}

			if (pelement)
			{
				HDC hdc = pelement->hdc;
				PIXPOINT dim = pelement->image_data.final->dim;
				if (hdc)
				{
					FLOAT64 dx = dim.x / 230.0;
					FLOAT64 dy = dim.y / 259.0;
					INT	cx = (INT)(115 * dx);
					INT cy = (INT)(130 * dy);

					// clear background
					SelectObject (hdc, GetStockObject (BLACK_BRUSH));
					SelectObject (hdc, GetStockObject (WHITE_PEN));
					Rectangle (hdc, 0, 0, dim.x, dim.y);
					
					// draw aircraft symbol
					{
						static POINT AircraftShape[] = {
							{ 0, -6},
							{ 2, -5},
							{ 2, -2},
							{ 6,  0},
							{ 6,  2},
							{ 2,  1},
							{ 2,  4},
							{ 4,  4},
							{ 4,  6},
							{-4,  6},
							{-4,  4},
							{-2,  4},
							{-2,  1},
							{-6,  2},
							{-6,  0},
							{-2, -2},
							{-2, -5},
						};
						INT lPoints = sizeof (AircraftShape)/sizeof (POINT);
						POINT ResizedShape[sizeof (AircraftShape)/sizeof (POINT)];
						int i;

						for (i = 0; i < lPoints; i++)
						{
							ResizedShape[i].x = (INT)(AircraftShape[i].x * dx) + cx;
							ResizedShape[i].y = (INT)(AircraftShape[i].y * dy) + cy;
						}

						SelectObject (hdc, GetStockObject (WHITE_BRUSH));
						SelectObject (hdc, GetStockObject (WHITE_PEN));
						Polygon (hdc, ResizedShape, lPoints);
					}
					// draw waypoints
					if (gpsinfo && gpsinfo->dwSize >= sizeof (GPS_INFO))
					{
						int i;
						POINT prevp;
						for (i = 0; i < gpsinfo->lWpCounts; i++)
						{
							if (gpsinfo->pWpData && gpsinfo->pWpData[i].dwSize == sizeof (GPS_WP_INFO))
							{
								POINT p = ScalePoint (&gpsinfo->vPosition, &gpsinfo->pWpData[i].vPosition, gpsinfo->dGroundHeading, dx, dy);
								p.x += cx;
								p.y += cy;
								if (gpsinfo->lNextWp == i)
									SelectObject (hdc, GetStockObject (WHITE_BRUSH));
								else
									SelectObject (hdc, GetStockObject (HOLLOW_BRUSH));
								SelectObject (hdc, GetStockObject (WHITE_PEN));

								Ellipse (hdc, p.x - 5, p.y - 5, p.x + 5, p.y + 5);

								if (i > 0)
								{
									MoveToEx (hdc, prevp.x, prevp.y, NULL);
									LineTo (hdc, p.x, p.y);
								}
								prevp = p;
							}
						}
					}

					SET_OFF_SCREEN (pelement);
				}
			}
		}
		break;
	}
}

__inline SIF48 SUB_SIF48_SIF48 (const SIF48 a, const SIF48 b)
{
	SIF48 r;
	r.i64 = a.i64 - b.i64;
	return r;
}
__inline SIF48 ADD_SIF48_SIF48 (const SIF48 a, const SIF48 b)
{
    SIF48 r;
    r.i64 = a.i64 + b.i64;
    return(r);
}
__inline ANGL48 SUB_ANGL48_ANGL48 (const ANGL48 a, const ANGL48 b)
{
	ANGL48 r;
	r.i64 = a.i64 - b.i64;
	return r;
}
__inline ANGL48 ADD_ANGL48_ANGL48 (const ANGL48 a, const ANGL48 b)
{
	ANGL48 r;
	r.i64 = a.i64 + b.i64;
	return r;
}
__inline FLOAT64 ANGL48_TO_FLOAT64 (const ANGL48 angl48)
{
	return (FLOAT64)(SINT64)(angl48.i64 & ~0xFFFF) * (1/4294967296.0);
}
__inline ANGL48 FLOAT64_TO_ANGL48 (const FLOAT64 angle)
{
	ANGL48 r;
	r.i64 = (SINT64)(angle * 4294967296.0);
	return r;
}
__inline FLOAT64 SIF48_TO_FLOAT64 (const SIF48 sif48)
{
	return (FLOAT64)(sif48.i64 & ~0xFFFF) * (1/4294967296.0);
}
__inline SIF48 FLOAT64_TO_SIF48 (FLOAT64 number)
{
	SIF48 r;
	r.i64 = (SINT64)(number * 4294967296.0);
	return r;
}
__inline FLOAT64 ANGL48_TO_DEGREES (const ANGL48 angle)
{
	return ANGL48_TO_FLOAT64(angle) * (360.0 / (65536.0 * 65536.0));
}
__inline ANGL48 DEGREES_TO_ANGL48 (const FLOAT64 angle)
{
	return FLOAT64_TO_ANGL48(angle * ((65536.0 * 65536.0) / 360.0));
}
__inline FLOAT64 LON_DEG_TO_METERS_AT_LAT_RAD (FLOAT64 dDeltaLonAngle, FLOAT64 dCurLatRad)
{
	return dDeltaLonAngle * (40075000.0 / 360) * cos(dCurLatRad);
}
__inline FLOAT64 METERS_TO_LON_DEG_AT_LAT_RAD (FLOAT64 dDeltaLonAngle, FLOAT64 dCurLatRad)
{
	return dDeltaLonAngle * (360 / 40075000.0) / cos(dCurLatRad);
}
__inline FLOAT64 LAT_METERS48_TO_RADIANS (SIF48 sif48Lat)
{
	return (FLOAT64)(SINT64)(sif48Lat.i64 & ~0xFFFF) * ((1.0 / (40007000.0/(3.14159265358*2))) * (1/4294967296.0));
}
__inline FLOAT64 LAT_RADIANS_TO_METERS48 (SIF48 sif48Lat)
{
	return (FLOAT64)(SINT64)(sif48Lat.i64 & ~0xFFFF) * ((1.0 / (40007000.0/(3.14159265358*2))) * (1/4294967296.0));
}

#define SCALE_MAP_FACTOR 0.005

static POINT ScalePoint (PLATLONALT llaRef, PLATLONALT llaPoint, FLOAT64 fRefDirection, FLOAT64 dx, FLOAT64 dy)
{
	ANGL48  anglDiff     = SUB_ANGL48_ANGL48 (llaPoint->lon, llaRef->lon );
	FLOAT64 fDeltaLonDeg = ANGL48_TO_DEGREES (anglDiff );
	FLOAT64 fLatRad      = LAT_METERS48_TO_RADIANS (llaRef->lat) ;
	FLOAT64 fLOMeters	 = LON_DEG_TO_METERS_AT_LAT_RAD (fDeltaLonDeg, fLatRad );
	FLOAT64 fLAMeters	 = SIF48_TO_FLOAT64 (SUB_SIF48_SIF48 (llaPoint->lat, llaRef->lat));
	FLOAT64 fSin		 = sin (fRefDirection);
	FLOAT64 fCos		 = cos (fRefDirection);
	FLOAT64 fX			 = fLOMeters * fCos - fLAMeters * fSin;
	FLOAT64 fY			 = -fLOMeters * fSin - fLAMeters * fCos;
	INT		lX			 = (INT) (fX * dx * SCALE_MAP_FACTOR);
	INT		lY			 = (INT) (fY * dy * SCALE_MAP_FACTOR);
	POINT   result		 = { lX, lY };

	return result;
}


static FLOAT64 FSAPI GPSARG_Get_GroundSpeed (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge)
{
	if (gpsinfo && gpsinfo->dwSize >= sizeof (GPS_INFO))
		return gpsinfo->dGroundSpeed * 1.943844;
	else
		return 0;
}
static FLOAT64 FSAPI GPSARG_Get_Track (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge)
{
	if (gpsinfo && gpsinfo->dwSize >= sizeof (GPS_INFO))
		return gpsinfo->dGroundTrack * 57.295780;
	else
		return 0;
}
static FLOAT64 FSAPI GPSARG_Get_WP_Distance (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge)
{
	if (gpsinfo && gpsinfo->dwSize >= sizeof (GPS_INFO) && gpsinfo->bIsActiveWaypoint)
		return gpsinfo->dWpDistance * 0.000539957;
	else
		return 0;
}
static PCSTRINGZ FSAPI GPSARG_Get_WP_ID (FLOAT64 number, ID id, PCSTRINGZ string, MODULE_VAR *source_var, PGAUGEHDR gauge)
{
	if (gpsinfo && gpsinfo->dwSize >= sizeof (GPS_INFO) && gpsinfo->bIsActiveWaypoint && 
		gpsinfo->lNextWp < gpsinfo->lWpCounts && gpsinfo->lNextWp >= 0 && gpsinfo->pWpData != NULL && 
		gpsinfo->pWpData[gpsinfo->lNextWp].dwSize == sizeof (GPS_WP_INFO))
		return gpsinfo->pWpData[gpsinfo->lNextWp].szName;
	else
		return "---";
}

static BOOL FSAPI AddNewWaypoint (PPIXPOINT pPoint, FLAGS32 flags)
{
	FLOAT64	lX = pPoint->x / GAUGEHDR_FOR_MOUSE_CALLBACK (pPoint)->x_adjust - 115.0;
	FLOAT64	lY = pPoint->y / GAUGEHDR_FOR_MOUSE_CALLBACK (pPoint)->y_adjust - 130.0;
	if (!gpsinfo)
	{
		MODULE_VAR var;
		initialize_var_by_name (&var, GPS_INFO_PANEL_VARIABLE_NAME);
		gpsinfo = (GPS_INFO*)var.var_ptr;
	}
	if (!gpsrequest)
	{
		MODULE_VAR var;
		initialize_var_by_name (&var, GPS_REQUEST_PANEL_VARIABLE_NAME);
		gpsrequest = (GPS_INFO*)var.var_ptr;
	}
	if (gpsinfo && gpsrequest && gpsinfo->pWpData && gpsrequest->pWpData && gpsinfo->lWpCounts >= 2)
	{
		UINT32 i;
		gpsrequest->dwUpdateRequest = 2;
		gpsrequest->vPosition = gpsinfo->vPosition;
		gpsrequest->bIsFlightPlanAvailable = TRUE;
		gpsrequest->bIsDirectTo = FALSE;
		gpsrequest->lNextWp = gpsinfo->lNextWp;
		gpsrequest->lWpCounts = gpsinfo->lWpCounts;
		for (i = 0; i < gpsinfo->lWpCounts; i++)
		{
			gpsrequest->pWpData[i] = gpsinfo->pWpData[i];
		}
		if (gpsrequest->lWpCounts < MAX_GPS_WAYPOINTS)
		{
			UINT32 i = gpsrequest->lWpCounts++;
			gpsrequest->pWpData[i].dwSize = sizeof (GPS_WP_INFO);
			{
				FLOAT64 fX  = lX / SCALE_MAP_FACTOR;
				FLOAT64 fY  = lY / SCALE_MAP_FACTOR;
				FLOAT64 fSin = sin (gpsinfo->dGroundHeading);
				FLOAT64 fCos = cos (gpsinfo->dGroundHeading);
				FLOAT64 fLOMeters = fX * fCos + fY * fSin;
				FLOAT64 fLAMeters = fX * fSin - fY * fCos;
				FLOAT64 fLatRad      = LAT_METERS48_TO_RADIANS (gpsinfo->vPosition.lat);
				FLOAT64 fDeltaLonDeg = METERS_TO_LON_DEG_AT_LAT_RAD (fLOMeters, fLatRad );
				ANGL48  anglDiff = DEGREES_TO_ANGL48 (fDeltaLonDeg);
				LATLONALT v;
				v.lon = ADD_ANGL48_ANGL48 (gpsinfo->vPosition.lon, anglDiff);
				v.lat = ADD_SIF48_SIF48 (gpsinfo->vPosition.lat, FLOAT64_TO_SIF48 (fLAMeters));
				v.alt = FLOAT64_TO_SIF48 (gpsrequest->pWpData[i-1].dAltitude);
				gpsrequest->pWpData[i].vPosition = v;
			}
			gpsrequest->pWpData[i].dHeading = gpsrequest->pWpData[i-1].dHeading;
			gpsrequest->pWpData[i].dSpeed = gpsrequest->pWpData[i-1].dSpeed;
			gpsrequest->pWpData[i].dAltitude = gpsrequest->pWpData[i-1].dAltitude;
			strcpy_s(gpsrequest->pWpData[i].szName, "???");
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#undef MAP_LEFT_BORDER
#undef MAP_TOP_BORDER
#undef SCALE_MAP_FACTOR

#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

