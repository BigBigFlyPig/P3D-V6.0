package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class TurbineScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var _turbineSpeed1:int;
		private var _turbineSpeed2:int;
		private var _rotorSpeed:int;
		
		public function TurbineScreen() {
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
			_turbineSpeed1 = int(Prepar3D.VarGet("A:TURB ENG N1:1", "percent"));
			_turbineSpeed2 = int(Prepar3D.VarGet("A:TURB ENG N2:1", "percent"));
			_rotorSpeed = int(Prepar3D.VarGet("A:ROTOR RPM PCT:1", "percent"));
			
			//display to screen
			_this.topNumber.text = String(_rotorSpeed);
			_this.leftNumber.text = String(_turbineSpeed1);
			_this.rightNumber.text = String(_turbineSpeed2);
			
			_this.turbineSpeed1.height = int(160 * (_turbineSpeed1/100));
			_this.turbineSpeed2.height = int(160 * (_turbineSpeed2/100));
			_this.rotorSpeed.height = int(150 * (_rotorSpeed/100));
			
			// USE FOR DEBUG
			//MovieClip(parent).setDebugTextDisplay("plane pitch = " + _semiCircle_degrees);
		}
	}
	
}
