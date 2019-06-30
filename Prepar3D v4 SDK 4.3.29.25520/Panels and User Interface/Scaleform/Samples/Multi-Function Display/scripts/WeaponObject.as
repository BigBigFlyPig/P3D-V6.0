package  {
	
	public class WeaponObject {
		
		private var _mediumNumber:String;
		private var _innerText:String;
		private var _status_text:String;

		public function WeaponObject() 
		{
			// constructor code
			_mediumNumber = "";
			_innerText = "";
			_status_text = "";
		}
		
		public function set mediumNumber(str:String):void
		{
			_mediumNumber = str;
		}
		
		public function get mediumNumber():String
		{
			return _mediumNumber;
		}
		
		public function set innerText(str:String):void
		{
			_innerText = str;
		}
		
		public function get innerText():String
		{
			return _innerText;
		}
		
		public function set statusText(str:String):void
		{
			_status_text = str;
		}
		
		public function get statusText():String
		{
			return _status_text;
		}

	}
	
}
