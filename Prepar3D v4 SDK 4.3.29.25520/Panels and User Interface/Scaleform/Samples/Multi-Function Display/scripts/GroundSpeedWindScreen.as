package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class GroundSpeedWindScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var _groundSpeed:Number;
		private var _windSpeed:Number;
		private var _windDirection:String;
		
		public function GroundSpeedWindScreen() {
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
			_groundSpeed = Prepar3D.VarGet("A:GPS GROUND SPEED", "meters per second");
			//_groundSpeed = Prepar3D.VarGet("A:GROUND VELOCITY", "knots");
			_windSpeed = Prepar3D.VarGet("A:AMBIENT WIND VELOCITY", "knots");
			_windDirection = Prepar3D.VarGet("A:AMBIENT WIND DIRECTION", "degrees");
			
			//display to screen
			_this.groundSpeed.text = String(_groundSpeed.toFixed(0));
			_this.windSpeed.text = String(_windSpeed.toFixed(0));
			_this.windDirection.rotation = _windDirection;
		}
		
	}
	
}
