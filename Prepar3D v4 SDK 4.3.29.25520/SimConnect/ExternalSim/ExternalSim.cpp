// ExternalSim.cpp : Test the SimConnect External Sim APIs
//

#include "stdafx.h"
#include "SimConnect.h"
#include "ExternalSim.h"

HANDLE g_hSimConnect = NULL;
VehicleList g_VehicleList;

#define EXTERNAL_SIM_CALLBACK_MASK (SIMCONNECT_EXTERNAL_SIM_CALLBACK_FLAG_CREATE | SIMCONNECT_EXTERNAL_SIM_CALLBACK_FLAG_DESTROY | SIMCONNECT_EXTERNAL_SIM_CALLBACK_FLAG_SIMULATE | SIMCONNECT_EXTERNAL_SIM_CALLBACK_FLAG_LOCATION_CHANGED | SIMCONNECT_EXTERNAL_SIM_CALLBACK_FLAG_EVENT)

void OnRecvExternalSimCreate(SIMCONNECT_RECV_EXTERNAL_SIM_CREATE *pCreate)
{
    VehicleListIterator iter = g_VehicleList.find(pCreate->dwObjectID);
    // check that we aren't already tracking this vehicle
    if (iter == g_VehicleList.end())
    {
        // create new vehicle data structure
        VehicleData *pVehData = new VehicleData;
        pVehData->dwObjectID = pCreate->dwObjectID;
        pVehData->cntSimVars = pCreate->dwExternalSimVarCount;
        pVehData->idxSimVarsBase = pCreate->dwExternalSimVarBase;
        float d1, d2, d3;
        sscanf_s(pCreate->szExternalSimData, "%f, %f, %f", &d1, &d2, &d3);
        pVehData->fMaxFrontBackVelocity = d1;
        pVehData->fMaxLeftRightVelocity = d2;
        pVehData->fMaxUpDownVelocity = d3;
        
        pVehData->bReset = true;

        // add vehicle to list
        g_VehicleList[pCreate->dwObjectID] = pVehData;
    }

    SimConnect_SynchronousUnblock(g_hSimConnect);
}

void OnRecvExternalSimDestroy(SIMCONNECT_RECV_EXTERNAL_SIM_DESTROY *pDestroy)
{
    VehicleListIterator iter = g_VehicleList.find(pDestroy->dwObjectID);
    // check that this object ID is being tracked currently
    if (iter != g_VehicleList.end() && iter->second != NULL)
    {
        // delete vehicle and remove from local list
        delete iter->second;
        g_VehicleList.erase(iter);
    }

    SimConnect_SynchronousUnblock(g_hSimConnect);
}

