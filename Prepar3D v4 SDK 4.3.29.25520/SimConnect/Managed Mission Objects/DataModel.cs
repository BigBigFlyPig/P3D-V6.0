using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Controls;

// Add these two statements to all SimConnect clients
using LockheedMartin.Prepar3D.SimConnect;
using System.Runtime.InteropServices;

namespace Managed_Mission_Objects
{
    public class DataModel : INotifyPropertyChanged
    {

        // SimConnect object
        public SimConnect SimConnectModule { get; private set; }

        public RichTextBox OutputLog { get; private set; }

        public IntPtr Handle { get; private set; }

        public bool IsConnected
        {
            get { return SimConnectModule != null; }
        }

        public string SimConnectStatus
        { get; private set; }

        public HwndSource HandleSource
        { get; private set; }

        private const int WM_USER_SIMCONNECT = 0x0402;

        private const string DEFAULT_SIMCONNECT_STATUS = "";

        enum DATA_REQUESTS
        {
            REQUEST_COUNT,
            REQUEST_INDEX,
            REQUEST_GUID,
        };

        enum EVENT_ID
        {
            FLIGHT_SEGMENT_READY_FOR_GRADING,
        }

        public DataModel(Window window, RichTextBox richTextBox)
        {
            this.SimConnectModule = null;

            // Create an event handle for the WPF window to listen for SimConnect events
            Handle = new WindowInteropHelper(window).Handle; // Get handle of main WPF Window
            HandleSource = HwndSource.FromHwnd(Handle); // Get source of handle in order to add event handlers to it
            HandleSource.AddHook(HandleSimConnectEvents);

            OutputLog = richTextBox;

            SimConnectStatus = DEFAULT_SIMCONNECT_STATUS;
        }

        ~DataModel()
        {
            if (HandleSource != null)
            {
                HandleSource.RemoveHook(HandleSimConnectEvents);
            }
        }

        private void WriteToOutput(string outputString)
        {
            OutputLog.AppendText("\n" + outputString);
            OutputLog.ScrollToEnd();
        }

        public void Connect()
        {
            if (SimConnectModule == null)
            {
                try
                {
                    // the constructor is similar to SimConnect_Open in the native API
                    SimConnectModule = new SimConnect("Managed Mission Object", this.Handle, WM_USER_SIMCONNECT, null, 0);

                    OnPropertyChanged("IsConnected");

                    SetupEventHandlers();

                }
                catch (COMException ex)
                {
                    WriteToOutput("Unable to connect to Prepar3D:\n\n" + ex.Message);
                }
            }
            else
            {
                WriteToOutput("Error - try again");
                Disconnect();
            }
        }

        private void SetupEventHandlers()
        {
            try
            {
                // listen to connect and quit msgs
                SimConnectModule.OnRecvOpen += new SimConnect.RecvOpenEventHandler(Simconnect_OnRecvOpen);
                SimConnectModule.OnRecvQuit += new SimConnect.RecvQuitEventHandler(Simconnect_OnRecvQuit);

                // listen to exceptions
                SimConnectModule.OnRecvException += new SimConnect.RecvExceptionEventHandler(Simconnect_OnRecvException);

                SimConnectModule.OnRecvGoal += new SimConnect.RecvGoalEventHandler(Simconnect_OnRecvGoal);

                SimConnectModule.OnRecvMissionObjectCount += new SimConnect.RecvMissionObjectCountEventHandler(SimConnect_OnRecvMissionObjectCount);

                SimConnectModule.OnRecvMissionObjective += new SimConnect.RecvMissionObjectiveEventHandler(Simconnect_OnRecvMissionObjective);

                SimConnectModule.OnRecvFlightSegment += new SimConnect.RecvFlightSegmentEventHandler(Simconnect_OnRecvFlightSegment);

                SimConnectModule.OnRecvParameterRange += new SimConnect.RecvParameterRangeEventHandler(Simconnect_OnRecvParameterRange);

                SimConnectModule.OnRecvFlightSegmentReadyForGrading += new SimConnect.RecvFlightSegmentReadyForGradingEventHandler(Simconnect_OnRecvFlightSegmentReadyForGrading);

                SimConnectModule.OnRecvMobileSceneryInRadius += new SimConnect.RecvMobileSceneryInRadiusEventHandler(SimConnectModule_OnRecvMobileSceneryInRadius);

                SimConnectModule.OnRecvMobileSceneryData += new SimConnect.RecvMobileSceneryDataEventHandler(SimConnectModule_OnRecvMobileSceneryData);

                SimConnectModule.SubscribeToSystemEvent(EVENT_ID.FLIGHT_SEGMENT_READY_FOR_GRADING, "FlightSegmentReadyForGrading");
                SimConnectModule.SetSystemEventState(EVENT_ID.FLIGHT_SEGMENT_READY_FOR_GRADING, SIMCONNECT_STATE.ON);
            }
            catch (COMException ex)
            {
                WriteToOutput(ex.Message);
            }
        }

