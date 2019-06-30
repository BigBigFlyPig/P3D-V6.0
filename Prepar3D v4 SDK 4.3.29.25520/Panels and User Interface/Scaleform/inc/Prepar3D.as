package
{
	import Prepar3DInterface;
	import flash.events.EventDispatcher;
	import flash.events.Event;
	
	public final class Prepar3D
	{
		/**
		* Events Prepar3D will invoke events as they occur in the system. This variable can be used to listen for
		* these incoming events and react to them.
		*/
		public static var Events:EventDispatcher = new EventDispatcher();

		public function Prepar3D() 
		{
		}
		
		/**
		* Init This function initializes the Prepar3D Interface. Because this function will be called automatically
		* after the first frame, call this function only if the Prepar3D Interface is needed during the first frame. 
		* None of the other Prepar3D interface functions will work until this funciton is called.
		*/
		public static function Init( )
		{
			Prepar3DInterface.Init()
		}

			
		/** 
		* GetVersion Prepar3D will use this function to determine which version of the scaleform Prepar3D Interface is being used to internally process the calls correctly.
		* 
		* @return A version identifier string in the following format: "Major.Minor.Build". This will be used internally by Prepar3D to determine how to react to Prepar3D Interface calls.
		*/ 
		static public function GetVersion() : String 
		{
			return Prepar3DInterface.Version;
		}
		
		
		//
		// Variables
		//

		
		/** 
		* VarHas Determine whether a variable of the specified units exists.
		* 
		* @param variableName See the <b>Variables Overview</b> article of the <b>Learning Center</b> to learn about systems that support variable access and the specific string format to use.
		* @param unitType See the <b>Units of Measurement</b> article in the <b>Learning Center</b> for acceptable unit types.
		*
		* @return A boolean value determing whether the variable was found or not. 
		*/ 
		static public function VarHas(variableName:String, unitType:String) : Boolean
		{
			return Prepar3DInterface.VarHas(variableName, unitType);
		}
		
		/** 
		* VarGet Get the value of a variable in the specified units.
		* 
		* @param variableName See the <b>Variables Overview</b> article of the <b>Learning Center</b> to learn about systems that support variable access and the specific string format to use.
		* @param unitType See the <b>Units of Measurement</b> article in the <b>Learning Center</b> for acceptable unit types.
		* 
		* @return The value, in the specified units, of the variable. The type of the variable is determined by the units used. If the variable was not found, then <b>null</b> is returned.
		*/ 
		static public function VarGet(variableName:String, unitType:String) 
		{
			return Prepar3DInterface.VarGet(variableName, unitType);
		}
		
		/** 
		* VarSet Set the value of a variable in the specified units.
		* 
		* @param variableName See the <b>Variables Overview</b> article of the <b>Learning Center</b> to learn about systems that support variable access and the specific string format to use.
		* @param unitType See the <b>Units of Measurement</b> article in the <b>Learning Center</b> for acceptable unit types.
		* @param value The value, in the specified units, to set the variable to. The type of the variable that should be used is determined by the units used.
		* 
		* @return A boolean value determining whether the variable was successfully set or not.
		*/ 
		static public function VarSet(variableName:String, unitType:String, value:*) : Boolean 
		{
			return Prepar3DInterface.VarSet(variableName, unitType, value);
		}

		/** 
		* P3DLog Internally log a message
		* 
		* @param message Send a message for internal logging.
		* 
		*/
		static public function P3DLog(message:String)
		{
			Prepar3DInterface.P3DLog(message);
		}


		//
		// Menu 
		//
		
		
		/** 
		* GetMenuBarID This function returns the root level menu bar ID.
		* 
		* @return The ID of the root level menu bar that should be used to query for its children.
		*/
		static public function GetMenuBarID() : uint
		{
			return Prepar3DInterface.GetMenuBarID();
		}
		
		/** 
		* IsMenuBarVisible This function returns whether the root level menu bar should be shown.
		* 
		* @return A boolean value determining whether the root level menu bar should be shown.
		*/
		static public function IsMenuBarVisible() : Boolean
		{
			return Prepar3DInterface.IsMenuBarVisible();
		}
		
		
		/** 
		* SetMenuBarFocus This function determines whether keyboard and event should be given or taken away from the menu bar.
		* 
		* @param focus A boolean value where true is defined as focus and false is defined as unfocus.
		*/
		static public function SetMenuBarFocus (focus:Boolean)
		{
			return Prepar3DInterface.SetMenuBarFocus(focus);
		}

		/** 
		* GetContextMenuID This function returns the root level context menu ID.
		* 
		* @return The ID of the root level context menu that should be used to query for its children.
		*/
		static public function GetContextMenuID() : uint
		{
			return Prepar3DInterface.GetContextMenuID();
		}
		
		/**
		* GetContextMenuX This function returns the X-coordinate of the upper left position that the context menu has been opened at. Prepar3D will automatically readjust the context menu position if it is too close to the window corners.
		* 
		* @return The X-coordinate of the context menu's upper left position.
		*/
		static public function GetContextMenuX() : int
		{
			return Prepar3DInterface.GetContextMenuX();
		}
		
		/** 
		* GetContextMenuY This function returns the Y-coordinate of the upper left position that the context menu has been opened at. Prepar3D will automatically readjust the context menu position if it is too close to the window corners.
		* 
		* @return The Y-coordinate of the context menu's upper left position.
		*/
		static public function GetContextMenuY() : int
		{
			return Prepar3DInterface.GetContextMenuY();
		}
		
		/** 
		* ActivateMenuItem  This function will cause Prepar3D to execute whatever action is associated with the menu item.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		*/ 
		static public function ActivateMenuItem(id:uint) : void 
		{
			Prepar3DInterface.ActivateMenuItem(id);
		}
		
		/** 
		* GetMenuItemChildCount This function will return the number of children contained within a menu item given a menu ID.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		* @return The number of children that the menu item contains.
		*/ 
		static public function GetMenuItemChildCount(id:uint) : uint 
		{
			return Prepar3DInterface.GetMenuItemChildCount(id);
		}
		
		/** 
		* GetMenuItemChildID This function will return a menu item ID of a child given the parent menu item ID and the child index. Use the <b>GetMenuItemChildCount</b> function to determine the number of child menu items.
		* 
		* @param id The ID of the parent menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* @param index the 0-based index of the child.
		* 
		* @return The menu ID of the child at the specified index. 
		*/ 
		static public function GetMenuItemChildID(id:uint, index:uint) : uint 
		{
			return Prepar3DInterface.GetMenuItemChildID(id,index);
		}
		
		/** 
		* GetMenuItemText This function returns the text that a menu item should display given a menu ID.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		* @return The displayable text for the menu item.
		*/
		static public function GetMenuItemText(id:uint) : String 
		{
			return Prepar3DInterface.GetMenuItemText(id);
		}

		/** 
		* GetMenuItemShortcutText This function returns the keyboard shortcut that a menu item should display given a menu ID.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		* @return The displayable keyboard shortcut the user should press to activate the menu item.
		*/
		static public function GetMenuItemShortcutText(id:uint) : String 
		{
			return Prepar3DInterface.GetMenuItemShortcutText(id);
		}
		
		/** 
		* GetMenuItemEnabled This function returns whether or not the menu item should be able to be interacted with given a menu ID.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		* @return A boolean determing whether or not the menu item should be able to be interacted with. 
		*/
		static public function GetMenuItemEnabled (id:uint) : Boolean
		{
			return Prepar3DInterface.GetMenuItemEnabled(id);
		}
		
		/** 
		* GetMenuItemCheckBoxState This function gets whether the menu item has a check box and its state given a menu ID.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		* @return Returns 0 if the item is not a menu item with a check box, 1 if the item is a menu item with a check box but is not checked, or 2 if the item is a menu item with a check box and is checked.
		*/
		static public function GetMenuItemCheckBoxState  (id:uint) : uint
		{
			return Prepar3DInterface.GetMenuItemCheckBoxState(id);
		}
		
		/** 
		* GetMenuItemAccKey This function gets the ASCII character keycode that the user can press to activate the menu item given by the menu ID.
		* 
		* @param id The ID of the menu item. Menu IDs typically around obtained by querying 'GetMenuItemChildID'. If desired, a list of Menu IDs are avaiable in the <b>Menu Configuration</b> article of the <b>Learning Center</b>.
		* 
		* @return  Returns the ASCII character that the user needs to press to activate the menu item.
		*/
		static public function GetMenuItemAccKey  (id:uint) : uint
		{
			return Prepar3DInterface.GetMenuItemAccKey(id);
		}	
		
		
		//
		// Scenario
		//
		
		
		/** 
		* GetMenuPromptOptionCount This function returns an unsigned integer specifying how many options 
		* the MenuPromptTrigger has available.
		* 
		* @return An unsigned integer specifying how many options are available. 
		*/
		static public function GetMenuPromptOptionCount() : uint
		{
			return Prepar3DInterface.GetMenuPromptOptionCount();
		}
		
		/** 
		* GetMenuPromptOption This function retrieves the text of the MenuItem associated with the option 
		* index in question.
		*
		* @param option The index of the option that the user wishes to retrieve.
		* 
		* @return The text, as a string, for the index option requested.
		*/
		static public function GetMenuPromptOption(option:uint) : String
		{
			return Prepar3DInterface.GetMenuPromptOption(option);
		}			

		/** 
		* SetMenuPromptOption This function executes the associated action for the chosen MenuItem.
		*
		* @param option The index of the option that the user wishes to select.
		*/
		static public function SetMenuPromptOption (option:uint)
		{
			return Prepar3DInterface.SetMenuPromptOption(option);
		}	
		
		/** 
		* GetMenuPromptPrompt This function retrieves the prompt text associated with the Menu Prompt.
		* 
		* @return The prompt of the Menu Prompt as a string.
		*/
		static public function GetMenuPromptPrompt() : String
		{
			return Prepar3DInterface.GetMenuPromptPrompt();
		}

		/** 
		* GetATCMessageCount This function returns the amount of ATC messages that are available.
		* 
		* @return An unsigned integer that denotes how many ATC messages are available to be queried for. 
		*/
		static public function GetATCMessageCount() : uint
		{
			return Prepar3DInterface.GetATCMessageCount();
		}

		/** 
		* GetATCMessage This function determines the text of a given ATC message, given a message index. 
		* 
		* @param index An index from 0 to the amount of messages available, queryable via GetATCMessageCount().
		* 
		* @return A string containing the message for the given message index.
		*/
		static public function GetATCMessage(index:uint) : String
		{
			return Prepar3DInterface.GetATCMessage(index);
		}				
		
		/** 
		* GetATCMessageColor This function determines the hexcode color of the text of a given ATC message, given a 
		* message index. 
		*
		* @param index An index from 0 to the amount of messages available, queryable via GetATCMessageCount().
		* 
		* @return A hexcode color that the message for the given message index should be.
		*/
		static public function GetATCMessageColor(index:uint) : uint
		{
			return Prepar3DInterface.GetATCMessageColor(index);
		}
		
		/** 
		* GetATCOptionCount This function returns an unsigned integer specifying how many options 
		* the ATC has available.
		* 
		* @return An unsigned integer specifying how many options are available. 
		*/
		static public function GetATCOptionCount() : uint
		{
			return Prepar3DInterface.GetATCOptionCount();
		}
		
		/** 
		* GetATCOption This function retrieves the text of the ATC option associated with the option 
		* index in question.
		*
		* @param option The index of the option that the user wishes to retrieve.
		* 
		* @return The text, as a string, for the index option requested.
		*/
		static public function GetATCOption(option:uint) : String
		{
			return Prepar3DInterface.GetATCOption(option);
		}			

		/** 
		* SetATCOption This function chooses an available ATC option.
		* 
		* @param index An index from 0 to 9 denoting the ATC option should be chosen.
		*/
		static public function SetATCOption  (option:uint) 
		{
			return Prepar3DInterface.SetATCOption(option);
		}
		
		/** 
		* ClearATCMessages This function can be used to determine whether the ATC messages should be cleared.
		* 
		* @return A boolean determing whether the ATC messages should be cleared.
		*/
		static public function ClearATCMessages () : Boolean
		{
			return Prepar3DInterface.ClearATCMessages();
		}
		
		/** 
		* GetATCPrompt This function retrieves the prompt text associated with the ATC.
		* 
		* @return The prompt of the ATC as a string.
		*/
		static public function GetATCPrompt() : String
		{
			return Prepar3DInterface.GetATCPrompt();
		}

		/** 
		* IsATCDisabled This function returns whether or not input to ATC is available.
		* 
		* @return The input state of the ATC window.
		*/
		static public function IsATCDisabled() : Boolean
		{
			return Prepar3DInterface.IsATCDisabled();
		}
		
		/** 
		* GetATCDisableReason Returns why the ATC is disabled i.e. paused or radio off.
		* 
		* @return The reason ATC is disabled as a string.
		*/
		static public function GetATCDisableReason() : String
		{
			return Prepar3DInterface.GetATCDisableReason();
		}

		/** 
		* ShouldShowATCMessageLog Returns the value of the setting for whether the ATC message log should be shown.
		* 
		* @return The boolean setting for whether the ATC message log should be shown.
		*/
		static public function ShouldShowATCMessageLog() : Boolean
		{ 
			return Prepar3DInterface.ShouldShowATCMessageLog();
		}
				
		
		
		/** 
		* GetWindowTitle This function returns the designated title of the window. For predefined user interface
		* components, Prepar3D will dictate the name of the window title (e.g. ATC window will be "ATC MENU").
		*
		* @return The title of the window as a string. 
		*/
		static public function GetWindowTitle() : String
		{
			return Prepar3DInterface.GetWindowTitle();
		}
		
		/** 
		* GetInstanceName This function returns the instance name associated with the Scaleform content. 
		* Scenario developers can add an instance name to Scaleform content (e.g. ScaleformPanel or ScaleformObject).
		* This can not only be used to create different movie instances of the same movie file 
		* but can be used to create more reusable movies by keying off of the instance name to manipulate different data
		* such as Scenario Variables.
		* @return A string containing the instance name that Prepar3D has registered for the scaleform content.
		*/
		static public function GetInstanceName() : String
		{
			return Prepar3DInterface.GetInstanceName();
		}
				
		//
		// InfoGen
		//
		
		/** 
		* GetInfoGenText This function returns the text that a specific row of the infogen system should have.
		* 
		* @param position A positional index that refers to a specific row at a specific corner of the screen
		* where the indices are as follows:
		* 0-5:   Top Right
		* 6-11:  Top Left
		* 12-17: Bottom Right
		* 18-23: Bottom Left
		* @return A string containing the text that should be displayed on the specified row of the infogen system.
		*/
		static public function GetInfoGenText(position:uint) : String
		{
			return Prepar3DInterface.GetInfoGenText(position);
		}
		
		/** 
		* HasInfoGenChanged This function, given text as a string, can determine if the text for the specified row
		* of the infogen system has changed values.
		* 
		* @param position A positional index that refers to a specific row at a specific corner of the screen
		* where the indices are as follows:
		* 0-5:   Top Right
		* 6-11:  Top Left
		* 12-17: Bottom Right
		* 18-23: Bottom Left
		* @param text The string that the system will use to compare and determine if the infogen system text 
		* has changed.
		* 
		* @return A boolean determining if the text for the specified row of the infogen system is different than the
		* expected value.
		*/
		static public function HasInfoGenChanged(position:uint, text:String) : Boolean
		{
			return Prepar3DInterface.HasInfoGenChanged(position, text);
		}
		
		/** 
		* GetInfoGenColor This function gets the color hexcode that the foreground of a specific row 
		* of the infogen system should have.
		*
		* @param position A positional index that refers to a specific row at a specific corner of the screen
		* where the indices are as follows:
		* 0-5:   Top Right
		* 6-11:  Top Left
		* 12-17: Bottom Right
		* 18-23: Bottom Left
		* 
		* @return The color hexcode, without alpha, as an integer that denotes the RGB color. 
		*/
		static public function GetInfoGenColor(position:uint) : uint
		{
			return Prepar3DInterface.GetInfoGenColor(position);
		}

		/** 
		* GetInfoGenBackgroundColor This function gets the color hexcode that the background of a specific row 
		* of the infogen system should have.
		* 
		* @param position A positional index that refers to a specific row at a specific corner of the screen
		* where the indices are as follows:
		* 0-5:   Top Right
		* 6-11:  Top Left
		* 12-17: Bottom Right
		* 18-23: Bottom Left
		* 
		* @return The color hexcode, without alpha, as an integer that denotes the RGB color. 
		*/
		static public function GetInfoGenBackgroundColor(position:uint) : uint
		{
			return Prepar3DInterface.GetInfoGenBackgroundColor(position);
		}
		
		/** 
		* GetInfoGenBackgroundAlpha This function gets the transparency value that the specific row of the infogen 
		* system should have.
		* 
		* @param position A positional index that refers to a specific row at a specific corner of the screen
		* where the indices are as follows:
		* 0-5:   Top Right
		* 6-11:  Top Left
		* 12-17: Bottom Right
		* 18-23: Bottom Left
		* 
		* @return A number from 0.0 to 1.0 denoting how transparent the row of the infogen system should be where 
		* 0.0 is fully transparent and 1.0 is fully opaque.
		*/
		static public function GetInfoGenBackgroundAlpha(position:uint) : Number
		{
			return Prepar3DInterface.GetInfoGenBackgroundAlpha(position);
		}

		//
		// Panels
		//
		
		/** 
		* GetNumPanels This function returns the total number of panels in the system.  This number includes vehicle and scenario panels.
		* This function along with the GetPanelIdByIndex function are required to get the necessary panel info for any modifications.
		* 
		* @return Number of panels.
		*/
		static public function GetNumPanels() : uint
		{ 
			return Prepar3DInterface.GetNumPanels(); 
		}
		
		/** 
		* GetPanelIdByIndex This function returns ID of the panel by looking up the panel by index.  This ID is needed to manipulate the panel.
		* This function along with the GetNumPanels function are required to get the necessary panel info for any modifications.
		* 
		* @param Index of panel in panel list
		* 
		* @return Panel ID.
		*/
		static public function GetPanelIdByIndex(index:uint)
		{
			return Prepar3DInterface.GetPanelIdByIndex(index);
		}
		
		/** 
		* GetPanelName This function returns name of the panel.  The ID is used to look up the panel.
		* 
		* @param ID of panel
		* 
		* @return Panel name.
		*/
		static public function GetPanelName(id:uint) : String
		{
			return Prepar3DInterface.GetPanelName(id);
		}
		
		/** 
		* GetPanelNameByIndex This function returns name of the panel.  The index is used to look up the panel.
		* 
		* @param index of panel
		* 
		* @return Panel name.
		*/
		static public function GetPanelNameByIndex(index:uint) : String
		{
			return Prepar3DInterface.GetPanelNameByIndex(index);
		}
		
		/** 
		* TogglePanel Toggles the open state of the panel.
		* 
		* @param ID of panel
		*/
		static public function TogglePanel(id:uint)
		{
			Prepar3DInterface.TogglePanel(id);
		}
		
		/** 
		* TogglePanelByIndex Toggles the open state of the panel using the index.
		* 
		* @param index of panel
		*/
		static public function TogglePanelByIndex(index:uint)
		{
			Prepar3DInterface.TogglePanelByIndex(index);
		}
		
		/** 
		* OpenPanel Opens the panel.
		* 
		* @param ID of panel
		*/
		static public function OpenPanel(id:uint)
		{
			Prepar3DInterface.OpenPanel(id);
		}
		
		/** 
		* ClosePanel Closes the panel.
		* 
		* @param ID of panel
		*/
		static public function ClosePanel(id:uint)
		{
			Prepar3DInterface.ClosePanel(id);
		}
		
		/** 
		* IsPanelOpen Returns the open state of the panel.
		* 
		* @param ID of panel
		*
		* @return The open state of the panel
		*/
		static public function IsPanelOpen(id:uint) : Boolean
		{
			return Prepar3DInterface.IsPanelOpen(id);
		}
		
		/** 
		* IsPanelOpen Returns the open state of the panel by index.
		* 
		* @param index of panel
		*
		* @return The open state of the panel
		*/
		static public function IsPanelOpenByIndex(index:uint) : Boolean
		{
			return Prepar3DInterface.IsPanelOpenByIndex(index);
		}
		
		/** 
		* DuplicatePanel Creates a duplicate panel.  Returns the ID of the newly created duplicate.
		* 
		* @param ID of the panel to duplicate
		*
		* @return The ID of the newly created duplicate
		*/
		static public function DuplicatePanel(id:uint) :uint
		{ 
			return Prepar3DInterface.DuplicatePanel(id);
		}
		
		/** 
		* DuplicatePanel Creates a duplicate panel by index.  Returns the ID of the newly created duplicate.
		* 
		* @param index of the panel to duplicate
		*
		* @return The ID of the newly created duplicate
		*/
		static public function DuplicatePanelByIndex(index:uint) :uint
		{ 
			return Prepar3DInterface.DuplicatePanelByIndex(index);
		}
		
		/** 
		* RemoveDuplicatePanel Removes a duplicated panel.
		* 
		* @param ID of the panel to remove
		*
		*/
		static public function RemoveDuplicatePanel(id:uint) :uint
		{ 
			return Prepar3DInterface.RemoveDuplicatePanel(id);
		}
		
		//
		// General
		//

		/** 
		* GetScaleformDpiScaling This function gets the scale of the DPI where 1.0 is defined to be the default DPI which is 96. For example, if the DPI was 120 then the DPI scaling would be 1.25.  
		* 
		* @return A number representing the scale of the current DPI compared to the default DPI which is 96.
		*/
		static public function GetScaleformDpiScaling() : Number
		{
			return Prepar3DInterface.GetScaleformDpiScaling();
		}
		
		/** 
		* GetWindowWidth This function uses the focused window and returns its width. This function should be called
		* at a point when it is known that the desired window is focused.
		* @return The focused window's width in pixels.
		*/
		static public function GetWindowWidth(mainView:Boolean) : int
		{
			return Prepar3DInterface.GetWindowWidth(mainView);
		}
		
		/** 
		* GetWindowWidth This function uses the focused window and returns its height. This function should be called
		* at a point when it is known that the desired window is focused.
		* @return The focused window's height in pixels. 
		*/
		static public function GetWindowHeight(mainView:Boolean) : int
		{
			return Prepar3DInterface.GetWindowHeight(mainView);
		}
		
		/** 
		* GetScaleformWindowType This function returns the type of window the movie should display.  This is used for .swf files
		* that share common functionality for multiple window types i.e. the menu system
		* Values: Default 0, Main Menu 1, Context Menu 2, ATC 3
		* @return The window type
		*/
		static public function GetScaleformWindowType(): uint
		{
			return Prepar3DInterface.GetScaleformWindowType();
		}
		
		/** 
		* IsSimPaused This function returns whether or not the sim is paused.
		* @return The paused state of the sim
		*/
		static public function IsSimPaused(): Boolean
		{
			return Prepar3DInterface.IsSimPaused();
		}
		/** 
		* SoundSetProperties Updates the properties of a sound by ID
      * @param soundID The id of the sound to update
      * @param play true = playing, false = stopped
      * @param loop Set true to make sounds loop
		*/
	    static public function SoundSetProperties(soundID:int, play:Boolean, loop:Boolean ) 
		{ 
			return Prepar3DInterface.SoundSetProperties(soundID,play,loop);
		}

		/** 
		* SoundCreate Create a sound 
      * @param fileName Filename of the sound to create
		* @return Sound ID used for updating and destroying sounds 
		*/
		static public function SoundCreate(fileName:String) : int
		{ 
			return Prepar3DInterface.SoundCreate(fileName); 
		}

		/** 
		* SoundOneShot Create and play a sound by filename.  The sound will destroy itself  
      * when it finishes playing.
		* @param fileName Filename of the sound to play
		*/
		static public function SoundOneShot(fileName:String)
		{ 
			return Prepar3DInterface.SoundOneShot(fileName); 		
		} 
		/** 
		* SoundOneShot Play a sound by filename.  The sound will destroy itself when it 
      * finishes playing.
		* @param soundID ID of the sound to destroy
		*/
		static public function SoundDestroy(soundID:int) 
		{ 
			return Prepar3DInterface.SoundDestroy(soundID); 		
		}
	}
}