void OnRecvExternalSimSimulate(SIMCONNECT_RECV_EXTERNAL_SIM_SIMULATE *pSimulate)
{
    VehicleListIterator iter = g_VehicleList.find(pSimulate->dwObjectID);
    // check that this object ID is being tracked currently
    if (iter != g_VehicleList.end() && iter->second != NULL)
    {
        // yes, we know this vehicle, simulate it
        VehicleData *pVehData = iter->second;
        PerVehicleSimulate *pData = (PerVehicleSimulate*)&pSimulate->dwData;
        PerVehicleOutput pvo = {
            // lat, lon, alt
            pData->Lat, pData->Lon, pData->Alt, 
            // pitch, roll/bank, yaw/heading, 
            pData->Pitch, pData->Bank, pData->Heading,
        };

        if (pSimulate->fDeltaTime > 1.5)
        {
            pSimulate->fDeltaTime = 0;
        }

        if (pVehData->bReset)
        {   // if reset requested, reset values
            pVehData->bReset = false;
	        pVehData->fLeftRight = 0;
	        pVehData->fFrontBack = 0;
	        pVehData->fUpDown = 0;
	        pVehData->fLeftRightVelocity = 0;
	        pVehData->fFrontBackVelocity = 0;
	        pVehData->fUpDownVelocity = 0;
            pvo.Pitch = 0;
            pvo.Bank = 0;
            pvo.Heading = 0;

            SimConnect_SetDataOnSimObject(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, pSimulate->dwObjectID, 0, 0, sizeof(PerVehicleOutput), &pvo);
        }
        else if (pSimulate->bShouldSimulate)
        {   // else if should simulate, then simulate this vehicle
            
            // process inputs
            pVehData->fLeftRightVelocity = pVehData->fMaxLeftRightVelocity * pVehData->fLeftRight;
            pVehData->fFrontBackVelocity = pVehData->fMaxFrontBackVelocity * pVehData->fFrontBack;
            pVehData->fUpDownVelocity = pVehData->fMaxUpDownVelocity * pVehData->fUpDown;
            
            // step time integrate
            pvo.Lat += LAT_METERS_TO_RADIANS(pVehData->fFrontBackVelocity * pSimulate->fDeltaTime);
            pvo.Lon += LON_METERS_TO_RADIANS(pVehData->fLeftRightVelocity * pSimulate->fDeltaTime, pvo.Lat);
            pVehData->GroundAltOffset += pVehData->fUpDownVelocity * pSimulate->fDeltaTime;
            if (pData->eSurfaceType == 2)
            {   // if SurfaceType == WATER, keep from offseting beyond BathAlt value
                if (pVehData->GroundAltOffset < pData->BathAlt)
                {
                    pVehData->GroundAltOffset = pData->BathAlt;
                }
            }
            else
            {   // if SurfaceType != WATER, keep from going below ground
                if (pVehData->GroundAltOffset < 0)
                {
                    pVehData->GroundAltOffset = 0;
                }
            }
            pvo.Alt = pData->GroundAlt + pVehData->GroundAltOffset;

            pvo.Pitch = 0;
            pvo.Bank = 0;
            if (pVehData->fFrontBackVelocity != 0 || pVehData->fLeftRightVelocity != 0)
            {
                pvo.Heading = atan2(pVehData->fLeftRightVelocity, pVehData->fFrontBackVelocity);
                while (pvo.Heading >= 2.0 * M_PI)
                {
                    pvo.Heading -= 2.0 * M_PI;
                }
                while (pvo.Heading < -2.0 * M_PI)
                {
                    pvo.Heading += 2.0 * M_PI;
                }
            }

            pvo.BodyVelX = 0.0;
            pvo.BodyVelY = pVehData->fUpDownVelocity;
            pvo.BodyVelZ = sqrt((pVehData->fLeftRightVelocity * pVehData->fLeftRightVelocity) + (pVehData->fFrontBackVelocity * pVehData->fFrontBackVelocity));
            pvo.WorldVelX = pVehData->fLeftRightVelocity;
            pvo.WorldVelY = pVehData->fUpDownVelocity;
            pvo.WorldVelZ = pVehData->fFrontBackVelocity;
            pvo.VerticalSpeed = pVehData->fUpDownVelocity;

            SimConnect_SetDataOnSimObject(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, pSimulate->dwObjectID, 0, 0, sizeof(PerVehicleOutput), &pvo);
        }
    }

    SimConnect_SynchronousUnblock(g_hSimConnect);
}

void OnRecvExternalSimLocationChanged(SIMCONNECT_RECV_EXTERNAL_SIM_LOCATION_CHANGED *pLocationChanged)
{
    if (pLocationChanged->bZeroSpeed)
    {
	    VehicleListIterator iter = g_VehicleList.find(pLocationChanged->dwObjectID);
	    if (iter != g_VehicleList.end())
	    {
	    	iter->second->bReset = true;
	    }
    }

    SimConnect_SynchronousUnblock(g_hSimConnect);
}