        public void Disconnect()
        {
            if (SimConnectModule != null)
            {
                SimConnectModule.UnsubscribeFromSystemEvent(EVENT_ID.FLIGHT_SEGMENT_READY_FOR_GRADING);
                // Dispose serves the same purpose as SimConnect_Close()
                SimConnectModule.Dispose();
                SimConnectModule = null;
                OnPropertyChanged("IsConnected");

                SimConnectStatus = "Disconnected from Prepar3D";
                OnPropertyChanged("SimConnectStatus");

                WriteToOutput("Connection closed");
            }
        }

        void Simconnect_OnRecvOpen(SimConnect sender, SIMCONNECT_RECV_OPEN data)
        {
            WriteToOutput("Connected to Prepar3D");
            SimConnectStatus = "Connected to Prepar3D";
            OnPropertyChanged("SimConnectStatus");
        }

        // The case where the user closes Prepar3D
        void Simconnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            WriteToOutput("Prepar3D has exited");
            Disconnect();
        }

        void Simconnect_OnRecvException(SimConnect sender, SIMCONNECT_RECV_EXCEPTION data)
        {
            WriteToOutput("Exception received: " + data.dwException);
        }

        public void Simconnect_OnRecvGoal(SimConnect sender, SIMCONNECT_RECV_GOAL data)
        {
            if ((DATA_REQUESTS)data.dwRequestID == DATA_REQUESTS.REQUEST_GUID)
            {
                WriteToOutput("Order: " + data.dwOrder);
                WriteToOutput("Point Value:   " + data.dwPointValue);
                WriteToOutput("Guid:   " + data.guidInstanceID);
                WriteToOutput("IsOptional:   " + (data.isOptional != 0));
                WriteToOutput("Text:   " + data.szGoalText);

                string goalState;

                switch (data.eGoalState)
                {
                    case SIMCONNECT_GOAL_STATE.GOAL_COMPLETED:
                        goalState = "Completed";
                        break;
                    case SIMCONNECT_GOAL_STATE.GOAL_FAILED:
                        goalState = "Failed";
                        break;
                    case SIMCONNECT_GOAL_STATE.GOAL_PENDING:
                        goalState = "Pending";
                        break;
                    default:
                        goalState = "Invalid goal state";
                        break;
                }

                WriteToOutput("State:   " + goalState);
            }

            else
            {

                SimConnectModule.RequestGoalDataByGUID(DATA_REQUESTS.REQUEST_GUID, data.guidInstanceID);
                WriteToOutput("Goal Request By GUID");
            }

        }

