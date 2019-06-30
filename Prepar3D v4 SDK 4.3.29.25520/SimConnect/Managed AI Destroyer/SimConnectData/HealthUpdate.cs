using System;
using System.ComponentModel;


namespace AIDestroyer
{
    public class HealthUpdate
    {
        #region Properties
        /// <summary>
        /// The object ID this HealthUpdate is for.
        /// </summary>
        public uint ObjectID
        { get; set; }

        /// <summary>
        /// The amount of health that this 
        /// </summary>
        public long HealthPoints
        { get; set; }
        #endregion

        #region Constructors
        public HealthUpdate()
            : this(string.Empty)
        {
        }

        public HealthUpdate(string title)
        {
            ObjectID = 1;
            HealthPoints = 100;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Get a string that describes this object. The title makes sense here.
        /// </summary>
        /// <returns>A string describing the aircraft.</returns>
        public override string ToString()
        {
            return HealthPoints.ToString();
        }
        #endregion
    }
}
