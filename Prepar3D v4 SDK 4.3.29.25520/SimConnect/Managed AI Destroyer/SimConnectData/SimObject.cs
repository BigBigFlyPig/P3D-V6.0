using System;
using System.ComponentModel;


namespace AIDestroyer
{
    public class SimObject : IComparable<SimObject>, IEquatable<SimObject>, INotifyPropertyChanged
    {
        #region Events
        /// <summary>
        /// This event is fired off when a property of this SimObject is changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;
        #endregion

        #region Fields
        public uint id;

        public bool is_user;

        public bool is_alive;

        public long health_points;

        public string title;
        #endregion

        #region Properties
        /// <summary>
        /// The object ID for this Aircraft.
        /// </summary>
        public uint ObjectID
        {
            get
            {
                return id;
            }

            set
            {
                id = value;
                OnPropertyChanged("ObjectID");
            }
        }

        /// <summary>
        /// If this sim object is the user's sim.
        /// </summary>
        public bool IsUserSim
        {
            get
            {
                return is_user;
            }

            set
            {
                is_user = value;
                OnPropertyChanged("IsUserSim");
            }
        }

        /// <summary>
        /// If this sim object is alive or not.
        /// </summary>
        public bool IsAlive
        {
            get
            {
                return is_alive;
            }

            set
            {
                is_alive = value;
                OnPropertyChanged("IsAlive");
            }
        }

        /// <summary>
        /// The amount of health that this 
        /// </summary>
        public long HealthPoints
        {
            get
            {
                return health_points;
            }

            set
            {
                health_points = value;
                OnPropertyChanged("HealthPoints");
            }
        }

        /// <summary>
        /// The title for the sim object. Since strings can't be unlimited length
        /// we marshal it with a max size of 256.
        /// </summary>
        public string Title
        {
            get
            {
                return title;
            }

            set
            {
                title = value;
                OnPropertyChanged("Title");
            }
        }
        #endregion

        #region Constructors
        public SimObject() : this(string.Empty)
        {
        }

        public SimObject(string title)
        {
            ObjectID = 1;
            IsUserSim = false;
            IsAlive = true;
            HealthPoints = 100;
            Title = title;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Copies the data from the given SimObject into this SimObject.
        /// </summary>
        /// <param name="simObject">The SimObject to copy.</param>
        public void Copy(SimObject simObject)
        {
            ObjectID = simObject.ObjectID;
            IsUserSim = simObject.IsUserSim;
            IsAlive = simObject.IsAlive;
            HealthPoints = simObject.HealthPoints;
            Title = simObject.Title;
        }

        /// <summary>
        /// Compare two Aircraft. Return a strcmp like value back.
        /// </summary>
        /// <param name="other">The other aircraft to compare to.</param>
        /// <returns>strcmp style answer.</returns>
        public int CompareTo(SimObject other)
        {
            if (other != null)
            {
                 if (other.ObjectID > ObjectID)
                 {
                     return 1;
                 }
                 else if (other.ObjectID < ObjectID)
                 {
                     return -1;
                 }

                 return 0;
            }

            return -1;
        }

        /// <summary>
        /// Check if two aircraft references are the same.
        /// </summary>
        /// <param name="other">The other aircraft reference to check.</param>
        /// <returns>true if they match; otherwise false.</returns>
        public bool Equals(SimObject other)
        {
            if (other != null)
            {
                if (other.ObjectID == ObjectID)
                {
                    return true;
                }
            }

            return false;
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
        /// Get a string that describes this object. The title makes sense here.
        /// </summary>
        /// <returns>A string describing the aircraft.</returns>
        public override string ToString()
        {
            return Title;
        }
        #endregion       
    }
}
