//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  
//
//
// Managed Facility Request sample
//
// Buttons in a dialog are used to requests data on facilities: Airports, Waypoints, NDBs, VORs
//

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

// Add these two statements to all SimConnect clients
using LockheedMartin.Prepar3D.SimConnect;
using System.Runtime.InteropServices;

namespace Managed_Facility_Request
{
    public partial class Form1 : Form
    {
        // User-defined win32 event
        const int WM_USER_SIMCONNECT = 0x0402;

        // SimConnect object
        SimConnect simconnect = null;

        enum DATA_REQUESTS
        {
            SUBSCRIBE_REQ,
            NONSUBSCRIBE_REQ,
        };

        enum EVENTS
        {
            ID0,
        };

        public Form1()
        {
            InitializeComponent();

            setButtons(true, false, false);
        }

        // Simconnect client will send a win32 message when there is 
        // a packet to process. ReceiveMessage must be called to
        // trigger the events. This model keeps simconnect processing on the main thread.

        protected override void DefWndProc(ref Message m)
        {
            if (m.Msg == WM_USER_SIMCONNECT)
            {
                if (simconnect != null)
                {
                    simconnect.ReceiveMessage();
                }
            }
            else
            {
                base.DefWndProc(ref m);
            }
        }

        private void setButtons(bool bConnect, bool bGet, bool bDisconnect)
        {
            buttonConnect.Enabled = bConnect;
            buttonRequestAirports.Enabled = buttonRequestWaypoints.Enabled 
                                          = buttonRequestNDBs.Enabled 
                                          = buttonRequestVORs.Enabled
                                          = buttonRequestTacans.Enabled
                                          = bGet;
            if (bDisconnect)
            {
                checkboxAirportSubscription.Checked = checkboxWaypointsSubscription.Checked
                                                    = checkboxNDBsSubscription.Checked
                                                    = checkboxVORsSubscription.Checked
                                                    = checkboxTacansSubscription.Checked
                                                    = false;
            }
            labelSubscriptions.Enabled = checkboxAirportSubscription.Enabled 
                                       = checkboxWaypointsSubscription.Enabled
                                       = checkboxNDBsSubscription.Enabled
                                       = checkboxVORsSubscription.Enabled
                                       = checkboxTacansSubscription.Enabled
                                       = bGet;
            buttonDisconnect.Enabled = bDisconnect;
        }

        private void closeConnection()
        {
            if (simconnect != null)
            {
                // Dispose serves the same purpose as SimConnect_Close()
                simconnect.Dispose();
                simconnect = null;
                displayText("Connection closed");
            }
        }

        // Set up all the SimConnect related data definitions and event handlers
        private void initDataRequest()
        {
            try
            {
                // listen to connect and quit msgs
                simconnect.OnRecvOpen += new SimConnect.RecvOpenEventHandler(simconnect_OnRecvOpen);
                simconnect.OnRecvQuit += new SimConnect.RecvQuitEventHandler(simconnect_OnRecvQuit);
                // listen to facilities types
                simconnect.OnRecvAirportList += new SimConnect.RecvAirportListEventHandler(simconnect_OnRecvAirportList);
                simconnect.OnRecvWaypointList += new SimConnect.RecvWaypointListEventHandler(simconnect_OnRecvWaypointList);
                simconnect.OnRecvNdbList += new SimConnect.RecvNdbListEventHandler(simconnect_OnRecvNdbList);
                simconnect.OnRecvVorList += new SimConnect.RecvVorListEventHandler(simconnect_OnRecvVorList);
                simconnect.OnRecvTacanList += new SimConnect.RecvTacanListEventHandler(simconnect_OnRecvTacanList);

                // listen to exceptions
                simconnect.OnRecvException += new SimConnect.RecvExceptionEventHandler(simconnect_OnRecvException);
            }
            catch (COMException ex)
            {
                displayText(ex.Message);
            }
        }

        void Dump(Object item)
        {
            String s = "";
            foreach (System.Reflection.FieldInfo f in item.GetType().GetFields())
            {
                if (!f.FieldType.IsArray)
                {
                    s += "  " + f.Name + ": " + f.GetValue(item);
                }
            }
            displayText(s);
        }