        public void Simconnect_OnRecvMissionObjective(SimConnect sender, SIMCONNECT_RECV_MISSION_OBJECTIVE data)
        {
            if ((DATA_REQUESTS)data.dwRequestID == DATA_REQUESTS.REQUEST_GUID)
            {
                WriteToOutput("Order: " + data.dwOrder);
                WriteToOutput("Guid:   " + data.guidInstanceID);
                WriteToOutput("IsOptional:   " + (data.isOptional != 0));
                WriteToOutput("Text:   " + data.szMissionObjectiveText);
                WriteToOutput("Point Value:   " + data.dwPointValue);
                WriteToOutput("Current Score:   " + data.dwCurrentScore);
                WriteToOutput("Pass Value:   " + data.dwPassValue);
                WriteToOutput("Total Possible:   " + data.dwTotalPossiblePoints);
                WriteToOutput("Passed:   " + (data.isObjectivePassed != 0));


                string missionObjectiveState;

                switch (data.eMissionObjectiveStatus)
                {
                    case SIMCONNECT_MISSION_OBJECTIVE_STATUS.PASSED:
                        missionObjectiveState = "Passed";
                        break;
                    case SIMCONNECT_MISSION_OBJECTIVE_STATUS.FAILED:
                        missionObjectiveState = "Failed";
                        break;
                    case SIMCONNECT_MISSION_OBJECTIVE_STATUS.PENDING:
                        missionObjectiveState = "Pending";
                        break;
                    default:
                        missionObjectiveState = "Invalid goal state";
                        break;
                }

                WriteToOutput("Status:   " + missionObjectiveState);
            }

            else
            {

                SimConnectModule.RequestMissionObjectiveDataByGUID(DATA_REQUESTS.REQUEST_GUID, data.guidInstanceID);
                WriteToOutput("Mission Objective Request By GUID");
            }

        }

        void SimConnect_OnRecvMissionObjectCount(SimConnect sender, SIMCONNECT_RECV_MISSION_OBJECT_COUNT data)
        {
            if (data.eMissionObjectType == SIMCONNECT_MISSION_OBJECT_TYPE.GOAL)
            {
                for (int i = 0; i < data.dwCount; ++i)
                {
                    SimConnectModule.RequestGoalDataByIndex(DATA_REQUESTS.REQUEST_INDEX, i);
                    WriteToOutput("Goal Request Sent");
                }
            }

            else if (data.eMissionObjectType == SIMCONNECT_MISSION_OBJECT_TYPE.MISSION_OBJECTIVE)
            {
                for (int i = 0; i < data.dwCount; ++i)
                {
                    SimConnectModule.RequestMissionObjectiveDataByIndex(DATA_REQUESTS.REQUEST_INDEX, i);
                    WriteToOutput("Mission Objective Request Sent");
                }
            }

            else if (data.eMissionObjectType == SIMCONNECT_MISSION_OBJECT_TYPE.FLIGHT_SEGMENT)
            {
                for (int i = 0; i < data.dwCount; ++i)
                {
                    SimConnectModule.RequestFlightSegmentDataByIndex(DATA_REQUESTS.REQUEST_INDEX, i);
                    WriteToOutput("Flight Segment Request Sent");
                }
            }
        }

        void Simconnect_OnRecvFlightSegment(SimConnect sender, SIMCONNECT_RECV_FLIGHT_SEGMENT data)
        {
            WriteToOutput("Parameter Count: " + data.dwParameterCount);
            WriteToOutput("Total Range Count:   " + data.dwTotalRangeCount);
            WriteToOutput("Guid:   " + data.guidInstanceID);
            WriteToOutput("Goal Guid:   " + data.guidSegmentGoalID);

            for (int i = 0; i < data.dwTotalRangeCount; ++i)
            {
                SimConnectModule.RequestFlightSegmentRangeData(DATA_REQUESTS.REQUEST_INDEX, data.guidInstanceID, i);
                WriteToOutput("Range Request Sent");
            }

            if ((DATA_REQUESTS)data.dwRequestID == DATA_REQUESTS.REQUEST_GUID)
            {
                if (data.guidSegmentGoalID != Guid.Empty) //If the flight segment has a goal, pass it
                {
                    SimConnectModule.ResolveGoal(data.guidSegmentGoalID, SIMCONNECT_GOAL_RESOLUTION.COMPLETED);
                }
            }
        }

        void Simconnect_OnRecvParameterRange(SimConnect sender, SIMCONNECT_RECV_PARAMETER_RANGE data)
        {
            WriteToOutput("Parameter:  " + data.szParameterName);
            WriteToOutput("Range:  " + data.szRangeName);
            WriteToOutput("MaxOverMeasured:  " + data.dwMaxOverMeasured);
            WriteToOutput("MinUnderMeasured:  " + data.dwMinUnderMeasured);
            WriteToOutput("Exceeded Count:  " + data.dwExceededCount);
        }

