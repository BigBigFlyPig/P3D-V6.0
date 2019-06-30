using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;
using System.Windows.Threading;

namespace AIDestroyer
{
    public class DestroyerModel : SimConnectModel
    {
        #region Events
        /// <summary>
        /// Defines a function that can be invoked for adding a SimObject to the SimObjectList.
        /// This is used to put the call on the right thread.
        /// </summary>
        /// <param name="simObject"></param>
        private delegate void AddSimObjectHandler(SimObject simObject);
        #endregion

        #region Fields
        /// <summary>
        /// The connection to the simulation.
        /// </summary>
        private DestroyerConnector simulation_connection;

        /// <summary>
        /// The selected object.
        /// </summary>
        private SimObject selected_object;
        #endregion

        #region Properties
        /// <summary>
        /// A list of all the SimObjects in the simulation that are within range of the User's simObject.
        /// </summary>
        public ObservableCollection<SimObject> SimObjectList
        { get; private set; }

        /// <summary>
        /// The currently selected object.
        /// </summary>
        public SimObject SelectedObject
        {
            get
            {
                return selected_object;
            }

            set
            {
                selected_object = value;

                simulation_connection.SetCurrentObject(selected_object);

                // Mark that some of the properties of this class have changed.
                OnPropertyChanged("SelectedObject");
                OnPropertyChanged("IsSimObjectAlive");
                OnPropertyChanged("IsSimObjectSelected");
            }
        }

        /// <summary>
        /// Returns if the currently selected SimObject is alive.
        /// </summary>
        public bool IsSimObjectAlive
        {
            get
            {
                if (selected_object != null)
                {
                    return selected_object.IsAlive;
                }

                // Default to the SimObject being dead.
                return false;
            }

            set
            {
                selected_object.IsAlive = value;

                if (selected_object.IsAlive)
                {
                    simulation_connection.SetHealth(1);
                }
                else
                {
                    simulation_connection.SetHealth(0);
                }

                OnPropertyChanged("IsSimObjectAlive");
            }
        }

        /// <summary>
        /// Returns whether or not a SimObject is currently selected.
        /// </summary>
        public bool IsSimObjectSelected
        {
            get
            {
                return selected_object != null;
            }
        }
        #endregion

        #region Constructors
        /// <summary>
        /// Create a new Weapon Data Model.
        /// </summary>
        public DestroyerModel()
        {
            // Create a new simulation connection.
            simulation_connection = new DestroyerConnector("AI Destroyer");
            SimulationConnection = simulation_connection;

            // Now initialize the observable collection.
            SimObjectList = new ObservableCollection<SimObject>();
        }
        #endregion

        #region Methods
        /// <summary>
        /// Initialize this data model.
        /// </summary>
        public override void Init()
        {
            base.Init();

            // Register to receive the sim object data requests from the weapon connector.
            simulation_connection.SimObjectDataRequests += new EventHandler<GenericEventArgs<SimObject>>(SimulationConnectionSimObjectDataRequests);
            simulation_connection.HealthStatusRequests += new EventHandler<GenericEventArgs<HealthUpdate>>(SimulationConnectionHealthStatusRequests);
        }

        /// <summary>
        /// Update the list of SimObject we know about that have weapons.
        /// </summary>
        public void UpdateSimList()
        {
            // Clear the SimObjectList.
            SimObjectList.Clear();

            // Now just get all the SimObject around the user again.
            simulation_connection.GetSimObjectData();
        }

        /// <summary>
        /// Selects the given SimObject to be used as the main SimObject.
        /// </summary>
        /// <param name="simObject"></param>
        public void SelectSimObject(SimObject simObject)
        {
            // Set the selected object.
            SelectedObject = simObject;
        }

        /// <summary>
        /// Add an simObject to the sim object list.
        /// It must have a weapon system.
        /// </summary>
        /// <param name="simObject"></param>
        private void AddSimObject(SimObject simObject)
        {
            // I only care about SimObject that are not the user and I don't want any repeats.
            if (!simObject.IsUserSim && !SimObjectList.Contains(simObject))
            {
                SimObjectList.Add(simObject);
            }
        }

        /// <summary>
        /// Receives the SimObjectDataRequest and adds the simObject to the SimObjectList.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void SimulationConnectionSimObjectDataRequests(object sender, GenericEventArgs<SimObject> args)
        {
            AddSimObjectHandler aircraftCreation;

            // Need to do this to get the add call on the right thread.
            aircraftCreation = AddSimObject;

            // Should make a multithreaded ObservableCollection instead......
            System.Windows.Application.Current.Dispatcher.Invoke(aircraftCreation, args.Value);
        }

        /// <summary>
        /// Receives the SimObjectDataRequest and adds the simObject to the SimObjectList.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void SimulationConnectionHealthStatusRequests(object sender, GenericEventArgs<HealthUpdate> args)
        {
            // Go through and find out what SimObject this HealthStatus is for and update it.
            foreach (SimObject simObject in SimObjectList)
            {
                if (simObject.ObjectID == args.Value.ObjectID)
                {
                    simObject.HealthPoints = args.Value.HealthPoints;
                }
            }
        }
        #endregion
    }
}
