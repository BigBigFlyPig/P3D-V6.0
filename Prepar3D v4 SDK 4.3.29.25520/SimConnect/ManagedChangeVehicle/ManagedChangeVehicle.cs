///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  All Rights Reserved
///  
///  Description:  Managed sample code for changing the vehicle Prepar3D is
///                running through SimConnect.
///
///-----------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;

using LockheedMartin.Prepar3D.SimConnect;

namespace ManagedChangeVehicle
{
    class ManagedChangeVehicle
    {
        static void Main(string[] args)
        {
            try
            {
                SimConnect simulation_connection = new SimConnect("Managed Change Vehicle", IntPtr.Zero, 0,
                                                       null, 0);

                // Sending the title of the Vehicle
                simulation_connection.ChangeVehicle("Mooney Bravo Retro");

                simulation_connection.Dispose();
                simulation_connection = null;
            }
            catch (Exception ex)
            {
                // We were unable to connect so let the user know why.
                System.Console.WriteLine("Sim Connect unable to connect to Prepar3D!\n\n{0}\n\n{1}",
                                         ex.Message, ex.StackTrace);
            }
        }
    }
}
