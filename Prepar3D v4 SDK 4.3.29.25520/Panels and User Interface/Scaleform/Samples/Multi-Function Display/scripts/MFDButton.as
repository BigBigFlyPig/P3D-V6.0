package  {

	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	import flash.events.*;
	import MFDButtonEvent;
	
	public class MFDButton extends SimpleButton {
		
		// properties
		private var buttonName:String = "";
		
		var _this = this;
		
		// constructor
		public function MFDButton() {
			
			// show the hand cursor
			//this.buttonMode = true;
			//this.mouseChildren = false;

			buttonName = _this.name;
			
			// create mouse listeners
			_this.addEventListener(MouseEvent.CLICK, onMfdButtonClick);
		}
		
		private function onMfdButtonClick(e:MouseEvent):void
		{			
			// send to Display Manager event that this VAB was clicked
			dispatchEvent(new MFDButtonEvent(MFDButtonEvent.MFD_BTN_SELECTED, true, false, buttonName));
		}
	}
	
}
