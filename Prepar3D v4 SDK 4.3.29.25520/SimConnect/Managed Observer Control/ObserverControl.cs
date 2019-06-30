//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  
//
//
// Managed Observer Control Sample
//
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

/**
 * The focus type used for observer focal-length and focal-target calculations.
 * This either causes the focus to be auto-calculated to be place on the terrain surface whenever possible, or to fix the focus within relative observer-space.
 */
enum OBSERVER_FOCUS_TYPE
{
    OBSERVER_FOCUS_WORLD,   /**< The observer focal point is automatically calculated during movement to intersect with the simulation world. */
    OBSERVER_FOCUS_LOCAL    /**< The observer focal point is locked relative to the camera position. Moving the camera will move the focal point, but will not change the focal distance. */
};

/**
 * The regime that the observer is allowed to operate within.
 */
enum OBSERVER_REGIME_TYPE
{
    OBSERVER_TELLURIAN,     /**< The observer is limited by the earth only; that is, the position is restricted from going below the sea-floor or below the ground, but may pass through water surfaces. */
    OBSERVER_TERRESTRIAL,   /**< The observer is limited to above-water movement; that is, the position is restricted from going below the ground or below any water surface. */
    OBSERVER_GHOST          /**< The observer is not limited by any terrain features; position may freely pass through any object or terrain feature. */
};

namespace ObserverControl
{
    public partial class ObserverControl : Form
    {        
        // User-defined win32 event
        const int WM_USER_SIMCONNECT = 0x0402;


        // SimConnect object
        SimConnect simconnect = null;

        enum DATA_REQUESTS
        {
            REQUEST_1,
        };

        enum NOTIFICATION_GROUPS
        {
            GROUP0,
        }

        public ObserverControl()
        {
            InitializeComponent();
            setButtons(true, false);
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

        private void setButtons(bool bConnect, bool bDisconnect)
        {
            buttonConnect.Enabled = bConnect;
            buttonDisconnect.Enabled = bDisconnect;
            buttonCreateObserver.Enabled = bDisconnect;

            buttonSetPosition.Enabled = bDisconnect;
            buttonSetRotation.Enabled = bDisconnect;
            buttonSetFieldOfView.Enabled = bDisconnect;
            buttonSetStepSize.Enabled = bDisconnect;
            buttonSetFocus.Enabled = bDisconnect;
            buttonSetRegime.Enabled = bDisconnect;
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

        // Set up all the SimConnect related event handlers
        private void initClientEvent()
        {
            try
            {
                // listen to connect and quit msgs
                simconnect.OnRecvOpen += new SimConnect.RecvOpenEventHandler(simconnect_OnRecvOpen);
                simconnect.OnRecvQuit += new SimConnect.RecvQuitEventHandler(simconnect_OnRecvQuit);

                // listen to exceptions
                simconnect.OnRecvException += new SimConnect.RecvExceptionEventHandler(simconnect_OnRecvException);

                // catch a simobject data request
                simconnect.OnRecvObserverData += new SimConnect.RecvObserverDataEventHandler(simconnect_OnRecvObserverData);
            }
            catch (COMException ex)
            {
                displayText(ex.Message);
            }
        }

        private float TryParseFloat(string value)
        {
            float result = 0;

            try
            {
                result = float.Parse(value);
            }
            catch
            {
            }

            return result;
        }

        private double TryParseDouble(string value)
        {
            double result = 0;

            try
            {
                result = double.Parse(value);
            }
            catch
            {
            }

            return result;
        }

        void simconnect_OnRecvOpen(SimConnect sender, SIMCONNECT_RECV_OPEN data)
        {
            displayText("Connected to Prepar3D");
        }

        // The case where the user closes Prepar3D
        void simconnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            displayText("Prepar3D has exited");
            closeConnection();
        }

        void simconnect_OnRecvException(SimConnect sender, SIMCONNECT_RECV_EXCEPTION data)
        {
            displayText("Exception received: " + data.dwException);
        }

        // The case where the user closes the client
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            closeConnection();
        }

