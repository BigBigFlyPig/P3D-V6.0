package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class EICASScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		// variables to track EICAS data
		private var _eicas_circle_digit1:Number;  // formatted to one decimal place
		private var _eicas_circle_digit2:Number;  // formatted to one decimal place
		private var _eicas_circle_digit3:Number;  // formatted to one decimal place
		private var _eicas_circle_digit4:Number;  // formatted to one decimal place

		private var _eicas_halfCircle1_needle:int; // degrees
		private var _eicas_halfCircle2_needle:int; // degrees 
		private var _eicas_halfCircle3_needle:int; // degrees
		private var _eicas_halfCircle4_needle:int; // degrees

		private var _eicas_percent1:Number; // formatted to one decimal place
		private var _eicas_pph1:Number; // no decimal
		private var _eicas_lb1:Number; // no decimal

		private var _eicas_percent2:Number; // formatted to one decimal place
		private var _eicas_pph2:Number; // no decimal
		private var _eicas_lb2:Number; // no decimal

		private var _eicas_pressLeft:int; // no decimal
		private var _eicas_pressRight:int // no decimal
		private var _eicas_tempLeft:int // no decimal
		private var	_eicas_tempRight:int // no decimal

		private var	_eicas_cabAlt:Number;
		private var	_eicas_cabP:Number;
		private var	_eicas_cabRate:Number;
		private var	_eicas_cabAPU:Boolean;

		private var	_eicas_ldgGear1:int;  // up or dn
		private var	_eicas_ldgGear2:int;  // up or dn
		private var	_eicas_ldgGear3:int;  // up or dn
		private var	_eicas_flaps:Number;
		private var	_eicas_splrs:Boolean;

		private var	_eicas_pitchDir:Boolean;  // up or dn
		private var	_eicas_pitchNum:Number;
		
		private var _eicas_rollNum:Number;
		private var _eicas_yawNum:Number;

		private var	_pitch_arrow:Number;
		private var	_roll_arrow:Number;
		private var	_yaw_arrow:Number;
		private var	_oil_press_left_arrow:Number;
		private var	_oil_press_right_arrow:Number;
		private var	_oil_temp_left_arrow:Number;
		private var	_oil_temp_right_arrow:Number;
		private var	_vib_lp_left_arrow:Number;
		private var	_vib_lp_right_arrow:Number;
		private var	_vib_hp_left_arrow:Number;
		private var	_vib_hp_right_arrow:Number;
		
		private var _awarenessMessage:String;
		
		private var pitchArrowY:Number = 526;
		private var yawArrowX:Number = 295;
		
		public function EICASScreen() {
			
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
			//
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		
		public function update():void
		{
			// **** GET DATA FROM PREPAR3D VARIABLES  ****
			
			// N1 and ITT boxes
			_eicas_circle_digit1 = Prepar3D.VarGet("A:TURB ENG N1:1", "Percent");
			_eicas_circle_digit2 = Prepar3D.VarGet("A:TURB ENG CORRECTED N1:1", "Percent");
			_eicas_circle_digit3 = Prepar3D.VarGet("A:TURB ENG ITT:1", "Rankine");
			_eicas_circle_digit4 = Prepar3D.VarGet("A:TURB ENG ITT:2", "Rankine");
			
			// N2 percentages
			_eicas_percent1 = Prepar3D.VarGet("A:TURB ENG N2:1", "Percent");
			_eicas_percent2 = Prepar3D.VarGet("A:TURB ENG CORRECTED N2:1", "Percent");
			
			// FF (FUEL FLOW) PPH
			_eicas_pph1 = Prepar3D.VarGet("A:ENG FUEL FLOW PPH:1", "pounds per hour");
			_eicas_pph2 = Prepar3D.VarGet("A:ENG FUEL FLOW PPH:2", "pounds per hour");
			
			// FQ (FUEL QUANTITY) WEIGHT
			_eicas_lb1 = Prepar3D.VarGet("A:FUEL TOTAL QUANTITY WEIGHT", "pounds");
			_eicas_lb2 = Prepar3D.VarGet("A:FUEL TOTAL QUANTITY WEIGHT", "pounds");
			
			// OIL PRESSURE and TEMP
			_eicas_pressLeft = int(Prepar3D.VarGet("A:ENG OIL PRESSURE:1", "Psi"));
			_eicas_pressRight = int(Prepar3D.VarGet("A:ENG OIL PRESSURE:2", "Psi"));
			_eicas_tempLeft = int(Prepar3D.VarGet("A:ENG OIL TEMPERATURE:1", "Rankine"));
			_eicas_tempRight = int(Prepar3D.VarGet("A:ENG OIL TEMPERATURE:2", "Rankine"));
			
			// CABIN PRESSURIZATION
			_eicas_cabRate = Prepar3D.VarGet("A:PRESSURIZATION CABIN ALTITUDE RATE", "feet per second");
			_eicas_cabAlt = Prepar3D.VarGet("A:PRESSURIZATION CABIN ALTITUDE", "feet");
			_eicas_cabP = Prepar3D.VarGet("A:PRESSURIZATION PRESSURE DIFFERENTIAL", "Psi");
			_eicas_cabAPU = Prepar3D.VarGet("A:APU GENERATOR SWITCH", "bool");
			
			// LANDINF GEAR
			_eicas_ldgGear1 = Prepar3D.VarGet("A:GEAR POSITION:1", "enum");
			_eicas_ldgGear2 = Prepar3D.VarGet("A:GEAR POSITION:2", "enum");
			_eicas_ldgGear3 = Prepar3D.VarGet("A:GEAR POSITION:3", "enum");
			_eicas_flaps = Prepar3D.VarGet("A:FLAPS HANDLE PERCENT", "percent");
			_eicas_splrs = Prepar3D.VarGet("A:SPOILERS ARMED", "bool");
			
			// PITCH
			_eicas_pitchNum = -1 * (Prepar3D.VarGet("A:PLANE PITCH DEGREES", "Degrees"));

			// ROLL
			_eicas_rollNum = -1 * Prepar3D.VarGet("A:PLANE BANK DEGREES", "Degrees");
	
			// YAW
			_eicas_yawNum = Prepar3D.VarGet("A:INCIDENCE BETA", "Degrees");
			//_eicas_yawNum = Prepar3D.VarGet("A:RUDDER POSITION", "Position");
			
			if (_eicas_pitchNum > 0)
			{
				_eicas_pitchDir = true;
			}
			else
			{
				_eicas_pitchDir = false;
			}
			
			// USE FOR DEBUG
			//MovieClip(parent).setDebugTextDisplay("_eicas_yawNum= " + _eicas_yawNum);
			
			
			// **** SET SCREEN DISPLAY  ****
			
			// N1 boxes
			_this.eicas_circle_digit1.text = _eicas_circle_digit1.toFixed(1);
			_this.eicas_circle_digit2.text = _eicas_circle_digit2.toFixed(1);
			
			// N1 gages
			_eicas_halfCircle1_needle = Math.round( (_eicas_circle_digit1 * 2) - 24 );
			_this.eicas_halfCircle1.needle.rotation = _eicas_halfCircle1_needle;
			_eicas_halfCircle2_needle = Math.round( (_eicas_circle_digit2 * 2) - 24 );
			_this.eicas_halfCircle2.needle.rotation = _eicas_halfCircle2_needle;
			
			// covert Rankine to Celsius
			var mfd_EngOilTempC_1:Number;
			if (_eicas_circle_digit3 > 0)
			{
				mfd_EngOilTempC_1 = (_eicas_circle_digit3-491.67)*0.55555555;
			}
			else
			{
				mfd_EngOilTempC_1 = 0;
			}
			
			var mfd_EngOilTempC_2:Number;
			if (_eicas_circle_digit4 > 0)
			{
				mfd_EngOilTempC_2 = (_eicas_circle_digit4-491.67)*0.55555555;
			}
			else
			{
				mfd_EngOilTempC_2 = 0;
			}
			
			// ITT boxes
			_this.eicas_circle_digit3.text = mfd_EngOilTempC_1.toFixed(0);
			_this.eicas_circle_digit4.text = mfd_EngOilTempC_2.toFixed(0);
			
			// ITT gages
			_eicas_halfCircle3_needle = Math.round( ((mfd_EngOilTempC_1/10) * 2) - 24 );
			if (_eicas_halfCircle3_needle < -24)
			{
				_eicas_halfCircle3_needle = -24;
			}
			if (_eicas_halfCircle3_needle > 176)
			{
				_eicas_halfCircle3_needle = 176;
			}
			_this.eicas_halfCircle3.needle.rotation = _eicas_halfCircle3_needle;
			
			
			_eicas_halfCircle4_needle = Math.round( ((mfd_EngOilTempC_2/10) * 2) - 24 );
			if (_eicas_halfCircle4_needle < -24)
			{
				_eicas_halfCircle4_needle = -24;
			}
			if (_eicas_halfCircle4_needle > 176)
			{
				_eicas_halfCircle4_needle = 176;
			}
			_this.eicas_halfCircle4.needle.rotation = _eicas_halfCircle4_needle;
			
			
			// N2 percentages
			_this.eicas_percent1.text = _eicas_percent1.toFixed(1);
			_this.eicas_percent2.text = _eicas_percent2.toFixed(1);
			
			// FF (FUEL FLOW) PPH
			_this.eicas_pph1.text = _eicas_pph1.toFixed(0);
			_this.eicas_pph2.text = _eicas_pph2.toFixed(0);
			
			// FQ (FUEL QUANTITY) WEIGHT
			_this.eicas_lb1.text = _eicas_lb1.toFixed(0);
			_this.eicas_lb2.text = _eicas_lb2.toFixed(0);
			
			// OIL PRESSURE and TEMP
			_this.eicas_pressLeft.text = String(_eicas_pressLeft);
			_this.eicas_pressRight.text = String(_eicas_pressRight);
			//
			var eicas_TempLeft_1:Number;
			if (_eicas_tempLeft > 0)
			{
				eicas_TempLeft_1 = (_eicas_tempLeft-491.67)*0.55555555;
			}
			else
			{
				eicas_TempLeft_1 = 0;
			}
			var eicas_TempRight_1:Number;
			if (_eicas_tempRight > 0)
			{
				eicas_TempRight_1 = (_eicas_tempRight-491.67)*0.55555555;
			}
			else
			{
				eicas_TempRight_1 = 0;
			}
			_this.eicas_tempLeft.text = eicas_TempLeft_1.toFixed(0);
			_this.eicas_tempRight.text = eicas_TempRight_1.toFixed(0);
			
			// CABIN PRESSURIZATION
			_this.eicas_cabAlt.text = _eicas_cabAlt.toFixed(0);
			_this.eicas_cabP.text = _eicas_cabP.toFixed(1);
			_this.eicas_cabRate.text = (_eicas_cabRate*60).toFixed(0);  // need feet per minute
			if (_eicas_cabAPU == true) 
			{
				_this.eicas_cabAPU.text = "ON";
			} 
			else 
			{
				_this.eicas_cabAPU.text = "OFF";
			}
			
			// LANDINF GEAR
			if (_eicas_ldgGear1 == 1)
			{
				_this.eicas_ldgGear1.text = "UP";
			}
			else if (_eicas_ldgGear1 == 2)
			{
				_this.eicas_ldgGear1.text = "DN";
			}
			else
			{
				_this.eicas_ldgGear1.text = "UN";
			}
			
			if (_eicas_ldgGear2 == 1)
			{
				_this.eicas_ldgGear2.text = "UP";
			}
			else if (_eicas_ldgGear2 == 2)
			{
				_this.eicas_ldgGear2.text = "DN";
			}
			else
			{
				_this.eicas_ldgGear2.text = "UN";
			}
			
			if (_eicas_ldgGear3 == 1)
			{
				_this.eicas_ldgGear3.text = "UP";
			}
			else if (_eicas_ldgGear3 == 2)
			{
				_this.eicas_ldgGear3.text = "DN";
			}
			else
			{
				_this.eicas_ldgGear3.text = "UN";
			}
			
			_this.eicas_flaps.text = _eicas_flaps.toFixed(0);
			
			if (_eicas_splrs == true)
			{
				_this.eicas_splrs.text = "ARM";
			}
			else
			{
				_this.eicas_splrs.text = "CLD";
			}
			
			
			// PITCH
			_this.eicas_pitchNum.text = Math.round(_eicas_pitchNum);
			
			if (_eicas_pitchDir == true)
			{	
				_this.eicas_pitchDir.text = "UP";
			}
			else
			{
				_this.eicas_pitchDir.text = "DN";
			}
			
			// PITCH ARROW
			var pitchTemp:Number = Math.round(pitchArrowY - _eicas_pitchNum/4.5);
			if (pitchTemp > 566)
			{
				pitchTemp = 566;
			}
			else if (pitchTemp < 486)
			{
				pitchTemp = 486;
			}
			_this.pitch_arrow.y = pitchTemp;
			
			// YAW ARROW
			var yawTemp = Math.round(yawArrowX + _eicas_yawNum * 40);
			if (yawTemp > 335)
			{
				yawTemp = 335;
			}
			else if (yawTemp < 255)
			{
				yawTemp = 255;
			}
			_this.yaw_arrow.x = yawTemp;
			
			// ROLL ARROW
			var rollTemp = _eicas_rollNum * 1.5 ; // my scale is from -44 to 44 degrees
			if (rollTemp > 44)
			{
				rollTemp = 44;
			}
			else if (rollTemp < -44)
			{
				rollTemp = -44;
			}
			_this.roll_arrow.rotation = rollTemp;
			
			// here is where you would add warning/caution/advisory message text
			// _this.awarenessMessage.text = _awarenessMessage;
		}
	}
	
}
