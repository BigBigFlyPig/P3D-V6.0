package  {
	
	/**
	*	Developed by: Mark Greenstone, July 2017
	*   @author Mark Greenstone
	*   Company: Lockheed Martin, RMS
	*/
	
	import flash.display.MovieClip;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	import flash.events.Event;
	
	/**
	* Class: ScreenManager
	* Description: This class is in charge of updating the MFD display.
	*              It creates instances of all the MFD components and
	*              adds several of them to form a default screen layout.
	*              When it receives a call to update the display, it determines
	*              how to change the display based on the MFD button that was
	*              selected.
	*
	*              Note that HOW the display changes depends on the functionality
	* 			   of the specific MFD this program is being used with. The
	*              functionality demonstrated below is only an example.
	*/
	public class ScreenManager extends MovieClip {
		
		// references to each MFD screen/display component
		private var _mainScreenText:mainScreenText;	
		
		private var _AIRSPEED_screen:AirspeedScreen;
		private var _TGT_screen:TurbineScreen;
		private var _FUEL_screen:FuelScreen;
		private var _VSI_screen:VerticalSpeedIndicator;
		private var _HSI_screen:HSIScreen;
		private var _ALT_B_screen:BaroAltimeterScreen;
		private var _ALT_R_screen:RadarAltimeterScreen;
		private var _stabIndicator:StabilizerIndicator;
		private var _side_VSI_screen:SideVerticalSpeedScreen;
		private var _VOR_FMS_screen:VorFmsScreen;
		private var _GS_Wind_screen:GroundSpeedWindScreen;
		private var _StoresManagement_screen:StoresManagementScreen;
		private var _WeaponStatus_screen:WeaponStatusScreen;
		private var _TSD_screen:TacticalSituationDisplay;
		private var _Threat_screen:Threat_screen;
		private var _WCA_screen:WarningsScreen;
		private var _MissionStatus_screen:MissionStatusScreen;
		private var _EICAS_screen:EICASScreen;
		private var _Sensor_screen:SensorScreen;
		private var _Comms_screen:CommsScreen;
		
		// flags to identify if a screen/display component is currently visible
		// we will only update their content when they are visible (to reduce processing time)
		private var _nm_shown:Boolean = true;
		private var _nd_shown:Boolean = false;
		private var _pfd_shown:Boolean = false;
		private var _eicas_shown:Boolean = false;
		//
		private var _airspeed_shown:Boolean = true;
		private var _tgt_shown:Boolean = true;
		private var _fuel_shown:Boolean = true;
		private var _vsi_shown:Boolean = true;
		private var _hsi_shown:Boolean = true;
		private var _alt_b_shown:Boolean = true;
		private var _alt_r_shown:Boolean = true;
		private var _stabIndicator_shown:Boolean = true;
		private var _side_VSI_shown:Boolean = true;
		private var _groundspeed_shown:Boolean = true;
		private var _vor_fms_shown:Boolean = true;
		private var _tac_shown:Boolean = false;
		private var _dsms_shown:Boolean = false;
		private var _wps_shown:Boolean = false;
		private var _threat_shown:Boolean = false;
		private var _wca_shown:Boolean = false;
		private var _ms_shown:Boolean = false;
		private var _comms_shown:Boolean = false;
		private var _sensor_shown:Boolean = false;
		
		private var _currentDisplay:String;
		private var _screenOn:Boolean = true;
		private var _buttonSelected:String;
		private var _brightness_level:int;
		private var _symbolBrightness_level:int;
		private var _current_SymbolBrightness:Number;
		
		private var _initWeaponStatus:Boolean = false;
		
		private var _this = this;

		/**
		* constructor
		*/
		public function ScreenManager() {
			// constructor code
			
			// now create instances of the screens/display components
			_mainScreenText = new mainScreenText();
			
			_AIRSPEED_screen = new AirspeedScreen();
			_TGT_screen = new TurbineScreen();
			_FUEL_screen = new FuelScreen();
			_VSI_screen = new VerticalSpeedIndicator();
			_HSI_screen = new HSIScreen();
			_ALT_B_screen = new BaroAltimeterScreen();
			_ALT_R_screen = new RadarAltimeterScreen();
			_stabIndicator = new StabilizerIndicator();
			_side_VSI_screen = new SideVerticalSpeedScreen();
			_VOR_FMS_screen = new VorFmsScreen();
			_GS_Wind_screen = new GroundSpeedWindScreen();
			_StoresManagement_screen = new StoresManagementScreen();
			_WeaponStatus_screen = new WeaponStatusScreen();
			_TSD_screen = new TacticalSituationDisplay();
			_Threat_screen = new Threat_screen();
			_WCA_screen = new WarningsScreen();
			_MissionStatus_screen = new MissionStatusScreen();
			_EICAS_screen = new EICASScreen();
			_Comms_screen = new CommsScreen();
			_Sensor_screen = new SensorScreen();
			
			// The _currentDisplay contains which screen layout is currently being displayed
			// initially the MFD will be in 'PrimaryConfig' display mode
			_currentDisplay = "PrimaryConfig";
			
			// other screen parameters
			_screenOn = true;
			_this.visible = true;
			_this.alpha = 1.0;
			_brightness_level = 5;
			_symbolBrightness_level = 5;
			_current_SymbolBrightness = 1.0;
			
			// load all of the default display elements
			setPrimaryScreenConfig();
		}
		
		/**
		* Description: This method is used to clear all the children within the screen container.
		*
		*/
		private function clearScreens():void
		{
			while (_this.numChildren > 0)
			{
				_this.removeChildAt(0);
			}
		}
		
		/**
		* Description: This method is used for updating the screen contents after a 
		*              MFD button is selected.
		*              > Here you will insert your own functions to handle whatever the REAL
		*              > functionality of the MFD does when these buttons are selected.
		*              > You can add conditional statements here as well to have a button 
		*              > do something different based on the current view shown.
		*               
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
				case "knob1":
					toggle_mfd_on();
					break;
				case "brt_up_btn":
					mfd_brightness("up");
					break;
				case "brt_down_btn":
					mfd_brightness("down");
					break;
				case "lum_down_btn":
					mfd_symbology_brightness("down");
					break;
				case "lum_up_btn":
					mfd_symbology_brightness("up");
					break;
				case "left_square_btn1":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_stabIndicator_view();
					}
					break;
				case "left_square_btn2":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_airspeed_view();
					}
					break;
				case "left_square_btn3":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_groundspeed_view();
					}
					break;
				case "left_square_btn4":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_tgt_view();
					}
					break;
				case "left_square_btn5":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_fuel_view();
					}
					break;
				case "left_square_btn6":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_hsi_view();
					}
					break;
				case "top_square_btn1":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_TWD_view();
					}
					break;					
				case "top_square_btn2":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_Comms_view();
					}
					break;					
				case "top_square_btn3":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_Sensor_view();
					}
					break;
				case "top_square_btn4":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_vsi_view();
					}
					break;
				case "right_square_btn1":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_side_vsi_view();
					}
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.zoomIn();
					}										
					break;
				case "right_square_btn2":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_alt_b_view();
					}
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.zoomOut();
					}										
					break;
				case "right_square_btn3":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_VOR_FMS_view();
					}
					else if (_currentDisplay == "TacticalDisplay")
					{
						_TSD_screen.rangeUp();
					}
					else if (_currentDisplay == "ThreatDisplay")
					{
						_Threat_screen.rangeUp();
					}
					else if (_currentDisplay == "CommsDisplay")
					{
						_Comms_screen.arrowUp();
					}					
					break;
				case "right_square_btn4":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_NM_view();
					}
					else if (_currentDisplay == "TacticalDisplay")
					{
						_TSD_screen.rangeDown();
					}
					else if (_currentDisplay == "ThreatDisplay")
					{
						_Threat_screen.rangeDown();
					}
					else if (_currentDisplay == "CommsDisplay")
					{
						_Comms_screen.arrowDown();
					}					
					break;
				case "right_square_btn5":
					if (_currentDisplay == "PrimaryConfig")
					{
						toggle_alt_r_view();
					}
					else if (_currentDisplay == "CommsDisplay")
					{
						_Comms_screen.swapValues();
					}					
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.tiltUp();
					}
					break;	
				case "right_square_btn6":
					if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.tiltDown();
					}
					else if (_currentDisplay == "CommsDisplay")
					{
						_Comms_screen.swapValues();
					}					
					break;
				case "bottom_square_btn1":
					if ((_currentDisplay == "StoresManagement") ||
					(_currentDisplay == "WeaponsStatus")  ||
					(_currentDisplay == "TacticalDisplay") ||
					(_currentDisplay == "WCA_Status") ||
					(_currentDisplay == "EICAS") ||							 
					(_currentDisplay == "MissionStatus") ||
					(_currentDisplay == "ThreatDisplay") ||
					(_currentDisplay == "SensorDisplay") ||
					(_currentDisplay == "CommsDisplay"))    
					{
						// MAIN button
						setPrimaryScreenConfig();
					}
					break;
				case "bottom_square_btn2":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_ND_view();
					}
					else if (_currentDisplay == "TacticalDisplay")
					{
						_TSD_screen.toggle_Map();
					}
					break;
				case "bottom_square_btn3":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_EICAS_view();
					}
					else if (_currentDisplay == "TacticalDisplay")
					{
						_TSD_screen.toggle_Declutter();
					}	
					else if (_currentDisplay == "ThreatDisplay")
					{
						_Threat_screen.cycle_Program();
					}					
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.cycleMode();
					}					
					
					break;
				case "bottom_square_btn4":
					if ((_currentDisplay == "MissionStatus") ||
					    (_currentDisplay == "EICAS") ||
					    (_currentDisplay == "TacticalDisplay"))
					{
						select_WCA_view();
					}
					else if (_currentDisplay == "StoresManagement")
					{
						select_WPS_view();
					}
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.setLock();
					}						
					break;
				case "bottom_square_btn5":
					if (_currentDisplay == "PrimaryConfig")
					{
						select_TAC_view();
					}
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.panLeft();
					}
						
					break;
				case "bottom_square_btn6":
					if ((_currentDisplay == "PrimaryConfig") ||
						(_currentDisplay == "WeaponsStatus")  ||
						(_currentDisplay == "EICAS") ||
						(_currentDisplay == "TacticalDisplay") ||
						(_currentDisplay == "ThreatDisplay") ||
						(_currentDisplay == "CommsDisplay"))  
					{
						select_DSMS_view();
					}
					else if (_currentDisplay == "WCA_Status")
					{
						select_MS_view();
					}
					else if (_currentDisplay == "SensorDisplay")
					{
						_Sensor_screen.panRight();
					}
					break;
			}
		}
		
		/**
		* Description: This event handler is called on every Enter Frame (24 times a second).
		*              The purpose of this event is to continually update each of the screens'
		*              data that contain Prepar3D aircraft inputs. Here I am only updating
		*              each screen's data only when it is visible (to reduce processing time).
		*
		* @param e:Event
		*/
		public function handleUpdateData(e:Event)
		{			
			if (_airspeed_shown)
			{
				_AIRSPEED_screen.update();
			}
			if (_tgt_shown)
			{
				_TGT_screen.update();
			}
			if (_fuel_shown)
			{
				_FUEL_screen.update();
			}
			if (_vsi_shown)
			{
				_VSI_screen.update();
			}
			if (_hsi_shown)
			{
				_HSI_screen.update();
			}
			if (_alt_b_shown)
			{
				_ALT_B_screen.update();
			}
			if (_alt_r_shown)
			{
				_ALT_R_screen.update();
			}
			if (_stabIndicator_shown)
			{
				_stabIndicator.update();
			}
			if (_side_VSI_shown)
			{
				_side_VSI_screen.update();
			}	
			if (_vor_fms_shown)
			{
				_VOR_FMS_screen.update();
			}
			if (_groundspeed_shown)
			{
				_GS_Wind_screen.update();
			}
			if (_dsms_shown)
			{
				_StoresManagement_screen.update();
			}
			if (_wps_shown)
			{
				_WeaponStatus_screen.update();
			}
			if (_tac_shown)
			{
				_TSD_screen.update();
			}
			if (_threat_shown)
			{
				_Threat_screen.update();
			}
			if (_wca_shown)
			{
				_WCA_screen.update();
			}
			if (_ms_shown)
			{
				_MissionStatus_screen.update();
			}
			if (_eicas_shown)
			{
				_EICAS_screen.update();
			}
			if (_comms_shown)
			{
				_Comms_screen.update();
			}
			if (_sensor_shown)
			{
				_Sensor_screen.update();
			}			
		}
		
		//-------------------------------------------------------------------------------------
		// DEBUG: This function is only used for DEBUG and displaying text to the screen
		public function setDebugTextDisplay(str:String):void
		{
			(parent as MovieClip).debugText.text = str;
		}
		//------------------------------------------------------------------------------------
		
		
		//-------------------------------------------------------------------------------------
		//	METHODS FOR CHANGING THE DISPLAY BASED ON MFD BUTTON SELECTIONS
		//-------------------------------------------------------------------------------------
		
		/**
		* Description: This method turns the MFD screen on or off (visible or invisible)
		*           
		*/
		private function toggle_mfd_on():void
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
		* Description: This method changes the brightness of the MFD display
		*           
		*/
		private function mfd_brightness(dir:String):void
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
		* Description: This method changes the symbology brightness of the MFD display
		*           
		*/
		private function mfd_symbology_brightness(dir:String):void
		{
			if (_screenOn)
			{
				if (dir == "up")
				{
					if (_symbolBrightness_level < 5)
					{
						switch(_symbolBrightness_level)
						{
							case 4:
								_symbolBrightness_level = 5;
								_current_SymbolBrightness = 1.0;
								break;
							case 3:
								_symbolBrightness_level = 4;
								_current_SymbolBrightness = 0.80;
								break;
							case 2:
								_symbolBrightness_level = 3;
								_current_SymbolBrightness = 0.60;
								break;
							case 1:
								_symbolBrightness_level = 2;
								_current_SymbolBrightness = 0.40;
								break;
						}
					}
				}
				else if (dir == "down")
				{
					if (_symbolBrightness_level > 1)
					{
						switch(_symbolBrightness_level)
						{
							case 5:
								_symbolBrightness_level = 4;
								_current_SymbolBrightness = 0.80;
								break;
							case 4:
								_symbolBrightness_level = 3;
								_current_SymbolBrightness = 0.60;
								break;
							case 3:
								_symbolBrightness_level = 2;
								_current_SymbolBrightness = 0.40;
								break;
							case 2:
								_symbolBrightness_level = 1;
								_current_SymbolBrightness = 0.20;
								break;
						}
					}
				}
				
				// now we need to send this to each component to adjust their symbol brightness
				updateSymbolBrightness(_current_SymbolBrightness);
			}
		}
		
		/**
		* Description: This method adds or removes the airspeed view
		*           
		*/
		private function toggle_airspeed_view():void
		{
			if (_airspeed_shown == true)
			{
				_airspeed_shown = false;
				_this.removeChild(_AIRSPEED_screen);
			}
			else
			{
				_airspeed_shown = true;
				_this.addChild(_AIRSPEED_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the ground speed view
		*           
		*/
		private function toggle_groundspeed_view():void
		{
			if (_groundspeed_shown == true)
			{
				_groundspeed_shown = false;
				_this.removeChild(_GS_Wind_screen);
			}
			else
			{
				_groundspeed_shown = true;
				_this.addChild(_GS_Wind_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the TGT view
		*           
		*/
		private function toggle_tgt_view():void
		{
			if (_tgt_shown == true)
			{
				_tgt_shown = false;
				_this.removeChild(_TGT_screen);
			}
			else
			{
				_tgt_shown = true;
				_this.addChild(_TGT_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the VSI view
		*           
		*/
		private function toggle_vsi_view():void
		{
			if (_vsi_shown == true)
			{
				_vsi_shown = false;
				_this.removeChild(_VSI_screen);
			}
			else
			{
				_vsi_shown = true;
				_this.addChild(_VSI_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the ALT B view
		*           
		*/
		private function toggle_alt_b_view():void
		{
			if (_alt_b_shown == true)
			{
				_alt_b_shown = false;
				_this.removeChild(_ALT_B_screen);
			}
			else
			{
				_alt_b_shown = true;
				_this.addChild(_ALT_B_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the VOR(VHF omnidirectional range)/FMS view 
		*           
		*/
		private function toggle_VOR_FMS_view():void
		{
			if (_vor_fms_shown == true)
			{
				_vor_fms_shown = false;
				_this.removeChild(_VOR_FMS_screen);
			}
			else
			{
				_vor_fms_shown = true;
				_this.addChild(_VOR_FMS_screen);
			}
		}
		
		
		/**
		* Description: This method performs some action (TBD) when the 'NM' is selected
		*           
		*/
		private function select_NM_view():void
		{
			if (_nm_shown == true)
			{
				_nm_shown = false;
				
			}
			else
			{
				_nm_shown = true;
				
			}
		}
		
		/**
		* Description: This method adds or removes the ALT R view
		*           
		*/
		private function toggle_alt_r_view():void
		{
			if (_alt_r_shown == true)
			{
				_alt_r_shown = false;
				_this.removeChild(_ALT_R_screen);
			}
			else
			{
				_alt_r_shown = true;
				_this.addChild(_ALT_R_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the FUEL view
		*           
		*/
		private function toggle_fuel_view():void
		{
			if (_fuel_shown == true)
			{
				_fuel_shown = false;
				_this.removeChild(_FUEL_screen);
			}
			else
			{
				_fuel_shown = true;
				_this.addChild(_FUEL_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the HSI view
		*           
		*/
		private function toggle_hsi_view():void
		{
			if (_hsi_shown == true)
			{
				_hsi_shown = false;
				_this.removeChild(_HSI_screen);
			}
			else
			{
				_hsi_shown = true;
				_this.addChild(_HSI_screen);
			}
		}
		
		/**
		* Description: This method adds or removes the STAB INDICATOR view
		*           
		*/
		private function toggle_stabIndicator_view():void
		{
			if (_stabIndicator_shown == true)
			{
				_stabIndicator_shown = false;
				_this.removeChild(_stabIndicator);
			}
			else
			{
				_stabIndicator_shown = true;
				_this.addChild(_stabIndicator);
			}
		}
		
		
		/**
		* Description: This method adds or removes the SIDE VSI view
		*           
		*/
		private function toggle_side_vsi_view():void
		{
			if (_side_VSI_shown == true)
			{
				_side_VSI_shown = false;
				_this.removeChild(_side_VSI_screen);
			}
			else
			{
				_side_VSI_shown = true;
				_this.addChild(_side_VSI_screen);
			}
		}
		
		/**
		* Description: This method performs the action of PFD view
		*           
		*/
		private function select_PFD_view():void
		{
			if (_pfd_shown == true)
			{
				_pfd_shown = false;
			}
			else
			{
				_pfd_shown = true;
			}
		}
		
		/**
		* Description: This method performs the action of ND (Navigation Display) view
		*           
		*/
		private function select_ND_view():void
		{
			if (_nd_shown == true)
			{
				_nd_shown = false;
			}
			else
			{
				_nd_shown = true;
			}
		}
		
		/**
		* Description: This method performs the action of EICAS view
		*           
		*/
		private function select_EICAS_view():void
		{
			if (_eicas_shown == true)
			{
				_eicas_shown = false;
			}
			else
			{
				_eicas_shown = true;
				set_EICAS_screen();
			}
		}
		
		/**
		* Description: This method performs the action of WCA view
		*           
		*/
		private function select_WCA_view():void
		{
			if (_wca_shown == true)
			{
				_wca_shown = false;
			}
			else
			{
				_wca_shown = true;
				set_WCA_screen();
			}
		}
		
		/**
		* Description: This method performs the action of MS view
		*           
		*/
		private function select_MS_view():void
		{
			if (_ms_shown == true)
			{
				_ms_shown = false;
			}
			else
			{
				_ms_shown = true;
				set_MS_screen();
			}
		}
		
		/**
		* Description: This method performs the action of TAC view
		*           
		*/
		private function select_TAC_view():void
		{
			if (_tac_shown == true)
			{
				_tac_shown = false;
			}
			else
			{
				_tac_shown = true;
				setTSDScreen();
			}
		}
		
		/**
		* Description: This method performs the action of TWD view
		*           
		*/
		private function select_TWD_view():void
		{
			if (_threat_shown == true)
			{
				_threat_shown = false;
			}
			else
			{
				_threat_shown = true;
				setTWDScreen();
			}
		}
		
		/**
		* Description: This method performs the action of Comms view
		*           
		*/
		private function select_Comms_view():void
		{
			if (_comms_shown == true)
			{
				_comms_shown = false;
			}
			else
			{
				_comms_shown = true;
				setCommsScreen();
			}
		}	

		/**
		* Description: This method performs the action of Sensor view
		*           
		*/
		private function select_Sensor_view():void
		{
			if (_sensor_shown == true)
			{
				_sensor_shown = false;
			}
			else
			{
				_sensor_shown = true;
				setSensorScreen();
			}
		}	
		
		/**
		* Description: This method performs the action of DSMS view
		*           
		*/
		private function select_DSMS_view():void
		{
			if (_dsms_shown == true)
			{
				//_dsms_shown = false;
			}
			else
			{
				_dsms_shown = true;
				setStoresManagementScreen();
			}
		}
		
		/**
		* Description: This method performs the action of WPS view
		*           
		*/
		private function select_WPS_view():void
		{
			if (_wps_shown == true)
			{
				//_wps_shown = false;
			}
			else
			{
				_wps_shown = true;
				
				if (_initWeaponStatus == false)
				{	
					// give the current weapon list in the Stores Managent to the Weapons Status 
					_WeaponStatus_screen.weaponList(_StoresManagement_screen.weaponList());
				
					_initWeaponStatus = true;
				}
				
				setWeaponStatusScreen();
			}
		}
		
		
		
		//-------------------------------------------------------------------------------------
		//	METHODS FOR CHANGING THE SCREEN TO DIFFERENT VIEWS
		//-------------------------------------------------------------------------------------
		
		/**
		* Description: This method sets the current screen to the Primary Config screen
		*           
		*/
		private function setPrimaryScreenConfig():void
		{
			clearScreens();
			_currentDisplay = "PrimaryConfig";
			setScreenConfigPositions();
			_this.addChild(_AIRSPEED_screen);
			_this.addChild(_TGT_screen);
			_this.addChild(_FUEL_screen);
			_this.addChild(_VSI_screen);
			_this.addChild(_HSI_screen);
			_this.addChild(_ALT_B_screen);
			_this.addChild(_ALT_R_screen);
			_this.addChild(_mainScreenText);
			_this.addChild(_stabIndicator);
			_this.addChild(_side_VSI_screen);
			_this.addChild(_VOR_FMS_screen);
			_this.addChild(_GS_Wind_screen);
			
			_airspeed_shown = true;
			_tgt_shown = true;
			_fuel_shown = true;
			_vsi_shown = true;
			_hsi_shown = true;
			_alt_b_shown = true;
			_alt_r_shown = true;
			_stabIndicator_shown = true;
			_side_VSI_shown = true;
			_groundspeed_shown = true;
			_vor_fms_shown = true;
			_nm_shown = true;
			_eicas_shown = false;
			_tac_shown = false;
			_dsms_shown = false;
			_wps_shown = false;
			_threat_shown = false;
			_wca_shown = false;
			_ms_shown = false;
			_comms_shown = false;
			_sensor_shown = false;			
		}
		
		
		/**
		* Description: This method sets the current screen to Stores Management System (DSMS)
		*				From here, you can select the WPS button to view Weapon Status.
		*           
		*/
		private function setStoresManagementScreen():void
		{
			clearScreens();
			_currentDisplay = "StoresManagement";
			_this.addChild(_StoresManagement_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = false;
			_wps_shown = false;
			_dsms_shown = true;
			_wca_shown = false;
			_ms_shown = false;
			_comms_shown = false;
			_sensor_shown = false;			
		}
		
		/**
		* Description: This method sets the current screen to Weapons Status (WPS)
		*           
		*/
		private function setWeaponStatusScreen():void
		{
			clearScreens();
			_currentDisplay = "WeaponsStatus";
			_this.addChild(_WeaponStatus_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = false;
			_wps_shown = true;
			_dsms_shown = false;
			_wca_shown = false;
			_ms_shown = false;
			_comms_shown = false;
			_sensor_shown = false;
		}
		
		/**
		* Description: This method sets the current screen to Tactical Situation Display (TSD)
		*           
		*/
		private function setTSDScreen():void
		{
			clearScreens();
			_currentDisplay = "TacticalDisplay";
			_this.addChild(_TSD_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = true;
			_threat_shown = false;
			_wps_shown = false;
			_dsms_shown = false;
			_wca_shown = false;
			_ms_shown = false;
			_comms_shown = false;
			_sensor_shown = false;			
		}		

		/**
		* Description: This method sets the current screen to Comms display
		*           
		*/
		private function setCommsScreen():void
		{
			clearScreens();
			_currentDisplay = "CommsDisplay";
			_this.addChild(_Comms_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = false;
			_comms_shown = true;
			_sensor_shown = false;
			
			_wps_shown = false;
			_dsms_shown = false;
		}	
		
		
		/**
		* Description: This method sets the current screen to Comms display
		*           
		*/
		private function setSensorScreen():void
		{
			clearScreens();
			_currentDisplay = "SensorDisplay";
			_this.addChild(_Sensor_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = false;
			_comms_shown = false;
			_sensor_shown = true;
			
			_wps_shown = false;
			_dsms_shown = false;
		}			
		
		/**
		* Description: This method sets the current screen to Threat Warning Display (TWD)
		*           
		*/
		private function setTWDScreen():void
		{
			clearScreens();
			_currentDisplay = "ThreatDisplay";
			_this.addChild(_Threat_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = true;
			_wps_shown = false;
			_dsms_shown = false;
			_wca_shown = false;
			_ms_shown = false;
			_comms_shown = false;
			_sensor_shown = false;
		}			
		
		/**
		* Description: This method sets the current screen to Warnings and Cautions.
		*              From here, you can select the MS button to view Mission Status.
		*           
		*/
		private function set_WCA_screen():void
		{
			clearScreens();
			_currentDisplay = "WCA_Status";
			_this.addChild(_WCA_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = false;
			_wps_shown = false;
			_dsms_shown = false;
			_ms_shown = false;
			_wca_shown = true;
			_comms_shown = false;
			_sensor_shown = false;
			
		}		
		
		/**
		* Description: This method sets the current screen to Mission Status.
		*           
		*/
		private function set_MS_screen():void
		{
			clearScreens();
			_currentDisplay = "MissionStatus";
			_this.addChild(_MissionStatus_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = false;
			_tac_shown = false;
			_threat_shown = false;
			_wps_shown = false;
			_dsms_shown = false;
			_ms_shown = true;
			_wca_shown = false;
			_comms_shown = false;
			_sensor_shown = false;
			
		}
		
		/**
		* Description: This method sets the current screen to EICAS.
		*           
		*/
		private function set_EICAS_screen():void
		{
			clearScreens();
			_currentDisplay = "EICAS";
			_this.addChild(_EICAS_screen);

			_tgt_shown = false;
			_fuel_shown = false;
			_vsi_shown = false;
			_hsi_shown = false;
			_alt_b_shown = false;
			_alt_r_shown = false;
			_stabIndicator_shown = false;
			_side_VSI_shown = false;
			_groundspeed_shown = false;
			_vor_fms_shown = false;
			_nm_shown = false;
			_eicas_shown = true;
			_tac_shown = false;
			_threat_shown = false;
			_wps_shown = false;
			_dsms_shown = false;
			_ms_shown = false;
			_wca_shown = false;
			_comms_shown = false;
			_sensor_shown = false;
			
		}
		
		
		//-----------------------------------------------------------------------------------
		
		/**
		* Description: This method sets the positions of components on a particular screen
		*           
		*/
		private function setScreenConfigPositions():void
		{
			switch (_currentDisplay)
			{
				case "PrimaryConfig":
					_FUEL_screen.x = 250;
					_FUEL_screen.y = 400;
				    _AIRSPEED_screen.x = 160;
					_AIRSPEED_screen.y = 160;
				    _TGT_screen.x = 70;
				    _TGT_screen.y = 270;
				    _VSI_screen.x = 472;
					_VSI_screen.y = 370;
					_VSI_screen.scaleX = 0.75;
					_VSI_screen.scaleY = 0.75;
					_HSI_screen.x = 480;
					_HSI_screen.y = 455;
					_HSI_screen.scaleX = 0.90;
					_HSI_screen.scaleY = 0.90;
					_ALT_B_screen.x = 800;
					_ALT_B_screen.y = 160;
					_ALT_R_screen.x = 740;
					_ALT_R_screen.y = 320;
					_ALT_R_screen.scaleX = 0.90;
					_ALT_R_screen.scaleY = 0.90;
				    _side_VSI_screen.x = 955;
					_side_VSI_screen.y = 180;
					_VOR_FMS_screen.x = 630;
					_VOR_FMS_screen.y = 300;
					_GS_Wind_screen.x = 240;
					_GS_Wind_screen.y = 300;
					break;
				default:
					_FUEL_screen.x = 250;
					_FUEL_screen.y = 400;
				    _AIRSPEED_screen.x = 160;
					_AIRSPEED_screen.y = 160;
				    _TGT_screen.x = 70;
				    _TGT_screen.y = 270;
				    _VSI_screen.x = 472;
					_VSI_screen.y = 370;
					_VSI_screen.scaleX = 0.75;
					_VSI_screen.scaleY = 0.75;
					_HSI_screen.x = 480;
					_HSI_screen.y = 455;
					_HSI_screen.scaleX = 0.90;
					_HSI_screen.scaleY = 0.90;
					_ALT_B_screen.x = 800;
					_ALT_B_screen.y = 160;
					_ALT_R_screen.x = 740;
					_ALT_R_screen.y = 320;
					_ALT_R_screen.scaleX = 0.90;
					_ALT_R_screen.scaleY = 0.90;
					_side_VSI_screen.x = 955;
					_side_VSI_screen.y = 180;
					_VOR_FMS_screen.x = 630;
					_VOR_FMS_screen.y = 300;
					_GS_Wind_screen.x = 240;
					_GS_Wind_screen.y = 300;
					break;
			}
		}
		
		/**
		* Description: This method updates a screen/display component's internal symbol brightness.
		*              Note that, like the other update, it only executes this for visible 
		*              screens/display components.
		*           
		*/
		private function updateSymbolBrightness(newAlpha:Number):void
		{
			if (_airspeed_shown)
			{
				_AIRSPEED_screen.updateSymbolBrightness(newAlpha);
			}
			if (_tgt_shown)
			{
				_TGT_screen.updateSymbolBrightness(newAlpha);
			}
			if (_fuel_shown)
			{
				_FUEL_screen.updateSymbolBrightness(newAlpha);
			}
			if (_vsi_shown)
			{
				_VSI_screen.updateSymbolBrightness(newAlpha);
			}
			if (_hsi_shown)
			{
				_HSI_screen.updateSymbolBrightness(newAlpha);
			}
			if (_alt_b_shown)
			{
				_ALT_B_screen.updateSymbolBrightness(newAlpha);
			}
			if (_alt_r_shown)
			{
				_ALT_R_screen.updateSymbolBrightness(newAlpha);
			}
			if (_stabIndicator_shown)
			{
				_stabIndicator.updateSymbolBrightness(newAlpha);
			}
			if (_side_VSI_shown)
			{
				_side_VSI_screen.updateSymbolBrightness(newAlpha);
			}	
			if (_vor_fms_shown)
			{
				_VOR_FMS_screen.updateSymbolBrightness(newAlpha);
			}
			if (_groundspeed_shown)
			{
				_GS_Wind_screen.updateSymbolBrightness(newAlpha);
			}
			if (_dsms_shown)
			{
				_StoresManagement_screen.updateSymbolBrightness(newAlpha);
			}
			if (_wps_shown)
			{
				_WeaponStatus_screen.updateSymbolBrightness(newAlpha);
			}
			if (_tac_shown)
			{
				_TSD_screen.updateSymbolBrightness(newAlpha);
			}
			if (_threat_shown)
			{
				_Threat_screen.updateSymbolBrightness(newAlpha);
			}
			if (_wca_shown)
			{
				_WCA_screen.updateSymbolBrightness(newAlpha);
			}
			if (_ms_shown)
			{
				_MissionStatus_screen.updateSymbolBrightness(newAlpha);
			}
			if (_eicas_shown)
			{
				_EICAS_screen.updateSymbolBrightness(newAlpha);
			}
			if (_comms_shown)
			{
				_Comms_screen.updateSymbolBrightness(newAlpha);
			}
			if (_sensor_shown)
			{
				_Sensor_screen.updateSymbolBrightness(newAlpha);
			}			
		}
		

	}  // end of class
	
}  // end of package