        void DumpArray(Array rgData)
        {
            foreach (Object item in rgData)
            {
                Dump(item);
            }
        }

        void simconnect_OnRecvAirportList(SimConnect sender, SIMCONNECT_RECV_AIRPORT_LIST data)
        {
            switch ((DATA_REQUESTS)data.dwRequestID)
            {
                case DATA_REQUESTS.SUBSCRIBE_REQ:
                case DATA_REQUESTS.NONSUBSCRIBE_REQ:
                    displayText("Airport List:");
                    Dump(data);
                    DumpArray(data.rgData);
                    break;

                default:
                    displayText("Unknown request ID: " + data.dwRequestID);
                    break;
            }   
        }

        void simconnect_OnRecvWaypointList(SimConnect sender, SIMCONNECT_RECV_WAYPOINT_LIST data)
        {
            switch ((DATA_REQUESTS)data.dwRequestID)
            {
                case DATA_REQUESTS.SUBSCRIBE_REQ:
                case DATA_REQUESTS.NONSUBSCRIBE_REQ:
                    displayText("Wayoints List:");
                    Dump(data);
                    DumpArray(data.rgData);
                    break;

                default:
                    displayText("Unknown request ID: " + data.dwRequestID);
                    break;
            }
        }

        void simconnect_OnRecvNdbList(SimConnect sender, SIMCONNECT_RECV_NDB_LIST data)
        {
            switch ((DATA_REQUESTS)data.dwRequestID)
            {
                case DATA_REQUESTS.SUBSCRIBE_REQ:
                case DATA_REQUESTS.NONSUBSCRIBE_REQ:
                    displayText("Waypoints List:");
                    Dump(data);
                    DumpArray(data.rgData);
                    break;

                default:
                    displayText("Unknown request ID: " + data.dwRequestID);
                    break;
            }
        }

        void simconnect_OnRecvVorList(SimConnect sender, SIMCONNECT_RECV_VOR_LIST data)
        {
            switch ((DATA_REQUESTS)data.dwRequestID)
            {
                case DATA_REQUESTS.SUBSCRIBE_REQ:
                case DATA_REQUESTS.NONSUBSCRIBE_REQ:
                    displayText("VOR List:");
                    Dump(data);
                    DumpArray(data.rgData);
                    break;

                default:
                    displayText("Unknown request ID: " + data.dwRequestID);
                    break;
            }
        }

        void simconnect_OnRecvTacanList(SimConnect sender, SIMCONNECT_RECV_TACAN_LIST data)
        {
            switch ((DATA_REQUESTS)data.dwRequestID)
            {
                case DATA_REQUESTS.SUBSCRIBE_REQ:
                case DATA_REQUESTS.NONSUBSCRIBE_REQ:
                    displayText("Tacan List:");
                    Dump(data);
                    DumpArray(data.rgData);
                    break;

                default:
                    displayText("Unknown request ID: " + data.dwRequestID);
                    break;
            }
        }

        void simconnect_OnRecvOpen(SimConnect sender, SIMCONNECT_RECV_OPEN data)
        {
            displayText("Connected...");
        }

        // The case where the user closes the app
        void simconnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            displayText("User has exited");
            closeConnection();
        }

