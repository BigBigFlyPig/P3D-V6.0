package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class BaroAltimeterScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var pressureAltitudeMeters:int;
		private var pressureAltitudeFeet:int;
		
		private var feet_minor_Y:Number;
		
		public function BaroAltimeterScreen() {
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
			//
			pressureAltitudeMeters = Math.round(Prepar3D.VarGet("A:PRESSURE ALTITUDE", "meters"));
			pressureAltitudeFeet = Math.round(pressureAltitudeMeters * 3.2808);
			
			_this.needle.rotation = calculateRotation(pressureAltitudeFeet);
			_this.readout.feet_major.text = String(int(pressureAltitudeFeet/100));
			var feet_minor_number:int = pressureAltitudeFeet - (int(pressureAltitudeFeet/100) * 100);
			_this.readout.feet_minor.y = feet_minor_number * 0.68 + 1;
		}
		
		private function calculateRotation(ft_val:int):int
		{
			var rot_val:int;
			
			rot_val = Math.round(ft_val * 0.36);
			
			return rot_val;
		}
		
	}
}
