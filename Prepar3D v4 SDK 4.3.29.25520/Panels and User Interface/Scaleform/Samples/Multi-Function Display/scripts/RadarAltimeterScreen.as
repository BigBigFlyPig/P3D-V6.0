package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class RadarAltimeterScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var radarAltitude:int;
		
		private var hi_value:int = 200;
		private var lo_value:int = 50;
		
		public function RadarAltimeterScreen() {
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
			radarAltitude = Math.round(Prepar3D.VarGet("A:RADIO HEIGHT", "feet"));
			//MovieClip(parent).setDebugTextDisplay("radarAltitude = " + radarAltitude);
			
			_this.HI_text.text = String(hi_value);
			_this.LO_text.text = String(lo_value);
			_this.RA_text.text = String(radarAltitude);
			
			// set needle position
			_this.ra_needle.rotation = calculateRotation(radarAltitude);
			
			_this.HI_mark.rotation = calculateRotation(hi_value) - 180;
			_this.LO_mark.rotation = calculateRotation(lo_value) - 180;
			
		}
		
		private function calculateRotation(ralt:int):int
		{
			var rot_val:int;
			var seg1:int;
			var seg2:int;
			var seg3:int;
			
			seg1 = Math.round(ralt * 1.8);
			seg2 = 180 + Math.round((ralt - 100) * 0.3);
			seg3 = 270 + Math.round((ralt - 400) * 0.075);
			
			if (ralt <= 100)
			{
				rot_val = seg1;
			}
			else if (ralt <= 400)
			{
				rot_val = seg2;
			}
			else if (ralt <= 1000)
			{
				rot_val = seg3;
			}
			else
			{
				rot_val = 315;
			}
			
			return rot_val;
		}
	}
	
}
