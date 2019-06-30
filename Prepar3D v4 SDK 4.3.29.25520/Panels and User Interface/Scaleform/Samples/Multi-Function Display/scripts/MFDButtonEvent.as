package 
{
	import flash.events.Event;
	/**
	 * ...
	 * @author ...
	 */
	public class MFDButtonEvent extends Event 
	{
		public static const MFD_BTN_SELECTED:String = "mfdButtonSelected";
		
		public var btn_name:String;
		
		public function MFDButtonEvent(type:String, bubbles:Boolean, cancelable:Boolean, button_name:String)
		{
			super(type, bubbles, cancelable);
			this.btn_name = button_name;
		}
		
		override public function clone():Event
		{
			return new MFDButtonEvent(type, bubbles, cancelable, btn_name);
		}
	}
	
}
