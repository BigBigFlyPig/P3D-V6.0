package  {
	
	/**
	*	Developed by: Mark Greenstone, July 2017
	*   @author Mark Greenstone
	*   Modified by: Brian Caldwell, Aug 2017
	*   Company: Lockheed Martin, RMS
	*/
	
	import flash.display.MovieClip;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	import flash.events.Event;
	
	/**
	* Class: ScreenManager
	* Description: This class is in charge of updating the CDU display.
	*              It creates instances of all the CDU components and
	*              adds several of them to form a default screen layout.
	*              When it receives a call to update the display, it determines
	*              how to change the display based on the CDU button that was
	*              selected.
	*
	*              Note that HOW the display changes depends on the functionality
	* 			   of the specific CDU this program is being used with. The
	*              functionality demonstrated below is only an example.
	*/
	public class ScreenManager extends MovieClip {
		
		private var _APPR_Plan_1:APPR_Plan_1;
		private var _FPL_Menu_1:FPL_Menu_1;
		private var _FUEL_1:FUEL_1;
		private var _MSG_1:MSG_1;
		
		private var apprPlan_1_shown:Boolean = true;
		private var apprPlan_fld_active:Boolean = false;
		private var fplMenu_1_shown:Boolean = true;
		private var fuel_1_shown:Boolean = true;
		private var msg_1_shown:Boolean = true;

		
		private var _currentDisplay:String;
		private var _screenOn:Boolean = true;
		private var _buttonSelected:String;
		private var _brightness_level:int;
		private var _whichPage:int;
		
		private var _this = this;
		private var textEntry:String = "";
		private var charEntry:String = "";

		/**
		* constructor
		*/
		public function ScreenManager() {
			// constructor code
			
			// now create instances of the screens
			_APPR_Plan_1 = new APPR_Plan_1();
			_FPL_Menu_1 = new FPL_Menu_1();
			_FUEL_1 = new FUEL_1();
			_MSG_1 = new MSG_1();
			
			// The _currentDisplay will hold the which screen layout is currently being displayed
			// initially the CDU will be in 'PrimaryConfig'
			_currentDisplay = "PrimaryConfig";
			
			// other screen parameters
			_screenOn = true;
			_this.visible = true;
			_this.alpha = 1.0;
			_brightness_level = 5;
			_whichPage = 1;
			
			// load all of the default display elements
			setPrimaryScreenConfig();
		}
		
		/**
		* Description: This method is used to clear the children within the screen container.
		*
		*/
		private function clearScreens():void
		{
			while (_this.numChildren > 0)
			{
				_this.removeChildAt(0);
			}
			apprPlan_1_shown = false;
			fplMenu_1_shown = false;
			fuel_1_shown = false;
			msg_1_shown = false;
			apprPlan_fld_active = false;

		}
		
		/**
		* Description: This method is used for updating the screen contents.
		*
		* @param buttonName:String
		* @param buttonValue:Boolean
		*
		*/
		public function updateScreen(buttonName:String, buttonValue:Boolean):void
		{	
			_buttonSelected = buttonName;
			
			switch(_buttonSelected)
			{
				case "btn_fpl":
					toggle_fplview();
					break;
				case "btn_fuel":
					toggle_fuelview();
					break;
				case "btn_msg":
					toggle_msgview();
					break;
				case "btn_data":
					toggle_dataview();
					break;
				case "btn_lsk_r5":
					if (apprPlan_1_shown) {
						toggle_fplview();
					}
					break;
				case "btn_lsk_l2":
					if (apprPlan_1_shown) {
						apprPlan_fld_active = true;
						activate_fld(_this._APPR_Plan_1["tf_lsk_l2"]);
					}
					break;
				case "btn_enter":
					completeText(_this._APPR_Plan_1["tf_lsk_l2"]);
					break;
				case "btn_back":
					charEntry = "*";
					applyText();
					break;
				case "btn_a":
					charEntry = "A";
					applyText();
					break;
				case "btn_b":
					charEntry = "B";
					applyText();
					break;
				case "btn_c":
					charEntry = "C";
					applyText();
					break;
				case "btn_d":
					charEntry = "D";
					applyText();
					break;
				case "btn_e":
					charEntry = "E";
					applyText();
					break;
				case "btn_f":
					charEntry = "F";
					applyText();
					break;
				case "btn_g":
					charEntry = "G";
					applyText();
					break;
				case "btn_h":
					charEntry = "H";
					applyText();
					break;
				case "btn_i":
					charEntry = "I";
					applyText();
					break;
				case "btn_j":
					charEntry = "J";
					applyText();
					break;
				case "btn_k":
					charEntry = "K";
					applyText();
					break;
				case "btn_l":
					charEntry = "L";
					applyText();
					break;
				case "btn_m":
					charEntry = "M";
					applyText();
					break;
				case "btn_n":
					charEntry = "N";
					applyText();
					break;
				case "btn_o":
					charEntry = "O";
					applyText();
					break;
				case "btn_p":
					charEntry = "P";
					applyText();
					break;
				case "btn_q":
					charEntry = "Q";
					applyText();
					break;
				case "btn_r":
					charEntry = "R";
					applyText();
					break;
				case "btn_s":
					charEntry = "S";
					applyText();
					break;
				case "btn_t":
					charEntry = "T";
					applyText();
					break;
				case "btn_u":
					charEntry = "U";
					applyText();
					break;
				case "btn_v":
					charEntry = "V";
					applyText();
					break;
				case "btn_w":
					charEntry = "W";
					applyText();
					break;
				case "btn_x":
					charEntry = "X";
					applyText();
					break;
				case "btn_y":
					charEntry = "Y";
					applyText();
					break;
				case "btn_z":
					charEntry = "Z";
					applyText();
					break;
				case "btn_1":
					charEntry = "1";
					applyText();
					break;
				case "btn_2":
					charEntry = "2";
					applyText();
					break;
				case "btn_3":
					charEntry = "3";
					applyText();
					break;
				case "btn_4":
					charEntry = "4";
					applyText();
					break;
				case "btn_5":
					charEntry = "5";
					applyText();
					break;
				case "btn_6":
					charEntry = "6";
					applyText();
					break;
				case "btn_7":
					charEntry = "7";
					applyText();
					break;
				case "btn_8":
					charEntry = "8";
					applyText();
					break;
				case "btn_9":
					charEntry = "9";
					applyText();
					break;
				case "btn_0":
					charEntry = "0";
					applyText();
					break;
			}
		}
		
		/**
		* Description: This event handler is called on every Enter Frame (24 times a second).
		*              The purpose of this event is to continually update each of the screens'
		*              data that contain Prepar3D aircraft inputs.
		*
		* @param e:Event
		*/
		public function handleUpdateData(e:Event)
		{			
			if (apprPlan_1_shown)
			{
				_APPR_Plan_1.update();
			}
			if (fplMenu_1_shown)
			{
				_FPL_Menu_1.update();
			}
			if (fuel_1_shown)
			{
				_FUEL_1.update();
			}
			if (msg_1_shown)
			{
				_MSG_1.update();
			}

		}
		
		// This function is only used for DEBUG and displaying text to the screen
		public function setDebugTextDisplay(str:String):void
		{
			(parent as MovieClip).debugText.text = str;
		}
		
		
		//-------------------------------------------------------------------------------------
		//	METHODS FOR CHANGING THE DISPLAY BASED ON CDU BUTTON SELECTIONS
		//-------------------------------------------------------------------------------------
		
		/**
		* Description: This method turns the CDU screen on or off (visible or invisible)
		*           
		*/
		private function toggle_cdu_on():void
		{
			if (_screenOn == true)
			{
				_this.visible = false;
				_screenOn = false;
			}
			else
			{
				_this.visible = true;
				_screenOn = true;
			}
		}
		
		/**
		* Description: This method changes the brightness of the CDU display
		*           
		*/
		private function cdu_brightness(dir:String):void
		{
			var new_alpha:Number;
			new_alpha = _this.alpha;
			
			if (_screenOn)
			{
				if (dir == "up")
				{
					if (_brightness_level < 5)
					{
						switch(_brightness_level)
						{
							case 4:
								_brightness_level = 5;
								new_alpha = 1.0;
								break;
							case 3:
								_brightness_level = 4;
								new_alpha = 0.80;
								break;
							case 2:
								_brightness_level = 3;
								new_alpha = 0.60;
								break;
							case 1:
								_brightness_level = 2;
								new_alpha = 0.40;
								break;
						}
					}
				}
				else if (dir == "down")
				{
					if (_brightness_level > 1)
					{
						switch(_brightness_level)
						{
							case 5:
								_brightness_level = 4;
								new_alpha = 0.80;
								break;
							case 4:
								_brightness_level = 3;
								new_alpha = 0.60;
								break;
							case 3:
								_brightness_level = 2;
								new_alpha = 0.40;
								break;
							case 2:
								_brightness_level = 1;
								new_alpha = 0.20;
								break;
						}
					}
				}
				
				_this.alpha = new_alpha;
				//setDebugTextDisplay("current alpha: " + new_alpha);
					
			}
		}
		
		/**
		* Description: This method toggles the FPL Menu view
		*           
		*/
		private function toggle_fplview():void
		{
			clearScreens();
			fplMenu_1_shown = true;
			_this.addChild(_FPL_Menu_1);
		}
		
		/**
		* Description: This method toggles FUEL Menu view
		*           
		*/
		private function toggle_fuelview():void
		{
			clearScreens();
			fuel_1_shown = true;
			_this.addChild(_FUEL_1);

		}
		
		/**
		* Description: This method toggles Message view
		*           
		*/
		private function toggle_msgview():void
		{
			clearScreens();
			msg_1_shown = true;
			_this.addChild(_MSG_1);

		}

		/**
		* Description: This method toggles Data view
		*           
		*/
		private function toggle_dataview():void
		{
			clearScreens();
			apprPlan_1_shown = true;
			_this.addChild(_APPR_Plan_1);

		}

		/**
		* Description: This method sets up text entry
		*           
		*/
		private function activate_fld(lskTextField):void
		{
			lskTextField.text = textEntry;
			lskTextField.backgroundColor = 0xFFFFFF;
			lskTextField.background = true;
			lskTextField.textColor  = 0x000000;
			stage.focus = lskTextField;
			
		}

		/**
		* Description: This method applies char to textfield
		*           
		*/
		private function applyText():void
		{
			if (apprPlan_fld_active) {
				if (charEntry != "*") {
					if (textEntry.length < 12) {
						textEntry += charEntry
					}
				} else {
					textEntry = textEntry.substr(0, textEntry.length - 1)
				}
				_APPR_Plan_1.tf_lsk_l2.text = textEntry;			
			}
		}
		
		/**
		* Description: This method completes textfield
		*           
		*/
		private function completeText(lskTextField):void
		{
			apprPlan_fld_active = false;
			lskTextField.textColor  = 0x00CC00;
			lskTextField.background = false;
			stage.focus = null;
		}

		//-------------------------------------------------------------------------------------
		
		/**
		* Description: This method updates screen based on ATT button selection.
		*           
		*/
		private function setPrimaryScreenConfig():void
		{
			clearScreens();
			_currentDisplay = "PrimaryConfig";
			setScreenConfigPositions();
			_this.addChild(_APPR_Plan_1);
			apprPlan_1_shown = true;
		}
		
		
		private function setScreenConfigPositions():void
		{
			switch (_currentDisplay)
			{
				case "PrimaryConfig":
				    _APPR_Plan_1.x = 0;
					_APPR_Plan_1.y = 0;
				    _FPL_Menu_1.x = 0;
					_FPL_Menu_1.y = 0;
				    _FUEL_1.x = 0;
					_FUEL_1.y = 0;
				    _MSG_1.x = 0;
					_MSG_1.y = 0;
					break;
			}
		}
		

	}  // end of class
	
}  // end of package
