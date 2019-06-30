//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Managed_Scenario_Controller
{
    static class Program
    {
        static void Main(string[] args)
        {
            ScenarioController Controller = new ScenarioController();
            Controller.Run();
        }
    }
}
