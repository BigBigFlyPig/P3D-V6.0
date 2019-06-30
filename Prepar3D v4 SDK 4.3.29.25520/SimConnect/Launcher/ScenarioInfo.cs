//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 

using System;
using System.Collections.Generic;
using System.Text;

namespace Launcher
{
    class ScenarioInfo
    {
        enum ScenarioType
        {
            Unknown,
            Flt,
            Fxml
        }
        
        // command line flags
        private const string SCENERY_FLAG = "-scenery:";
        private const string MP_FLAG = "-mp:";
        private const string FLT_FLAG = "-flt:";
        private const string FXML_FLAG = "-fxml:";

        // member variables
        private string scenarioName; // The name of the scenario
        private string scenarioFilePath; // The path to the Flight File
        private string scenarioDescription; // The text description of the scenario
        private string scenarioImage;  // The image to be displayed with the scenario
        private bool isMultiplayer;
        private string sceneryTerrainOverideFolder; // folder specified to override the scenery and/or terrain files
        private ScenarioType scenarioType;

        // constructor     
        public ScenarioInfo (string scenarioPath, string name, string imagePath, bool multiplayer, string description, string sceneryOveride)
        {
            scenarioFilePath = scenarioPath;
            scenarioName = name;
            scenarioImage = imagePath;
            isMultiplayer = multiplayer;
            sceneryTerrainOverideFolder = sceneryOveride;
            scenarioDescription = description;

            scenarioType = ScenarioType.Unknown;

            if (!String.IsNullOrWhiteSpace(scenarioFilePath))
            {
                string extension = System.IO.Path.GetExtension(scenarioFilePath);
                if(String.Compare(".flt", extension, true) == 0)
                {
                    scenarioType = ScenarioType.Flt;
                }
                else if (String.Compare(".fxml", extension, true) == 0)
                {
                    scenarioType = ScenarioType.Fxml;
                }
            }
        }

        // property Scenario name
        public string Name
        {
            get
            {
                return scenarioName;
            }
        }

        // property flight file path
        public string FLTPath
        {
            get
            {
                return scenarioFilePath;
            }

        }

        // property Image path
        public string ImagePath
        {
            get
            {
                return scenarioImage;
            }
        }

        // property Description
        public string Description
        {
            get
            {
                return scenarioDescription;
            }
        }

        // property multiplayer scenario
        public bool Multiplayer
        {
            get
            {
                return isMultiplayer;
            }
        }

        // property SceneryPortion - (includes tag) for commandline - used in LaunchParameters as well as MP Join
        public string SceneryPortion
        {
            get
            {
                if (sceneryTerrainOverideFolder != null && sceneryTerrainOverideFolder != "")
                {
                    return ApplyQuotes(SCENERY_FLAG + sceneryTerrainOverideFolder) + " ";
                }
                else
                {
                    return "";
                }
            }
        }
        
        // property LaunchParameters - the command line parameters to launch the specified mission
        public string LaunchParameters
        {
            get
            {
                string parameters = "";               
                parameters += SceneryPortion;
                
                if (isMultiplayer)
                {
                    parameters += MP_FLAG + ApplyQuotes(scenarioFilePath);
                }                
                else if (!String.IsNullOrEmpty(scenarioFilePath))
                {
                    if (scenarioType == ScenarioType.Flt)
                    {
                        parameters += FLT_FLAG + ApplyQuotes(scenarioFilePath);
                    }
                    else if (scenarioType == ScenarioType.Fxml)
                    {
                        parameters += FXML_FLAG + ApplyQuotes(scenarioFilePath);
                    }
                    else
                    {
                        parameters += ApplyQuotes(scenarioFilePath);
                    }
                }

                return parameters;
            }
        }

        // adds quotes to paths so that long file names can be passed on the command line
        private string ApplyQuotes(string input)
        {           
            return "\"" + input + "\"";
        }
    }
}
