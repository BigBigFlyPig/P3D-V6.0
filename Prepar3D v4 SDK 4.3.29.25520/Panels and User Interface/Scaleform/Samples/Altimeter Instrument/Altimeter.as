package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class Altimeter extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var pressureAltitudeMeters:int;
		private var pressureAltitudeFeet:int;
		
		private var feet_minor_Y:Number;
		
		public function Altimeter() {
			// Only initialize once
			if (initialized == false)
			{
				Init();
				initialized = true;
			}
		}
		
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
			// set any initial values

			// add update event
			stage.addEventListener(Event.ENTER_FRAME, update);
		}
				
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function update(e:Event):void
		{
			//
			pressureAltitudeMeters = Math.round(Prepar3D.VarGet("A:PRESSURE ALTITUDE", "meters"));
			pressureAltitudeFeet = Math.round(pressureAltitudeMeters * 3.2808);
			
			_this.Alt.needle.rotation = calculateRotation(pressureAltitudeFeet);
			_this.Alt.readout.feet_major.text = String(int(pressureAltitudeFeet/100));
			var feet_minor_number:int = pressureAltitudeFeet - (int(pressureAltitudeFeet/100) * 100);
			_this.Alt.readout.feet_minor.y = feet_minor_number * 0.68 + 1;
		}
		
		private function calculateRotation(ft_val:int):int
		{
			var rot_val:int;
			
			rot_val = Math.round(ft_val * 0.36);
			
			return rot_val;
		}
		
	}
}
