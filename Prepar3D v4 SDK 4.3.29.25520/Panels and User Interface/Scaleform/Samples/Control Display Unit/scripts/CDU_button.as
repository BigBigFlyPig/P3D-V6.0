package  {

	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	import flash.events.*;
	import CDU_Btn_Event;
	
	public class CDU_button extends SimpleButton {
		
		// properties
		private var buttonName:String = "";
		
		var _this = this;
		
		// constructor
		public function CDU_button() {
			
			// show the hand cursor
			//this.buttonMode = true;
			//this.mouseChildren = false;

			buttonName = _this.name;
			
			// create mouse listeners
			_this.addEventListener(MouseEvent.CLICK, onCduButtonClick);
		}
		
		private function onCduButtonClick(e:MouseEvent):void
		{	
			// send to Display Manager event that this VAB was clicked
			dispatchEvent(new CDU_Btn_Event(CDU_Btn_Event.CDU_BTN_SELECTED, true, false, buttonName));
		}
	}
	
}
