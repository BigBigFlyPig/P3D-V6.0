package {
	
	/**
	*	Developed by: Brian Caldwell, Aug 2017
	*   @author Brian Caldwell
	*   Based on code by: Mark Greenstone
	*   Company: Lockheed Martin, RMS
	*/
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.net.URLRequest;
	import flash.display.Loader;
	import CDU_Btn_Event;
	
	/**
	* Class: main2D
	* Description: This is the main/document class for the CDU Sample.
	*			   Here we initialize Prepar3D, load the CDU Display SWF, and 
	*              listen for/handle the CDU button events. 
	*/
	public class main2D extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		// tracking states of buttons
		private var LSK_L1_state:Number = 0;
		private var LSK_L2_state:Number = 0;
		private var LSK_L3_state:Number = 0;
		private var LSK_L4_state:Number = 0;
		private var LSK_L5_state:Number = 0;
		private var LSK_R1_state:Number = 0;
		private var LSK_R2_state:Number = 0;
		private var LSK_R3_state:Number = 0;
		private var LSK_R4_state:Number = 0;
		private var LSK_R5_state:Number = 0;
		private var DATA_state:Number = 0;
		private var NAV_state:Number = 0;
		private var VNAV_state:Number = 0;
		private var DTO_state:Number = 0;
		private var LIST_state:Number = 0;
		private var PREV_state:Number = 0;
		private var FUEL_state:Number = 0;
		private var FPL_state:Number = 0;
		private var PERF_state:Number = 0;
		private var TUNE_state:Number = 0;
		private var MENU_state:Number = 0;
		private var NEXT_state:Number = 0;
		private var A_state:Number = 0;
		private var B_state:Number = 0;
		private var C_state:Number = 0;
		private var D_state:Number = 0;
		private var E_state:Number = 0;
		private var F_state:Number = 0;
		private var G_state:Number = 0;
		private var H_state:Number = 0;
		private var I_state:Number = 0;
		private var J_state:Number = 0;
		private var K_state:Number = 0;
		private var L_state:Number = 0;
		private var M_state:Number = 0;
		private var N_state:Number = 0;
		private var O_state:Number = 0;
		private var P_state:Number = 0;
		private var Q_state:Number = 0;
		private var R_state:Number = 0;
		private var S_state:Number = 0;
		private var T_state:Number = 0;
		private var U_state:Number = 0;
		private var V_state:Number = 0;
		private var W_state:Number = 0;
		private var X_state:Number = 0;
		private var Y_state:Number = 0;
		private var Z_state:Number = 0;
		private var N1_state:Number = 0;
		private var N2_state:Number = 0;
		private var N3_state:Number = 0;
		private var N4_state:Number = 0;
		private var N5_state:Number = 0;
		private var N6_state:Number = 0;
		private var N7_state:Number = 0;
		private var N8_state:Number = 0;
		private var N9_state:Number = 0;
		private var N0_state:Number = 0;
		private var BACK_state:Number = 0;
		private var MSG_state:Number = 0;
		private var ONOFFDIM_state:Number = 0;
		private var PLUSMINUS_state:Number = 0;
		private var ENTER_state:Number = 0;
		
		private var cduLetMode:Number = 0;
		private var cduNumMode:Number = 0;
		private var cduLskMode:Number = 0;
		private var cduButtonStateNum:Number = 0;
		
		/**
		 * constructor
		 */
		public function main2D() {
			// constructor code
			
			// Only initialize once
			if (initialized == false) {
				Init();
				initialized = true;
			}
		} // end of constructor
		

		/**
		* Function: Init
		* Description: Initialize Prepar3D, load the display SWF, and create
		*              listeners for the button click events.
		* @param none
		* @return none
		*/
		private function Init():void {
			
			// Initialize the Prepar3D Interface because we will need it to initialize
			try {
				Prepar3D.Init();
			} catch (e:Error) {
				trace("Prepar3D init failed.");
			}
			
			// Here we are loading the SWF for the CDU display.
			// The display is in a separate SWF so that it can also be attached to a texture
			//    in the aircraft model. In that case, the display listens for buttons events from
			//    3D environment aircraft.
 			var cdu_display:URLRequest = new URLRequest("CDU.swf");
			var loader:Loader = new Loader();
			loader.load(cdu_display);
			_this.displayContainer.addChild(loader);
			
			// Finally add a listener to listen for when a button is selected
			this.addEventListener(CDU_Btn_Event.CDU_BTN_SELECTED, CDU_buttonClick);
		}
		
		/**
		* Function: CDU_buttonClick
		* Description: Handler for when an CDU button is clicked.
		* @param e:CDU_Btn_Event
		* @return none
		*/
		private function CDU_buttonClick(e:CDU_Btn_Event):void {
			var buttonPushed:String;
			buttonPushed = e.btn_name;
			trace("buttonName = " + buttonPushed);
			
			// The Local Prepar3D variables below are only example variables for the Sample.
			// Replace these variable names with ones from the actual MFD model.mdf file.
			switch (buttonPushed) {
				case "btn_lsk_l1": 
					if (LSK_L1_state == 0) {
						LSK_L1_state = 1;
					} else {
						LSK_L1_state = 0;
					}
					trace("Pushed LSK L1");
					Prepar3D.VarSet("L:x3d_lskKey_l1", "bool", LSK_L1_state);
					break;
				case "btn_lsk_l2": 
					if (LSK_L2_state == 0) {
						LSK_L2_state = 1;
					} else {
						LSK_L2_state = 0;
					}
					trace("Pushed LSK L2");
					Prepar3D.VarSet("L:x3d_lskKey_l2", "bool", LSK_L2_state);
					break;
				case "btn_lsk_l3": 
					if (LSK_L3_state == 0) {
						LSK_L3_state = 1;
					} else {
						LSK_L3_state = 0;
					}
					trace("Pushed LSK L3");
					Prepar3D.VarSet("L:x3d_lskKey_l3", "bool", LSK_L3_state);
					break;
				case "btn_lsk_l4": 
					if (LSK_L4_state == 0) {
						LSK_L4_state = 1;
					} else {
						LSK_L4_state = 0;
					}
					trace("Pushed LSK L4");
					Prepar3D.VarSet("L:x3d_lskKey_l4", "bool", LSK_L4_state);
					break;
				case "btn_lsk_l5": 
					if (LSK_L5_state == 0) {
						LSK_L5_state = 1;
					} else {
						LSK_L5_state = 0;
					}
					trace("Pushed LSK L5");
					Prepar3D.VarSet("L:x3d_lskKey_l5", "bool", LSK_L5_state);
					break;
				case "btn_lsk_r1": 
					if (LSK_R1_state == 0) {
						LSK_R1_state = 1;
					} else {
						LSK_R1_state = 0;
					}
					trace("Pushed LSK R1");
					Prepar3D.VarSet("L:x3d_lskKey_r1", "bool", LSK_R1_state);
					break;
				case "btn_lsk_r2": 
					if (LSK_R2_state == 0) {
						LSK_R2_state = 1;
					} else {
						LSK_R2_state = 0;
					}
					trace("Pushed LSK R2");
					Prepar3D.VarSet("L:x3d_lskKey_r2", "bool", LSK_R2_state);
					break;
				case "btn_lsk_r3": 
					if (LSK_R3_state == 0) {
						LSK_R3_state = 1;
					} else {
						LSK_R3_state = 0;
					}
					trace("Pushed LSK R3");
					Prepar3D.VarSet("L:x3d_lskKey_r3", "bool", LSK_R3_state);
					break;
				case "btn_lsk_r4": 
					if (LSK_R4_state == 0) {
						LSK_R4_state = 1;
					} else {
						LSK_R4_state = 0;
					}
					trace("Pushed LSK R4");
					Prepar3D.VarSet("L:x3d_lskKey_r4", "bool", LSK_R4_state);
					break;
				case "btn_lsk_r5": 
					if (LSK_R5_state == 0) {
						LSK_R5_state = 1;
					} else {
						LSK_R5_state = 0;
					}
					trace("Pushed LSK R5");
					Prepar3D.VarSet("L:x3d_lskKey_r5", "bool", LSK_R5_state);
					break;
				case "btn_data": 
					if (DATA_state == 0) {
						DATA_state = 1;
					} else {
						DATA_state = 0;
					}
					trace("Pushed Data");
					Prepar3D.VarSet("L:x3d_cduData", "bool", DATA_state);
					break;
				case "btn_nav": 
					if (NAV_state == 0) {
						NAV_state = 1;
					} else {
						NAV_state = 0;
					}
					trace("Pushed Nav");
					Prepar3D.VarSet("L:x3d_cduNav", "bool", NAV_state);
					break;
				case "btn_vnav": 
					if (VNAV_state == 0) {
						VNAV_state = 1;
					} else {
						VNAV_state = 0;
					}
					trace("Pushed VNav");
					Prepar3D.VarSet("L:x3d_cduVnav", "bool", VNAV_state);
					break;
				case "btn_dto": 
					if (DTO_state == 0) {
						DTO_state = 1;
					} else {
						DTO_state = 0;
					}
					trace("Pushed DTO");
					Prepar3D.VarSet("L:x3d_cduDto", "bool", DTO_state);
					break;
				case "btn_list": 
					if (LIST_state == 0) {
						LIST_state = 1;
					} else {
						LIST_state = 0;
					}
					trace("Pushed List");
					Prepar3D.VarSet("L:x3d_cduList", "bool", LIST_state);
					break;
				case "btn_prev": 
					if (PREV_state == 0) {
						PREV_state = 1;
					} else {
						PREV_state = 0;
					}
					trace("Pushed Prev");
					Prepar3D.VarSet("L:x3d_cduPrev", "bool", PREV_state);
					break;
				case "btn_fuel": 
					if (FUEL_state == 0) {
						FUEL_state = 1;
					} else {
						FUEL_state = 0;
					}
					trace("Pushed Fuel");
					Prepar3D.VarSet("L:x3d_cduFuel", "bool", FUEL_state);
					break;
				case "btn_fpl": 
					if (FPL_state == 0) {
						FPL_state = 1;
					} else {
						FPL_state = 0;
					}
					trace("Pushed FPL");
					Prepar3D.VarSet("L:x3d_cduFpl", "bool", FPL_state);
					break;
				case "btn_perf": 
					if (PERF_state == 0) {
						PERF_state = 1;
					} else {
						PERF_state = 0;
					}
					trace("Pushed Perf");
					Prepar3D.VarSet("L:x3d_cduPerf", "bool", PERF_state);
					break;
				case "btn_tune": 
					if (TUNE_state == 0) {
						TUNE_state = 1;
					} else {
						TUNE_state = 0;
					}
					trace("Pushed Tune");
					Prepar3D.VarSet("L:x3d_cduTune", "bool", TUNE_state);
					break;
				case "btn_menu": 
					if (MENU_state == 0) {
						MENU_state = 1;
					} else {
						MENU_state = 0;
					}
					trace("Pushed Menu");
					Prepar3D.VarSet("L:x3d_cduMenu", "bool", MENU_state);
					break;
				case "btn_next": 
					if (NEXT_state == 0) {
						NEXT_state = 1;
					} else {
						NEXT_state = 0;
					}
					trace("Pushed Next");
					Prepar3D.VarSet("L:x3d_cduNext", "bool", NEXT_state);
					break;
				case "btn_a": 
					if (A_state == 0) {
						A_state = 1;
					} else {
						A_state = 0;
					}
					trace("Pushed Letter A");
					Prepar3D.VarSet("L:x3d_cduLetterA", "bool", A_state);
					break;
				case "btn_b": 
					if (B_state == 0) {
						B_state = 1;
					} else {
						B_state = 0;
					}
					trace("Pushed Letter B");
					Prepar3D.VarSet("L:x3d_cduLetterB", "bool", B_state);
					break;
				case "btn_c": 
					if (C_state == 0) {
						C_state = 1;
					} else {
						C_state = 0;
					}
					trace("Pushed Letter C");
					Prepar3D.VarSet("L:x3d_cduLetterC", "bool", C_state);
					break;
				case "btn_d": 
					if (D_state == 0) {
						D_state = 1;
					} else {
						D_state = 0;
					}
					trace("Pushed Letter D");
					Prepar3D.VarSet("L:x3d_cduLetterD", "bool", D_state);
					break;
				case "btn_e": 
					if (E_state == 0) {
						E_state = 1;
					} else {
						E_state = 0;
					}
					trace("Pushed Letter A");
					Prepar3D.VarSet("L:x3d_cduLetterE", "bool", E_state);
					break;
				case "btn_f": 
					if (F_state == 0) {
						F_state = 1;
					} else {
						F_state = 0;
					}
					trace("Pushed Letter F");
					Prepar3D.VarSet("L:x3d_cduLetterF", "bool", F_state);
					break;
				case "btn_g": 
					if (G_state == 0) {
						G_state = 1;
					} else {
						G_state = 0;
					}
					trace("Pushed Letter G");
					Prepar3D.VarSet("L:x3d_cduLetterG", "bool", G_state);
					break;
				case "btn_h": 
					if (H_state == 0) {
						H_state = 1;
					} else {
						H_state = 0;
					}
					trace("Pushed Letter H");
					Prepar3D.VarSet("L:x3d_cduLetterH", "bool", H_state);
					break;
				case "btn_i": 
					if (I_state == 0) {
						I_state = 1;
					} else {
						I_state = 0;
					}
					trace("Pushed Letter I");
					Prepar3D.VarSet("L:x3d_cduLetterI", "bool", I_state);
					break;
				case "btn_j": 
					if (J_state == 0) {
						J_state = 1;
					} else {
						J_state = 0;
					}
					trace("Pushed Letter J");
					Prepar3D.VarSet("L:x3d_cduLetterJ", "bool", J_state);
					break;
				case "btn_k": 
					if (K_state == 0) {
						K_state = 1;
					} else {
						K_state = 0;
					}
					trace("Pushed Letter K");
					Prepar3D.VarSet("L:x3d_cduLetterK", "bool", K_state);
					break;
				case "btn_l": 
					if (L_state == 0) {
						L_state = 1;
					} else {
						L_state = 0;
					}
					trace("Pushed Letter L");
					Prepar3D.VarSet("L:x3d_cduLetterL", "bool", L_state);
					break;
				case "btn_m": 
					if (M_state == 0) {
						M_state = 1;
					} else {
						M_state = 0;
					}
					trace("Pushed Letter M");
					Prepar3D.VarSet("L:x3d_cduLetterM", "bool", M_state);
					break;
				case "btn_n": 
					if (N_state == 0) {
						N_state = 1;
					} else {
						N_state = 0;
					}
					trace("Pushed Letter N");
					Prepar3D.VarSet("L:x3d_cduLetterN", "bool", N_state);
					break;
				case "btn_o": 
					if (O_state == 0) {
						O_state = 1;
					} else {
						O_state = 0;
					}
					trace("Pushed Letter O");
					Prepar3D.VarSet("L:x3d_cduLetterO", "bool", O_state);
					break;
				case "btn_p": 
					if (P_state == 0) {
						P_state = 1;
					} else {
						P_state = 0;
					}
					trace("Pushed Letter P");
					Prepar3D.VarSet("L:x3d_cduLetterP", "bool", P_state);
					break;
				case "btn_q": 
					if (Q_state == 0) {
						Q_state = 1;
					} else {
						Q_state = 0;
					}
					trace("Pushed Letter Q");
					Prepar3D.VarSet("L:x3d_cduLetterQ", "bool", Q_state);
					break;
				case "btn_r": 
					if (R_state == 0) {
						R_state = 1;
					} else {
						R_state = 0;
					}
					trace("Pushed Letter R");
					Prepar3D.VarSet("L:x3d_cduLetterR", "bool", R_state);
					break;
				case "btn_s": 
					if (S_state == 0) {
						S_state = 1;
					} else {
						S_state = 0;
					}
					trace("Pushed Letter S");
					Prepar3D.VarSet("L:x3d_cduLetterS", "bool", S_state);
					break;
				case "btn_t": 
					if (T_state == 0) {
						T_state = 1;
					} else {
						T_state = 0;
					}
					trace("Pushed Letter T");
					Prepar3D.VarSet("L:x3d_cduLetterT", "bool", T_state);
					break;
				case "btn_u": 
					if (U_state == 0) {
						U_state = 1;
					} else {
						U_state = 0;
					}
					trace("Pushed Letter U");
					Prepar3D.VarSet("L:x3d_cduLetterU", "bool", U_state);
					break;
				case "btn_v": 
					if (V_state == 0) {
						V_state = 1;
					} else {
						V_state = 0;
					}
					trace("Pushed Letter V");
					Prepar3D.VarSet("L:x3d_cduLetterV", "bool", V_state);
					break;
				case "btn_w": 
					if (W_state == 0) {
						W_state = 1;
					} else {
						W_state = 0;
					}
					trace("Pushed Letter W");
					Prepar3D.VarSet("L:x3d_cduLetterW", "bool", W_state);
					break;
				case "btn_x": 
					if (X_state == 0) {
						X_state = 1;
					} else {
						X_state = 0;
					}
					trace("Pushed Letter X");
					Prepar3D.VarSet("L:x3d_cduLetterX", "bool", X_state);
					break;
				case "btn_y": 
					if (Y_state == 0) {
						Y_state = 1;
					} else {
						Y_state = 0;
					}
					trace("Pushed Letter Y");
					Prepar3D.VarSet("L:x3d_cduLetterY", "bool", Y_state);
					break;
				case "btn_z": 
					if (Z_state == 0) {
						Z_state = 1;
					} else {
						Z_state = 0;
					}
					trace("Pushed Letter Z");
					Prepar3D.VarSet("L:x3d_cduLetterZ", "bool", Z_state);
					break;
				case "btn_1": 
					if (N1_state == 0) {
						N1_state = 1;
					} else {
						N1_state = 0;
					}
					trace("Pushed Number 1");
					Prepar3D.VarSet("L:x3d_cduNumber1", "bool", N1_state);
					break;
				case "btn_2": 
					if (N2_state == 0) {
						N2_state = 1;
					} else {
						N2_state = 0;
					}
					trace("Pushed Number 2");
					Prepar3D.VarSet("L:x3d_cduNumber2", "bool", N2_state);
					break;
				case "btn_3": 
					if (N3_state == 0) {
						N3_state = 1;
					} else {
						N3_state = 0;
					}
					trace("Pushed Number 3");
					Prepar3D.VarSet("L:x3d_cduNumber3", "bool", N3_state);
					break;
				case "btn_4": 
					if (N4_state == 0) {
						N4_state = 1;
					} else {
						N4_state = 0;
					}
					trace("Pushed Number 4");
					Prepar3D.VarSet("L:x3d_cduNumber4", "bool", N4_state);
					break;
				case "btn_5": 
					if (N5_state == 0) {
						N5_state = 1;
					} else {
						N5_state = 0;
					}
					trace("Pushed Number 5");
					Prepar3D.VarSet("L:x3d_cduNumber5", "bool", N5_state);
					break;
				case "btn_6": 
					if (N6_state == 0) {
						N6_state = 1;
					} else {
						N6_state = 0;
					}
					trace("Pushed Number 6");
					Prepar3D.VarSet("L:x3d_cduNumber6", "bool", N6_state);
					break;
				case "btn_7": 
					if (N7_state == 0) {
						N7_state = 1;
					} else {
						N7_state = 0;
					}
					trace("Pushed Number 7");
					Prepar3D.VarSet("L:x3d_cduNumber7", "bool", N7_state);
					break;
				case "btn_8": 
					if (N8_state == 0) {
						N8_state = 1;
					} else {
						N8_state = 0;
					}
					trace("Pushed Number 8");
					Prepar3D.VarSet("L:x3d_cduNumber8", "bool", N8_state);
					break;
				case "btn_9": 
					if (N9_state == 0) {
						N9_state = 1;
					} else {
						N9_state = 0;
					}
					trace("Pushed Number 9");
					Prepar3D.VarSet("L:x3d_cduNumber9", "bool", N9_state);
					break;
				case "btn_0": 
					if (N0_state == 0) {
						N0_state = 1;
					} else {
						N0_state = 0;
					}
					trace("Pushed Number 0");
					Prepar3D.VarSet("L:x3d_cduNumber0", "bool", N0_state);
					break;
				case "btn_back": 
					if (BACK_state == 0) {
						BACK_state = 1;
					} else {
						BACK_state = 0;
					}
					trace("Pushed Back");
					Prepar3D.VarSet("L:x3d_cduBack", "bool", BACK_state);
					break;
				case "btn_msg": 
					if (MSG_state == 0) {
						MSG_state = 1;
					} else {
						MSG_state = 0;
					}
					trace("Pushed MSG");
					Prepar3D.VarSet("L:x3d_cduMsg", "bool", MSG_state);
					break;
				case "btn_dim": 
					if (ONOFFDIM_state == 0) {
						ONOFFDIM_state = 1;
					} else {
						ONOFFDIM_state = 0;
					}
					trace("Pushed ON/OFF DIM");
					Prepar3D.VarSet("L:x3d_cduOnOff", "bool", ONOFFDIM_state);
					break;
				case "btn_pm": 
					if (PLUSMINUS_state == 0) {
						PLUSMINUS_state = 1;
					} else {
						PLUSMINUS_state = 0;
					}
					trace("Pushed PLUS/MINUS");
					Prepar3D.VarSet("L:x3d_cduPlusMin", "bool", PLUSMINUS_state);
					break;
				case "btn_enter": 
					if (ENTER_state == 0) {
						ENTER_state = 1;
					} else {
						ENTER_state = 0;
					}
					trace("Pushed ENTER");
					Prepar3D.VarSet("L:x3d_cduEnter", "bool", ENTER_state);
					break;
				
			}
		}
	}

}
