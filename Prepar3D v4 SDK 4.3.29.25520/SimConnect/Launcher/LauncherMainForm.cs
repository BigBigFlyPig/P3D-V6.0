//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
// Sim connect so we can take focus from the application on the end mission event.
using LockheedMartin.Prepar3D.SimConnect;
using System.Runtime.InteropServices;


namespace Launcher
{
    
    public partial class Launcher : Form
    {
        // Constants to find the application (in the registry and running)
        const string APPNAME = "Prepar3D.exe";
        const string APPREGPATH = "HKEY_CURRENT_USER\\SOFTWARE\\Lockheed Martin\\Prepar3D v4";
        const string APPREGKEY = "AppPath";
        const string APPLICATION_EXTENSION = "Executable | *.exe";  // the extension to the application


        // Constants to pass to the application
        const string MPJOIN = "-mpjoin";                            //for Joining Multiplayer sessions
        const string MP = "-mp";                                    //for hosting multiplayer sessions

        //  Constants for opening the manifest 
        const string MANIFEST_EXTENSION = "Manifest File | *.xml";  // the extension to the manifest
        const string DEFAULT_MANIFEST = "Default.xml";

        const string ERROR_LOADING_MANIFEST = "There was an error loading the manifest {0} error: {1}";
        const string MULTIPLAYER_FREE_FLIGHT_DESCRIPTION = "Host a multiplayer free flight.";
        const string MULTIPLAYER_JOIN_DESCRIPTION = "Join a multiplayer Session";
        const string DESCRIPTION_NOT_FOUND = "No Description was found";
        const string ABOUT_LAUNCHER = "Lockheed Martin Prepar3D Sample Solution Launcher.";
        const string SCENERY_OVERRIDE = "Scenery Override";
        const string SELECT_SCENARIO = "Select Scenario";
        
        //Specifies if we are to pass along the "-dev" switch to the runtime
        //const string DEV_FLAG = "-dev";

        // SimConnect constant
        const int WM_USER_SIMCONNECT = 0x0402;
        const string MISSION_COMPLETED = "MissionCompleted";
    
        //Member variables
        SolutionManifest theManifest = null;                    //the manifest currently opened.
        System.Collections.ArrayList currentScenarios = null;   //the list of scenarios 
        Selection currentPressed = Selection.SOLO;              //the currently selected button (start at the top)
        string thisAppDirectory;                                //for storing the starting current directory
        SimConnect simconnect = null;                           // SimConnect object for receiving end mission event from application so we can take focus
        string applicationPath = null;                          //Store the path (including the application Name) of the application
        bool devMode = false;

        // Enumerators
        enum Selection // Specifies which button is selected
        {
            SOLO,
            HOST_MP_MISSION,
            JOIN,
        };
        
        enum Event // the one event we are looking for from SimConnect
        {
            MISSION_END,          
        };

        public Launcher()
        {
            InitializeComponent();
        }

        public string GetLauncherPath()
        {
            return thisAppDirectory;
        }

        private void Launcher_Load(object sender, EventArgs e)
        {           
            // get the current directory and save it
            thisAppDirectory = System.Environment.CurrentDirectory;
            
            // find a manifest to load
            string defaultManifest = System.IO.Path.Combine(thisAppDirectory, DEFAULT_MANIFEST);
            string[] args = System.Environment.GetCommandLineArgs();
            
            for (int i = 1; i < args.Length; i++) //ignore the first command line arg it is the app
            {
                // look for command line switches
                if (args[i][0] == '-')
                {

                }
                else // assume argument refers to a manifest
                {
                    defaultManifest = args[i];
                }
            }

            devMode = true;
            this.DevModeLabel.Visible = devMode;
            
            //if the manifest exists try to load it
            if (System.IO.File.Exists(defaultManifest))
            {
                LoadManifest(defaultManifest);
            }
            else //if it isn't there ask the user to specify one.
            {
                SelectManifest();
            }

            SetSelection(currentPressed);
            SetCurrentFlightInfo();

            //Get the application path for population of scenarios
            GetAppPath();
        }

        // Request events mission completed, sim stop and, application quit events from simConnect so we can take focus 
        private void initFocusGrab()
        {
            try
            {
                closeFocusGrab();
                simconnect = new SimConnect("Launcher SimConnect Interface", this.Handle, WM_USER_SIMCONNECT, null, 0);
                simconnect.OnRecvQuit += new SimConnect.RecvQuitEventHandler(simconnect_OnRecvQuit);    
                simconnect.OnRecvEvent += new SimConnect.RecvEventEventHandler(simconnect_OnRecvEvent);
                simconnect.SubscribeToSystemEvent(Event.MISSION_END, MISSION_COMPLETED);
                simconnect.SetSystemEventState(Event.MISSION_END, SIMCONNECT_STATE.ON);
            }
            catch //if the app has not started yet this will throw
            {
                closeFocusGrab();
            }
        }

