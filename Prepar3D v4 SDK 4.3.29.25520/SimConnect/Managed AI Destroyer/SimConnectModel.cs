using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Threading;

using LockheedMartin.Prepar3D.SimConnect;


namespace AIDestroyer
{
    public class SimConnectModel : INotifyPropertyChanged
    {
        #region Constants
        /// <summary>
        /// The connected status string.
        /// </summary>
        private const string CONNECTED_STATUS = "Connected";

        /// <summary>
        /// The disconnected status string.
        /// </summary>
        private const string DISCONNECTED_STATUS = "Disconnected";
        
        /// <summary>
        /// The error status string.
        /// </summary>
        private const string ERROR_STATUS = "Error";
        #endregion

        #region Events
        /// <summary>
        /// Fired off when a property is changed.
        /// This allows the data model to be bound to the view.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// An event that is raised when errors in the data model occur.
        /// </summary>
        public event EventHandler<MessageEventArgs> Error;
        #endregion

        #region Fields
        /// <summary>
        /// The status of the current simulation connection.
        /// </summary>
        private string connection_status;
        #endregion

        #region Properties
        /// <summary>
        /// Get the status of the current simulation connection.
        /// </summary>
        public string Status
        {
            get
            {
                return connection_status;
            }

            private set
            {
                connection_status = value;

                // Mark that this property has changed so any bound items can be updated.
                OnPropertyChanged("Status");
            }
        }

        /// <summary>
        /// Gets or sets the simulation connection currently being used.
        /// If you derive from this class then assign this to the
        /// SimConnector class derivative you are using.
        /// </summary>
        protected SimConnector SimulationConnection
        { get; set; }
        #endregion

        #region Constructors
        /// <summary>
        /// Creates the data model for this SDK sample.
        /// </summary>
        public SimConnectModel()
        {
            SimulationConnection = null;

            Status = DISCONNECTED_STATUS;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Initialize the data model.
        /// </summary>
        public virtual void Init()
        {
            // Register for the events about the simulation connection.
            if (SimulationConnection != null)
            {
                SimulationConnection.Connected += new EventHandler<EventArgs>(SimConnectorConnected);
                SimulationConnection.Disconnected += new EventHandler<EventArgs>(SimConnectorDisconnected);
                SimulationConnection.Error += new EventHandler<MessageEventArgs>(SimConnectorError);
            }
        }

        /// <summary>
        /// Connect to the Prepar3D simulation.
        /// </summary>
        public void ConnectToSimulation()
        {
            if (SimulationConnection != null)
            {
                SimulationConnection.Connect();
            }
        }

        /// <summary>
        /// Disconnect from the Prepar3D simulation.
        /// </summary>
        public void DisconnectFromSimulation()
        {
            if (SimulationConnection != null)
            {
                SimulationConnection.Disconnect();
            }
        }

        /// <summary>
        /// Notify any interested parties that a property has changed.
        /// </summary>
        /// <param name="name"></param>
        protected void OnPropertyChanged(string name)
        {
            PropertyChangedEventHandler handler;
            
            // Needs a safe invoke that does this check for you. :-(
            handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(name));
            }
        }

        /// <summary>
        /// Sends out an error event.
        /// </summary>
        /// <param name="message">The message to send.</param>
        protected void SendError(string message)
        {
            // Send an error event.
            Error.SafeInvoke(this, new MessageEventArgs(message));
        }

        /// <summary>
        /// Handle the event of the SimConnector connecting to Prepar3D.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        protected virtual void SimConnectorConnected(object sender, EventArgs args)
        {
            Status = CONNECTED_STATUS;
        }

        /// <summary>
        /// Handle the event of the SimConnector disconnecting to Prepar3D.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        protected virtual void SimConnectorDisconnected(object sender, EventArgs args)
        {
            Status = DISCONNECTED_STATUS;
        }

        /// <summary>
        /// Handle the event of the SimConnector having an error.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        protected virtual void SimConnectorError(object sender, MessageEventArgs args)
        {
            Status = ERROR_STATUS;

            // Forward on this error to any interested parties.
            // I do this instead of send error because I don't have to create another MessageEventArg then.
            Error.SafeInvoke(this, args);
        }
        #endregion
    }
}
