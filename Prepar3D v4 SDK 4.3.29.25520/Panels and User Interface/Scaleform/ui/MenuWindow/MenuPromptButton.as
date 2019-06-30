package
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.text.TextFieldAutoSize;
	
	public class MenuPromptButton extends Sprite
	{
		private var index;
		private var disabled:Boolean = false;
		
		public var shiftUpOnPress:Boolean = false;
		public var shiftDownOnPress:Boolean = false;
		
		public function MenuPromptButton( index: uint )
		{
			this.index = index;
			buttonMode = true;
			this.addEventListener(MouseEvent.MOUSE_UP, handleRelease);
			this.addEventListener(MouseEvent.MOUSE_DOWN, handlePress);
			this.addEventListener(MouseEvent.MOUSE_OVER, handleRollOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, handleRollOut);
		}

		public function SetText(text: String, pad:uint)
		{
			buttonText.text = text;
			buttonText.autoSize = TextFieldAutoSize.LEFT;
			textBG.width = width;

			buttonText.multiline = true;
			buttonText.wordWrap = true;

			textBG.height = buttonText.height;
			
			buttonText.width = width;
			
			if (text == "")
			{
				visible = false;
			}
			else
			{
				visible = true;
			}
		}

		function handleRelease( e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 1.0;
				if (!shiftDownOnPress && !shiftUpOnPress)
				{
					MovieClip( root ).SetOption(index);
					MenuWindow.buttonModifierY = 0;
				}
				else if (shiftUpOnPress)
				{
					MenuWindow.buttonModifierY--;
					shiftUpOnPress = false;
				}
				else if (shiftDownOnPress)
				{
					MenuWindow.buttonModifierY++;
					shiftDownOnPress = false;
				}
			}
		}
		
		function handlePress(e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 0.5;
			}
		}
		
		function handleRollOver(e: MouseEvent): void
		{
			if (!disabled)
			{
				textBG.alpha = 0.8;
			}
		}
		
		function handleRollOut(e: MouseEvent): void
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
	}
}