void OnRecvExternalSimEvent(SIMCONNECT_RECV_EXTERNAL_SIM_EVENT *pEvent)
{
    VehicleListIterator iter = g_VehicleList.find(pEvent->dwObjectID);
    // check that this object ID is being tracked currently
    if (iter != g_VehicleList.end() && iter->second != NULL)
    {
        VehicleData *pVehData = iter->second;

        switch (pEvent->uEventID)
        {
            case MOVEMENT_CENTER_EVENT:
                {
                    pVehData->fFrontBack = 0;
                    pVehData->fLeftRight = 0;
                    pVehData->fUpDown = 0;
                }
                break;

            case LEFTRIGHT_INCREMENT_EVENT:
                {
                    pVehData->fLeftRight += 0.1;
                    if (pVehData->fLeftRight > 1.0)
                    {
                        pVehData->fLeftRight = 1.0;
                    }
                }
                break;

            case LEFTRIGHT_DECREMENT_EVENT:
                {
                    pVehData->fLeftRight -= 0.1;
                    if (pVehData->fLeftRight < -1.0)
                    {
                        pVehData->fLeftRight = -1.0;
                    }
                }
                break;

            case LEFTRIGHT_SET_EVENT:
                {
                    pVehData->fLeftRight = ((int)pEvent->dwData) / 100000.0;
                    if (pVehData->fLeftRight > 1.0)
                    {
                        pVehData->fLeftRight = 1.0;
                    }
                    if (pVehData->fLeftRight < -1.0)
                    {
                        pVehData->fLeftRight = -1.0;
                    }
                }
                break;

            case FRONTBACK_INCREMENT_EVENT:
                {
                    pVehData->fFrontBack += 0.1;
                    if (pVehData->fFrontBack > 1.0)
                    {
                        pVehData->fFrontBack = 1.0;
                    }
                }
                break;

            case FRONTBACK_DECREMENT_EVENT:
                {
                    pVehData->fFrontBack -= 0.1;
                    if (pVehData->fFrontBack < -1.0)
                    {
                        pVehData->fFrontBack = -1.0;
                    }
                }
                break;

            case FRONTBACK_SET_EVENT:
                {
                    pVehData->fFrontBack = ((int)pEvent->dwData) / 100000.0;
                    if (pVehData->fFrontBack > 1.0)
                    {
                        pVehData->fFrontBack = 1.0;
                    }
                    if (pVehData->fFrontBack < -1.0)
                    {
                        pVehData->fFrontBack = -1.0;
                    }
                }
                break;

            case UPDOWN_MAX_EVENT:
                {
                    pVehData->fUpDown = 1.0;
                }
                break;

            case UPDOWN_MIN_EVENT:
                {
                    pVehData->fUpDown = -1.0;
                }
                break;

            case UPDOWN_INCREMENT_EVENT_1:
            case UPDOWN_INCREMENT_EVENT_2:
            case UPDOWN_INCREMENT_EVENT_3:
                {
                    pVehData->fUpDown += 0.1;
                    if (pVehData->fUpDown > 1.0)
                    {
                        pVehData->fUpDown = 1.0;
                    }
                }
                break;

            case UPDOWN_DECREMENT_EVENT_1:
            case UPDOWN_DECREMENT_EVENT_2:
            case UPDOWN_DECREMENT_EVENT_3:
                {
                    pVehData->fUpDown -= 0.1;
                    if (pVehData->fUpDown < -1.0)
                    {
                        pVehData->fUpDown = -1.0;
                    }
                }
                break;

            case UPDOWN_SET_EVENT:
                {
                    pVehData->fUpDown = pEvent->dwData / 100000.0;
                    if (pVehData->fUpDown > 1.0)
                    {
                        pVehData->fUpDown = 1.0;
                    }
                    if (pVehData->fUpDown < -1.0)
                    {
                        pVehData->fUpDown = -1.0;
                    }
                }
                break;

            case LEFTRIGHT_AXIS_EVENT:
                {
                    pVehData->fLeftRight = -(int)pEvent->dwData / 16384.0;
                }
                break;

            case FRONTBACK_AXIS_EVENT:
                {
                    pVehData->fFrontBack = (int)pEvent->dwData / 16384.0;
                }
                break;

            case UPDOWN_AXIS_EVENT:
                {
                    pVehData->fUpDown = (int)pEvent->dwData / 16384.0;
                }
                break;
        }
    }

    SimConnect_SynchronousUnblock(g_hSimConnect);
}

void OnRecvException(SIMCONNECT_RECV_EXCEPTION *pExceptionData)
{
}

void OnRecvQuit()
{
    SimConnect_UnregisterExternalSim(g_hSimConnect, g_guidExternalSim);
}

