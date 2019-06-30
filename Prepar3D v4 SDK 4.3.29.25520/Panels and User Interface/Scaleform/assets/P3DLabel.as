package
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.events.Event;
	import Prepar3D;
	
	public class P3DLabel extends P3DAsset
	{
		private var dpiScale:Number = 1;
		private var disabled:Boolean = false;
		
		public function P3DLabel()
		{
			dpiScale = Prepar3D.GetScaleformDpiScaling();
			
			this.addEventListener(Event.ENTER_FRAME, HandleUpdate);
		}
		
		public function SetText(text: String, pad:uint)
		{
			labelText.text = text;
			
			labelText.multiline = true;
			labelText.wordWrap = true;
		}
		
		public function UpdateSize()
		{
			labelBG.width = width;
			labelBG.height = height;
		}
		
		public function SetWidth(w:int)
		{
			labelBG.width = w;
		}
		
		public function SetHeight(h:int)
		{
			labelBG.height = h;
		}
		
		function HandleUpdate(e:Event) 
		{
		}
	}
}