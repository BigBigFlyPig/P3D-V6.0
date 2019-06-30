package 
{
	import flash.events.Event;
	/**
	 * ...
	 * @author ...
	 */
	public class CDU_Btn_Event extends Event 
	{
		public static const CDU_BTN_SELECTED:String = "mfdButtonSelected";
		
		public var btn_name:String;
		
		public function CDU_Btn_Event(type:String, bubbles:Boolean, cancelable:Boolean, button_name:String)
		{
			super(type, bubbles, cancelable);
			this.btn_name = button_name;
		}
		
		override public function clone():Event
		{
			return new CDU_Btn_Event(type, bubbles, cancelable, btn_name);
		}
	}
	
}
