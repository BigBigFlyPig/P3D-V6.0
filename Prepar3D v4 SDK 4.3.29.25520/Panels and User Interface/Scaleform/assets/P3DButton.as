package
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.events.Event;
	
	public class P3DButton extends P3DAsset
	{
		private var disabled:Boolean = false;
		
		public function P3DButton()
		{
			this.addEventListener(MouseEvent.MOUSE_UP, handleRelease);
			this.addEventListener(MouseEvent.MOUSE_DOWN, handlePress);
			this.addEventListener(MouseEvent.MOUSE_OVER, handleMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, handleMouseOut);
		}
		
		public function SetText(text: String)
		{
			buttonText.text = text;
			
			buttonText.multiline = true;
			buttonText.wordWrap = true;

			UpdateSize();
			
			if (text == "")
			{
				visible = false;
			}
			else
			{
				visible = true;
			}
		}
		
		public function UpdateSize()
		{
			textBG.width = buttonText.width;
			textBG.height = buttonText.height;
		}

		function handleRelease( e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 1.0;
			}
		}
		
		function handlePress(e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 0.5;
			}
		}
		
		function handleMouseOver(e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 0.8;
			}
		}
		
		function handleMouseOut(e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 1.0;
			}
		}
		
		public function SetDisabled(dis:Boolean)
		{
			if (disabled != dis)
			{
				if (dis)
				{
					buttonText.alpha = 0.5;
					textBG.alpha = 0.5;
				}
				else
				{
					buttonText.alpha = 1.0;
					textBG.alpha = 1.0;
				}
			}
			disabled = dis;
		}
		
		public function IsDisabled() : Boolean
		{
			return disabled;
		}
	}
}