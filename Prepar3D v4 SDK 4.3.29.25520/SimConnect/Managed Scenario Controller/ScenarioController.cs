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
    public class ScenarioController
    {
        private enum Selection 
        {
            Quit = 0,
            LaunchScenario = 1,
            Max,
        };
        
        private const string SubKeyString = @"Software\Lockheed Martin\Prepar3D v4";
        private const string ExecutableFilename = "Prepar3D.exe";

        private String mSetupPath = "";             // The Prepar3D application directory.
        private String mExecutableFilePath = "";    // The Prepar3D.exe file path.
        private String mScenarioFilename = "";      // The filename of the scenario to load.

        // Boolean to determine if the application should quit.
        private bool mQuit = false;

        // Boolean representing whether or not the Prepar3D process is running.
        private bool mProcRunning = false;

        public void Run()
        {
            System.Console.WriteLine("Prepar3D Scenario Controller Sample\n");

            // Attempt to retrieve the Prepar3D application directory from the registry.
            bool validSetupPath = LookupSetupPath();

            // If we fail to locate it from the registry, prompt the user for the location.
            if (!validSetupPath)
            {
                validSetupPath = PromptForSetupPath();
            }

            // Continue if the provided path is valid.
            if (validSetupPath)
            {
                // Confirm the executable exists.
                bool validExecutablePath = InitializeExecutablePath();
                if (validExecutablePath)
                {
                    // User selection loop until they quit.
                    while (!mQuit)
                    {
                        // Get the selected option from the user.
                        Selection selection = PromptForSelection();
                        switch(selection)
                        {
                            case Selection.LaunchScenario:
                            {
                                // Get the scenario file name from the user.
                                if (PromptForScenario())
                                {
                                    // Launch the Prepar3D application with the selected scenario.
                                    LaunchScenario();

                                    // Confirm the application successfully started.
                                    if (mProcRunning)
                                    {
                                        // Create and start the SimConnect based scenario data provider.
                                        ScenarioDataProvider DataProvider = new ScenarioDataProvider();
                                        if (DataProvider.Start())
                                        {
                                            // Wait for the Prepar3D process to exit.
                                            while (mProcRunning)
                                            {
                                                Thread.Sleep(1000);
                                            }
                                        }

                                        mProcRunning = false;
                                    }
                                }
                                break;
                            }
                            case Selection.Quit:
                            {
                                mQuit = true;
                                break;
                            }
                        }
                    }
                }
            }

            // Leave the console open to display errors if the user didn't manually quit.
            if (!mQuit)
            {
                System.Console.WriteLine("Press enter to exit the application...");
                System.Console.ReadLine();
            }
        }

        private void LaunchScenario()
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();

            startInfo.FileName = mExecutableFilePath;
            startInfo.Arguments = "\"" + mScenarioFilename + "\"";

            System.Console.WriteLine(String.Format("Attempting to launch scenario: \"{0}\".", mScenarioFilename));

            try
            {
                Process proc = Process.Start(startInfo);
                proc.EnableRaisingEvents = true;
                proc.Exited += Proc_Exited;
                mProcRunning = true;
                System.Console.WriteLine("Prepar3D process started.");
            }
            catch
            {
                System.Console.WriteLine("ERROR: Failed to start Prepar3D process.");
            }
        }

        private void Proc_Exited(object sender, EventArgs e)
        {
            System.Console.WriteLine("Prepar3D process closed.");
            System.Console.WriteLine("");
            mProcRunning = false;
        }

        private Selection PromptForSelection()
        {
            Selection result = Selection.Quit;
            
            while (true)
            {
                System.Console.WriteLine("Select an option:");
                System.Console.WriteLine("1. Launch Scenario");
                System.Console.WriteLine("0. Quit");

                uint selection = 0;
                String userInput = System.Console.ReadLine();
                System.Console.WriteLine("");
                if (String.IsNullOrWhiteSpace(userInput) || !UInt32.TryParse(userInput, out selection) || selection >= (uint)Selection.Max)
                {
                    System.Console.WriteLine("ERROR: Invalid option.");
                }
                else
                {
                    result = (Selection)selection;
                    break;
                }
            }

            return result;
        }
        
        private bool PromptForScenario()
        {
            bool result = false;

            string scenarioFilename = "";

            while (String.IsNullOrWhiteSpace(scenarioFilename))
            {
                System.Console.WriteLine("Enter the filename of the scenario to launch:");
                scenarioFilename = System.Console.ReadLine();
                System.Console.WriteLine("");
                if (String.IsNullOrWhiteSpace(scenarioFilename))
                {
                    System.Console.WriteLine("ERROR: Please enter a valid scenario filename.");
                }
            }

            mScenarioFilename = scenarioFilename;
            result = true;

            return result;
        }

        private bool InitializeExecutablePath()
        {
            bool result = false;

            string executableFilePath = mSetupPath + ExecutableFilename;

            if (File.Exists(executableFilePath))
            {
                mExecutableFilePath = executableFilePath;
                result = true;
            }
            else
            {
                System.Console.WriteLine(String.Format("ERROR: Failed to locate \"{0}\".", executableFilePath));
            }

            return result;
        }

        private bool LookupSetupPath()
        {
            bool result = false;

            // Force the 64-bit version location.
            Microsoft.Win32.RegistryKey BaseKey = Microsoft.Win32.RegistryKey.OpenBaseKey(Microsoft.Win32.RegistryHive.LocalMachine,
                                                                                          Microsoft.Win32.RegistryView.Registry64);
            if (BaseKey != null)
            {
                Microsoft.Win32.RegistryKey SubKey = BaseKey.OpenSubKey(SubKeyString);
                if (SubKey != null)
                {
                    string setupPath = SubKey.GetValue("SetupPath") as string;
                    if (!string.IsNullOrEmpty(setupPath))
                    {
                        if (Directory.Exists(setupPath))
                        {
                            mSetupPath = setupPath;
                            result = true;
                        }
                    }
                }
            }

            if (!result)
            {
                System.Console.WriteLine("WARNING: Failed to lookup Prepar3D installation directory from registry.");
            }

            return result;
        }

        private bool PromptForSetupPath()
        {
            bool result = false;

            System.Console.WriteLine("Enter the installation directory of Prepar3D:");

            string setupPath = "";

            while (!Directory.Exists(setupPath))
            {
                setupPath = System.Console.ReadLine();
                if (!Directory.Exists(setupPath))
                {
                    System.Console.WriteLine(String.Format("ERROR: The directory \"{0}\" does not exist. Please enter a valid directory.", setupPath));
                }
            }

            mSetupPath = setupPath;
            result = true;

            return result;
        }
    }
}