        // After calling RequestObserverData, Prepar3D sends back SIMCONNECT_RECV_OBSERVER_DATA
        void simconnect_OnRecvObserverData(SimConnect sender, SIMCONNECT_RECV_OBSERVER_DATA data)
        {

            switch ((DATA_REQUESTS)data.dwRequestID)
            {
                case DATA_REQUESTS.REQUEST_1:
                    displayText("Received data for " + data.szObserverName);

                    textBoxMPosLat.Text = data.ObserverData.Position.Latitude.ToString();
                    textBoxMPosLong.Text = data.ObserverData.Position.Longitude.ToString();
                    textBoxMPosAlt.Text = data.ObserverData.Position.Altitude.ToString();

                    textBoxMHeading.Text = data.ObserverData.Rotation.Heading.ToString();
                    textBoxMPitch.Text = data.ObserverData.Rotation.Pitch.ToString();
                    textBoxMBank.Text = data.ObserverData.Rotation.Bank.ToString();

                    textBoxMFOVH.Text = data.ObserverData.FieldOfViewH.ToString();
                    textBoxMFOVV.Text = data.ObserverData.FieldOfViewV.ToString();

                    textBoxMSSLinear.Text = data.ObserverData.LinearStep.ToString();
                    textBoxMSSAngular.Text = data.ObserverData.AngularStep.ToString();

                    textBoxMFocalLength.Text = data.ObserverData.FocalLength.ToString();

                    // Focus
                    if (data.ObserverData.FocusFixed == (int)OBSERVER_FOCUS_TYPE.OBSERVER_FOCUS_LOCAL)
                    {
                        radioButtonMFocusPoint.Checked = true;
                    }
                    else
                    {
                        radioButtonMFocusWorld.Checked = true;
                    }

                    // Regime
                    if (data.ObserverData.Regime == (int)OBSERVER_REGIME_TYPE.OBSERVER_GHOST)
                    {
                        checkBoxMGhostMode.Checked = true;
                        checkBoxMPassWater.Checked = true;
                    }
                    else if (data.ObserverData.Regime == (int)OBSERVER_REGIME_TYPE.OBSERVER_TELLURIAN)
                    {
                        checkBoxMGhostMode.Checked = false;
                        checkBoxMPassWater.Checked = true;
                    }
                    else
                    {
                        checkBoxMGhostMode.Checked = false;
                        checkBoxMPassWater.Checked = false;
                    }

                    break;

                default:
                    displayText("Unknown request ID: " + data.dwRequestID);
                    break;
            }
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (simconnect == null)
            {
                try
                {
                    // the constructor is similar to SimConnect_Open in the native API
                    simconnect = new SimConnect("Managed ObserverControl", this.Handle, WM_USER_SIMCONNECT, null, 0);

                    setButtons(false, true);

                    initClientEvent();

                }
                catch (COMException ex)
                {
                    displayText("Unable to connect to Prepar3D " + ex.Message);
                }
            }
            else
            {
                displayText("Error - try again");
                closeConnection();

                setButtons(true, false);
            }
        }

        private void buttonDisconnect_Click(object sender, EventArgs e)
        {
            closeConnection();
            setButtons(true, false);
        }

        // Response number
        int response = 1;

        // Output text - display a maximum of 5 lines
        string output = "\n\n\n\n";

        void displayText(string s)
        {
            // remove first string from output
            output = output.Substring(output.IndexOf("\n") + 1);

            // add the new string
            output += "\n" + response++ + ": " + s;

            // display it
            richResponse.Text = output;
        }

        // Create Observer Calls --------------------------------------------

        // Uses all data entered in the create tab to create the named observer
        unsafe private void buttonCreateObserver_Click(object sender, EventArgs e)
        {
            SIMCONNECT_DATA_OBSERVER observerData = new SIMCONNECT_DATA_OBSERVER();

            observerData.Position.Latitude = TryParseDouble(textBoxCPosLat.Text);
            observerData.Position.Longitude = TryParseDouble(textBoxCPosLong.Text);
            observerData.Position.Altitude = TryParseDouble(textBoxCPosAlt.Text);

            observerData.Rotation.Heading = TryParseDouble(textBoxCHeading.Text);
            observerData.Rotation.Pitch = TryParseDouble(textBoxCPitch.Text);
            observerData.Rotation.Bank = TryParseDouble(textBoxCBank.Text);

            observerData.FieldOfViewH = TryParseFloat(textBoxCFOVH.Text);
            observerData.FieldOfViewV = TryParseFloat(textBoxCFOVV.Text);

            observerData.LinearStep = TryParseFloat(textBoxCSSLinear.Text);
            observerData.AngularStep = TryParseFloat(textBoxCSSAngular.Text);

            observerData.FocalLength = TryParseFloat(textBoxCFocalLength.Text);

            if (radioButtonCFocusPoint.Checked)
            {
                observerData.FocusFixed = (int)OBSERVER_FOCUS_TYPE.OBSERVER_FOCUS_LOCAL;
            }
            else
            {
                observerData.FocusFixed = (int)OBSERVER_FOCUS_TYPE.OBSERVER_FOCUS_WORLD;
            }

            if (checkBoxCGhostMode.Checked)
            {
                observerData.Regime = (int)OBSERVER_REGIME_TYPE.OBSERVER_GHOST;
            }
            else if (checkBoxCPassWater.Checked)
            {
                observerData.Regime = (int)OBSERVER_REGIME_TYPE.OBSERVER_TELLURIAN;
            }
            else
            {
                observerData.Regime = (int)OBSERVER_REGIME_TYPE.OBSERVER_TERRESTRIAL;
            }

            simconnect.CreateObserver(textBoxCObserverName.Text, observerData);

            displayText("Observer \"" + textBoxCObserverName.Text + "\" created");
        }