        // The case where the user closes the client
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            closeConnection();
        }

        void simconnect_OnRecvException(SimConnect sender, SIMCONNECT_RECV_EXCEPTION data)
        {
            displayText("Exception received: " + data.dwException);
        }


        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (simconnect == null)
            {
                try
                {
                    // the constructor is similar to SimConnect_Open in the native API
                    simconnect = new SimConnect("Managed Facilities Data Request", this.Handle, WM_USER_SIMCONNECT, null, 0);
                    initDataRequest();
                    setButtons(false, true, true);
                }
                catch (COMException)
                {
                    displayText("Unable to connect");
                }
            }
            else
            {
                displayText("Error - try again");
                closeConnection();

                setButtons(true, false, false);
            }
        }

        private void buttonDisconnect_Click(object sender, EventArgs e)
        {
            closeConnection();
            setButtons(true, false, false);
        }

        void displayText(string s)
        {
            richResponse.AppendText(s + "\n");
            richResponse.ScrollToCaret();
        }


        private void buttonRequestAirports_Click(object sender, EventArgs e)
        {
            simconnect.RequestFacilitiesList(SIMCONNECT_FACILITY_LIST_TYPE.AIRPORT, DATA_REQUESTS.NONSUBSCRIBE_REQ);
            displayText("Airports request sent...");
        }

        private void buttonRequestWaypoints_Click(object sender, EventArgs e)
        {
            simconnect.RequestFacilitiesList(SIMCONNECT_FACILITY_LIST_TYPE.WAYPOINT, DATA_REQUESTS.NONSUBSCRIBE_REQ);
            displayText("Waypoints request sent...");
        }

        private void buttonRequestNDBs_Click(object sender, EventArgs e)
        {
            simconnect.RequestFacilitiesList(SIMCONNECT_FACILITY_LIST_TYPE.NDB, DATA_REQUESTS.NONSUBSCRIBE_REQ);
            displayText("NDBs request sent...");
        }

        private void buttonRequestVORs_Click(object sender, EventArgs e)
        {
            simconnect.RequestFacilitiesList(SIMCONNECT_FACILITY_LIST_TYPE.VOR, DATA_REQUESTS.NONSUBSCRIBE_REQ);
            displayText("VORs request sent...");
        }

        private void buttonRequestTacans_Click(object sender, EventArgs e)
        {
            simconnect.RequestFacilitiesList(SIMCONNECT_FACILITY_LIST_TYPE.TACAN, DATA_REQUESTS.NONSUBSCRIBE_REQ);
            displayText("Tacans request sent...");
        }

        private void checkboxAirportSubscription_CheckedChanged(object sender, EventArgs e)
        {
            if (checkboxAirportSubscription.Enabled)
            {
                if (checkboxAirportSubscription.Checked)
                {
                    simconnect.SubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.AIRPORT, DATA_REQUESTS.SUBSCRIBE_REQ);
                }
                else
                {
                    simconnect.UnsubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.AIRPORT);
                }
            }
        }

        private void checkboxWaypointsSubscription_CheckedChanged(object sender, EventArgs e)
        {
            if (checkboxWaypointsSubscription.Enabled)
            {
                if (checkboxWaypointsSubscription.Checked)
                {
                    simconnect.SubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.WAYPOINT, DATA_REQUESTS.SUBSCRIBE_REQ);
                }
                else
                {
                    simconnect.UnsubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.WAYPOINT);
                }
            }
        }

        private void checkboxNDBsSubscription_CheckedChanged(object sender, EventArgs e)
        {
            if (checkboxNDBsSubscription.Enabled)
            {
                if (checkboxNDBsSubscription.Checked)
                {
                    simconnect.SubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.NDB, DATA_REQUESTS.SUBSCRIBE_REQ);
                }
                else
                {
                    simconnect.UnsubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.NDB);
                }
            }
        }

        private void checkboxVORsSubscription_CheckedChanged(object sender, EventArgs e)
        {
            if (checkboxVORsSubscription.Enabled)
            {
                if (checkboxVORsSubscription.Checked)
                {
                    simconnect.SubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.VOR, DATA_REQUESTS.SUBSCRIBE_REQ);
                }
                else
                {
                    simconnect.UnsubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.VOR);
                }
            }
        }

        private void checkboxTacansSubscription_CheckedChanged(object sender, EventArgs e)
        {
            if (checkboxTacansSubscription.Enabled)
            {
                if (checkboxTacansSubscription.Checked)
                {
                    simconnect.SubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.TACAN, DATA_REQUESTS.SUBSCRIBE_REQ);
                }
                else
                {
                    simconnect.UnsubscribeToFacilities(SIMCONNECT_FACILITY_LIST_TYPE.TACAN);
                }
            }
        }
    }
}
// End of sample
