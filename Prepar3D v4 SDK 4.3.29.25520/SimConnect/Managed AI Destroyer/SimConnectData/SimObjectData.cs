using System.Runtime.InteropServices;


namespace AIDestroyer
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    struct SimObjectData
    {
        /// <summary>
        /// If this sim object is the user's sim.
        /// </summary>
        public double IsUserSim;

        /// <summary>
        /// The amount of health that this 
        /// </summary>
        public long HealthPoints;

        /// <summary>
        /// The title for the sim object. Since strings can't be unlimited length
        /// we marshal it with a max size of 256.
        /// </summary>
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string Title;
    }
}