package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class MSG_1 extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
				
		public function MSG_1() {
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
		
		public function update():void
		{
			//
		}
	}
	
}
