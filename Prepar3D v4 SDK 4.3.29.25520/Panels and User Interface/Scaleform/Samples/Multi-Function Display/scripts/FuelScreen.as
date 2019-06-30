package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class FuelScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var fuelTotal:int;
		private var fuelLeft:int;
		private var fuelRight:int;
		
		private var fuelLeftCapacity:int;
		private var fuelRightCapacity:int;
		
		public function FuelScreen() {
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
			fuelLeft = Math.round(Prepar3D.VarGet("A:FUEL LEFT QUANTITY", "gallons"));
			fuelRight = Math.round(Prepar3D.VarGet("A:FUEL RIGHT QUANTITY", "gallons"));
			fuelTotal = Math.round(Prepar3D.VarGet("A:FUEL TOTAL QUANTITY", "gallons"));
			
			fuelLeftCapacity = Math.round(Prepar3D.VarGet("A:FUEL LEFT CAPACITY", "gallons"));
			fuelRightCapacity = Math.round(Prepar3D.VarGet("A:FUEL RIGHT CAPACITY", "gallons"));
			
			// set movieclip objects
			_this.FuelTotal.text = String(fuelTotal);
			var tempRight:int = Math.round(fuelRight/fuelRightCapacity * 100);
			if (tempRight > 100)
			{
				tempRight = 100;
			}
			else if (tempRight == 0)
			{
				tempRight = 3;
			}
			
			var tempLeft:int = Math.round(fuelLeft/fuelLeftCapacity * 100);
			if (tempLeft > 100)
			{
				tempLeft = 100;
			}
			else if (tempLeft == 0)
			{
				tempLeft = 3;
			}
			_this.FuelRight.height = tempRight;
			_this.FuelLeft.height = tempLeft;
			
		}
	}
	
}
