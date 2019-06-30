package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class SideVerticalSpeedScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var _side_VSI_needle_angle:int;
		private var _vsi_val:int;
		
		public function SideVerticalSpeedScreen() 
		{
			// constructor code
			// Only initialize once
			if (initialized == false)
			{
				Init();
				initialized = true;
			}
		}
		
		private function Init():void
		{
			//
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function update():void
		{
			_vsi_val = Math.round(Prepar3D.VarGet("A:VERTICAL SPEED", "feet per minute"));
			if (_vsi_val >= 0)
			{
				_this.vsi_up_arrow.visible = true;
				_this.vsi_down_arrow.visible = false;
			}
			else
			{
				_this.vsi_up_arrow.visible = false;
				_this.vsi_down_arrow.visible = true;
			}
			
			// set the needle position
			_side_VSI_needle_angle = speedToDegrees(_vsi_val);
			
			_this.side_VSI_needle.rotation = _side_VSI_needle_angle;
			
			//MovieClip(parent).setDebugTextDisplay("needle angle = " + _side_VSI_needle_angle);
			
			// set the digital readout text
			_this.vsi_readout.text = String(Math.abs(_vsi_val));
			
		}
		
		private function speedToDegrees(sp:int):int
		{
			var deg:int = 0;
			var positiveVal:Boolean = true;
			var spPos:int = Math.abs(sp);
			
			if (sp < 0)
			{
				positiveVal = false;
			}
			else
			{
				positiveVal = true;
			}
			
			if (spPos <= 500)
			{
				deg = Math.round(spPos/12.5);
			}
			else if (spPos <= 1000)
			{
				var tempVal:Number = (spPos-500)/25.0;
				deg = Math.round(tempVal + 40);
			}
			else if (spPos <= 2000)
			{
				var tempVal2:Number = (spPos-1000)/66.667;
				deg = Math.round(tempVal2 + 60);
			}
			else if (spPos <= 3000)
			{
				var tempVal3:Number = (spPos-2000)/200.0;
				deg = Math.round(tempVal3 + 75);
			}
			else if (spPos <= 4000)
			{
				var tempVal4:Number = (spPos-3000)/200.0;
				deg = Math.round(tempVal4 + 80);
			}
			else if (spPos > 4000)
			{
				deg = 88;
			}
			
			// now determine if angle should be negative
			if (positiveVal == false)
			{
				return -deg;
			}
			else
			{
				return deg;
			}
		}
	}
}
