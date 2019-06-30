using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Add these two statements to all SimConnect clients
using LockheedMartin.Prepar3D.SimConnect;
using System.Runtime.InteropServices;

using System.Windows.Interop;
using System.Windows;
using System.ComponentModel;
using System.Windows.Controls;
using System.Collections.ObjectModel;

namespace Managed_Weapon_Station_Selection
{
    public class DataModel : INotifyPropertyChanged
    {
        public DataModel(Window window, ItemsControl itemsControl)
        {
            this.SimConnect = null;

            // Create an event handle for the WPF window to listen for SimConnect events
            Handle = new WindowInteropHelper(window).Handle; // Get handle of main WPF Window
            HandleSource = HwndSource.FromHwnd(Handle); // Get source of handle in order to add event handlers to it
            HandleSource.AddHook(HandleSimConnectEvents);

            // Set default values for properties
            this.NumberOfStations          = DEFAULT_NUMBER_OF_STATIONS;
            this.NumberOfStationsProcessed = DEFAULT_NUMBER_OF_STATIONS_PROCESSED;
            this.Title                       = DEFAULT_TITLE;
            this.SimConnectStatus            = DEFAULT_SIMCONNECT_STATUS;
            this.Stations = new ObservableCollection<Station>();

            // Set binding source 
            itemsControl.ItemsSource = this.Stations;
        }

        ~DataModel()
        {
            if (HandleSource != null)
            {
                HandleSource.RemoveHook(HandleSimConnectEvents);
            }
        }


        // User-defined win32 event
        private const int WM_USER_SIMCONNECT = 0x0402;

        private const int    DEFAULT_NUMBER_OF_STATIONS           = 0;
        private const int    DEFAULT_NUMBER_OF_STATIONS_PROCESSED = 0;
        private const string DEFAULT_TITLE                          = "[None]";
        private const string DEFAULT_SIMCONNECT_STATUS              = "";


        // SimConnect object
        public SimConnect SimConnect
        { get; private set; }

        public bool IsConnected
        {
            get {  return SimConnect != null; }
        }

        public IntPtr Handle
        { get; private set; }

        public HwndSource HandleSource
        { get; private set; }


        public enum DataIdentifier
        {
            RecieveSimObjectGeneral = 0,
            RecieveStationAtIndex = 1,
            RecieveStationEnd     = 1000,
            SendStationAtIndex    = 1001,
            SendStationEnd        = 2000
        }

        public enum EventIdentifier
        {
            Every6Hz = 0
        }


        // Data Structure associated with STATIONS_GENERAL
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        struct SimObjectGeneral
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public String Title;
            public double NumberOfStations;
        }