void OnRecvOpen(SIMCONNECT_RECV_OPEN *pOpenData)
{
    // this struct/request used to get data via the External Sim Simulate callback
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("PLANE LATITUDE"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("PLANE LONGITUDE"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("PLANE ALTITUDE"), ("meters"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("PLANE PITCH DEGREES"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("PLANE BANK DEGREES"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("PLANE HEADING DEGREES TRUE"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("GROUND ALTITUDE"), ("meters"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("BATH ALTITUDE"), ("meters"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_SIMULATE_DEFINITION, ("SURFACE TYPE"), ("enum"), SIMCONNECT_DATATYPE_INT32);

    // this struct/request used to send data back to the sim from the External Sim Simulate callback
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("PLANE LATITUDE"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("PLANE LONGITUDE"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("PLANE ALTITUDE"), ("meters"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("PLANE PITCH DEGREES"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("PLANE BANK DEGREES"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("PLANE HEADING DEGREES TRUE"), ("radians"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VELOCITY BODY X"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VELOCITY BODY Y"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VELOCITY BODY Z"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VELOCITY WORLD X"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VELOCITY WORLD Y"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VELOCITY WORLD Z"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);
    SimConnect_AddToDataDefinition(g_hSimConnect, PER_VEHICLE_OUTPUT_DEFINITION, ("VERTICAL SPEED"), ("m/s"), SIMCONNECT_DATATYPE_FLOAT64);

    // register as an external sim client
    SimConnect_RegisterExternalSim(g_hSimConnect, g_guidExternalSim, EXTERNAL_SIM_CALLBACK_MASK, PER_VEHICLE_SIMULATE_DEFINITION);

    // map some events
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, MOVEMENT_CENTER_EVENT, ("CENTER_AILER_RUDDER"));

    SimConnect_MapClientEventToSimEvent(g_hSimConnect, LEFTRIGHT_INCREMENT_EVENT, ("AILERONS_RIGHT"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, LEFTRIGHT_DECREMENT_EVENT, ("AILERONS_LEFT"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, LEFTRIGHT_SET_EVENT, ("AILERON_SET"));

    SimConnect_MapClientEventToSimEvent(g_hSimConnect, FRONTBACK_INCREMENT_EVENT, ("ELEV_DOWN"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, FRONTBACK_DECREMENT_EVENT, ("ELEV_UP"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, FRONTBACK_SET_EVENT, ("ELEVATOR_SET"));

    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_MAX_EVENT, ("THROTTLE_FULL"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_MIN_EVENT, ("THROTTLE_CUT"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_INCREMENT_EVENT_1, ("THROTTLE_INCR"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_INCREMENT_EVENT_2, ("THROTTLE_INCR_SMALL"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_INCREMENT_EVENT_3, ("INCREASE_THROTTLE"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_DECREMENT_EVENT_1, ("THROTTLE_DECR"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_DECREMENT_EVENT_2, ("THROTTLE_DECR_SMALL"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_DECREMENT_EVENT_3, ("DECREASE_THROTTLE"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_SET_EVENT, ("THROTTLE_SET"));

    SimConnect_MapClientEventToSimEvent(g_hSimConnect, LEFTRIGHT_AXIS_EVENT, ("AXIS_AILERONS_SET"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, FRONTBACK_AXIS_EVENT, ("AXIS_ELEVATOR_SET"));
    SimConnect_MapClientEventToSimEvent(g_hSimConnect, UPDOWN_AXIS_EVENT, ("AXIS_THROTTLE_SET"));
}

void CALLBACK SimConnectProcess(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext)
{
    // process SIMCONNECT_RECV_ID_XXX values here as needed
    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EXTERNAL_SIM_CREATE:
            {
                OnRecvExternalSimCreate((SIMCONNECT_RECV_EXTERNAL_SIM_CREATE*)pData);
            }
            break;

        case SIMCONNECT_RECV_ID_EXTERNAL_SIM_DESTROY:
            {
                OnRecvExternalSimDestroy((SIMCONNECT_RECV_EXTERNAL_SIM_DESTROY*)pData);
            }
            break;

        case SIMCONNECT_RECV_ID_EXTERNAL_SIM_SIMULATE:
            {
                OnRecvExternalSimSimulate((SIMCONNECT_RECV_EXTERNAL_SIM_SIMULATE*)pData);
            }
            break;

        case SIMCONNECT_RECV_ID_EXTERNAL_SIM_LOCATION_CHANGED:
            {
                OnRecvExternalSimLocationChanged((SIMCONNECT_RECV_EXTERNAL_SIM_LOCATION_CHANGED*)pData);
            }
            break;

        case SIMCONNECT_RECV_ID_EXTERNAL_SIM_EVENT:
            {
                OnRecvExternalSimEvent((SIMCONNECT_RECV_EXTERNAL_SIM_EVENT*)pData);
            }
            break;

        case SIMCONNECT_RECV_ID_EXCEPTION:
            {
                OnRecvException((SIMCONNECT_RECV_EXCEPTION*)pData);
            }
            break;

        case SIMCONNECT_RECV_ID_QUIT:
            {
                OnRecvQuit();
            }
            break;

        case SIMCONNECT_RECV_ID_OPEN:
            {
                OnRecvOpen((SIMCONNECT_RECV_OPEN*)pData);
            }
            break;
    }
}

void __stdcall DLLStart( void )
{
    // open connection to local SimConnect server
    if ( SUCCEEDED(SimConnect_Open(&g_hSimConnect, ("ExternalSim"), NULL, 0, NULL, SIMCONNECT_OPEN_CONFIGINDEX_LOCAL)) && g_hSimConnect != NULL)
    {
        // register callback routine for message processing
        SimConnect_CallDispatch(g_hSimConnect, SimConnectProcess, NULL);
    }
}

void __stdcall DLLStop( void )
{
    if (g_hSimConnect != NULL)
    {
        SimConnect_Close(g_hSimConnect);
        g_hSimConnect = NULL;
    }
    g_VehicleList.clear();
}
