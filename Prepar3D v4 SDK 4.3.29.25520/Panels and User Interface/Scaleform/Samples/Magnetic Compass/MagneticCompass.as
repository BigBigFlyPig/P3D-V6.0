package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;	

	public class MagneticCompass extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;

	
		public function MagneticCompass() {
			
			// constructor code
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
		
		public function update(e:Event):void
		{
			// GET FROM PREPAR3D AIRCRAFT VARS
			
			// Magnetic Heading
			var _magHeading:Number = Prepar3D.VarGet("A:WISKEY COMPASS INDICATION DEGREES", "degrees");

			_this.magHeading.x = _magHeading * (840 / 360) + 300;
		}		
		
	}
}