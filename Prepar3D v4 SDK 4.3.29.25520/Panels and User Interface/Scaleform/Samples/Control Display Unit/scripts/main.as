package  {
	
	/**
	*	Developed by: Brian Caldwell, Aug 2017
	*   @author Brian Caldwell
	*   Based on code by: Mark Greenstone
	*   Company: Lockheed Martin, RMS
	*/
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	/**
	* Class: main
	* Description: This is the main/document class for CDU Display SWF.
	*			   Here we initialize Prepar3D, listen for CDU button events,
	*              store button states, and if a button state changes, tells the
	*              ScreenManager to update the screen.
	*/
	public class main extends MovieClip {
		
		private var initialized:Boolean;
		
		// These properties capture the current state of each button from Prepar3D
		private var LSK_L1_state:Boolean = false;
		private var LSK_L2_state:Boolean = false;
		private var LSK_L3_state:Boolean = false;
		private var LSK_L4_state:Boolean = false;
		private var LSK_L5_state:Boolean = false;
		private var LSK_R1_state:Boolean = false;
		private var LSK_R2_state:Boolean = false;
		private var LSK_R3_state:Boolean = false;
		private var LSK_R4_state:Boolean = false;
		private var LSK_R5_state:Boolean = false;
		private var DATA_state:Boolean = false;
		private var NAV_state:Boolean = false;
		private var VNAV_state:Boolean = false;
		private var DTO_state:Boolean = false;
		private var LIST_state:Boolean = false;
		private var PREV_state:Boolean = false;
		private var FUEL_state:Boolean = false;
		private var FPL_state:Boolean = false;
		private var PERF_state:Boolean = false;
		private var TUNE_state:Boolean = false;
		private var MENU_state:Boolean = false;
		private var NEXT_state:Boolean = false;
		private var A_state:Boolean = false;
		private var B_state:Boolean = false;
		private var C_state:Boolean = false;
		private var D_state:Boolean = false;
		private var E_state:Boolean = false;
		private var F_state:Boolean = false;
		private var G_state:Boolean = false;
		private var H_state:Boolean = false;
		private var I_state:Boolean = false;
		private var J_state:Boolean = false;
		private var K_state:Boolean = false;
		private var L_state:Boolean = false;
		private var M_state:Boolean = false;
		private var N_state:Boolean = false;
		private var O_state:Boolean = false;
		private var P_state:Boolean = false;
		private var Q_state:Boolean = false;
		private var R_state:Boolean = false;
		private var S_state:Boolean = false;
		private var T_state:Boolean = false;
		private var U_state:Boolean = false;
		private var V_state:Boolean = false;
		private var W_state:Boolean = false;
		private var X_state:Boolean = false;
		private var Y_state:Boolean = false;
		private var Z_state:Boolean = false;
		private var N1_state:Boolean = false;
		private var N2_state:Boolean = false;
		private var N3_state:Boolean = false;
		private var N4_state:Boolean = false;
		private var N5_state:Boolean = false;
		private var N6_state:Boolean = false;
		private var N7_state:Boolean = false;
		private var N8_state:Boolean = false;
		private var N9_state:Boolean = false;
		private var N0_state:Boolean = false;
		private var BACK_state:Boolean = false;
		private var MSG_state:Boolean = false;
		private var ONOFFDIM_state:Boolean = false;
		private var PLUSMINUS_state:Boolean = false;
		private var ENTER_state:Boolean = false;
		
		// These properties store the current state of each button from Prepar3D
		private var knob1_store:Boolean;
		private var LSK_L1_store:Boolean;
		private var LSK_L2_store:Boolean;
		private var LSK_L3_store:Boolean;
		private var LSK_L4_store:Boolean;
		private var LSK_L5_store:Boolean;
		private var LSK_R1_store:Boolean;
		private var LSK_R2_store:Boolean;
		private var LSK_R3_store:Boolean;
		private var LSK_R4_store:Boolean;
		private var LSK_R5_store:Boolean;
		private var DATA_store:Boolean;
		private var NAV_store:Boolean;
		private var VNAV_store:Boolean;
		private var DTO_store:Boolean;
		private var LIST_store:Boolean;
		private var PREV_store:Boolean;
		private var FUEL_store:Boolean;
		private var FPL_store:Boolean;
		private var PERF_store:Boolean;
		private var TUNE_store:Boolean;
		private var MENU_store:Boolean;
		private var NEXT_store:Boolean;
		private var A_store:Boolean;
		private var B_store:Boolean;
		private var C_store:Boolean;
		private var D_store:Boolean;
		private var E_store:Boolean;
		private var F_store:Boolean;
		private var G_store:Boolean;
		private var H_store:Boolean;
		private var I_store:Boolean;
		private var J_store:Boolean;
		private var K_store:Boolean;
		private var L_store:Boolean;
		private var M_store:Boolean;
		private var N_store:Boolean;
		private var O_store:Boolean;
		private var P_store:Boolean;
		private var Q_store:Boolean;
		private var R_store:Boolean;
		private var S_store:Boolean;
		private var T_store:Boolean;
		private var U_store:Boolean;
		private var V_store:Boolean;
		private var W_store:Boolean;
		private var X_store:Boolean;
		private var Y_store:Boolean;
		private var Z_store:Boolean;
		private var N1_store:Boolean;
		private var N2_store:Boolean;
		private var N3_store:Boolean;
		private var N4_store:Boolean;
		private var N5_store:Boolean;
		private var N6_store:Boolean;
		private var N7_store:Boolean;
		private var N8_store:Boolean;
		private var N9_store:Boolean;
		private var N0_store:Boolean;
		private var BACK_store:Boolean;
		private var MSG_store:Boolean;
		private var ONOFFDIM_store:Boolean;
		private var PLUSMINUS_store:Boolean;
		private var ENTER_store:Boolean;
		
		private var _this = this;
		
		private var currentDisplay:String;
		private var buttonSelected:String;
		private var buttonState:Boolean;
		private var buttonChanged:Boolean;
		
		/**
		* constructor
		*/
		public function main() {
			// constructor code
			
			// Only initialize once
			if (initialized == false)
			{
				Init();
				initialized = true;
			}

		} // end of constructor
		
		/**
		* Description: This method is used to initialize the MFD program.
		*              It initializes the Prepar3D, stores the initial button states,
		*              and creates the event listeners to constantly update 
		*              the MFD variables and monitor the button events.
		*
		*/
		private function Init():void
		{	
			
			// Initialize the Prepar3D Interface because we will need it to initialize
			try{
				Prepar3D.Init();
			}
			catch(e:Error)
			{
				trace("Prepar3D init failed.");
			}
			
			buttonSelected = "";
			currentDisplay = "On";
			
			storeCurrentStates();
			
			// on EnterFrame event, update the display
			this.addEventListener(Event.ENTER_FRAME, handleUpdate);
			this.addEventListener(Event.ENTER_FRAME, cduScreenManager.handleUpdateData);
			
			// DEBUG
			
		}

		/**
		* Function: handleUpdate
		* Description: This event handler is called on every Enter Frame (24 times a second).
		*              Each time it captures the current state of the MFD buttons and if any
		*              have changed state, will have the ScreenManager update the screen.
		*              Finally, it stores the current states to use for comparison on next 
		*              event call.
		* @param e:Event
		* @return none
		*/
		private function handleUpdate(e:Event)
		{				
			// Get the MFD button Prepar3D variables and see if any have changed state
			LSK_L1_state = Prepar3D.VarGet("L:x3d_lskKey_l1", "bool");
			LSK_L2_state = Prepar3D.VarGet("L:x3d_lskKey_l2", "bool");
			LSK_L3_state = Prepar3D.VarGet("L:x3d_lskKey_l3", "bool");
			LSK_L4_state = Prepar3D.VarGet("L:x3d_lskKey_l4", "bool");
			LSK_L5_state = Prepar3D.VarGet("L:x3d_lskKey_l5", "bool");
			LSK_R1_state = Prepar3D.VarGet("L:x3d_lskKey_r1", "bool");
			LSK_R2_state = Prepar3D.VarGet("L:x3d_lskKey_r2", "bool");
			LSK_R3_state = Prepar3D.VarGet("L:x3d_lskKey_r3", "bool");
			LSK_R4_state = Prepar3D.VarGet("L:x3d_lskKey_r4", "bool");
			LSK_R5_state = Prepar3D.VarGet("L:x3d_lskKey_r5", "bool");
			DATA_state = Prepar3D.VarGet("L:x3d_cduData", "bool");
			NAV_state = Prepar3D.VarGet("L:x3d_cduNav", "bool");
			VNAV_state = Prepar3D.VarGet("L:x3d_cduVnav", "bool");
			DTO_state = Prepar3D.VarGet("L:x3d_cduDto", "bool");
			LIST_state = Prepar3D.VarGet("L:x3d_cduList", "bool");
			PREV_state = Prepar3D.VarGet("L:x3d_cduPrev", "bool");
			FUEL_state = Prepar3D.VarGet("L:x3d_cduFuel", "bool");
			FPL_state = Prepar3D.VarGet("L:x3d_cduFpl", "bool");
			PERF_state = Prepar3D.VarGet("L:x3d_cduPerf", "bool");
			TUNE_state = Prepar3D.VarGet("L:x3d_cduTune", "bool");
			MENU_state = Prepar3D.VarGet("L:x3d_cduMenu", "bool");
			NEXT_state = Prepar3D.VarGet("L:x3d_cduNext", "bool");
			A_state = Prepar3D.VarGet("L:x3d_cduLetterA", "bool");
			B_state = Prepar3D.VarGet("L:x3d_cduLetterB", "bool");
			C_state = Prepar3D.VarGet("L:x3d_cduLetterC", "bool");
			D_state = Prepar3D.VarGet("L:x3d_cduLetterD", "bool");
			E_state = Prepar3D.VarGet("L:x3d_cduLetterE", "bool");
			F_state = Prepar3D.VarGet("L:x3d_cduLetterF", "bool");
			G_state = Prepar3D.VarGet("L:x3d_cduLetterG", "bool");
			H_state = Prepar3D.VarGet("L:x3d_cduLetterH", "bool");
			I_state = Prepar3D.VarGet("L:x3d_cduLetterI", "bool");
			J_state = Prepar3D.VarGet("L:x3d_cduLetterJ", "bool");
			K_state = Prepar3D.VarGet("L:x3d_cduLetterK", "bool");
			L_state = Prepar3D.VarGet("L:x3d_cduLetterL", "bool");
			M_state = Prepar3D.VarGet("L:x3d_cduLetterM", "bool");
			N_state = Prepar3D.VarGet("L:x3d_cduLetterN", "bool");
			O_state = Prepar3D.VarGet("L:x3d_cduLetterO", "bool");
			P_state = Prepar3D.VarGet("L:x3d_cduLetterP", "bool");
			Q_state = Prepar3D.VarGet("L:x3d_cduLetterQ", "bool");
			R_state = Prepar3D.VarGet("L:x3d_cduLetterR", "bool");
			S_state = Prepar3D.VarGet("L:x3d_cduLetterS", "bool");
			T_state = Prepar3D.VarGet("L:x3d_cduLetterT", "bool");
			U_state = Prepar3D.VarGet("L:x3d_cduLetterU", "bool");
			V_state = Prepar3D.VarGet("L:x3d_cduLetterV", "bool");
			W_state = Prepar3D.VarGet("L:x3d_cduLetterW", "bool");
			X_state = Prepar3D.VarGet("L:x3d_cduLetterX", "bool");
			Y_state = Prepar3D.VarGet("L:x3d_cduLetterY", "bool");
			Z_state = Prepar3D.VarGet("L:x3d_cduLetterZ", "bool");
			N1_state = Prepar3D.VarGet("L:x3d_cduNumber1", "bool");
			N2_state = Prepar3D.VarGet("L:x3d_cduNumber2", "bool");
			N3_state = Prepar3D.VarGet("L:x3d_cduNumber3", "bool");
			N4_state = Prepar3D.VarGet("L:x3d_cduNumber4", "bool");
			N5_state = Prepar3D.VarGet("L:x3d_cduNumber5", "bool");
			N6_state = Prepar3D.VarGet("L:x3d_cduNumber6", "bool");
			N7_state = Prepar3D.VarGet("L:x3d_cduNumber7", "bool");
			N8_state = Prepar3D.VarGet("L:x3d_cduNumber8", "bool");
			N9_state = Prepar3D.VarGet("L:x3d_cduNumber9", "bool");
			N0_state = Prepar3D.VarGet("L:x3d_cduNumber0", "bool");
			BACK_state = Prepar3D.VarGet("L:x3d_cduBack", "bool");
			MSG_state = Prepar3D.VarGet("L:x3d_cduMsg", "bool");
			ONOFFDIM_state = Prepar3D.VarGet("L:x3d_cduOnOff", "bool");
			PLUSMINUS_state = Prepar3D.VarGet("L:x3d_cduPlusMin", "bool");
			ENTER_state = Prepar3D.VarGet("L:x3d_cduEnter", "bool");
		
			buttonChanged = buttonStatesChange();
			if (buttonChanged == true)
			{
				cduScreenManager.updateScreen(buttonSelected,buttonState);
				
				storeCurrentStates();
			}
			
		} // end of function
		
		/**
		* Description: This method is used to store the current state of all the buttons.
		*           
		*/
		private function storeCurrentStates():void
		{	
			LSK_L1_store = LSK_L1_state;
			LSK_L2_store = LSK_L2_state;
			LSK_L3_store = LSK_L3_state;
			LSK_L4_store = LSK_L4_state;
			LSK_L5_store = LSK_L5_state;
			LSK_R1_store = LSK_R1_state;
			LSK_R2_store = LSK_R2_state;
			LSK_R3_store = LSK_R3_state;
			LSK_R4_store = LSK_R4_state;
			LSK_R5_store = LSK_R5_state;
			DATA_store = DATA_state;
			NAV_store = NAV_state;
			VNAV_store = VNAV_state;
			DTO_store = DTO_state;
			LIST_store = LIST_state;
			PREV_store = PREV_state;
			FUEL_store = FUEL_state;
			FPL_store = FPL_state;
			PERF_store = PERF_state;
			TUNE_store = TUNE_state;
			MENU_store = MENU_state;
			NEXT_store = NEXT_state;
			A_store = A_state;
			B_store = B_state;
			C_store = C_state;
			D_store = D_state;
			E_store = E_state;
			F_store = F_state;
			G_store = G_state;
			H_store = H_state;
			I_store = I_state;
			J_store = J_state;
			K_store = K_state;
			L_store = L_state;
			M_store = M_state;
			N_store = N_state;
			O_store = O_state;
			P_store = P_state;
			Q_store = Q_state;
			R_store = R_state;
			S_store = S_state;
			T_store = T_state;
			U_store = U_state;
			V_store = V_state;
			W_store = W_state;
			X_store = X_state;
			Y_store = Y_state;
			Z_store = Z_state;
			N1_store = N1_state;
			N2_store = N2_state;
			N3_store = N3_state;
			N4_store = N4_state;
			N5_store = N5_state;
			N6_store = N6_state;
			N7_store = N7_state;
			N8_store = N8_state;
			N9_store = N9_state;
			N0_store = N0_state;
			BACK_store = BACK_state;
			MSG_store = MSG_state;
			ONOFFDIM_store = ONOFFDIM_state;
			PLUSMINUS_store = PLUSMINUS_state;
			ENTER_store = ENTER_state;
		}
		
		/**
		* Description: This method is used to determine if any states have changed since
		*              the last frame change. If no changes have occurred (idle), then 
		*              this will return boolean 'false'. If there is a change, it will
		*              return boolean 'true'.
		*
		* @return change:Boolean
		*/
		private function buttonStatesChange():Boolean
		{
			var change:Boolean = false;	
			
			if (LSK_L1_store != LSK_L1_state)
			{			
				change = true;
				buttonSelected = "btn_lsk_l1";
				buttonState = LSK_L1_state;
			}
			else if (LSK_L2_store != LSK_L2_state)
			{
				change = true;
				buttonSelected = "btn_lsk_l2";
				buttonState = LSK_L2_state;
			}
			else if (LSK_L3_store != LSK_L3_state)
			{
				change = true;
				buttonSelected = "btn_lsk_l3";
				buttonState = LSK_L3_state;
			}
			else if (LSK_L4_store != LSK_L4_state)
			{
				change = true;
				buttonSelected = "btn_lsk_l4";
				buttonState = LSK_L4_state;
			}
			else if (LSK_L5_store != LSK_L5_state)
			{
				change = true;
				buttonSelected = "btn_lsk_l5";
				buttonState = LSK_L5_state;
			}
			else if (LSK_R1_store != LSK_R1_state)
			{			
				change = true;
				buttonSelected = "btn_lsk_r1";
				buttonState = LSK_R1_state;
			}
			else if (LSK_R2_store != LSK_R2_state)
			{
				change = true;
				buttonSelected = "btn_lsk_r2";
				buttonState = LSK_R2_state;
			}
			else if (LSK_R3_store != LSK_R3_state)
			{
				change = true;
				buttonSelected = "btn_lsk_r3";
				buttonState = LSK_R3_state;
			}
			else if (LSK_R4_store != LSK_R4_state)
			{
				change = true;
				buttonSelected = "btn_lsk_r4";
				buttonState = LSK_R4_state;
			}
			else if (LSK_R5_store != LSK_R5_state)
			{
				change = true;
				buttonSelected = "btn_lsk_r5";
				buttonState = LSK_R5_state;
			}
			else if (DATA_store != DATA_state)
			{
				change = true;
				buttonSelected = "btn_data";
				buttonState = DATA_state;
			}
			else if (NAV_store != NAV_state)
			{
				change = true;
				buttonSelected = "btn_nav";
				buttonState = NAV_state;
			}
			else if (VNAV_store != VNAV_state)
			{
				change = true;
				buttonSelected = "btn_vnav";
				buttonState = VNAV_state;
			}
			else if (DTO_store != DTO_state)
			{
				change = true;
				buttonSelected = "btn_dto";
				buttonState = DTO_state;
			}
			else if (LIST_store != LIST_state)
			{
				change = true;
				buttonSelected = "btn_list";
				buttonState = LIST_state;
			}
			else if (PREV_store != PREV_state)
			{
				change = true;
				buttonSelected = "btn_prev";
				buttonState = PREV_state;
			}
			else if (FUEL_store != FUEL_state)
			{
				change = true;
				buttonSelected = "btn_fuel";
				buttonState = FUEL_state;
			}
			else if (FPL_store != FPL_state)
			{
				change = true;
				buttonSelected = "btn_fpl";
				buttonState = FPL_state;
			}
			else if (PERF_store != PERF_state)
			{
				change = true;
				buttonSelected = "btn_perf";
				buttonState = PERF_state;
			}
			else if (TUNE_store != TUNE_state)
			{
				change = true;
				buttonSelected = "btn_tune";
				buttonState = TUNE_state;
			}
			else if (MENU_store != MENU_state)
			{
				change = true;
				buttonSelected = "btn_menu";
				buttonState = MENU_state;
			}
			else if (NEXT_store != NEXT_state)
			{
				change = true;
				buttonSelected = "btn_next";
				buttonState = NEXT_state;
			}
			else if (A_store != A_state)
			{
				change = true;
				buttonSelected = "btn_a";
				buttonState = A_state;
			}
			else if (B_store != B_state)
			{
				change = true;
				buttonSelected = "btn_b";
				buttonState = B_state;
			}
			else if (C_store != C_state)
			{
				change = true;
				buttonSelected = "btn_c";
				buttonState = C_state;
			}
			else if (D_store != D_state)
			{
				change = true;
				buttonSelected = "btn_d";
				buttonState = D_state;
			}
			else if (E_store != E_state)
			{
				change = true;
				buttonSelected = "btn_e";
				buttonState = E_state;
			}
			else if (F_store != F_state)
			{
				change = true;
				buttonSelected = "btn_f";
				buttonState = F_state;
			}
			else if (G_store != G_state)
			{
				change = true;
				buttonSelected = "btn_g";
				buttonState = G_state;
			}
			else if (H_store != H_state)
			{
				change = true;
				buttonSelected = "btn_h";
				buttonState = H_state;
			}
			else if (I_store != I_state)
			{
				change = true;
				buttonSelected = "btn_i";
				buttonState = I_state;
			}
			else if (J_store != J_state)
			{
				change = true;
				buttonSelected = "btn_j";
				buttonState = J_state;
			}
			else if (K_store != K_state)
			{
				change = true;
				buttonSelected = "btn_k";
				buttonState = K_state;
			}
			else if (L_store != L_state)
			{
				change = true;
				buttonSelected = "btn_l";
				buttonState = L_state;
			}
			else if (M_store != M_state)
			{
				change = true;
				buttonSelected = "btn_m";
				buttonState = M_state;
			}
			else if (N_store != N_state)
			{
				change = true;
				buttonSelected = "btn_n";
				buttonState = N_state;
			}
			else if (O_store != O_state)
			{
				change = true;
				buttonSelected = "btn_o";
				buttonState = O_state;
			}
			else if (P_store != P_state)
			{
				change = true;
				buttonSelected = "btn_p";
				buttonState = P_state;
			}
			else if (Q_store != Q_state)
			{
				change = true;
				buttonSelected = "btn_q";
				buttonState = Q_state;
			}
			else if (R_store != R_state)
			{
				change = true;
				buttonSelected = "btn_r";
				buttonState = R_state;
			}
			else if (S_store != S_state)
			{
				change = true;
				buttonSelected = "btn_s";
				buttonState = S_state;
			}
			else if (T_store != T_state)
			{
				change = true;
				buttonSelected = "btn_t";
				buttonState = T_state;
			}
			else if (U_store != U_state)
			{
				change = true;
				buttonSelected = "btn_u";
				buttonState = U_state;
			}
			else if (V_store != V_state)
			{
				change = true;
				buttonSelected = "btn_v";
				buttonState = V_state;
			}
			else if (W_store != W_state)
			{
				change = true;
				buttonSelected = "btn_w";
				buttonState = W_state;
			}
			else if (X_store != X_state)
			{
				change = true;
				buttonSelected = "btn_x";
				buttonState = X_state;
			}
			else if (Y_store != Y_state)
			{
				change = true;
				buttonSelected = "btn_y";
				buttonState = Y_state;
			}
			else if (Z_store != Z_state)
			{
				change = true;
				buttonSelected = "btn_z";
				buttonState = Z_state;
			}
			else if (N1_store != N1_state)
			{
				change = true;
				buttonSelected = "btn_1";
				buttonState = N1_state;
			}
			else if (N2_store != N2_state)
			{
				change = true;
				buttonSelected = "btn_2";
				buttonState = N2_state;
			}
			else if (N3_store != N3_state)
			{
				change = true;
				buttonSelected = "btn_3";
				buttonState = N3_state;
			}
			else if (N4_store != N4_state)
			{
				change = true;
				buttonSelected = "btn_4";
				buttonState = N4_state;
			}
			else if (N5_store != N5_state)
			{
				change = true;
				buttonSelected = "btn_5";
				buttonState = N5_state;
			}
			else if (N6_store != N6_state)
			{
				change = true;
				buttonSelected = "btn_6";
				buttonState = N6_state;
			}
			else if (N7_store != N7_state)
			{
				change = true;
				buttonSelected = "btn_7";
				buttonState = N7_state;
			}
			else if (N8_store != N8_state)
			{
				change = true;
				buttonSelected = "btn_8";
				buttonState = N8_state;
			}
			else if (N9_store != N9_state)
			{
				change = true;
				buttonSelected = "btn_9";
				buttonState = N9_state;
			}
			else if (N0_store != N0_state)
			{
				change = true;
				buttonSelected = "btn_0";
				buttonState = N0_state;
			}
			else if (BACK_store != BACK_state)
			{
				change = true;
				buttonSelected = "btn_back";
				buttonState = BACK_state;
			}
			else if (MSG_store != MSG_state)
			{
				change = true;
				buttonSelected = "btn_msg";
				buttonState = MSG_state;
			}
			else if (ONOFFDIM_store != ONOFFDIM_state)
			{
				change = true;
				buttonSelected = "btn_dim";
				buttonState = ONOFFDIM_state;
			}
			else if (PLUSMINUS_store != PLUSMINUS_state)
			{
				change = true;
				buttonSelected = "btn_pm";
				buttonState = PLUSMINUS_state;
			}
			else if (ENTER_store != ENTER_state)
			{
				change = true;
				buttonSelected = "btn_enter";
				buttonState = ENTER_state;
			}

			return change;
		}
		
	}	// end of class
}  // end of package