        // Data Structure associated with STATION_AT_INDEX
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        struct StationAtIndex
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public String Title;
            public double IsSelected;
        }

        public string SimConnectStatus
        { get; private set; }


        public string Title
        { get; private set; }

        public int NumberOfStations
        { get; private set; }

        private int NumberOfStationsProcessed
        { get; set; }

        public ObservableCollection<Station> Stations
        { get;  set; }

        #region Connection Methods

        public bool Connect(out string errorMessage)
        {
            if (this.SimConnect == null)
            {
                try
                {
                    if (Handle == null)
                    {
                        errorMessage = "Do not have access to Window Handle";
                        return false;
                    }


                    // the constructor is similar to SimConnect_Open in the native API
                    this.SimConnect = new SimConnect("Managed Weapon Station Selection", Handle, WM_USER_SIMCONNECT, null, 0);

                    OnPropertyChanged("IsConnected");

                    // Subscribe to events that we care about. We would like to know when we finish connecting, when the application exits,
                    // when an exception is raised, when events happen, and when simobject data is returned (respectively).
                    this.SimConnect.OnRecvOpen                += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvOpenEventHandler(SimConnect_OnRecvOpen);
                    this.SimConnect.OnRecvQuit                += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvQuitEventHandler(SimConnect_OnRecvQuit);
                    this.SimConnect.OnRecvException           += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvExceptionEventHandler(SimConnect_OnRecvException);
                    this.SimConnect.OnRecvEvent               += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvEventEventHandler(SimConnect_OnRecvEvent);
                    this.SimConnect.OnRecvSimobjectDataBytype += new LockheedMartin.Prepar3D.SimConnect.SimConnect.RecvSimobjectDataBytypeEventHandler(SimConnect_OnRecvSimobjectDataBytype);

                    // Subscribe to system events
                    this.SimConnect.SubscribeToSystemEvent(EventIdentifier.Every6Hz, "1sec"); //"6Hz");

                    // Initially turn the events off
                    this.SimConnect.SetSystemEventState(EventIdentifier.Every6Hz, SIMCONNECT_STATE.OFF);


                    // Successfully connected
                    errorMessage = string.Empty;
                    return true;
                }
                catch (COMException ex)
                {
                    // Something went wrong, Main window can display it to user.
                    errorMessage = "Unable to connect to Prepar3D:\n\n" + ex.Message;
                    return false;
                }
            }
            else
            {
                errorMessage = "Already connected to SimConnect.";
                return false;
            }

        }


        private IntPtr HandleSimConnectEvents(IntPtr hWnd, int message, IntPtr wParam, IntPtr lParam, ref bool isHandled)
        {
            isHandled = false;

            switch (message)
            {
                case WM_USER_SIMCONNECT:
                    {
                        if (SimConnect != null)
                        {
                            try
                            {
                                this.SimConnect.ReceiveMessage();
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

        public void Disconnect()
        {
            if (SimConnect != null)
            {
                StopMonitoring();

                // Dispose serves the same purpose as SimConnect_Close()
                this.SimConnect.Dispose();
                this.SimConnect = null;
                OnPropertyChanged("IsConnected");


                SimConnectStatus = "Disconnected from Prepar3D";
                OnPropertyChanged("SimConnectStatus");

                // Clear objects
                Stations.Clear();
                Title = DEFAULT_TITLE;
                NumberOfStationsProcessed = DEFAULT_NUMBER_OF_STATIONS_PROCESSED;
                NumberOfStations = DEFAULT_NUMBER_OF_STATIONS;
                OnPropertyChanged("Title");
                OnPropertyChanged("NumberOfStations");
            }

        }


        #endregion

        public void StartMonitoring()
        {
            try
            {
                this.SimConnect.SetSystemEventState(EventIdentifier.Every6Hz, SIMCONNECT_STATE.ON);
            }
            catch { }
        }

        public void StopMonitoring()
        {
            try
            {
                this.SimConnect.SetSystemEventState(EventIdentifier.Every6Hz, SIMCONNECT_STATE.ON);
            }
            catch { }
        }


        public void QuerySimObjectGeneralQuery()
        {
            //
            // NOTE:
            // Please refer to the Simulation Variables section in the SDK for specific variable names
            //
            try
            {
                this.SimConnect.AddToDataDefinition(DataIdentifier.RecieveSimObjectGeneral,
                                                    "Title",                        // Simulation Variable
                                                    null,                           // Units - for strings put 'null'
                                                    SIMCONNECT_DATATYPE.STRING256,  // Datatype
                                                    0.0f,
                                                    SimConnect.SIMCONNECT_UNUSED);
                this.SimConnect.AddToDataDefinition(DataIdentifier.RecieveSimObjectGeneral,
                                                    "Station Count",    // Simulation Variable
                                                    "number",                    // Units - unitless "number". Note: use lowercase.
                                                    SIMCONNECT_DATATYPE.FLOAT64, // Datatype
                                                    0.0f,
                                                    SimConnect.SIMCONNECT_UNUSED);



                // IMPORTANT: register it with the simconnect managed wrapper marshaller
                // if you skip this step, you will only receive a uint in the .dwData field.
                this.SimConnect.RegisterDataDefineStruct<SimObjectGeneral>(DataIdentifier.RecieveSimObjectGeneral);


                this.SimConnect.RequestDataOnSimObjectType(DataIdentifier.RecieveSimObjectGeneral,
                                                           DataIdentifier.RecieveSimObjectGeneral,
                                                           0,
                                                           SIMCONNECT_SIMOBJECT_TYPE.USER);
            }
            catch { RecoverFromError(); }

        }

        public void RecieveSimObjectGeneralQuery(SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
            // Try to cast
            SimObjectGeneral? simObjectGeneral = data.dwData[0] as SimObjectGeneral?;

            if (simObjectGeneral.HasValue)
            {
                // Doesn't check if stations have CHANGED but are the same amount as before.
                if (HaveStationsChanged(simObjectGeneral.Value.Title, simObjectGeneral.Value.NumberOfStations))
                {
                    Stations.Clear();
                }

                // Update SimObject display information
                Title = simObjectGeneral.Value.Title;
                OnPropertyChanged("Title");
                NumberOfStations = (int)simObjectGeneral.Value.NumberOfStations;
                OnPropertyChanged("NumberOfStations");

                // Now query all the indices now that we know how many there are
                QueryStationAtAllIndices();
            }
            else
            {
                // Cast failed
            }
        }

        private bool HaveStationsChanged(string title, double numberOfStations)
        {
            return !Title.Equals(title, StringComparison.InvariantCultureIgnoreCase) || numberOfStations != NumberOfStations;
        }

        public void QueryStationAtAllIndices()
        {
            for (int i = 0; i < NumberOfStations; i++)
            {  
                QueryStationAtIndex(i);
            }
        }

        public void QueryStationAtIndex(int i)
        {
            try
            {
                this.SimConnect.ClearDataDefinition(DataIdentifier.RecieveStationAtIndex + i);

                // Register each station with a unique request ID (1 - 1000) is reserved for stations in this example
                this.SimConnect.RegisterDataDefineStruct<StationAtIndex>(DataIdentifier.RecieveStationAtIndex + i);

                //
                // NOTE:
                // Please refer to the Simulation Variables section in the SDK for specific variable names
                //

                // Build request definition of what we want returned
                this.SimConnect.AddToDataDefinition(DataIdentifier.RecieveStationAtIndex + i,
                                                    "Weapon System Station Object Name:" + i, // Simulation Variable which is an array, index it with <Variable Name>:<index>
                                                    null,                                // Units - for strings put 'null'
                                                    SIMCONNECT_DATATYPE.STRING256,
                                                    0.0f,
                                                    SimConnect.SIMCONNECT_UNUSED);
                this.SimConnect.AddToDataDefinition(DataIdentifier.RecieveStationAtIndex + i,
                                                    "Weapon System Station Selected:" + i,           // Simulation Variable which is an array, index it with <Variable Name>:<index>
                                                    "bool",                                     // Units - for strings put 'null'
                                                    SIMCONNECT_DATATYPE.FLOAT64,                // Datatype (bools can use FLOAT64's)
                                                    0.0f,
                                                    SimConnect.SIMCONNECT_UNUSED);



                // Request data
                this.SimConnect.RequestDataOnSimObjectType(DataIdentifier.RecieveStationAtIndex + i, DataIdentifier.RecieveStationAtIndex + i, 0, SIMCONNECT_SIMOBJECT_TYPE.USER);
            }
            catch { RecoverFromError(); }

        }

        /// <summary>
        /// Method called every time a specific station data query is returned
        /// </summary>
        /// <param name="data">Data associated with the query </param>
        public void RecieveStationAtIndexQuery(SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
             // Try to cast
            StationAtIndex? stationAtIndex = data.dwData[0] as StationAtIndex?;

            if (stationAtIndex.HasValue)
            {
                // TODO: Do I need or not
                // Unregister request ID so it can be used again
                //this.SimConnect.ClearDataDefinition((DataIdentifier)data.dwRequestID);

                Station station = new Station(this);
                
                // Convert request ID to station ID
                int stationIndex = (int)(data.dwRequestID - (uint)DataIdentifier.RecieveStationAtIndex);

                // Fill data class
                station.Name = stationAtIndex.Value.Title;
                station.Index = stationIndex;
                station.IsSelected = stationAtIndex.Value.IsSelected == 1 ? true : false;
                station.SelectionChanged += new StationSendDataHandler(StationSelectionChanged);

                // Does this station exist?
                int cachedIndex = IndexOfStation(station);

                // Doesn't exist, add it
                if (cachedIndex == -1)
                {
                    Stations.Add(station);
                }
                // Does exist. Update it.
                else
                {
                    // Is it still being processed?
                    if (Stations[cachedIndex].BeingProcessed)
                    {
                        // If the expected value matches the returned value (System correctly updated the selected station state)
                        // then we're done processing that station
                        if (Stations[cachedIndex].ExpectedValueForIsSelected == station.IsSelected)
                        {
                            Stations[cachedIndex].BeingProcessed = false;
                        }
                        // Some extra logic in case SimConnect acts up and doesn't update properly.
                        else
                        {
                            if (Stations[cachedIndex].CurrentProcessRequests < Station.MAX_TIMES_TIL_RESEND)
                            {
                                Stations[cachedIndex].CurrentProcessRequests++;
                            }
                            else // Somehow the system STILL didn't get the request. Resend.
                            {
                                Stations[cachedIndex].CurrentProcessRequests = 0;
                                SetStation(new StationEventArgs(cachedIndex, Stations[cachedIndex].ExpectedValueForIsSelected));
                            }
                        }
                    }
                    // We are free to overwrite the previous station
                    else
                    {
                        Stations[cachedIndex] = station;
                        OnPropertyChanged("Stations");
                    }
                }

                NumberOfStationsProcessed++;

                if (NumberOfStationsProcessed == NumberOfStations)
                {
                    // Reset amount of stations processed
                    NumberOfStationsProcessed = 0;

                    FinishStationQuery();
                }
            }
        }

        public int IndexOfStation(Station point)
        {
            for (int i = 0; i < Stations.Count; i++)
            {
                if (Stations[i].Index == point.Index)
                    return i;
            }

            return -1;
        }

        private void StationSelectionChanged(StationEventArgs stationArgs)
        {
            SetStation(stationArgs);
        }

        public void FinishStationQuery()
        {
        }

        public void SetStation(StationEventArgs station)
        {
            try
            {
                this.SimConnect.ClearDataDefinition(DataIdentifier.SendStationAtIndex + station.Index);

                // Build data structure to send to P3D
                this.SimConnect.AddToDataDefinition(DataIdentifier.SendStationAtIndex + station.Index,
                                                    "Weapon System Station Selected:" + station.Index,  // Simulation Variable which is an array, index it with <Variable Name>:<index>
                                                    "bool",                                          // Units - "bool". Note: use lowercase.
                                                    SIMCONNECT_DATATYPE.INT32,
                                                    0.0f,
                                                    SimConnect.SIMCONNECT_UNUSED);

                this.SimConnect.SetDataOnSimObject(DataIdentifier.SendStationAtIndex + station.Index,
                                                   (uint)SIMCONNECT_SIMOBJECT_TYPE.USER,
                                                   SIMCONNECT_DATA_SET_FLAG.DEFAULT,
                                                   station.IsSelected ? 1 : 0); // Cast bool to int when working with type "BOOL"
            }
            catch { RecoverFromError(); }


        }

        private void SimConnect_OnRecvSimobjectDataBytype(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
            // Must be general SimObject information
            if (data.dwRequestID == (uint)DataIdentifier.RecieveSimObjectGeneral)
            {
                RecieveSimObjectGeneralQuery(data);
            }

            // Must be station data at a specific index
            if (data.dwRequestID >= (uint)DataIdentifier.RecieveStationAtIndex &&
                data.dwRequestID <= (uint)DataIdentifier.RecieveStationEnd)
            {
                RecieveStationAtIndexQuery(data);
            }

        }

        void SimConnect_OnRecvEvent(SimConnect sender, SIMCONNECT_RECV_EVENT data)
        {
            switch (data.uEventID)
            {
                case (uint)EventIdentifier.Every6Hz:
                {
                    // Check SimObject properties either to:
                    // a) Recieve information for the first time
                    // b) Check if station / SimObject information has changed
                    QuerySimObjectGeneralQuery();

                    break;
                }
            }

        }

        private void SimConnect_OnRecvException(SimConnect sender, SIMCONNECT_RECV_EXCEPTION data)
        {
            //SimConnectStatus = "Exception received: " + data.dwException;
            //OnPropertyChanged("SimConnectStatus");
        }

        private void SimConnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            this.SimConnect.SetSystemEventState(EventIdentifier.Every6Hz, SIMCONNECT_STATE.OFF);

            SimConnectStatus = "Prepar3D has exited";
            OnPropertyChanged("SimConnectStatus");
        }

        private void SimConnect_OnRecvOpen(SimConnect sender, SIMCONNECT_RECV_OPEN data)
        {
            SimConnectStatus = "Connected to Prepar3D";
            OnPropertyChanged("SimConnectStatus");
        }

        private void RecoverFromError()
        {
            string errorMessage;
            Disconnect();
            
            bool wasSuccess = Connect(out errorMessage);

            // Start monitoring the user's SimObject. This will continously monitor information
            // about the user's Stations attached to their SimObject.
            if (wasSuccess)
            {
                StartMonitoring();
            }
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