        private void checkBoxCGhostMode_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxCGhostMode.Checked)
            {
                checkBoxCPassWater.Checked = true;
                checkBoxCPassWater.Enabled = false;
            }
            else
            {
                checkBoxCPassWater.Enabled = true;
            }
        }


        // Modify Observer Calls --------------------------------------------
        private void buttonPopulateFields_Click(object sender, EventArgs e)
        {
            simconnect.RequestObserverData(DATA_REQUESTS.REQUEST_1, textBoxMObserverName.Text);
        }

        private void buttonSetPosition_Click(object sender, EventArgs e)
        {
            SIMCONNECT_DATA_LATLONALT observerLLA = new SIMCONNECT_DATA_LATLONALT();

            observerLLA.Latitude = TryParseDouble(textBoxMPosLat.Text);
            observerLLA.Longitude = TryParseDouble(textBoxMPosLong.Text);
            observerLLA.Altitude = TryParseDouble(textBoxMPosAlt.Text);

            simconnect.SetObserverPosition(textBoxMObserverName.Text, observerLLA);
        }

        private void buttonSetRotation_Click(object sender, EventArgs e)
        {
            SIMCONNECT_DATA_PBH observerPBH = new SIMCONNECT_DATA_PBH();

            observerPBH.Pitch = TryParseDouble(textBoxMPitch.Text);
            observerPBH.Bank = TryParseDouble(textBoxMBank.Text);
            observerPBH.Heading = TryParseDouble(textBoxMHeading.Text);

            simconnect.SetObserverRotation(textBoxMObserverName.Text, observerPBH);
        }

        private void buttonSetFieldOfView_Click(object sender, EventArgs e)
        {
            simconnect.SetObserverFieldOfView(textBoxMObserverName.Text, TryParseFloat(textBoxMFOVH.Text), TryParseFloat(textBoxMFOVV.Text));
        }

        private void buttonSetStepSize_Click(object sender, EventArgs e)
        {
            simconnect.SetObserverStepSize(textBoxMObserverName.Text, TryParseFloat(textBoxMSSLinear.Text), TryParseFloat(textBoxMSSAngular.Text));
        }

        private void buttonSetFocus_Click(object sender, EventArgs e)
        {
            simconnect.SetObserverFocusFixed(textBoxMObserverName.Text, radioButtonMFocusPoint.Checked);
            simconnect.SetObserverFocalLength(textBoxMObserverName.Text, TryParseFloat(textBoxMFocalLength.Text));
        }

        private void buttonSetRegime_Click(object sender, EventArgs e)
        {
            if (checkBoxMGhostMode.Checked)
            {
                simconnect.SetObserverRegime(textBoxMObserverName.Text, (int)OBSERVER_REGIME_TYPE.OBSERVER_GHOST);
            }
            else if (checkBoxMPassWater.Checked)
            {
                simconnect.SetObserverRegime(textBoxMObserverName.Text, (int)OBSERVER_REGIME_TYPE.OBSERVER_TELLURIAN);
            }
            else
            {
                simconnect.SetObserverRegime(textBoxMObserverName.Text, (int)OBSERVER_REGIME_TYPE.OBSERVER_TERRESTRIAL);
            }
        }

        private void checkBoxMGhostMode_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxMGhostMode.Checked)
            {
                checkBoxMPassWater.Checked = true;
                checkBoxMPassWater.Enabled = false;
            }
            else
            {
                checkBoxMPassWater.Enabled = true;
            }
        }
        
    }
}