        void Simconnect_OnRecvFlightSegmentReadyForGrading(SimConnect sender, SIMCONNECT_RECV_FLIGHT_SEGMENT_READY_FOR_GRADING recEvent)
        {
            switch (recEvent.uEventID)
            {
                case (uint)EVENT_ID.FLIGHT_SEGMENT_READY_FOR_GRADING:
                    WriteToOutput("Flight Segment with GUID " + recEvent.guidInstanceID + " Ready For Grading");
                    SimConnectModule.RequestFlightSegmentDataByGUID(DATA_REQUESTS.REQUEST_GUID, recEvent.guidInstanceID);
                    break;

                default:
                    WriteToOutput("Unknown event");
                    break;
            }
        }

        public void RequestGoals()
        {
            SimConnectModule.RequestGoalCount(DATA_REQUESTS.REQUEST_COUNT);
        }

        public void RequestMissionObjectives()
        {
            SimConnectModule.RequestMissionObjectiveCount(DATA_REQUESTS.REQUEST_COUNT);
        }

        public void RequestFlightSegments()
        {
            SimConnectModule.RequestFlightSegmentCount(DATA_REQUESTS.REQUEST_COUNT);
        }

        public void RequestAllMobileSceneryObjects()
        {
            const uint meterRadius = 21000; //Over 20,000 meters so will retrieve all mobile scenery objects
            SimConnectModule.RequestMobileSceneryInRadius(DATA_REQUESTS.REQUEST_COUNT, meterRadius);
        }

        void SimConnectModule_OnRecvMobileSceneryInRadius(SimConnect sender, SIMCONNECT_RECV_MOBILE_SCENERY_IN_RADIUS data)
        {
            for (uint i = 0; i < data.dwArraySize; ++i)
            {
                SimConnectModule.RequestMobileSceneryDataByID(DATA_REQUESTS.REQUEST_INDEX, (data.rgData[i] as SIMCONNECT_DATA_MOBILE_SCENERY_INFO).dwObjectID);
                WriteToOutput("Mobile Scenery Request Sent");
                WriteToOutput("Name: " + (data.rgData[i] as SIMCONNECT_DATA_MOBILE_SCENERY_INFO).szMobileSceneryName);
                WriteToOutput("ID: " + (data.rgData[i] as SIMCONNECT_DATA_MOBILE_SCENERY_INFO).dwObjectID);
            }
        }

        private double RadiansToDegrees(double radians)
        {
            return (radians) * (180d / Math.PI);
        }

        void SimConnectModule_OnRecvMobileSceneryData(SimConnect sender, SIMCONNECT_RECV_MOBILE_SCENERY_DATA data)
        {
            WriteToOutput("Data For Mobile Scenery Object ID: " + data.dwObjectID);
            WriteToOutput("Instance ID: " + data.guidInstanceID);
            WriteToOutput("IsOnGround: " + data.isOnGround);
            WriteToOutput("Latitude: " + data.llaWorldPosition.Latitude);
            WriteToOutput("Longitude: " + data.llaWorldPosition.Longitude);
            WriteToOutput("Altitude: " + data.llaWorldPosition.Altitude);
            WriteToOutput("Pitch: " + RadiansToDegrees(data.pbhWorldRotation.Pitch));
            WriteToOutput("Bank: " + RadiansToDegrees(data.pbhWorldRotation.Bank));
            WriteToOutput("Heading: " + RadiansToDegrees(data.pbhWorldRotation.Heading));
            WriteToOutput("Speed: " + data.speedKnots);
            WriteToOutput("Scale: " + data.scale);                         
        }

        private IntPtr HandleSimConnectEvents(IntPtr hWnd, int message, IntPtr wParam, IntPtr lParam, ref bool isHandled)
        {
            isHandled = false;

            switch (message)
            {
                case WM_USER_SIMCONNECT:
                    {
                        if (SimConnectModule != null)
                        {
                            try
                            {
                                this.SimConnectModule.ReceiveMessage();
                            }
                            catch { RecoverFromError(); }

                            isHandled = true;
                        }
                    }
                    break;

                default:
                    break;
            }

            return IntPtr.Zero;
        }


        private void RecoverFromError()
        {
            Disconnect();

            Connect();

        }

        #region INotifyPropertyChanged : Allows properties that are bound to tell their controls that they're bound to that they've been updated

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyname)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(propertyname));
            }
        }

        #endregion
    }
}
