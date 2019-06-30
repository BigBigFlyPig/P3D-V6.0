package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class Airspeed extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var AirspeedIndicated:int;
		private var AirspeedTemp:int;
		private var AirspeedTrue:int;
		private var rotationAngle:int;
		private var AdjFactor:Number = 0.75; 
		
		public function Airspeed() {
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
			AirspeedIndicated = Prepar3D.VarGet("A:AIRSPEED INDICATED", "knots");
						
			//MovieClip(parent).setDebugTextDisplay("AirspeedIndicated = " + AirspeedIndicated);
			
			// rotate needle to correct position
			// values from 0-200; if greater than 200, don't rotate any further
			if (AirspeedIndicated > 200) {
				AirspeedIndicated = 200;
			}
			
			if (AirspeedIndicated <= 60) {
				rotationAngle = (Math.round(AirspeedIndicated * AdjFactor));
				_this.needle.rotation = rotationAngle;			
			} else if (AirspeedIndicated > 60) {
				if (AdjFactor < 1.3) {
					AdjFactor = AdjFactor + 0.002
					if (AdjFactor > 1.3) {
						AdjFactor = 1.3
					}
				}
				rotationAngle = (Math.round(AirspeedIndicated * AdjFactor));
				_this.needle.rotation = rotationAngle;
			}
		}
	}
	
}
