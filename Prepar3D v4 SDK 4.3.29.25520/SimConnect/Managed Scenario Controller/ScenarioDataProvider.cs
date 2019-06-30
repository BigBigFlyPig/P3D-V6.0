//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

// Add these two statements to all SimConnect clients
using LockheedMartin.Prepar3D.SimConnect;
using System.Runtime.InteropServices;

namespace Managed_Scenario_Controller
{
    public class ScenarioDataProvider
    {
        /// <summary>
        /// SimConnect Enum for Mission Completion Status types
        /// </summary>
        enum MissionStatus
        {
            MissionStatusFailed,
            MissionStatusCrashed,
            MissionStatusSucceeded,
        };

        public enum EventIdentifier
        {
            MissionCompleted = 0,
        }

        private SimConnect SimConnect = null;
        private Thread SimConnectThread = null;
        private bool mQuit = false;

        public ScenarioDataProvider()
        {
        }

        /// <summary>
        /// Starts the SimConnect based thread.
        /// </summary>
        public bool Start()
        {
            bool result = false;

            try
            {
                SimConnectThread = new Thread(new ThreadStart(StartSimConnectThread));
                SimConnectThread.IsBackground = true;
                SimConnectThread.Start();
                result = true;
            }
            catch
            {
            }

            return result;
        }

        /// <summary>
        /// Attempt to create the SimConnect connection with Prepar3D.
        /// </summary>
        private bool PollForConnection()
        {            
            int retryCounter = 1000;

            while (SimConnect == null && retryCounter > 0)
            {
                try
                {
                    SimConnect = new SimConnect("Managed Scenario Controller", IntPtr.Zero, 0, null, 0);
                    if (SimConnect != null)
                    {
                        SimConnect.OnRecvOpen += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvOpenEventHandler(SimConnect_OnRecvOpen);
                        SimConnect.OnRecvQuit += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvQuitEventHandler(SimConnect_OnRecvQuit);
                        SimConnect.OnRecvEvent += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvEventEventHandler(SimConnect_OnRecvEvent);

                        // Subscribe to system events
                        SimConnect.SubscribeToSystemEvent(EventIdentifier.MissionCompleted, "MissionCompleted");

                        // Turn the events on
                        SimConnect.SetSystemEventState(EventIdentifier.MissionCompleted, SIMCONNECT_STATE.ON);
                    }
                }
                catch
                {
                }

                if (SimConnect == null)
                {
                    Thread.Sleep(500);
                    --retryCounter;
                }
            }

            return (retryCounter > 0);
        }

        /// <summary>
        /// Creates the SimConnect connection with Prepar3D and
        /// continuously requests to receive messages.
        /// </summary>
        private void StartSimConnectThread()
        {
            Thread.Sleep(2500);

            if (PollForConnection())
            {
                if (SimConnect != null)
                {
                    while (!mQuit)
                    {
                        try
                        {
                            SimConnect.ReceiveMessage();
                        }
                        catch
                        {
                        }

                        Thread.Sleep(500);
                    }

                    if (mQuit)
                    {
                        SimConnect.Dispose();
                        SimConnect = null;
                    }
                }
            }
            else
            {
                System.Console.WriteLine("ERROR: SimConnect failed to connect (timed out).");
            }
        }

        /// <summary>
        /// Callback method for initial Receive-Open SimConnect operation
        /// </summary>
        /// <param name="sender">SimConnect object initiating the callback</param>
        /// <param name="data">Receive-Open SimConnect data object associated with the event</param>
        private void SimConnect_OnRecvOpen(SimConnect sender, SIMCONNECT_RECV_OPEN data)
        {
            System.Console.WriteLine("SimConnect connected to Prepar3D.");
        }

        /// <summary>
        /// Callback method for initial Receive-Quit SimConnect operation
        /// </summary>
        /// <param name="sender">SimConnect object initiating the callback</param>
        /// <param name="data">Receive-Quit SimConnect data object associated with the event</param>
        private void SimConnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            System.Console.WriteLine("SimConnect disconnected from Prepar3D.");
            mQuit = true;
        }

        /// <summary>
        /// Callback method for Receive-Event SimConnect operation
        /// </summary>
        /// <param name="sender">SimConnect object initiating the callback</param>
        /// <param name="data">Receive-Event SimConnect data object associated with the event</param>
        void SimConnect_OnRecvEvent(SimConnect sender, SIMCONNECT_RECV_EVENT data)
        {
            switch ((EventIdentifier)data.uEventID)
            {
                // Mission completion result.
                // This type of scenario information could potentially be used by an LMS,
                // however is beyond the scope of this sample.
                case EventIdentifier.MissionCompleted:
                {
                    switch ((MissionStatus)data.dwData)
                    {
                        case MissionStatus.MissionStatusFailed:
                        {
                            System.Console.WriteLine("Mission completion status: FAILED.");
                            break;
                        }
                        case MissionStatus.MissionStatusCrashed:
                        {
                            System.Console.WriteLine("Mission completion status: CRASHED.");
                            break;
                        }
                        case MissionStatus.MissionStatusSucceeded:
                        {
                            System.Console.WriteLine("Mission completion status: SUCCEEDED.");
                            break;
                        }
                    }

                    break;
                }
            }
        }
    }
}
