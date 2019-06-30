using System;
using System.Runtime.InteropServices;


namespace AIDestroyer
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    struct HealthStatus
    {
        /// <summary>
        /// The amount of health that the SimObject will be given.
        /// </summary>
        public long LifePoints;

        public HealthStatus(long lifePoints)
        {
            LifePoints = lifePoints;
        }
    }
}