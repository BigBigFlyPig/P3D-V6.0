package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class HSIScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var PlaneHeading:int;
		private var PlaneHeading_String:String;
		
		public function HSIScreen() {
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
			PlaneHeading = int  (Math.floor(Prepar3D.VarGet("A:PLANE HEADING DEGREES GYRO", "degrees")));
			
			if (PlaneHeading > 359)
			{
				PlaneHeading %= 360;
			}
			if (PlaneHeading < 0)
			{
				PlaneHeading = PlaneHeading * -1;
			}
			
			// we need to display the result string as a 3 digit integer
			if (PlaneHeading < 10)
			{
				PlaneHeading_String = "00" + String(PlaneHeading);
			}
			else if (PlaneHeading < 100)
			{
				PlaneHeading_String = "0" + String(PlaneHeading);
			}
			else
			{
				PlaneHeading_String = String(PlaneHeading);
			}
			//
			_this.planeHeading.text = PlaneHeading_String;
			
			_this.compass.rotation = 360 - PlaneHeading;  // need to rotate CCW
			
			//for (var i=1; i<21; i++)
			//{
			//	_this.compassRose["compass_"+i].rotation = PlaneHeading;
			//}
		}
	}
	
}
