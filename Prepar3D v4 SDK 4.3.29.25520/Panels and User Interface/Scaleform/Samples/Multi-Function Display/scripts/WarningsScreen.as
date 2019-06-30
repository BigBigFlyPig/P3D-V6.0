package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class WarningsScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var _FuelTankLeftQuant:Number;
		private var _FuelPressEng1:Number;
		private var _OilPressEng1:Number;
		private var _OilTempEng1:Number;
		private var _ChipEngine1:Number;
		private var _FuelFilterBypass1:Number;
		private var _EngineStarter1:Boolean;
		private var _PriServoPress1:Number;
		private var _TailRtrQuadrant:Number;
		private var _MainXmsnOilTemp:Number;
		private var _BoostServoOff:Boolean;
		private var _LeftPitotHeat:Boolean;
		private var _ChipInputMdlLh:Number;
		private var _ChipAccessMdlLh:Number;
		private var _MrDeIceFail:Boolean;
		private var _MainXmsnOilPressure:Number;
		private var _AntiIceEng1:Boolean;
		private var _APU:Boolean;
		private var _ApuAccumLow:Number;
		private var _MfdMalf1:Boolean;
		private var _Gen1:Boolean;
		private var _GenBrg1:Number;
		private var _Conv1:Boolean;
		private var _AcEssBusOff:Boolean;
		private var _BattLowCharge:Number;
		private var _GustLock:Number;
		private var _OilFltrBypass1:Number;
		private var _HydPump1:Number;
		private var _IrcmInop:Number;
		private var _IntXmsnOilTemp:Number;
		private var _Stabilator:Number;
		private var _FltPathStab:Boolean;
		private var _ChipIntXmsn:Number;
		private var _ChipMainMdlSump:Number;
		private var _MrDeIceFault:Number;
		private var _RsvrLow1:Number;
		private var _InlAntiIceOn1:Boolean;
		private var _ApuGenOn:Boolean;
		private var _SearchLtOn:Boolean;
		private var _CargoHookOpen:Number;
		private var _ParkingBrakeOn:Boolean;
		private var _Gen2:Boolean;
		private var _GenBrg2:Number;
		private var _Conv2:Boolean;
		private var _DcEssBusOff:Boolean;
		private var _OilFltrBypass2:Number;
		private var _HydPump2:Number;
		private var _AuxFuel:Number;
		private var _TailXmsnOilTemp:Number;
		private var _SasOff:Number;
		private var _Iff:Boolean;
		private var _ChipTailXmsn:Boolean;
		private var _ApuFail:Boolean;
		private var _TrDeIceFail:Number;
		private var _RsvrLow2:Number;
		private var _InlAntiIceOn2:Boolean;
		private var _PrimeBoostPmpOn:Boolean;
		private var _LdgLtOn:Boolean;
		private var _HookArmed:Boolean;
		private var _ExtPwrConnected:Boolean;
		private var _FuelTankRightQuant:Number;
		private var _FuelPressEng2:Number;
		private var _OilPressEng2:Number;
		private var _OilTempEng2:Number;
		private var _ChipEngine2:Number;
		private var _FuelFilterBypass2:Number;
		private var _EngineStarter2:Boolean;
		private var _PriServoPress2:Number;
		private var _TailRotorServo1:Number;
		private var _ApuOilTempHigh:Number;
		private var _TrimFail:Number;
		private var _RightPitotHeat:Boolean;
		private var _ChipInputMdlRh:Number;
		private var _ChipAccessMdlRh:Number;
		private var _IceDetected:Number;
		private var _BackupRsvrLow:Boolean;
		private var _AntiIceEng2:Boolean;
		private var _BackupPumpOn:Number;
		private var _TlRtrServoOn:Boolean;
		private var _MfdMalf2:Boolean;
		
		public function WarningsScreen() {
			// Only initialize once
			if (initialized == false)
			{
				Init();
				initialized = true;
			}
		}
		
		private function Init():void
		{
			// Initialize the Prepar3D Interface because we will need it to initialize
			// our scenario variables
			//Prepar3D.Init();
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function update():void
		{
			_FuelTankLeftQuant = Prepar3D.VarGet("A:FUEL TANK LEFT MAIN QUANTITY", "gallons");
			_FuelPressEng1 = Prepar3D.VarGet("A:GENERAL ENG FUEL PRESSURE:1", "psi");
			_OilPressEng1 = Prepar3D.VarGet("A:GENERAL ENG OIL PRESSURE:1", "psi");
			_OilTempEng1 = Prepar3D.VarGet("A:GENERAL ENG OIL TEMPERATURE:1", "celsius");
			_ChipEngine1 = Prepar3D.VarGet("A:ROTOR RPM PCT:1", "percent");
			_FuelFilterBypass1 = _ChipEngine1;
			_EngineStarter1 = Prepar3D.VarGet("A:GENERAL ENG STARTER:1", "bool");
			_PriServoPress1 = _OilPressEng1;
			_TailRtrQuadrant = Prepar3D.VarGet("A:ROTOR RPM PCT:1", "percent");
			_MainXmsnOilTemp = Prepar3D.VarGet("A:ROTOR TEMPERATURE", "celsius");
			_BoostServoOff = Prepar3D.VarGet("A:GENERAL ENG FUEL PUMP SWITCH:1", "bool");
			_LeftPitotHeat = Prepar3D.VarGet("A:PITOT HEAT", "bool");
			_ChipInputMdlLh = _TailRtrQuadrant;
			_ChipAccessMdlLh = _ChipInputMdlLh;
			_MrDeIceFail = Prepar3D.VarGet("A:STRUCTURAL DEICE SWITCH", "bool");
			_MainXmsnOilPressure = _OilPressEng1;
			_AntiIceEng1 = Prepar3D.VarGet("A:GENERAL ENG ANTI ICE POSITION:1", "bool");
			_APU = Prepar3D.VarGet("A:APU GENERATOR SWITCH", "bool");
			_ApuAccumLow = _OilPressEng1;
			_MfdMalf1 = Prepar3D.VarGet("A:AVIONICS MASTER SWITCH", "bool");
			_Gen1 = Prepar3D.VarGet("A:GENERAL ENG MASTER ALTERNATOR:1", "bool");
			_GenBrg1 = _OilPressEng1;
			_Conv1 = Prepar3D.VarGet("A:CIRCUIT GENERAL PANEL ON", "bool");
			_AcEssBusOff = _Conv1;
			_BattLowCharge = Prepar3D.VarGet("A:ELECTRICAL BATTERY LOAD", "amp");
			_GustLock = _OilPressEng1;
			_OilFltrBypass1 = _OilPressEng1;
			_HydPump1 = _OilPressEng1;
			_IrcmInop = _OilPressEng1;
			_IntXmsnOilTemp = _OilTempEng1;
			_Stabilator = _OilPressEng1;
			_FltPathStab = Prepar3D.VarGet("A:AUTOPILOT FLIGHT DIRECTOR ACTIVE", "bool");
			_ChipIntXmsn = _OilPressEng1;
			_ChipMainMdlSump = _OilPressEng1;
			_MrDeIceFault = _OilPressEng1;
			_RsvrLow1 = _OilPressEng1;
			_InlAntiIceOn1 = _AntiIceEng1;
			_ApuGenOn = _APU;
			_SearchLtOn = Prepar3D.VarGet("A:LIGHT LANDING", "bool");
			_CargoHookOpen = Prepar3D.VarGet("A:EXIT OPEN", "percent");
			_ParkingBrakeOn = Prepar3D.VarGet("A:BRAKE PARKING POSITION", "bool");
			_Gen2 = _Gen1;
			_GenBrg2 = Prepar3D.VarGet("A:GENERAL ENG OIL PRESSURE:2", "psi");
			_Conv2 = _Conv1;
			_DcEssBusOff = _Conv2;
			_OilFltrBypass2 = _GenBrg2;
			_HydPump2 = _GenBrg2;
			_AuxFuel = Prepar3D.VarGet("A:FUEL TOTAL CAPACITY", "gallons");
			_TailXmsnOilTemp = Prepar3D.VarGet("A:ROTOR RPM PCT:2", "percent");
			_SasOff = _GenBrg2;
			_Iff = _Conv1;
			_ChipTailXmsn = _Conv1;
			_ApuFail = _APU;
			_TrDeIceFail = _TailXmsnOilTemp;
			_RsvrLow2 = _GenBrg2;
			_InlAntiIceOn2 = Prepar3D.VarGet("A:GENERAL ENG ANTI ICE POSITION:2", "bool");
			_PrimeBoostPmpOn = _BoostServoOff;
			_LdgLtOn = _SearchLtOn;
			_HookArmed = _Conv1;
			_ExtPwrConnected = _Conv1;
			_FuelTankRightQuant = Prepar3D.VarGet("A:FUEL TANK RIGHT MAIN QUANTITY", "gallons");
			_FuelPressEng2 = Prepar3D.VarGet("A:GENERAL ENG FUEL PRESSURE:2", "psi");
			_OilPressEng2 = Prepar3D.VarGet("A:GENERAL ENG OIL PRESSURE:2", "psi");
			_OilTempEng2 = Prepar3D.VarGet("A:GENERAL ENG OIL TEMPERATURE:2", "celsius");
			_ChipEngine2 = _TailXmsnOilTemp;
			_FuelFilterBypass2 = _ChipEngine2;
			_EngineStarter2 = Prepar3D.VarGet("A:GENERAL ENG STARTER:2", "bool");
			_PriServoPress2 = _GenBrg2;
			_TailRotorServo1 = _TailXmsnOilTemp;
			_ApuOilTempHigh = _MainXmsnOilTemp;
			_TrimFail = _TailRtrQuadrant;
			_RightPitotHeat = Prepar3D.VarGet("A:PITOT HEAT", "bool");
			_ChipInputMdlRh = _TailXmsnOilTemp;
			_ChipAccessMdlRh = _ChipInputMdlRh;
			_IceDetected = Prepar3D.VarGet("A:AMBIENT TEMPERATURE", "celsius");
			_BackupRsvrLow = _Conv1;
			_AntiIceEng2 = _InlAntiIceOn2;
			_BackupPumpOn = _OilPressEng1;
			_TlRtrServoOn = _Conv1;
			_MfdMalf2 = _MfdMalf1;
			
			// USE FOR DEBUG
			//MovieClip(parent).setDebugTextDisplay("_FuelTankLeftQuant= " + _FuelTankLeftQuant);
			
			
			// Now set the visibility of each text field in the display
			//----------------------------------------------------------
			if (_FuelTankLeftQuant < 25.7)
			{
				FuelTankLeft.visible = true;
			}
			else
			{
				FuelTankLeft.visible = false;
			}
			//
			if (_FuelPressEng1 < 2)
			{
				FuelPressEng1.visible = true;
			}
			else
			{
				FuelPressEng1.visible = false;
			}
			//
			if (_OilPressEng1 < 8)
			{
				OilPressEng1.visible = true;
			}
			else
			{
				OilPressEng1.visible = false;
			}
			//
			if (_OilTempEng1 > 150)
			{
				OilTempEng1.visible = true;
			}
			else
			{
				OilTempEng1.visible = false;
			}
			//
			if (_ChipEngine1 < 3)
			{
				ChipEng1.visible = true;
			}
			else
			{
				ChipEng1.visible = false;
			}
			//
			if (_FuelFilterBypass1 < 5)
			{
				FuelFilterBypass1.visible = true;
			}
			else
			{
				FuelFilterBypass1.visible = false;
			}
			//
			if (_EngineStarter1 == true)
			{
				EngineStarter1.visible = true;
			}
			else
			{
				EngineStarter1.visible = false;
			}
			//
			if (_PriServoPress1 < 3)
			{
				PriServoPress1.visible = true;
			}
			else
			{
				PriServoPress1.visible = false;
			}
			//
			if (_TailRtrQuadrant < 1)
			{
				TailRtrQuadrant.visible = true;
			}
			else
			{
				TailRtrQuadrant.visible = false;
			}
			//
			if (_MainXmsnOilTemp > 120)
			{
				MainXmsnOilTemp.visible = true;
			}
			else
			{
				MainXmsnOilTemp.visible = false;
			}
			//
			if (_BoostServoOff == true)
			{
				BoostServoOff.visible = true;
			}
			else
			{
				BoostServoOff.visible = false;
			}
			//
			if (_LeftPitotHeat == true)
			{
				LeftPitotHeat.visible = true;
			}
			else
			{
				LeftPitotHeat.visible = false;
			}
			//
			if (_ChipInputMdlLh < 2)
			{
				ChipInputMdlLh.visible = true;
			}
			else
			{
				ChipInputMdlLh.visible = false;
			}
			//
			if (_ChipAccessMdlLh < 1)
			{
				ChipAccessMdlLh.visible = true;
			}
			else
			{
				ChipAccessMdlLh.visible = false;
			}
			//
			if (_MrDeIceFail == true)
			{
				MrDeIceFail.visible = true;
			}
			else
			{
				MrDeIceFail.visible = false;
			}
			//
			if (_MainXmsnOilPressure < 14)
			{
				MainXmsnOilPress.visible = true;
			}
			else
			{
				MainXmsnOilPress.visible = false;
			}
			//
			if (_AntiIceEng1 == true)
			{
				AntiIceEng1.visible = true;
			}
			else
			{
				AntiIceEng1.visible = false;
			}
			//
			if (_APU == true)
			{
				ApuOn.visible = true;
			}
			else
			{
				ApuOn.visible = false;
			}
			//
			if (_ApuAccumLow < 5)
			{
				ApuAccumLow.visible = true;
			}
			else
			{
				ApuAccumLow.visible = false;
			}
			//
			if (_MfdMalf1 == false)
			{
				MfdMalf1.visible = true;
			}
			else
			{
				MfdMalf1.visible = false;
			}
			//
			if (_Gen1 == false)
			{
				Gen1.visible = true;
			}
			else
			{
				Gen1.visible = false;
			}
			//
			if (_GenBrg1 < 2)
			{
				GenBrg1.visible = true;
			}
			else
			{
				GenBrg1.visible = false;
			}
			//
			if (_Conv1 == false)
			{
				Conv1.visible = true;
			}
			else
			{
				Conv1.visible = false;
			}
			//
			if (_AcEssBusOff == false)
			{
				AcEssBusOff.visible = true;
			}
			else
			{
				AcEssBusOff.visible = false;
			}
			//
			if (_BattLowCharge > 16)
			{
				BattLowCharge.visible = true;
			}
			else
			{
				BattLowCharge.visible = false;
			}
			//
			if (_GustLock < 8)
			{
				GustLock.visible = true;
			}
			else
			{
				GustLock.visible = false;
			}
			//
			if (_OilFltrBypass1 < 12)
			{
				OilFltrBypass1.visible = true;
			}
			else
			{
				OilFltrBypass1.visible = false;
			}
			//
			if (_HydPump1 < 5)
			{
				HydPump1.visible = true;
			}
			else
			{
				HydPump1.visible = false;
			}
			//
			if (_IrcmInop < 2)
			{
				IrcmInop.visible = true;
			}
			else
			{
				IrcmInop.visible = false;
			}
			//
			if (_IntXmsnOilTemp > 100)
			{
				IntXmsnOilTemp.visible = true;
			}
			else
			{
				IntXmsnOilTemp.visible = false;
			}
			//
			if (_Stabilator < 12)
			{
				Stabilator.visible = true;
			}
			else
			{
				Stabilator.visible = false;
			}
			//
			if (_FltPathStab == true)
			{
				FltPathStab.visible = true;
			}
			else
			{
				FltPathStab.visible = false;
			}
			//
			if (_ChipIntXmsn < 2)
			{
				ChipIntXmsn.visible = true;
			}
			else
			{
				ChipIntXmsn.visible = false;
			}
			//
			if (_ChipMainMdlSump < 12)
			{
				ChipMainMdlSump.visible = true;
			}
			else
			{
				ChipMainMdlSump.visible = false;
			}
			//
			if (_MrDeIceFault < 7)
			{
				MrDeIceFault.visible = true;
			}
			else
			{
				MrDeIceFault.visible = false;
			}
			//
			if (_RsvrLow1 < 60)
			{
				RsvrLow1.visible = true;
			}
			else
			{
				RsvrLow1.visible = false;
			}
			//
			if (_InlAntiIceOn1 == true)
			{
				InlAntiIceOn1.visible = true;
			}
			else
			{
				InlAntiIceOn1.visible = false;
			}
			//
			if (_ApuGenOn == true)
			{
				ApuGenOn.visible = true;
			}
			else
			{
				ApuGenOn.visible = false;
			}
			//
			if (_SearchLtOn == true)
			{
				SearchLtOn.visible = true;
			}
			else
			{
				SearchLtOn.visible = false;
			}
			//
			if (_CargoHookOpen > 2)
			{
				CargoHookOpen.visible = true;
			}
			else
			{
				CargoHookOpen.visible = false;
			}
			//
			if (_ParkingBrakeOn == true)
			{
				ParkingBrakeOn.visible = true;
			}
			else
			{
				ParkingBrakeOn.visible = false;
			}
			//
			if (_Gen2 == false)
			{
				Gen2.visible = true;
			}
			else
			{
				Gen2.visible = false;
			}
			//
			if (_GenBrg2 < 3.5)
			{
				GenBrg2.visible = true;
			}
			else
			{
				GenBrg2.visible = false;
			}
			//
			if (_Conv2 == false)
			{
				Conv2.visible = true;
			}
			else
			{
				Conv2.visible = false;
			}
			//
			if (_DcEssBusOff == false)
			{
				DcEssBusOff.visible = true;
			}
			else
			{
				DcEssBusOff.visible = false;
			}
			//
			if (_OilFltrBypass2 < 15)
			{
				OilFltrBypass2.visible = true;
			}
			else
			{
				OilFltrBypass2.visible = false;
			}
			//
			if (_HydPump2 < 35)
			{
				HydPump2.visible = true;
			}
			else
			{
				HydPump2.visible = false;
			}
			//
			if (_AuxFuel < 20)
			{
				AuxFuel.visible = true;
			}
			else
			{
				AuxFuel.visible = false;
			}
			//
			if (_TailXmsnOilTemp < 2)
			{
				TailXmsnOilTemp.visible = true;
			}
			else
			{
				TailXmsnOilTemp.visible = false;
			}
			//
			if (_SasOff < 6)
			{
				SasOff.visible = true;
			}
			else
			{
				SasOff.visible = false;
			}
			//
			if (_Iff == false)
			{
				Iff.visible = true;
			}
			else
			{
				Iff.visible = false;
			}
			//
			if (_ChipTailXmsn == false)
			{
				ChipTailXmsn.visible = true;
			}
			else
			{
				ChipTailXmsn.visible = false;
			}
			//
			if (_ApuFail == false)
			{
				ApuFail.visible = true;
			}
			else
			{
				ApuFail.visible = false;
			}
			//
			if (_TrDeIceFail < 1)
			{
				TrDeIceFail.visible = true;
			}
			else
			{
				TrDeIceFail.visible = false;
			}
			//
			if (_RsvrLow2 < 65)
			{
				RsvrLow2.visible = true;
			}
			else
			{
				RsvrLow2.visible = false;
			}
			//
			if (_InlAntiIceOn2 == true)
			{
				InlAntiIceOn2.visible = true;
			}
			else
			{
				InlAntiIceOn2.visible = false;
			}
			//
			if (_PrimeBoostPmpOn == true)
			{
				PrimeBoostPmpOn.visible = true;
			}
			else
			{
				PrimeBoostPmpOn.visible = false;
			}
			//
			if (_LdgLtOn == true)
			{
				LdgLtOn.visible = true;
			}
			else
			{
				LdgLtOn.visible = false;
			}
			//
			if (_HookArmed == true)
			{
				HookArmed.visible = true;
			}
			else
			{
				HookArmed.visible = false;
			}
			//
			if (_ExtPwrConnected == true)
			{
				ExtPwrConnected.visible = true;
			}
			else
			{
				ExtPwrConnected.visible = false;
			}
			//
			if (_FuelTankRightQuant < 25.7)
			{
				FuelTankRight.visible = true;
			}
			else
			{
				FuelTankRight.visible = false;
			}
			//
			if (_FuelPressEng2 < 5)
			{
				FuelPressEng2.visible = true;
			}
			else
			{
				FuelPressEng2.visible = false;
			}
			//
			if (_OilPressEng2 < 4)
			{
				OilPressEng2.visible = true;
			}
			else
			{
				OilPressEng2.visible = false;
			}
			//
			if (_OilTempEng2 > 142)
			{
				OilTempEng2.visible = true;
			}
			else
			{
				OilTempEng2.visible = false;
			}
			//
			if (_ChipEngine2 < 2)
			{
				ChipEng2.visible = true;
			}
			else
			{
				ChipEng2.visible = false;
			}
			//
			if (_FuelFilterBypass2 < 6)
			{
				FuelFilterBypass2.visible = true;
			}
			else
			{
				FuelFilterBypass2.visible = false;
			}
			//
			if (_EngineStarter2 == true)
			{
				EngineStarter2.visible = true;
			}
			else
			{
				EngineStarter2.visible = false;
			}
			//
			if (_PriServoPress2 < 4.3)
			{
				PriServoPress2.visible = true;
			}
			else
			{
				PriServoPress2.visible = false;
			}
			//
			if (_TailRotorServo1 < 1)
			{
				TailRtrServo1.visible = true;
			}
			else
			{
				TailRtrServo1.visible = false;
			}
			//
			if (_ApuOilTempHigh > 87)
			{
				ApuOilTempHigh.visible = true;
			}
			else
			{
				ApuOilTempHigh.visible = false;
			}
			//
			if (_TrimFail < 80)
			{
				TrimFail.visible = true;
			}
			else
			{
				TrimFail.visible = false;
			}
			//
			if (_RightPitotHeat == true)
			{
				RightPitotHeat.visible = true;
			}
			else
			{
				RightPitotHeat.visible = false;
			}
			//
			if (_ChipInputMdlRh < 15.5)
			{
				ChipInputMdlRh.visible = true;
			}
			else
			{
				ChipInputMdlRh.visible = false;
			}
			//
			if (_ChipAccessMdlRh < 3.5)
			{
				ChipAccessMdlRh.visible = true;
			}
			else
			{
				ChipAccessMdlRh.visible = false;
			}
			//
			if (_IceDetected < -2)
			{
				IceDetected.visible = true;
			}
			else
			{
				IceDetected.visible = false;
			}
			//
			if (_BackupRsvrLow == false)
			{
				BackupRsvrLow.visible = true;
			}
			else
			{
				BackupRsvrLow.visible = false;
			}
			//
			if (_AntiIceEng2 == true)
			{
				AntiIceEng2.visible = true;
			}
			else
			{
				AntiIceEng2.visible = false;
			}
			//
			if (_BackupPumpOn < 30)
			{
				BackupPumpOn.visible = true;
			}
			else
			{
				BackupPumpOn.visible = false;
			}
			//
			if (_TlRtrServoOn == true)
			{
				TlRtrServoOn.visible = true;
			}
			else
			{
				TlRtrServoOn.visible = false;
			}
			//
			if (_MfdMalf2 == false)
			{
				MfdMalf2.visible = true;
			}
			else
			{
				MfdMalf2.visible = false;
			}
			
		}
	}
	
}
