package
{	
	import flash.external.*;

	public final class Prepar3DInterface
	{		
		public function Prepar3DInterface() 
		{
		}
		
		public static function Init( )
		{
			ExternalInterface.call("InitP3D");
		}
		static public var Initialized:Boolean = false;

		// 
		// The Prepar3D Interface Version
		// 
		static public var Version:String = "1.0.0";
		
		
		//
		// Variables
		//
		
		static public var VarHas:Function						= function (variableName:String, unitType:String) { return true; };
		static public var VarGet:Function						= function (variableName:String, unitType:String) { return 0; };
		static public var VarSet:Function						= function (variableName:String, unitType:String, value:*) { return true; };		
		static public var P3DLog:Function 						= function (str:String) { trace(str); }

		//
		// Scenario Functions
		// 
		
		static public var GetInstanceName:Function     		= function () { return ""; };
		
		static public var GetMenuPromptOptionCount:Function = function () { return 4; };
		static public var GetMenuPromptOption:Function      = function (option:uint) { return "Option " + option; };
		static public var SetMenuPromptOption:Function      = function (option:uint) { };
		static public var GetMenuPromptPrompt:Function    	= function () { return "Menu Prompt"; };
		
		static public var GetATCMessageCount:Function 	   = function () { return 0; };
		static public var GetATCMessage:Function   		   = function (index:uint) { return "Message...."; };
		static public var GetATCMessageColor:Function	   = function (index:uint) { return 0; };
		static public var GetATCOptionCount:Function 	   = function () { return 4; };
		static public var GetATCOption:Function      	   = function (option:uint) { return "Option " + option; };
		static public var SetATCOption:Function     	   = function (option:uint) { };
		static public var ClearATCMessages:Function		   = function () { return false; };
		static public var GetATCPrompt:Function    		   = function () { return "ATC Prompt"; };
		static public var IsATCDisabled:Function		   = function () { return false; };
		static public var GetATCDisableReason:Function     = function () { return "" };
		static public var ShouldShowATCMessageLog:Function = function () { return true; };
		
		static public var GetWindowTitle:Function      	= function () { return "Window Title"; };
		
		//
		// Menu Functions
		//
		
		static public var GetMenuBarID:Function          		= function () { return 0; };
		static public var IsMenuBarVisible:Function			 	= function () { return false; };
		static public var SetMenuBarFocus:Function           	= function (focus:Boolean) { };
		static public var GetContextMenuID:Function          	= function () { return 2; };
		static public var GetContextMenuX:Function		     	= function () { return 0; };
		static public var GetContextMenuY:Function		 	 	= function () { return 0; };
		static public var ActivateMenuItem:Function          	= function () { trace("Activate"); };
		static public var GetMenuItemChildCount:Function     	= function () { return 5; };
		static public var GetMenuItemChildID:Function        	= function () { return 0; };
		static public var GetMenuItemText:Function           	= function (id:uint) { return "Menu Item " + id; };
		static public var GetMenuItemShortcutText:Function   	= function () { return "CTRL+j"; };
		static public var GetMenuItemEnabled:Function        	= function () { return 1; };
		static public var GetMenuItemCheckBoxState:Function  	= function () { return 2; };
		static public var GetMenuItemAccKey:Function         	= function () { return 0; };
		
		// 
		// InfoGen Functions
		//
		
		static public var GetInfoGenText:Function            	= function (position:uint) { return ""; };
		static public var HasInfoGenChanged:Function         	= function (position:uint, text:String) { return false; };
		static public var GetInfoGenColor:Function           	= function (position:uint) { return 0; };
		static public var GetInfoGenBackgroundColor:Function 	= function (position:uint) { return 0; };
		static public var GetInfoGenBackgroundAlpha:Function 	= function (position:uint) { return 0; };
		
		//
		// Panel Functions
		//
		
		static public var GetNumPanels:Function = function() { return 0; };
		static public var GetPanelIdByIndex:Function = function(index:uint) { return 0; };
		static public var GetPanelName:Function = function(id:uint) { return ""; };
		static public var GetPanelNameByIndex:Function = function(index:uint) { return ""; };
		static public var TogglePanel:Function = function(id:uint) { };
		static public var TogglePanelByIndex:Function = function(index:uint) { };
		static public var OpenPanel:Function = function(id:uint) { };
		static public var ClosePanel:Function = function(id:uint) { };
		static public var IsPanelOpen:Function = function(id:uint) { return false; };
		static public var IsPanelOpenByIndex:Function = function(index:uint) { return false; };
		static public var DuplicatePanel:Function = function(id:uint) { return 0; };
		static public var DuplicatePanelByIndex:Function = function(index:uint) { return 0; };
		static public var RemoveDuplicatePanel:Function = function(id:uint) { };
		
		//
		// General
		//
		
		static public var GetScaleformDpiScaling:Function    	= function () { return 1; };
		static public var GetWindowWidth:Function			 	= function (mainView:Boolean) { return 0; }; // Set mainView to true if width of main view is desired, otherwise width of focused view is returned
		static public var GetWindowHeight:Function			 	= function (mainView:Boolean) { return 0; }; // Set mainView to true if height of main view is desired, otherwise height of focused view is returned
		static public var GetScaleformWindowType:Function		= function () { return 0; }; // Default 0, Main Menu 1, Context Menu 2, ATC 3
		static public var IsSimPaused:Function					= function () { return 0; };
		static public var SoundSetProperties:Function			= function (soundID:int, play:Boolean, loop:Boolean ) { return 0; }; // play sounds or change properties of sounds that are playing
		static public var SoundCreate:Function = function(fileName:String) { return -1; }
		static public var SoundOneShot:Function = function(fileName:String) { } 
		static public var SoundDestroy:Function = function(soundID:int) { }
	}
}