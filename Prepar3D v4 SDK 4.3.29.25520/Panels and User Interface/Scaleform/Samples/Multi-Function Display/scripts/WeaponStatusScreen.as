package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	import WeaponObject;
	import flash.events.MouseEvent;
	
	public class WeaponStatusScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		// CLASS PROPERTIES
		private var _statusSmall_text:String;
		private var _statusLarge_text:String;
		
		private var _wp_sys_on:Boolean;
		private var _wp_sys_arm:Boolean;
		private var _wp_sys_safe:Boolean;
		
		private var _weaponDoorsOpen:Boolean;  // true if open; false if closed
		private var _weaponSelected:Boolean;
		private var _weaponSelectedName:String;
		private var _weaponSelectedID:int;
		
		private var _chaff_quantity:int;
		private var _flare_quantity:int;
		
		private var NUM_OF_WEAPONS:int = 12;
		private var _weaponsArray:Array;
		private var _currentWeaponIcon:int;
		private var _currentWeaponSelected:MovieClip;
		
		public function WeaponStatusScreen() {
			
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
			_statusSmall_text = "LIVE";
			_statusLarge_text = "NO WPN SELECTED";
			_weaponDoorsOpen = false;
			
			_weaponSelected = false;
			_weaponSelectedName = "NONE";
			_weaponSelectedID = 0;
			_currentWeaponSelected = null;
			
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function weaponList(wpArr:Array):void
		{
			_weaponsArray = wpArr;
			
			var weaponType:String;
			for (var w=0; w < NUM_OF_WEAPONS; w++)
			{
				weaponType = _weaponsArray[w].statusText;
				_currentWeaponIcon = w+1;
				switch (weaponType)
				{
					case "NONE":
					case "":
						break;
					
					case "AAM":
						add_AIM_icon(_weaponsArray[w].innerText);
						break;
					
					case "ASM":
						add_AGM_icon(_weaponsArray[w].innerText);
						break;
					
					case "BLU":
						add_BLU_icon(_weaponsArray[w].innerText);
						break;
					
					case "GBU":
						add_GBU_icon(_weaponsArray[w].innerText);
						break;
					
					case "SAM":
						add_V750_icon(_weaponsArray[w].innerText);
						break;
				}
			}
		}
		
		public function update():void
		{
			// GET FROM PREPAR3D WEAPON SYSTEM STATUS
			_wp_sys_on = Prepar3D.VarGet("A:WEAPON SYSTEM ON", "bool");
			_wp_sys_arm = Prepar3D.VarGet("A:WEAPON SYSTEM ARMED", "bool");
			_wp_sys_safe = Prepar3D.VarGet("A:WEAPON SYSTEM SAFETY ENGAGED", "bool");
			
			// SET SMALL TEXT STATUS/INFO
			//******************************************************************
			if (_wp_sys_on == true)
			{
				if (_wp_sys_arm == true)
				{
					_statusSmall_text = "ARM";
				}
				else if (_wp_sys_safe == true)
				{
					_statusSmall_text = "SAFE";
				}
				else
				{
					_statusSmall_text = "LIVE";
				}
			}
			else
			{
				_statusSmall_text = "OFF";
			}

			_this.status_small.status_small_text.text = _statusSmall_text;
			//******************************************************************
			
			// SET WEAPON SELECTION STATUS
			//******************************************************************
			setWeaponStatusText();
			
			_this.status_large.status_large_text.text = _statusLarge_text;
			//*******************************************************************
			
			// SET DOOR STATUS
			//******************************************************************
			if ((_statusSmall_text == "ARM") || (_statusSmall_text == "LIVE"))
			{
				_weaponDoorsOpen = true;
			}
			else
			{
				_weaponDoorsOpen = false;
			}
			//
			// IF DOORS ARE OPEN, SHOW THE STATE OF 'OPEN' WITH BOX AROUND IT.
			// ELSE, IF DOORS ARE CLOSED, SET THE STATE OF 'CLOSED' WITH BOX AROUND IT.
			if (_weaponDoorsOpen == true)
			{
				_this.weaponDoorsOpen.gotoAndStop(2);
				_this.weaponDoorsClosed.gotoAndStop(1);
			}
			else
			{
				_this.weaponDoorsOpen.gotoAndStop(1);
				_this.weaponDoorsClosed.gotoAndStop(2);
			}
			//*******************************************************************
			
			// SET CHAFF AND FLARE COUNTS
			//******************************************************************
			
			// Flares (station 0)
			_flare_quantity = int(Prepar3D.VarGet("A:COUNTERMEASURE SYSTEM STATION OBJECT COUNT:0", "number"));
			_this.chaffFlare_mc.flare_number.text = appendZero(_flare_quantity);
			
			// Chaffs (station 1)
			_chaff_quantity = int(Prepar3D.VarGet("A:COUNTERMEASURE SYSTEM STATION OBJECT COUNT:1", "number"));
			_this.chaffFlare_mc.chaff_number.text = appendZero(_chaff_quantity);
			
			//*******************************************************************
			
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
		
		private function add_AIM_icon(wpnName:String):void
		{
			var aimIcon:AIM_Sidewinder = new AIM_Sidewinder();

			clearContainer(_this["iconContainer"+_currentWeaponIcon]);
			aimIcon.addEventListener(MouseEvent.CLICK, weaponSelected);
			aimIcon.buttonMode = true;
			aimIcon.name = wpnName;
			aimIcon.id = _currentWeaponIcon;
			_this["iconContainer"+_currentWeaponIcon].addChild(aimIcon);
		}
		
		private function add_AGM_icon(wpnName:String):void
		{
			var agmIcon:AGM = new AGM();
			
			clearContainer(_this["iconContainer"+_currentWeaponIcon]);
			agmIcon.addEventListener(MouseEvent.CLICK, weaponSelected);
			agmIcon.buttonMode = true;
			agmIcon.name = wpnName;
			agmIcon.id = _currentWeaponIcon;
			_this["iconContainer"+_currentWeaponIcon].addChild(agmIcon);
		}
		
		private function add_GBU_icon(wpnName:String):void
		{
			var gbuIcon:GBU_JDAM = new GBU_JDAM();
			
			clearContainer(_this["iconContainer"+_currentWeaponIcon]);
			gbuIcon.addEventListener(MouseEvent.CLICK, weaponSelected);
			gbuIcon.buttonMode = true;
			gbuIcon.name = wpnName;
			gbuIcon.id = _currentWeaponIcon;
			_this["iconContainer"+_currentWeaponIcon].addChild(gbuIcon);
		}
		
		private function add_BLU_icon(wpnName:String):void
		{
			var bluIcon:BLU = new BLU();
			
			clearContainer(_this["iconContainer"+_currentWeaponIcon]);
			bluIcon.addEventListener(MouseEvent.CLICK, weaponSelected);
			bluIcon.buttonMode = true;
			bluIcon.name = wpnName;
			bluIcon.id = _currentWeaponIcon;
			_this["iconContainer"+_currentWeaponIcon].addChild(bluIcon);
		}
		
		private function add_V750_icon(wpnName:String):void
		{
			var v750Icon:V750 = new V750();
			
			clearContainer(_this["iconContainer"+_currentWeaponIcon]);
			v750Icon.addEventListener(MouseEvent.CLICK, weaponSelected);
			v750Icon.buttonMode = true;
			v750Icon.name = wpnName;
			v750Icon.id = _currentWeaponIcon;
			_this["iconContainer"+_currentWeaponIcon].addChild(v750Icon);
		}
		
		private function clearContainer(container:MovieClip):void
		{
			while (container.numChildren > 0)
			{
				container.removeChildAt(0);
			}
		}
		
		private function weaponSelected(evt:MouseEvent):void
		{			
			if (_currentWeaponSelected != null)
			{
				// change color back to white
				_currentWeaponSelected.gotoAndStop(1);
			}
			
			_currentWeaponSelected = evt.currentTarget as MovieClip;
			
			if (_weaponSelectedID != _currentWeaponSelected.id) // selected a different weapon
			{
				_weaponSelectedName = _currentWeaponSelected.name;
				_weaponSelectedID = _currentWeaponSelected.id;
				_weaponSelected = true;
				// change color to green
				_currentWeaponSelected.gotoAndStop(2);
			}
			else  // selected same weapon
			{
				// reset the variables
				_weaponSelectedName = "NONE";
				_weaponSelected = false;
				_weaponSelectedID = 0;
				_currentWeaponSelected = null;
			}
			
			// finally, change large text
			setWeaponStatusText();
			
		}
		
		private function setWeaponStatusText():void
		{
			if (_weaponSelected == false)
			{
				_statusLarge_text = "NO WPN SELECTED";
			}
			else
			{
				_statusLarge_text = _weaponSelectedName + " SELECTED";
			}
		}
		
	}
	
}
