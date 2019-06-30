package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class VorFmsScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var vor_nm:Number;
		private var fms_nm:Number;
		
		public function VorFmsScreen() {
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
			vor_nm = Prepar3D.VarGet("A:NAV DME", "nautical miles");
			fms_nm = Prepar3D.VarGet("A:HSI DISTANCE", "nautical miles");
			
			//display to screen
			_this.VOR_nm.text = String(vor_nm.toFixed(1));
			_this.FMS_nm.text = String(fms_nm.toFixed(1));
		}
		
	}
	
}
