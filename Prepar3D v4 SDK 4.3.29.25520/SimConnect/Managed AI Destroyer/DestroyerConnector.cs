using System;
using System.Collections.Generic;
using System.Threading;

using LockheedMartin.Prepar3D.SimConnect;


namespace AIDestroyer
{
    class DestroyerConnector : SimConnector
    {
        #region Constants
        /// <summary>
        /// The maximum radius allowed for querying AI traffic as stated in the SDK.
        /// </summary>
        private const uint MAXIMUM_RADIUS = 200000;
        #endregion

        #region Events
        /// <summary>
        /// Fired off when a simobject request is received.
        /// </summary>
        public event EventHandler<GenericEventArgs<SimObject>> SimObjectDataRequests;

        /// <summary>
        /// Fired off when a health status request is received.
        /// </summary>
        public event EventHandler<GenericEventArgs<HealthUpdate>> HealthStatusRequests;
        #endregion

        #region Fields
        /// <summary>
        /// The current aircraft. This is used in setting values.
        /// Mostly just need the ObjectID from it.
        /// </summary>
        private SimObject current_object;
        #endregion

        #region Constructors
        /// <summary>
        /// Create a new simulation connection.
        /// </summary>
        /// <param name="connectionName">The name for this connection.</param>
        public DestroyerConnector(string connectionName) : base(connectionName)
	    {
	    }
        #endregion

        #region Methods
        /// <summary>
        /// Get the SimObject data for all the simulation objects near the user's aircraft.
        /// </summary>
        public void GetSimObjectData()
        {
            // Only do this if there is a valid simulation connection.
            if (simulation_connection != null)
            {
                try
                {
                    // Get the sim objects at the maximum distance radius allowed.
                    simulation_connection.RequestDataOnSimObjectType(DataIdentifier.RetreiveSimObjects, StructureType.SimObject, MAXIMUM_RADIUS, SIMCONNECT_SIMOBJECT_TYPE.AIRCRAFT);
                }
                catch (Exception ex)
                {
                    SendError(ex.Message);
                }
            }
        }

        public void SetCurrentObject(SimObject simObject)
        {
            UnloadCurrentObjectData();

            current_object = simObject;
        }

        public void SetHealth(long health)
        {
            if (current_object != null)
            {
                // Set the object's health value.
                simulation_connection.SetDataOnSimObject(StructureType.HealthStatus,
                                                         current_object.ObjectID,
                                                         SIMCONNECT_DATA_SET_FLAG.DEFAULT,
                                                         new HealthStatus(health));

                // Now ask for an update on the objects health value.
                simulation_connection.RequestDataOnSimObjectType(DataIdentifier.RetreiveHealthInfo, StructureType.HealthStatus, MAXIMUM_RADIUS, SIMCONNECT_SIMOBJECT_TYPE.AIRCRAFT);
            }
        }

        /// <summary>
        /// Called when the connector connects to Prepar3D.
        /// </summary>
        protected override void OnConnected()
        {
            base.OnConnected();

            // Get the SimObject data for this aircraft.
            GetSimObjectData();
        }

        /// <summary>
        /// Register for any events that need to be listened to.
        /// </summary>
        protected override void SimConnectEventRegistration()
        {
            // Register for the base classes desired events.
            base.SimConnectEventRegistration();

            // Register for the sim connect events we care about.
            simulation_connection.OnRecvSimobjectDataBytype += new SimConnect.RecvSimobjectDataBytypeEventHandler(SimulationConnectionSimobjectDataByType);
        }

        /// <summary>
        /// Define the data structures that we need for talking to Prepar3D.
        /// </summary>
        protected override void DefineDataStructures()
        {
            // Define any base class data structures that are needed.
            base.DefineDataStructures();

            DefineSimObjectStructure();
            DefineHealthStatusStructure();
        }

        /// <summary>
        /// Unloads the current aicraft if it exists.
        /// </summary>
        private void UnloadCurrentObjectData()
        {
            // Only do this if there is an aircraft already.
            if (current_object != null)
            {
                try
                {
                    current_object = null;
                }
                catch (Exception ex)
                {
                    SendError(ex.Message);
                }
            }
        }

