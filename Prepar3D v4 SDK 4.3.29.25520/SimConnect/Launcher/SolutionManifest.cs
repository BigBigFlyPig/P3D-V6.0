//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 

using System;
using System.Collections.Generic;
using System.Text;

namespace Launcher
{
    class SolutionManifest
    {
        // XML tags for reading manifest file
        const string SCENARIO_TAG = "Scenario";
        const string NAME_TAG = "Name";
        const string DESCRIPTION_TAG = "Description";
        const string VERSIONMAJOR_TAG = "VersionMajor";
        const string VERSIONMINOR_TAG = "VersionMinor";
        const string PUBLISHER_TAG = "Publisher";
        const string SUPPORTPHONE_TAG = "SupportPhone";
        const string SUPPORTEMAIL_TAG = "SupportEmail";
        const string HELPURL_TAG = "HelpURL";
        const string SCENARIO_NAME_TAG = "Name";
        const string SCENARIO_DESCRIPTION_TAG = "Description";
        const string SCENARIO_SCENERY_TAG = "Scenery";
        const string SCENARIO_MULTIPLAYER_TAG = "IsMultiplayer";
        const string SCENARIO_FILE_TAG = "ScenarioFile";
        const string SCENARIO_IMAGE_TAG = "Image";
        const string RUNTIME_PATH_TOKEN = "%PREPAR3DPATH%";

        const string DEFAULT_FLIGHT_NAME = "Default Flight";
        const string DEFAULT_FLIGHT_DESCRIPTION = "This will launch the application using the default flight scenario.";
        const string DEFAULT_FLIGHT_FILE = "\\Lockheed Martin\\Prepar3D v4\\Prepar3D_Default.fxml";

        const string FREE_FLIGHT_NAME = "Free Flight";
        const string FREE_FLIGHT_DESCRIPTION = "This will launch the application and enable selection of a free flight multiplayer scenario.";
        
        const string DEFAULT_SCENERY_NAME = "Default Scenery";
        const string DEFAULT_SCENERY_DESCRIPTION = "This will use the default runtime scenery when joining an existing Multiplayer scenario.";
        
        const string DEFAULT_IMAGE = "\\default.jpg";
        
        const string P3D_W_SLASH = "\\Prepar3D.exe";
        
        // member variables
        System.Xml.XmlTextReader theManifest;
        System.Collections.ArrayList SoloScenarios = null;
        System.Collections.ArrayList MultiplayerScenarios = null;
        System.Collections.ArrayList MultiplayerJoinScenarios = null;
        string solutionName;
        string solutionDescription;
        string majorVersion;
        string minorVersion;
        string publisher;
        string supportPhone;
        string supportEmail;
        string helpURL;

     
        // constructor
        public SolutionManifest(string path, string P3DAppPath, string thisAppPath)
        {
            theManifest = new System.Xml.XmlTextReader(path);
            SoloScenarios = new System.Collections.ArrayList();
            MultiplayerScenarios = new System.Collections.ArrayList();
            MultiplayerJoinScenarios = new System.Collections.ArrayList();

            // Load the default image
            string defaultImage = thisAppPath + DEFAULT_IMAGE;

            // Add a Default Flight entry which allows for entry into the application without specifying a scenario
            string appDataLocalPath = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            ScenarioInfo DefaultFlight = new ScenarioInfo(appDataLocalPath + DEFAULT_FLIGHT_FILE, DEFAULT_FLIGHT_NAME, defaultImage, false, DEFAULT_FLIGHT_DESCRIPTION, null);
            SoloScenarios.Add(DefaultFlight);
                    
            // Add a Free Flight entry which allows for entry into the application with "-mp" and free flight selection
            ScenarioInfo FreeFlight = new ScenarioInfo(null, FREE_FLIGHT_NAME, defaultImage, true, FREE_FLIGHT_DESCRIPTION, null);
            MultiplayerScenarios.Add(FreeFlight);

            ScenarioInfo DefaultScenery = new ScenarioInfo(null, DEFAULT_SCENERY_NAME, defaultImage, true, DEFAULT_SCENERY_DESCRIPTION, null);
            MultiplayerJoinScenarios.Add(DefaultScenery);

            // We read until we find the tag are looking for
            while (theManifest.Read() && theManifest.Name != SCENARIO_TAG)
            {
                if (theManifest.NodeType == System.Xml.XmlNodeType.Element)
                {
                    if (theManifest.Name == NAME_TAG)
                    {
                        theManifest.Read(); // advance to the value                        
                        solutionName = theManifest.Value;
                    }

                    else if (theManifest.Name == DESCRIPTION_TAG)
                    {
                        theManifest.Read();
                        solutionDescription = theManifest.Value;
                    }

                    else if (theManifest.Name == VERSIONMAJOR_TAG)
                    {
                        theManifest.Read();
                        majorVersion = theManifest.Value;
                    }

                    else if (theManifest.Name == VERSIONMINOR_TAG)
                    {
                        theManifest.Read();
                        minorVersion = theManifest.Value;
                    }

                    else if (theManifest.Name == PUBLISHER_TAG)
                    {
                        theManifest.Read();
                        publisher = theManifest.Value;
                    }

                    else if (theManifest.Name == SUPPORTPHONE_TAG)
                    {
                        theManifest.Read();
                        supportPhone = theManifest.Value;
                    }

                    else if (theManifest.Name == SUPPORTEMAIL_TAG)
                    {
                        theManifest.Read();
                        supportEmail = theManifest.Value;
                    }

                    else if (theManifest.Name == HELPURL_TAG)
                    {
                        theManifest.Read();
                        helpURL = theManifest.Value;
                    }                 
                }
            }

            // parse all of the scenarios
            ScenarioInfo tempScenario = ParseScenarioFromManifest(theManifest, P3DAppPath);
            while (tempScenario != null)
            {
                if (tempScenario.Multiplayer && tempScenario.FLTPath != null)
                {
                    MultiplayerScenarios.Add(tempScenario);

                    //Add to list of scenery override options
                    if (tempScenario.SceneryPortion.Length > 0)
                    {
                        MultiplayerJoinScenarios.Add(tempScenario);
                    }
                }
                else if (tempScenario.FLTPath != null)
                {
                    SoloScenarios.Add(tempScenario);
                }

                tempScenario = ParseScenarioFromManifest(theManifest, P3DAppPath);
            }

            theManifest.Close();                              
        }