        //release our simconnect object
        private void closeFocusGrab()
        {
            if (simconnect != null)
            {
                try //The application may have been shut down and this will throw 
                {
                    simconnect.UnsubscribeFromSystemEvent(Event.MISSION_END);                    
                    simconnect.Dispose();                
                }
                catch  //do nothing here we are closing simconnect after this.
                {}
                simconnect = null;
            }
        }

        //returns the path for Prepar3D from the registry
        public string GetAppPath()
        {
            if (applicationPath != null)
            {
                return applicationPath;
            }

            try
            {
                // first we'll look in the registry for an installed version of the runtime
                string pathStart = Microsoft.Win32.Registry.GetValue(APPREGPATH, APPREGKEY,"").ToString();
                pathStart = pathStart.Replace("\0", "");
                applicationPath = System.IO.Path.Combine(pathStart, APPNAME);
            }
            catch
            {
                // allow the user to specify the location 
                MessageBox.Show("Application not found. Please select the location of " + APPNAME + ".", "File Not Found");

                OpenFileDialog findApp = new OpenFileDialog();
                findApp.Filter = APPLICATION_EXTENSION;
                DialogResult fileSelected = findApp.ShowDialog();
                if (fileSelected == DialogResult.Yes || fileSelected == DialogResult.OK)
                {
                    if (findApp.CheckFileExists)
                        applicationPath = findApp.FileName;
                    else
                    {
                        //The launcher is designed to run with the runtime.
                        MessageBox.Show("File not found. Please check to make sure the application is installed correctly. This program will now terminate.");
                    }
                }
                else
                {
                    //If they cancel, terminate
                    MessageBox.Show(APPNAME + " was not found. Please check to make sure the application is installed correctly. This program will now terminate.");
                    this.Close();    
                }

            }

            return applicationPath;

        }

 
        // callback function for simconnect
        void simconnect_OnRecvEvent(SimConnect sender, SIMCONNECT_RECV_EVENT recEvent)
        {
            // verify that this is one of the events we are looking for
            if (recEvent.uEventID == (uint) Event.MISSION_END)
            {
                System.Threading.Thread.Sleep(2000); // let the user see what happened
                this.Activate();  //take focus                             
            }                 
        }

        // callback from simconnect (we get this if Prepar3D is shut down)
       void simconnect_OnRecvQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            System.Threading.Thread.Sleep(2000);
            this.Activate();  //take focus
           

            // release the simconnect object as we need to create a new one when the app is launched again
            closeFocusGrab();
        }

        // override DefWndProc so we can grab the SimConnect messages
        protected override void DefWndProc(ref Message m)
        {
            if (m.Msg == WM_USER_SIMCONNECT)
            {
                if (simconnect != null)
                {
                    try // the Sim Connection may no longer be valid
                    {
                        simconnect.ReceiveMessage();
                    }
                    catch // do nothing here we will not be receiving any more simconnect message if the Prepar3D runtime has closed down
                    {}
                }
            }
            else
            {
                base.DefWndProc(ref m);
            }
        }
       
        //Launches Prepar3D with the parameters provided
        private void LaunchApp(string parameters)
        {
            string sAppPath = GetAppPath();
            if (sAppPath != null)
            {
                System.Diagnostics.Process.Start(sAppPath, parameters); // Could store Prepar3D Process here            
                System.Threading.Thread.Sleep(10000);  // sleep so that we don't get clicked multiple time while we are waiting for load                                                   
                initFocusGrab();
            }
        }
        