        /// <summary>
        /// Defines the SimObject data structure that will be exchanged between this app and the Prepar3D simulation.
        /// </summary>
        private void DefineSimObjectStructure()
        {
            // Define the SimObject data structure. These MUST be added in the EXACT same order as they are listed in the structure.
            simulation_connection.AddToDataDefinition(StructureType.SimObject, "Is User Sim", "Bool", SIMCONNECT_DATATYPE.FLOAT64, 0.0f, SimConnect.SIMCONNECT_UNUSED);
            simulation_connection.AddToDataDefinition(StructureType.SimObject, "Health Points", "Number", SIMCONNECT_DATATYPE.INT64, 0.0f, SimConnect.SIMCONNECT_UNUSED);
            simulation_connection.AddToDataDefinition(StructureType.SimObject, "Title", null, SIMCONNECT_DATATYPE.STRING256, 0.0f, SimConnect.SIMCONNECT_UNUSED);

            // IMPORTANT: register it with the simconnect managed wrapper marshaller
            // if you skip this step, you will only receive a uint in the .dwData field.
            simulation_connection.RegisterDataDefineStruct<SimObjectData>(StructureType.SimObject);
        }

        /// <summary>
        /// Defines the HealthStatus data structure.
        /// </summary>
        private void DefineHealthStatusStructure()
        {
            // Define the health status data structure. These MUST be added in the EXACT same order as they are listed in the structure.
            simulation_connection.AddToDataDefinition(StructureType.HealthStatus, "Health Points", "Number",
                                                      SIMCONNECT_DATATYPE.INT64, 0.0f, SimConnect.SIMCONNECT_UNUSED);

            // IMPORTANT: register it with the simconnect managed wrapper marshaller
            // if you skip this step, you will only receive a uint in the .dwData field.
            simulation_connection.RegisterDataDefineStruct<HealthStatus>(StructureType.HealthStatus);
        }

        /// <summary>
        /// Turn a sim object into an aircraft class.
        /// </summary>
        /// <param name="simObject"></param>
        /// <returns></returns>
        private SimObject SimObjectToClass(SimObjectData simObject, uint objectID)
        {
            SimObject data;

            // Create the new aircraft.
            data = new SimObject();

            // Set the values of the SimObject based on the sim object's data.
            data.ObjectID = objectID;
            data.IsUserSim = (simObject.IsUserSim == 0) ? false : true;
            data.IsAlive = (simObject.HealthPoints > 0) ? true : false;
            data.HealthPoints = simObject.HealthPoints;
            data.Title = simObject.Title;

            // Return the SimObject.
            return data;
        }

        /// <summary>
        /// Turn a sim object into an aircraft class.
        /// </summary>
        /// <param name="simObject"></param>
        /// <returns></returns>
        private HealthUpdate HealthStatusToClass(HealthStatus status, uint objectID)
        {
            HealthUpdate update;

            // Create the new aircraft.
            update = new HealthUpdate();

            // Set the values of the SimObject based on the sim object's data.
            update.ObjectID = objectID;
            update.HealthPoints = status.LifePoints;

            // Return the SimObject.
            return update;
        }

        /// <summary>
        /// This is were we will be receiving SimObject information. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="data"></param>
        private void SimulationConnectionSimobjectDataByType(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
            // Determine what type of SimObject data we received by checking the data ID against our DataIdentifier Enum.
            switch ((DataIdentifier)data.dwRequestID)
            {
                // Handle SimObject data.
                case DataIdentifier.RetreiveSimObjects:
                {
                    SimObjectData simObject;

                    // Convert the SimObject to a SimObject and let whoever cares know that there is a SimObject.
                    simObject = (SimObjectData)data.dwData[0];
                    SimObjectDataRequests.SafeInvoke(this, new GenericEventArgs<SimObject>(SimObjectToClass(simObject, data.dwObjectID)));
                    break;
                }

                case DataIdentifier.RetreiveHealthInfo:
                {
                    HealthStatus status;

                    // Convert this to a HealthStatusUpdate.
                    status = (HealthStatus)data.dwData[0];
                    HealthStatusRequests.SafeInvoke(this, new GenericEventArgs<HealthUpdate>(HealthStatusToClass(status, data.dwObjectID)));
                    break;
                }

                // No idea what this is.
                default:
                {
                    SendError("Unknown request ID: " + data.dwRequestID.ToString());
                    break;
                }
            }
        }
        #endregion
    }
}
