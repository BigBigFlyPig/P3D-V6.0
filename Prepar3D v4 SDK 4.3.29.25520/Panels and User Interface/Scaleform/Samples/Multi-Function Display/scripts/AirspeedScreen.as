package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class AirspeedScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var AirspeedIndicated:int;
		private var AirspeedTrue:int;
		
		private var airspeed_major:int;
		private var airspeed_minor:int;
		
		public function AirspeedScreen() {
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
			AirspeedIndicated = Prepar3D.VarGet("A:AIRSPEED INDICATED", "knots");
			
			//MovieClip(parent).setDebugTextDisplay("AirspeedIndicated = " + AirspeedIndicated);
			
			// rotate needle to correct position
			// values from 0-200; if greater than 200, don't rotate any further
			if (AirspeedIndicated > 200)
			{
				AirspeedIndicated = 200;
			}
			_this.needle.rotation = (Math.round(AirspeedIndicated * 1.5));
			
			
			// digits; cycle right digit and change left digits accordingly
			airspeed_major = Math.floor(AirspeedIndicated/10);
			airspeed_minor = AirspeedIndicated - (airspeed_major * 10);
			
			_this.readout.majorDigit.text = String(airspeed_major);
			
			_this.readout.minorDigitSlide.y = 14.85 * airspeed_minor + 82;
			 
		}
	}
	
}