        private static ScenarioInfo ParseScenarioFromManifest(System.Xml.XmlTextReader manifestFile, string P3DAppPath)
        {
            string scenarioName = null;
            string sceneryPath = null;
            string scenarioFile = null;
            string imagePath = null;
            bool multiplayer = false;
            string description = null;

            P3DAppPath = P3DAppPath.ToLower().Replace(P3D_W_SLASH.ToLower(), null); // Strip the exe name to store the root path of the runtime

            // read the next item and verify that it is not an end scenario tag
            while (manifestFile.Read() && !(manifestFile.NodeType == System.Xml.XmlNodeType.EndElement && manifestFile.Name == SCENARIO_TAG))
            {
                // advance until we get a starting element                
                if (manifestFile.NodeType == System.Xml.XmlNodeType.Element)
                {
                    // see if the element is one we are interested in (all others are ignored)
                    if (manifestFile.Name == SCENARIO_NAME_TAG)
                    {
                        manifestFile.Read(); //advance to the value
                        scenarioName = manifestFile.Value;
                    }

                    else if (manifestFile.Name == SCENARIO_MULTIPLAYER_TAG)
                    {
                        manifestFile.Read();
                        multiplayer = bool.Parse(manifestFile.Value);
                    }

                    else if (manifestFile.Name == SCENARIO_SCENERY_TAG)
                    {
                        manifestFile.Read();
                        sceneryPath = manifestFile.Value;
                    }

                    else if (manifestFile.Name == SCENARIO_DESCRIPTION_TAG)
                    {
                        manifestFile.Read();
                        description = manifestFile.Value;
                    }

                    else if (manifestFile.Name == SCENARIO_IMAGE_TAG)
                    {
                        manifestFile.Read();
                        imagePath = manifestFile.Value;
                        imagePath = imagePath.Replace(RUNTIME_PATH_TOKEN, P3DAppPath); // If a token is passed in, replace it with the runtime path
                    }

                    else if (manifestFile.Name == SCENARIO_FILE_TAG)
                    {
                        manifestFile.Read();
                        scenarioFile = manifestFile.Value;
                        scenarioFile = scenarioFile.Replace(RUNTIME_PATH_TOKEN, P3DAppPath);     // If a token is passed in, replace it with the runtime path
                    }
                }
            }

            // verify that we at least have a scenario name
            if (scenarioName != null)
            {
                return new ScenarioInfo(scenarioFile, scenarioName, imagePath, multiplayer, description, sceneryPath);
            }
            else
            {
                return null;
            }


        }

        public System.Collections.ArrayList GetSoloScenarios()
        {                    
            return SoloScenarios;
        }

        public System.Collections.ArrayList GetMultiplayerScenarios()
        {            
            return MultiplayerScenarios;
        }

        public System.Collections.ArrayList GetMultiplayerJoinScenarios()
        {
            return MultiplayerJoinScenarios;
        }


        // generate the string for solution info
        public string GetSolutionInfo()
        {
            StringBuilder info = new StringBuilder();

            if (solutionName != null && solutionName != "")
                info.AppendLine(solutionName);
            if (solutionDescription != null && solutionDescription != "")
                info.AppendLine("Description: " + solutionDescription);
            if (majorVersion != null && majorVersion != "")
                info.AppendLine("Major Version: " + majorVersion);
            if (minorVersion != null && minorVersion != "")
                info.AppendLine("Minor version: " + minorVersion);
            if (publisher != null && publisher != "")
                info.AppendLine("Publisher: " + publisher);
            if (supportPhone != null && supportPhone != "")
                info.AppendLine("Support phone: " + supportPhone);
            if (supportEmail != null && supportEmail != "")
                info.AppendLine("Support E-mail: " + supportEmail);
            if (helpURL != null && helpURL != "")
                info.AppendLine("Help URL: " + helpURL);
            return info.ToString();
        }
    }
}
