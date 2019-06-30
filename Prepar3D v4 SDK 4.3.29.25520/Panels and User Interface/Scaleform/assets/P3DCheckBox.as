package
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
		
	public class P3DCheckBox extends P3DAsset
	{
		private var disabled:Boolean = false;
		private var checked:Boolean = false;
		
		public function P3DCheckBox()
		{
			this.addEventListener(MouseEvent.MOUSE_UP, handleRelease);
			this.addEventListener(MouseEvent.MOUSE_DOWN, handlePress);
			this.addEventListener(MouseEvent.MOUSE_OVER, handleMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, handleMouseOut);
			check.visible = checked;
		}
		
		function handleRelease( e: MouseEvent): void
		{
			if (!disabled)
			{
				checked = !checked;
				check.visible = checked;
				background.alpha = 1.0;
			}
		}
		
		function handlePress(e: MouseEvent): void
		{
			if (!disabled)
			{
				background.alpha = 0.5;
			}
		}
		
		function handleMouseOver(e: MouseEvent): void
		{
			if (!disabled)
			{
				background.alpha = 0.8;
			}
		}
		
		function handleMouseOut(e: MouseEvent): void
		{
			if (!disabled)
			{
				background.alpha = 1.0;
			}
		}
		
		public function SetChecked(ch:Boolean)
		{
			checked = ch;
			check.visible = checked;
		}
		
		public function IsChecked() : Boolean
		{
			return checked;
		}
		
		public function SetDisabled(dis:Boolean)
		{
			if (disabled != dis)
			{
				if (dis)
				{
					check.alpha = 0.5;
					background.alpha = 0.5;
				}
				else
				{
					check.alpha = 1.0;
					background.alpha = 1.0;
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
