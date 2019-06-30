package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class StabilizerIndicator extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var stabAngle:Number;
		private var stabAngleDeg:int;
		
		public function StabilizerIndicator() {
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
			// position: -1.0 full down; 1.0 full up
			stabAngle = Prepar3D.VarGet("A:ELEVATOR POSITION", "position");
			
			stabAngleDeg = ConvertToDeg(stabAngle);
			
			_this.stabNumber.text = String(stabAngleDeg);
			_this.stabShape.rotation = int(_this.stabNumber.text);
		}
		
		private function ConvertToDeg(pos:Number):int
		{
			var deg:int;
			
			if (pos >= 0)
			{
				deg = 0;
			}
			else
			{
				//deg = rad * (180 * Math.PI);
				deg = Math.round(pos * (-90));
			}
			
			return deg;
		}
	}
	
}
