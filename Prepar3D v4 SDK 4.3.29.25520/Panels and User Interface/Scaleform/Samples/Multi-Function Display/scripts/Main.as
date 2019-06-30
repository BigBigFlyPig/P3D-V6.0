package  {
	
	/**
	*	Developed by: Mark Greenstone, July 2017
	*   @author Mark Greenstone
	*   Company: Lockheed Martin, RMS
	*/
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	/**
	* Class: main
	* Description: This is the main/document class for MFD Display SWF.
	*			   Here we initialize Prepar3D, listen for MFD button events,
	*              store button states, and if a button state changes, tells the
	*              ScreenManager to update the screen.
	*/
	public class Main extends MovieClip {
		
		private var initialized:Boolean;
		
		// These properties capture the current state of each button from Prepar3D
		private var knob1_state:Boolean = false;
		private var top_square_btn1_state:Boolean = false;
		private var top_square_btn2_state:Boolean = false;
		private var top_square_btn3_state:Boolean = false;
		private var top_square_btn4_state:Boolean = false;
		private var top_square_btn5_state:Boolean = false;
		private var top_square_btn6_state:Boolean = false;
		private var brt_up_btn_state:Boolean = false;
		private var brt_down_btn_state:Boolean = false;
		private var right_square_btn1_state:Boolean = false;
		private var right_square_btn2_state:Boolean = false;
		private var right_square_btn3_state:Boolean = false;
		private var right_square_btn4_state:Boolean = false;
		private var right_square_btn5_state:Boolean = false;
		private var right_square_btn6_state:Boolean = false;
		private var cont_up_btn_state:Boolean = false;
		private var cont_down_btn_state:Boolean = false;
		private var bottom_square_btn1_state:Boolean = false;
		private var bottom_square_btn2_state:Boolean = false;
		private var bottom_square_btn3_state:Boolean = false;
		private var bottom_square_btn4_state:Boolean = false;
		private var bottom_square_btn5_state:Boolean = false;
		private var bottom_square_btn6_state:Boolean = false;
		private var lum_down_btn_state:Boolean = false;
		private var lum_up_btn_state:Boolean = false;
		private var left_square_btn1_state:Boolean = false;
		private var left_square_btn2_state:Boolean = false;
		private var left_square_btn3_state:Boolean = false;
		private var left_square_btn4_state:Boolean = false;
		private var left_square_btn5_state:Boolean = false;
		private var left_square_btn6_state:Boolean = false;
		
		// These properties store the current state of each button from Prepar3D
		private var knob1_store:Boolean;
		private var top_square_btn1_store:Boolean;
		private var top_square_btn2_store:Boolean;
		private var top_square_btn3_store:Boolean;
		private var top_square_btn4_store:Boolean;
		private var top_square_btn5_store:Boolean;
		private var top_square_btn6_store:Boolean;
		private var brt_up_btn_store:Boolean;
		private var brt_down_btn_store:Boolean;
		private var right_square_btn1_store:Boolean;
		private var right_square_btn2_store:Boolean;
		private var right_square_btn3_store:Boolean;
		private var right_square_btn4_store:Boolean;
		private var right_square_btn5_store:Boolean;
		private var right_square_btn6_store:Boolean;
		private var cont_up_btn_store:Boolean;
		private var cont_down_btn_store:Boolean;
		private var bottom_square_btn1_store:Boolean;
		private var bottom_square_btn2_store:Boolean;
		private var bottom_square_btn3_store:Boolean;
		private var bottom_square_btn4_store:Boolean;
		private var bottom_square_btn5_store:Boolean;
		private var bottom_square_btn6_store:Boolean;
		private var lum_down_btn_store:Boolean;
		private var lum_up_btn_store:Boolean;
		private var left_square_btn1_store:Boolean;
		private var left_square_btn2_store:Boolean;
		private var left_square_btn3_store:Boolean;
		private var left_square_btn4_store:Boolean;
		private var left_square_btn5_store:Boolean;
		private var left_square_btn6_store:Boolean;
		
		private var _this = this;
		
		private var currentDisplay:String;
		private var buttonSelected:String;
		private var buttonState:Boolean;
		private var buttonChanged:Boolean;
		
		/**
		* constructor
		*/
		public function Main() {
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
			this.addEventListener(Event.ENTER_FRAME, mfdScreenManager.handleUpdateData);
			
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
			knob1_state = Prepar3D.VarGet("L:mfd_knob1", "bool");
			top_square_btn1_state = Prepar3D.VarGet("L:mfd_top_square_btn1", "bool");
			top_square_btn2_state = Prepar3D.VarGet("L:mfd_top_square_btn2", "bool");
			top_square_btn3_state = Prepar3D.VarGet("L:mfd_top_square_btn3", "bool");
			top_square_btn4_state = Prepar3D.VarGet("L:mfd_top_square_btn4", "bool");
			top_square_btn5_state = Prepar3D.VarGet("L:mfd_top_square_btn5", "bool");
			top_square_btn6_state = Prepar3D.VarGet("L:mfd_top_square_btn6", "bool");
			brt_up_btn_state = Prepar3D.VarGet("L:mfd_brt_up_btn", "bool");
			brt_down_btn_state = Prepar3D.VarGet("L:mfd_brt_down_btn", "bool");
			right_square_btn1_state = Prepar3D.VarGet("L:mfd_right_square_btn1", "bool");
			right_square_btn2_state = Prepar3D.VarGet("L:mfd_right_square_btn2", "bool");
			right_square_btn3_state = Prepar3D.VarGet("L:mfd_right_square_btn3", "bool");
			right_square_btn4_state = Prepar3D.VarGet("L:mfd_right_square_btn4", "bool");
			right_square_btn5_state = Prepar3D.VarGet("L:mfd_right_square_btn5", "bool");
			right_square_btn6_state = Prepar3D.VarGet("L:mfd_right_square_btn6", "bool");
			cont_up_btn_state = Prepar3D.VarGet("L:mfd_cont_up_btn", "bool");
			cont_down_btn_state = Prepar3D.VarGet("L:mfd_cont_down_btn", "bool");
			bottom_square_btn1_state = Prepar3D.VarGet("L:mfd_bottom_square_btn1", "bool");
			bottom_square_btn2_state = Prepar3D.VarGet("L:mfd_bottom_square_btn2", "bool");
			bottom_square_btn3_state = Prepar3D.VarGet("L:mfd_bottom_square_btn3", "bool");
			bottom_square_btn4_state = Prepar3D.VarGet("L:mfd_bottom_square_btn4", "bool");
			bottom_square_btn5_state = Prepar3D.VarGet("L:mfd_bottom_square_btn5", "bool");
			bottom_square_btn6_state = Prepar3D.VarGet("L:mfd_bottom_square_btn6", "bool");
			lum_down_btn_state = Prepar3D.VarGet("L:mfd_lum_down_btn", "bool");
			lum_up_btn_state = Prepar3D.VarGet("L:mfd_lum_up_btn", "bool");
			left_square_btn1_state = Prepar3D.VarGet("L:mfd_left_square_btn1", "bool");
			left_square_btn2_state = Prepar3D.VarGet("L:mfd_left_square_btn2", "bool");
			left_square_btn3_state = Prepar3D.VarGet("L:mfd_left_square_btn3", "bool");
			left_square_btn4_state = Prepar3D.VarGet("L:mfd_left_square_btn4", "bool");
			left_square_btn5_state = Prepar3D.VarGet("L:mfd_left_square_btn5", "bool");
			left_square_btn6_state = Prepar3D.VarGet("L:mfd_left_square_btn6", "bool");
			
			buttonChanged = buttonStatesChange();
			if (buttonChanged == true)
			{
				mfdScreenManager.updateScreen(buttonSelected,buttonState);
				
				storeCurrentStates();
			}
			
		} // end of function
		
		/**
		* Description: This method is used to store the current state of all the buttons.
		*           
		*/
		private function storeCurrentStates():void
		{
			knob1_store = knob1_state;
			top_square_btn1_store = top_square_btn1_state;
			top_square_btn2_store = top_square_btn2_state;
			top_square_btn3_store = top_square_btn3_state;
			top_square_btn4_store = top_square_btn4_state;
			top_square_btn5_store = top_square_btn5_state;
			top_square_btn6_store = top_square_btn6_state;
			brt_up_btn_store = brt_up_btn_state;
			brt_down_btn_store = brt_down_btn_state;
			right_square_btn1_store = right_square_btn1_state;
			right_square_btn2_store = right_square_btn2_state;
			right_square_btn3_store = right_square_btn3_state;
			right_square_btn4_store = right_square_btn4_state;
			right_square_btn5_store = right_square_btn5_state;
			right_square_btn6_store = right_square_btn6_state;
			cont_up_btn_store = cont_up_btn_state;
			cont_down_btn_store = cont_down_btn_state;
			bottom_square_btn1_store = bottom_square_btn1_state;
			bottom_square_btn2_store = bottom_square_btn2_state;
			bottom_square_btn3_store = bottom_square_btn3_state;
			bottom_square_btn4_store = bottom_square_btn4_state;
			bottom_square_btn5_store = bottom_square_btn5_state;
			bottom_square_btn6_store = bottom_square_btn6_state;
			lum_down_btn_store = lum_down_btn_state;
			lum_up_btn_store = lum_up_btn_state;
			left_square_btn1_store = left_square_btn1_state;
			left_square_btn2_store = left_square_btn2_state;
			left_square_btn3_store = left_square_btn3_state;
			left_square_btn4_store = left_square_btn4_state;
			left_square_btn5_store = left_square_btn5_state;
			left_square_btn6_store = left_square_btn6_state;
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
			
			if (knob1_store != knob1_state)
			{			
				change = true;
				buttonSelected = "knob1";
				buttonState = knob1_state;
			}
			else if (top_square_btn1_store != top_square_btn1_state)
			{
				change = true;
				buttonSelected = "top_square_btn1";
				buttonState = top_square_btn1_state;
			}
			else if (top_square_btn2_store != top_square_btn2_state)
			{
				change = true;
				buttonSelected = "top_square_btn2";
				buttonState = top_square_btn2_state;
			}
			else if (top_square_btn3_store != top_square_btn3_state)
			{
				change = true;
				buttonSelected = "top_square_btn3";
				buttonState = top_square_btn3_state;
			}
			else if (top_square_btn4_store != top_square_btn4_state)
			{
				change = true;
				buttonSelected = "top_square_btn4";
				buttonState = top_square_btn4_state;
			}
			else if (top_square_btn5_store != top_square_btn5_state)
			{
				change = true;
				buttonSelected = "top_square_btn5";
				buttonState = top_square_btn5_state;
			}
			else if (top_square_btn6_store != top_square_btn6_state)
			{
				change = true;
				buttonSelected = "top_square_btn6";
				buttonState = top_square_btn6_state;
			}
			else if (brt_up_btn_store != brt_up_btn_state)
			{
				change = true;
				buttonSelected = "brt_up_btn";
				buttonState = brt_up_btn_state;
			}
			else if (brt_down_btn_store != brt_down_btn_state)
			{
				change = true;
				buttonSelected = "brt_down_btn";
				buttonState = brt_down_btn_state;
			}
			else if (right_square_btn1_store != right_square_btn1_state)
			{
				change = true;
				buttonSelected = "right_square_btn1";
				buttonState = right_square_btn1_state;
			}
			else if (right_square_btn2_store != right_square_btn2_state)
			{
				change = true;
				buttonSelected = "right_square_btn2";
				buttonState = right_square_btn2_state;
			}
			else if (right_square_btn3_store != right_square_btn3_state)
			{
				change = true;
				buttonSelected = "right_square_btn3";
				buttonState = right_square_btn3_state;
			}
			else if (right_square_btn4_store != right_square_btn4_state)
			{
				change = true;
				buttonSelected = "right_square_btn4";
				buttonState = right_square_btn4_state;
			}
			else if (right_square_btn5_store != right_square_btn5_state)
			{
				change = true;
				buttonSelected = "right_square_btn5";
				buttonState = right_square_btn5_state;
			}
			else if (right_square_btn6_store != right_square_btn6_state)
			{
				change = true;
				buttonSelected = "right_square_btn6";
				buttonState = right_square_btn6_state;
			}
			else if (cont_up_btn_store != cont_up_btn_state)
			{
				change = true;
				buttonSelected = "cont_up_btn";
				buttonState = cont_up_btn_state;
			}
			else if (cont_down_btn_store != cont_down_btn_state)
			{
				change = true;
				buttonSelected = "cont_down_btn";
				buttonState = cont_down_btn_state;
			}
			else if (bottom_square_btn1_store != bottom_square_btn1_state)
			{
				change = true;
				buttonSelected = "bottom_square_btn1";
				buttonState = bottom_square_btn1_state;
			}
			else if (bottom_square_btn2_store != bottom_square_btn2_state)
			{
				change = true;
				buttonSelected = "bottom_square_btn2";
				buttonState = bottom_square_btn2_state;
			}
			else if (bottom_square_btn3_store != bottom_square_btn3_state)
			{
				change = true;
				buttonSelected = "bottom_square_btn3";
				buttonState = bottom_square_btn3_state;
			}
			else if (bottom_square_btn4_store != bottom_square_btn4_state)
			{
				change = true;
				buttonSelected = "bottom_square_btn4";
				buttonState = bottom_square_btn4_state;
			}
			else if (bottom_square_btn5_store != bottom_square_btn5_state)
			{
				change = true;
				buttonSelected = "bottom_square_btn5";
				buttonState = bottom_square_btn5_state;
			}
			else if (bottom_square_btn6_store != bottom_square_btn6_state)
			{
				change = true;
				buttonSelected = "bottom_square_btn6";
				buttonState = bottom_square_btn6_state;
			}
			else if (lum_down_btn_store != lum_down_btn_state)
			{
				change = true;
				buttonSelected = "lum_down_btn";
				buttonState = lum_down_btn_state;
			}
			else if (lum_up_btn_store != lum_up_btn_state)
			{
				change = true;
				buttonSelected = "lum_up_btn";
				buttonState = lum_up_btn_state;
			}
			else if (left_square_btn1_store != left_square_btn1_state)
			{
				change = true;
				buttonSelected = "left_square_btn1";
				buttonState = left_square_btn1_state;
			}
			else if (left_square_btn2_store != left_square_btn2_state)
			{
				change = true;
				buttonSelected = "left_square_btn2";
				buttonState = left_square_btn2_state;
			}
			else if (left_square_btn3_store != left_square_btn3_state)
			{
				change = true;
				buttonSelected = "left_square_btn3";
				buttonState = left_square_btn3_state;
			}
			else if (left_square_btn4_store != left_square_btn4_state)
			{
				change = true;
				buttonSelected = "left_square_btn4";
				buttonState = left_square_btn4_state;
			}
			else if (left_square_btn5_store != left_square_btn5_state)
			{
				change = true;
				buttonSelected = "left_square_btn5";
				buttonState = left_square_btn5_state;
			}
			else if (left_square_btn6_store != left_square_btn6_state)
			{
				change = true;
				buttonSelected = "left_square_btn6";
				buttonState = left_square_btn6_state;
			}
			
			
			return change;
		}
		
	}	// end of class
}  // end of package
