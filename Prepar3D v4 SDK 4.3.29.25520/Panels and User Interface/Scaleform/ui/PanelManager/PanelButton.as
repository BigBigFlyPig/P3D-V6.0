package
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.events.Event;
	
	public class PanelLabel extends P3DLabel
	{
		private var panelId;
		
		public function PanelLabel( panelId: uint )
		{
			this.panelId = panelId;
			
			var ch:P3DCheckBox = new P3DCheckBox();
			addChild(ch);
			ch.x = 50;
			ch.y = 50;
		}
	}
}
