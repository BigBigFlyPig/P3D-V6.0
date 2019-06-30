package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class VerticalSpeedIndicator extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var AttIndicatorBankDegrees:Number;
		private var AttIndicatorPitchDegrees:Number;
		
		private var ladderShiftFactor:Number;
		private var bankOffset:Number;
		
		private var ladderStart_Y:Number;
		private var mfdHorizon_Y:Number;
		
		public function VerticalSpeedIndicator() {
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
			ladderShiftFactor = 6;
			bankOffset = 1; //9/10;
			
			ladderStart_Y = _this.attAssembly.ladder.y;
			mfdHorizon_Y = _this.MFD_Horizon.y;
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function update():void
		{			
			AttIndicatorBankDegrees = Prepar3D.VarGet("A:ATTITUDE INDICATOR BANK DEGREES", "degrees");
			AttIndicatorPitchDegrees = Prepar3D.VarGet("A:ATTITUDE INDICATOR PITCH DEGREES", "degrees");
			
			// minimum = -90; maximum = +90
			if (AttIndicatorPitchDegrees > 90)
			{
				AttIndicatorPitchDegrees = 90;
			}
			else if (AttIndicatorPitchDegrees < -90)
			{
				AttIndicatorPitchDegrees = -90;
			}
			
			// minimum = -90; maximum = +90
			if (AttIndicatorBankDegrees > 90)
			{
				AttIndicatorBankDegrees = 90;
			}
			else if (AttIndicatorBankDegrees < -90)
			{
				AttIndicatorBankDegrees = -90;
			}
			
			//MovieClip(parent).setDebugTextDisplay("AttIndicatorPitchDegrees = " + AttIndicatorPitchDegrees);
			
			// set the rotation of the attitude assembly
			//----------------------------------------------
			_this.attAssembly.rotation = (AttIndicatorBankDegrees) * bankOffset;
			_this.MFD_Horizon.rotation = (AttIndicatorBankDegrees) * bankOffset;
			
			// set the VSI Pitch Indicator
			_this.attAssembly.ladder.y = ladderStart_Y - int(AttIndicatorPitchDegrees * ladderShiftFactor);
			_this.MFD_Horizon.y = mfdHorizon_Y - int(AttIndicatorPitchDegrees * ladderShiftFactor);

		}
	}
	
}
