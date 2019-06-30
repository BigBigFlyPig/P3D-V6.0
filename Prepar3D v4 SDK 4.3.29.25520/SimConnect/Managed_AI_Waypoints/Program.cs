//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Managed_AI_Waypoints
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new AI_Waypoints());
        }
    }
}