        private void ScenarioList_SelectedIndexChanged(object sender, EventArgs e)
        {
            //when a new flight is selected update the flight info
            SetCurrentFlightInfo();
        }              

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //open the manifest selector
            SelectManifest();
        }

        //loads a new manifest
        private void SelectManifest()
        {
            OpenFileDialog findManifest = new OpenFileDialog();
            findManifest.Filter = MANIFEST_EXTENSION;
            DialogResult fileSelected = findManifest.ShowDialog();
            if (fileSelected == DialogResult.Yes || fileSelected == DialogResult.OK)
            {
                LoadManifest(findManifest.FileName);
            }
            else if (theManifest == null)   // A valid manifest must be loaded for the application to continue.
            {
                DialogResult result = MessageBox.Show("A manifest must be loaded to continue. Try Again?", "Error", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    SelectManifest();
                }
                else
                {
                    MessageBox.Show("This application will now exit.", "Exit");
                    this.Close();
                }
            }
        }

        private void LoadManifest(string manifestPath)
        {
            try
            {
                theManifest = new SolutionManifest(manifestPath, GetAppPath(), thisAppDirectory);
            }
            catch (Exception ex)
            {
                theManifest = null;
                MessageBox.Show(string.Format(ERROR_LOADING_MANIFEST, manifestPath, ex.Message));
            }


            // set the current scenario top element in the list
            SetCurrentScenarios();
        }

        // Switch button image based on what is active
        private void SetSelection(Selection button)
        {
            currentPressed = button;

            FlySolo.Visible = (currentPressed == Selection.SOLO) ? false : true;
            FlySoloOn.Visible = (currentPressed == Selection.SOLO) ? true : false;

            HostMP.Visible = (currentPressed == Selection.HOST_MP_MISSION) ? false : true;
            HostMPOn.Visible = (currentPressed == Selection.HOST_MP_MISSION) ? true : false;
                                
            Join.Visible = (currentPressed == Selection.JOIN) ? false : true;
            JoinOn.Visible = (currentPressed == Selection.JOIN) ? true : false;
        }

        // set the member variable current scenarios to the list of scenarios that matches the current button selected
        private void SetCurrentScenarios()
        {
            if (theManifest == null)
                return;

            // default to the scenario selection label
            groupBox1.Text = SELECT_SCENARIO;

            switch (currentPressed)
            {
                case Selection.SOLO:
                    currentScenarios = theManifest.GetSoloScenarios();
                    break;

                case Selection.HOST_MP_MISSION:
                    currentScenarios = theManifest.GetMultiplayerScenarios();
                    break;

                case Selection.JOIN:
                    currentScenarios = theManifest.GetMultiplayerJoinScenarios();
                    groupBox1.Text = SCENERY_OVERRIDE;      // update the menu label to reflect change in behavior
                    break;

                default:
                    currentScenarios = null;
                    break;
            }
            
            ScenarioList.Items.Clear();            

            if (currentScenarios != null)
            {
                ScenarioList.Enabled = true;
                foreach (ScenarioInfo currentScenario in currentScenarios)
                {
                    ScenarioList.Items.Add(currentScenario.Name);                      
                }              
            }
            else
            {
                ScenarioList.Enabled = false;
            }
            
            SetCurrentFlightInfo();

        }  

        // sets the flight info based on the current select button\mission
        private void SetCurrentFlightInfo()
        {
            try
            {
                if (ScenarioList.SelectedIndex == -1 && ScenarioList.Items.Count > 0)
                {
                    ScenarioList.SelectedIndex = 0;
                }

                ScenarioInfo currentScenario = (ScenarioInfo)currentScenarios[ScenarioList.SelectedIndex];

                if (System.IO.File.Exists(currentScenario.ImagePath))
                {
                    //set the picture
                    ScenarioPicture.ImageLocation = currentScenario.ImagePath;
                }
                else
                {
                    ScenarioPicture.ImageLocation = null;
                }

                //set the Description
                ScenarioDescription.Text = currentScenario.Description;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // terminate the runtime process if running
            // If you have another application called Prepar3D this will terminate it.
            // If this is a problem consider using the process object created by calling Process.Start
            System.Diagnostics.Process[] procs = System.Diagnostics.Process.GetProcessesByName(System.IO.Path.GetFileNameWithoutExtension(APPNAME));
            foreach (System.Diagnostics.Process curProc in procs)
            {
                curProc.Kill();
            }
            this.Close();
        }  
     
        // handle the button clicks
        private void FlySolo_Click(object sender, EventArgs e)
        {
            SetSelection(Selection.SOLO);
            SetCurrentScenarios();
        }

        private void HostMP_Click(object sender, EventArgs e)
        {
            SetSelection(Selection.HOST_MP_MISSION);
            SetCurrentScenarios();
        }

        private void Join_Click(object sender, EventArgs e)
        {
            SetSelection(Selection.JOIN);
            SetCurrentScenarios();
        }

        private void Next_Click(object sender, EventArgs e)
        {
            ScenarioInfo currentScenario;
            
            // swap active image
            Next.Visible = false;
            NextOn.Visible = true;
            
            switch (currentPressed)
            {
                case Selection.JOIN:
                    if (ScenarioList.SelectedIndex != 0)
                    {
                        //get the current scenario so we can use the scenery
                        currentScenario = (ScenarioInfo)currentScenarios[ScenarioList.SelectedIndex];
                        LaunchApp(MPJOIN + currentScenario.SceneryPortion);
                    }
                    else
                    {
                        LaunchApp(MPJOIN);
                    }
                    break;

                case Selection.SOLO:
                    currentScenario = (ScenarioInfo)currentScenarios[ScenarioList.SelectedIndex];
                    LaunchApp(currentScenario.LaunchParameters);
                    break;

                case Selection.HOST_MP_MISSION:
                    if (ScenarioList.SelectedIndex != 0)
                    {
                        currentScenario = (ScenarioInfo)currentScenarios[ScenarioList.SelectedIndex];
                        LaunchApp(currentScenario.LaunchParameters);
                    }
                    else
                    {
                        LaunchApp(MP);
                    }
                    break;
            }

            // reset launch button image to non-active
            Next.Visible = true;
            NextOn.Visible = false;

        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // show information from the manifest
            if (theManifest == null)
            {
                MessageBox.Show("There is no solution loaded.");
            }
            else
            {
                string solutionInfo = theManifest.GetSolutionInfo();
                if (solutionInfo == "")
                {
                    MessageBox.Show("No solution information provided.");
                }
                else
                {
                    MessageBox.Show(solutionInfo);
                }
            }

        }      

        private void solutionInfoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // show information about the launcher
            MessageBox.Show(ABOUT_LAUNCHER);
        }            
    }
}
