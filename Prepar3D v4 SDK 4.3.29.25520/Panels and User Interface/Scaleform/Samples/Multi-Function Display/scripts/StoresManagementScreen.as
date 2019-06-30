package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	import WeaponObject;
	
	public class StoresManagementScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var _statusSmall_text:String;
		private var _statusLarge_text:String;
		private var _statusLarge2_text:String;
		
		private var _semiCircle_degrees:Number;  // used for rotation: degrees  0 - 90
		private var _semiCircle_largeDigit:int;
		private var _semiCircle_smallDigit:int;
		
		private var NUM_OF_WEAPONS:int = 12;
		
		private var _weaponsArray:Array;
		
		public function StoresManagementScreen() {
			
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
			// temp init values
			_statusSmall_text = "TRAINING";
			_statusLarge_text = "WPNS STATUS";
			_statusLarge2_text = "1234 TP";
			_semiCircle_degrees = -8.9325;
			_semiCircle_largeDigit = getLargeDigit(_semiCircle_degrees);
			_semiCircle_smallDigit = getSmallDigit(_semiCircle_degrees);
			
			// CREATE WEAPON OBJECTS TO HOLD WEAPON DATA
			_weaponsArray = new Array(NUM_OF_WEAPONS);
			for (var w=0; w < NUM_OF_WEAPONS; w++)
			{
				var weaponObj:WeaponObject = new WeaponObject();
				_weaponsArray[w] = weaponObj;
			}
			
			// SET LARGE TEXT (UNDER THE WEAPON STATUS FIELD) i.e. STATION NAME
			//******************************************************************
			_statusLarge2_text = Prepar3D.VarGet("A:STATION DESCRIPTION", "string");
			_this.status_large_2.status_large_text.text = _statusLarge2_text;
			//******************************************************************
			
			// SET WEAPON NAMES
			_weaponsArray[0].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:0", "string");
			_this["weapon1"].innerText.text = _weaponsArray[0].innerText;
			_weaponsArray[1].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:1", "string");
			_this["weapon2"].innerText.text = _weaponsArray[1].innerText;
			_weaponsArray[2].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:2", "string");
			_this["weapon3"].innerText.text = _weaponsArray[2].innerText;
			_weaponsArray[3].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:3", "string");
			_this["weapon4"].innerText.text = _weaponsArray[3].innerText;
			_weaponsArray[4].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:4", "string");
			_this["weapon5"].innerText.text = _weaponsArray[4].innerText;
			_weaponsArray[5].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:5", "string");
			_this["weapon6"].innerText.text = _weaponsArray[5].innerText;
			_weaponsArray[6].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:6", "string");
			_this["weapon7"].innerText.text = _weaponsArray[6].innerText;
			_weaponsArray[7].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:7", "string");
			_this["weapon8"].innerText.text = _weaponsArray[7].innerText;
			_weaponsArray[8].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:8", "string");
			_this["weapon9"].innerText.text = _weaponsArray[8].innerText;
			_weaponsArray[9].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:9", "string");
			_this["weapon10"].innerText.text = _weaponsArray[9].innerText;
			_weaponsArray[10].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:10", "string");
			_this["weapon11"].innerText.text = _weaponsArray[10].innerText;
			_weaponsArray[11].innerText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT NAME:11", "string");
			_this["weapon12"].innerText.text = _weaponsArray[11].innerText;
			
			// SET WEAPON TYPES
			_weaponsArray[0].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:0", "string");
			_this["weapon1"].status_text.text = _weaponsArray[0].statusText;
			_weaponsArray[1].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:1", "string");
			_this["weapon2"].status_text.text = _weaponsArray[1].statusText;
			_weaponsArray[2].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:2", "string");
			_this["weapon3"].status_text.text = _weaponsArray[2].statusText;
			_weaponsArray[3].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:3", "string");
			_this["weapon4"].status_text.text = _weaponsArray[3].statusText;
			_weaponsArray[4].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:4", "string");
			_this["weapon5"].status_text.text = _weaponsArray[4].statusText;
			_weaponsArray[5].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:5", "string");
			_this["weapon6"].status_text.text = _weaponsArray[5].statusText;
			_weaponsArray[6].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:6", "string");
			_this["weapon7"].status_text.text = _weaponsArray[6].statusText;
			_weaponsArray[7].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:7", "string");
			_this["weapon8"].status_text.text = _weaponsArray[7].statusText;
			_weaponsArray[8].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:8", "string");
			_this["weapon9"].status_text.text = _weaponsArray[8].statusText;
			_weaponsArray[9].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:9", "string");
			_this["weapon10"].status_text.text = _weaponsArray[9].statusText;
			_weaponsArray[10].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:10", "string");
			_this["weapon11"].status_text.text = _weaponsArray[10].statusText;
			_weaponsArray[11].statusText = Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT TYPE:11", "string");
			_this["weapon12"].status_text.text = _weaponsArray[11].statusText;
			
			
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function weaponList():Array
		{
			return _weaponsArray;
		}
		
		public function update():void
		{			
			
			// SET SMALL TEXT STATUS/INFO
			//******************************************************************
			_this.status_small.status_small_text.text = _statusSmall_text;
			//******************************************************************
			
			// SET WEAPON SYSTEM STATUS
			//******************************************************************
			if ( Prepar3D.VarGet("A:WEAPON SYSTEM ON", "bool") == true)
			{
				_statusLarge_text = "WPNS READY";
			}
			else
			{
				_statusLarge_text = "WPNS OFF";
			}
			_this.status_large.status_large_text.text = _statusLarge_text;
			//*******************************************************************
			
			
			// SET PLANE PITCH
			//***********************************************************************************
			_semiCircle_degrees = Prepar3D.VarGet("A:PLANE PITCH DEGREES", "degrees");
			//MovieClip(parent).setDebugTextDisplay("plane pitch = " + _semiCircle_degrees);
			_this.semiCircle_mc.rotation = (roundToOneDecimal(_semiCircle_degrees)).toFixed(1);
			_semiCircle_largeDigit = getLargeDigit(_semiCircle_degrees);
			_semiCircle_smallDigit = getSmallDigit(_semiCircle_degrees);
			_this.semiCircle.semiCircle_small_text.text = _semiCircle_smallDigit;
			_this.semiCircle.semiCircle_large_text.text = appendZero(_semiCircle_largeDigit);
			//***********************************************************************************
			
			// SET WEAPON COUNT NUMBER FOR EACH WEAPON
			//***********************************************************************************
			_weaponsArray[0].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:0", "number"));
			if (_weaponsArray[0].innerText != "")
			{	
				_this["weapon1"].mediumNumber.text = _weaponsArray[0].mediumNumber;
			}
			else
			{
				_this["weapon1"].mediumNumber.text = "";
			}
			
			_weaponsArray[1].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:1", "number"));
			if (_weaponsArray[1].innerText != "")
			{	
				_this["weapon2"].mediumNumber.text = _weaponsArray[1].mediumNumber;
			}
			else
			{
				_this["weapon2"].mediumNumber.text = "";
			}
			
			_weaponsArray[2].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:2", "number"));
			if (_weaponsArray[2].innerText != "")
			{	
				_this["weapon3"].mediumNumber.text = _weaponsArray[2].mediumNumber;
			}
			else
			{
				_this["weapon3"].mediumNumber.text = "";
			}
			
			_weaponsArray[3].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:3", "number"));
			if (_weaponsArray[3].innerText != "")
			{	
				_this["weapon4"].mediumNumber.text = _weaponsArray[3].mediumNumber;
			}
			else
			{
				_this["weapon4"].mediumNumber.text = "";
			}
			
			_weaponsArray[4].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:4", "number"));
			if (_weaponsArray[4].innerText != "")
			{	
				_this["weapon5"].mediumNumber.text = _weaponsArray[4].mediumNumber;
			}
			else
			{
				_this["weapon5"].mediumNumber.text = "";
			}
			
			_weaponsArray[5].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:5", "number"));
			if (_weaponsArray[5].innerText != "")
			{	
				_this["weapon6"].mediumNumber.text = _weaponsArray[5].mediumNumber;
			}
			else
			{
				_this["weapon6"].mediumNumber.text = "";
			}
			
			_weaponsArray[6].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:6", "number"));
			if (_weaponsArray[6].innerText != "")
			{	
				_this["weapon7"].mediumNumber.text = _weaponsArray[6].mediumNumber;
			}
			else
			{
				_this["weapon7"].mediumNumber.text = "";
			}
			
			_weaponsArray[7].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:7", "number"));
			if (_weaponsArray[7].innerText != "")
			{	
				_this["weapon8"].mediumNumber.text = _weaponsArray[7].mediumNumber;
			}
			else
			{
				_this["weapon8"].mediumNumber.text = "";
			}
			
			_weaponsArray[8].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:8", "number"));
			if (_weaponsArray[8].innerText != "")
			{	
				_this["weapon9"].mediumNumber.text = _weaponsArray[8].mediumNumber;
			}
			else
			{
				_this["weapon9"].mediumNumber.text = "";
			}
			
			_weaponsArray[9].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:9", "number"));
			if (_weaponsArray[9].innerText != "")
			{	
				_this["weapon10"].mediumNumber.text = _weaponsArray[9].mediumNumber;
			}
			else
			{
				_this["weapon10"].mediumNumber.text = "";
			}
			
			_weaponsArray[10].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:10", "number"));
			if (_weaponsArray[10].innerText != "")
			{	
				_this["weapon11"].mediumNumber.text = _weaponsArray[10].mediumNumber;
			}
			else
			{
				_this["weapon11"].mediumNumber.text = "";
			}
			
			_weaponsArray[11].mediumNumber = int(Prepar3D.VarGet("A:WEAPON SYSTEM STATION OBJECT COUNT:11", "number"));
			if (_weaponsArray[11].innerText != "")
			{	
				_this["weapon12"].mediumNumber.text = _weaponsArray[11].mediumNumber;
			}
			else
			{
				_this["weapon12"].mediumNumber.text = "";
			}
			//*******************************************************************
			
			//MovieClip(parent).setDebugTextDisplay("_semiCircle_degrees= " + _semiCircle_degrees);
			
		}
		
		private function appendZero(dig:int):String
		{
			var absDig:int = Math.abs(dig);
			var temp:String;
			if (absDig < 10)
			{
				temp = "0" + absDig;
			}
			else
			{
				temp = String(absDig);
			}
			
			return temp;
		}
		
		private function roundToOneDecimal(deg:Number):Number
		{
			var temp:Number = Math.floor(deg * 10);
			return (temp * 0.10);
		}
		
		private function getLargeDigit(deg:Number):int
		{
			if (deg < 0)
			{
				return int(Math.ceil(deg));
			}
			else
			{
				return int(Math.floor(deg));
			}
			
		}
		
		private function getSmallDigit(deg:Number):int
		{
			var temp:Number;
			if (deg < 0)
			{
				temp = Math.abs(deg - Math.ceil(deg));
			}
			else
			{
				temp = Math.abs(deg - Math.floor(deg));
			}
			 
			return int(Math.floor(temp*10));
		}
	}
	
}
