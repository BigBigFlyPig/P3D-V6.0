package  {
	
	/**
	*	Developed by: Mark Greenstone, July 2017
	*   @author Mark Greenstone
	*   Company: Lockheed Martin, RMS
	*/
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.net.URLRequest;
	import flash.display.Loader;
	import MFDButtonEvent;
	
	/**
	* Class: main2D
	* Description: This is the main/document class for the MFD Sample.
	*			   Here we initialize Prepar3D, load the MFD Display SWF, and 
	*              listen for/handle the MFD button events. 
	*/
	public class Main2D extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var mfdMode:Number;
		
		// button toggle state variables to store current state of buttons
		private var knob1_state:Number = 0;
		private var top_square_btn1_state:Number = 0;
		private var top_square_btn2_state:Number = 0;
		private var top_square_btn3_state:Number = 0;
		private var top_square_btn4_state:Number = 0;
		private var top_square_btn5_state:Number = 0;
		private var top_square_btn6_state:Number = 0;
		private var brt_up_btn_state:Number = 0;
		private var brt_down_btn_state:Number = 0;
		private var right_square_btn1_state:Number = 0;
		private var right_square_btn2_state:Number = 0;
		private var right_square_btn3_state:Number = 0;
		private var right_square_btn4_state:Number = 0;
		private var right_square_btn5_state:Number = 0;
		private var right_square_btn6_state:Number = 0;
		private var cont_up_btn_state:Number = 0;
		private var cont_down_btn_state:Number = 0;
		private var bottom_square_btn1_state:Number = 0;
		private var bottom_square_btn2_state:Number = 0;
		private var bottom_square_btn3_state:Number = 0;
		private var bottom_square_btn4_state:Number = 0;
		private var bottom_square_btn5_state:Number = 0;
		private var bottom_square_btn6_state:Number = 0;
		private var lum_down_btn_state:Number = 0;
		private var lum_up_btn_state:Number = 0;
		private var left_square_btn1_state:Number = 0;
		private var left_square_btn2_state:Number = 0;
		private var left_square_btn3_state:Number = 0;
		private var left_square_btn4_state:Number = 0;
		private var left_square_btn5_state:Number = 0;
		private var left_square_btn6_state:Number = 0;
		
		/**
		* constructor
		*/
		public function Main2D() {
			// constructor code
			
			// Only initialize once
			if (initialized == false)
			{
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
		private function Init():void
		{	
			
			// Initialize the Prepar3D Interface
			try{
				Prepar3D.Init();
			}
			catch(e:Error)
			{
				trace("Prepar3D init failed.");
			}
			
			// Here we are loading the SWF for the MFD display.
			// The display is in a separate SWF so that it can also be attached to a texture
			//    in the aircraft model. In that case, the display listens for buttons events from
			//    3D environment aircraft.
			var mfd_display:URLRequest = new URLRequest("Multi-FunctionDisplay.swf");
			var loader:Loader = new Loader();
			loader.load(mfd_display);
			_this.displayContainer.addChild(loader);

			mfdMode = 0;
			
			// Finally add a listener to listen for when a button is clicked
			this.addEventListener(MFDButtonEvent.MFD_BTN_SELECTED, MFD_buttonClick);
		}
		
		
		/**
		* Function: MFD_buttonClick
		* Description: Handler for when an MFD button is clicked.
		* @param e:MFDButtonEvent
		* @return none
		*/
		private function MFD_buttonClick(e:MFDButtonEvent):void
		{
			var buttonPushed:String;
			buttonPushed = e.btn_name;
			trace("buttonName = " + buttonPushed); 
			
			// The Local Prepar3D variables below are only example variables for the Sample.
			// Replace these variable names with ones from the actual MFD model.mdf file.
			switch(buttonPushed)
			{
				case "knob1":
					if (knob1_state == 0)
					{
						knob1_state = 1;
					}
					else
					{
						knob1_state = 0;
					}
					Prepar3D.VarSet("L:mfd_knob1", "bool", knob1_state);
				break;
				
				case "top_square_btn1":
					if (top_square_btn1_state == 0)
					{
						top_square_btn1_state = 1;
					}
					else
					{
						top_square_btn1_state = 0;
					}
					Prepar3D.VarSet("L:mfd_top_square_btn1", "bool", top_square_btn1_state);
				break;
				
				case "top_square_btn2":
					if (top_square_btn2_state == 0)
					{
						top_square_btn2_state = 1;
					}
					else
					{
						top_square_btn2_state = 0;
					}
					Prepar3D.VarSet("L:mfd_top_square_btn2", "bool", top_square_btn2_state);
				break;
				
				case "top_square_btn3":
					if (top_square_btn3_state == 0)
					{
						top_square_btn3_state = 1;
					}
					else
					{
						top_square_btn3_state = 0;
					}
					Prepar3D.VarSet("L:mfd_top_square_btn3", "bool", top_square_btn3_state);
				break;
				
				case "top_square_btn4":
					if (top_square_btn4_state == 0)
					{
						top_square_btn4_state = 1;
					}
					else
					{
						top_square_btn4_state = 0;
					}
					Prepar3D.VarSet("L:mfd_top_square_btn4", "bool", top_square_btn4_state);
				break;
				
				case "top_square_btn5":
					if (top_square_btn5_state == 0)
					{
						top_square_btn5_state = 1;
					}
					else
					{
						top_square_btn5_state = 0;
					}
					Prepar3D.VarSet("L:mfd_top_square_btn5", "bool", top_square_btn5_state);
				break;
				
				case "top_square_btn6":
					if (top_square_btn6_state == 0)
					{
						top_square_btn6_state = 1;
					}
					else
					{
						top_square_btn6_state = 0;
					}
					Prepar3D.VarSet("L:mfd_top_square_btn6", "bool", top_square_btn6_state);
				break;
				
				case "brt_up_btn":
					if (brt_up_btn_state == 0)
					{
						brt_up_btn_state = 1;
					}
					else
					{
						brt_up_btn_state = 0;
					}
					Prepar3D.VarSet("L:mfd_brt_up_btn", "bool", brt_up_btn_state);
				break;
				
				case "brt_down_btn":
					if (brt_down_btn_state == 0)
					{
						brt_down_btn_state = 1;
					}
					else
					{
						brt_down_btn_state = 0;
					}
					Prepar3D.VarSet("L:mfd_brt_down_btn", "bool", brt_down_btn_state);
				break;
				
				case "right_square_btn1":
					if (right_square_btn1_state == 0)
					{
						right_square_btn1_state = 1;
					}
					else
					{
						right_square_btn1_state = 0;
					}
					Prepar3D.VarSet("L:mfd_right_square_btn1", "bool", right_square_btn1_state);
				break;
				
				case "right_square_btn2":
					if (right_square_btn2_state == 0)
					{
						right_square_btn2_state = 1;
					}
					else
					{
						right_square_btn2_state = 0;
					}
					Prepar3D.VarSet("L:mfd_right_square_btn2", "bool", right_square_btn2_state);
				break;
				
				case "right_square_btn3":
					if (right_square_btn3_state == 0)
					{
						right_square_btn3_state = 1;
					}
					else
					{
						right_square_btn3_state = 0;
					}
					Prepar3D.VarSet("L:mfd_right_square_btn3", "bool", right_square_btn3_state);
				break;
				
				case "right_square_btn4":
					if (right_square_btn4_state == 0)
					{
						right_square_btn4_state = 1;
					}
					else
					{
						right_square_btn4_state = 0;
					}
					Prepar3D.VarSet("L:mfd_right_square_btn4", "bool", right_square_btn4_state);
				break;
				
				case "right_square_btn5":
					if (right_square_btn5_state == 0)
					{
						right_square_btn5_state = 1;
					}
					else
					{
						right_square_btn5_state = 0;
					}
					Prepar3D.VarSet("L:mfd_right_square_btn5", "bool", right_square_btn5_state);
				break;
				
				case "right_square_btn6":
					if (right_square_btn6_state == 0)
					{
						right_square_btn6_state = 1;
					}
					else
					{
						right_square_btn6_state = 0;
					}
					Prepar3D.VarSet("L:mfd_right_square_btn6", "bool", right_square_btn6_state);
				break;
				
				case "cont_up_btn":
					if (cont_up_btn_state == 0)
					{
						cont_up_btn_state = 1;
					}
					else
					{
						cont_up_btn_state = 0;
					}
					Prepar3D.VarSet("L:mfd_cont_up_btn", "bool", cont_up_btn_state);
				break;
				
				case "cont_down_btn":
					if (cont_down_btn_state == 0)
					{
						cont_down_btn_state = 1;
					}
					else
					{
						cont_down_btn_state = 0;
					}
					Prepar3D.VarSet("L:mfd_cont_down_btn", "bool", cont_down_btn_state);
				break;
				
				case "bottom_square_btn1":
					if (bottom_square_btn1_state == 0)
					{
						bottom_square_btn1_state = 1;
					}
					else
					{
						bottom_square_btn1_state = 0;
					}
					Prepar3D.VarSet("L:mfd_bottom_square_btn1", "bool", bottom_square_btn1_state);
				break;
				
				case "bottom_square_btn2":
					if (bottom_square_btn2_state == 0)
					{
						bottom_square_btn2_state = 1;
					}
					else
					{
						bottom_square_btn2_state = 0;
					}
					Prepar3D.VarSet("L:mfd_bottom_square_btn2", "bool", bottom_square_btn2_state);
				break;
				
				case "bottom_square_btn3":
					if (bottom_square_btn3_state == 0)
					{
						bottom_square_btn3_state = 1;
					}
					else
					{
						bottom_square_btn3_state = 0;
					}
					Prepar3D.VarSet("L:mfd_bottom_square_btn3", "bool", bottom_square_btn3_state);
				break;
				
				case "bottom_square_btn4":
					if (bottom_square_btn4_state == 0)
					{
						bottom_square_btn4_state = 1;
					}
					else
					{
						bottom_square_btn4_state = 0;
					}
					Prepar3D.VarSet("L:mfd_bottom_square_btn4", "bool", bottom_square_btn4_state);
				break;
				
				case "bottom_square_btn5":
					if (bottom_square_btn5_state == 0)
					{
						bottom_square_btn5_state = 1;
					}
					else
					{
						bottom_square_btn5_state = 0;
					}
					Prepar3D.VarSet("L:mfd_bottom_square_btn5", "bool", bottom_square_btn5_state);
				break;
				
				case "bottom_square_btn6":
					if (bottom_square_btn6_state == 0)
					{
						bottom_square_btn6_state = 1;
					}
					else
					{
						bottom_square_btn6_state = 0;
					}
					Prepar3D.VarSet("L:mfd_bottom_square_btn6", "bool", bottom_square_btn6_state);
				break;
				
				case "lum_down_btn":
					if (lum_down_btn_state == 0)
					{
						lum_down_btn_state = 1;
					}
					else
					{
						lum_down_btn_state = 0;
					}
					Prepar3D.VarSet("L:mfd_lum_down_btn", "bool", lum_down_btn_state);
				break;
				
				case "lum_up_btn":
					if (lum_up_btn_state == 0)
					{
						lum_up_btn_state = 1;
					}
					else
					{
						lum_up_btn_state = 0;
					}
					Prepar3D.VarSet("L:mfd_lum_up_btn", "bool", lum_up_btn_state);
				break;
				
				case "left_square_btn1":
					if (left_square_btn1_state == 0)
					{
						left_square_btn1_state = 1;
					}
					else
					{
						left_square_btn1_state = 0;
					}
					Prepar3D.VarSet("L:mfd_left_square_btn1", "bool", left_square_btn1_state);
				break;
				
				case "left_square_btn2":
					if (left_square_btn2_state == 0)
					{
						left_square_btn2_state = 1;
					}
					else
					{
						left_square_btn2_state = 0;
					}
					Prepar3D.VarSet("L:mfd_left_square_btn2", "bool", left_square_btn2_state);
				break;
				
				case "left_square_btn3":
					if (left_square_btn3_state == 0)
					{
						left_square_btn3_state = 1;
					}
					else
					{
						left_square_btn3_state = 0;
					}
					Prepar3D.VarSet("L:mfd_left_square_btn3", "bool", left_square_btn3_state);
				break;
				
				case "left_square_btn4":
					if (left_square_btn4_state == 0)
					{
						left_square_btn4_state = 1;
					}
					else
					{
						left_square_btn4_state = 0;
					}
					Prepar3D.VarSet("L:mfd_left_square_btn4", "bool", left_square_btn4_state);
				break;
				
				case "left_square_btn5":
					if (left_square_btn5_state == 0)
					{
						left_square_btn5_state = 1;
					}
					else
					{
						left_square_btn5_state = 0;
					}
					Prepar3D.VarSet("L:mfd_left_square_btn5", "bool", left_square_btn5_state);
				break;
				
				case "left_square_btn6":
					if (left_square_btn6_state == 0)
					{
						left_square_btn6_state = 1;
					}
					else
					{
						left_square_btn6_state = 0;
					}
					Prepar3D.VarSet("L:mfd_left_square_btn6", "bool", left_square_btn6_state);
				break;
				
			}
				
		}
	}
	
}
