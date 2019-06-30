package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;	

	public class VerticalSpeed extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;

	
		public function VerticalSpeed() {
			
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
			
			// Vertical Speed
			var _vert_speed:Number = Prepar3D.VarGet("A:VERTICAL SPEED", "feet per minute") / 100;


			if (_vert_speed > 20)
			{
				_vert_speed = 20;
			}
			if (_vert_speed < -20)
			{
				_vert_speed = -20;
			}
			
			_this.VS_Needle.rotation = _vert_speed * 9;

		}		
		
	}
}