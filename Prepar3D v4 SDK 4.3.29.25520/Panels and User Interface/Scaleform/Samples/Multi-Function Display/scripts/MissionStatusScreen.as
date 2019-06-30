package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	
	public class MissionStatusScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		// CREATE A VARIABLE FOR EACH STATUS ELEMENT
		// CHANGE, ADD, OR DELETE VARIABLES TO MATCH SPECIFIC AIRCRAFT STATUS
		private var _engine_go:Boolean;
		private var _cas_go:int;
		private var _eps_go:Boolean;
		private var _fcs_go:Boolean;
		private var _fps_go:Boolean;
		private var _fuel_go:int;
		private var _gear_go:Boolean;
		private var _hyd_go:Boolean;
		private var _lift_go:Boolean;
		private var _ptms_go:Boolean;
		private var _vsp_go:Boolean;
		private var _ews_go:Boolean;
		private var _com_go:int;
		private var _cni_go:Boolean;
		private var _das_go:Boolean;
		private var _eots_go:Boolean;
		private var _ew_go:Boolean;
		private var _gps_go:Boolean;
		private var _gun_go:Boolean;
		private var _icp_go:Boolean;
		private var _ins_go:Boolean;
		private var _pvi_go:Boolean;
		private var _radar_go:Boolean;
		private var _sms_go:Boolean;
		
		public function MissionStatusScreen() {
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
			// HERE IS WHERE YOU GET THE STATUS FROM PREPAR3D
			// NOTE: IN MY EXAMPLE, ONLY HAVE SOME PREPAR3D GET'S; ALL OTHERS ARE HARD-CODED
			//       FOR NOW TO TRUE.
				
			_engine_go = Prepar3D.VarGet("A:GENERAL ENG FAILED:1", "bool");
			_cas_go = Prepar3D.VarGet("A:PARTIAL PANEL AIRSPEED", "enum");
			_eps_go = true;
			_fcs_go = true;
			_fps_go = true;
			_fuel_go = Prepar3D.VarGet("A:PARTIAL PANEL FUEL INDICATOR", "enum");
			_gear_go = Prepar3D.VarGet("A:IS GEAR RETRACTABLE", "bool");
			_hyd_go = Prepar3D.VarGet("A:HYDRAULIC SWITCH", "bool");
			_lift_go = true;
			_ptms_go = true;
			_vsp_go = true;
			_ews_go = true;
			_com_go = Prepar3D.VarGet("A:COM STATUS", "enum");
			_cni_go = true;
			_das_go = true;
			_eots_go = true;
			_ew_go = true;
			_gps_go = Prepar3D.VarGet("A:GPS DRIVES NAV1", "bool");
			_gun_go = Prepar3D.VarGet("A:GUN SYSTEM IS ACTIVE", "bool");
			_icp_go = true;
			_ins_go = true;
			_pvi_go = true;
			_radar_go = true;
			_sms_go = true;
			
			// USE FOR DEBUG
			//MovieClip(parent).setDebugTextDisplay("engine= " + _engine_go + "\nradar= " + _radar_go);
			
			// NEXT, WE NEED TO UPDATE THE SCREEN DISPLAY
			// IF 'GO' SET COLOR OF TEXT TO GREEN (0x00FF00)
			// IF 'NO GO' SET COLOR OF TEXT TO RED (0xFF0000)
			
			//1 - since this is a FAIL flag, if false, then engine is good
			if (_engine_go == false)
			{
				_this.engine_go.textColor = 0x00FF00;
				_this.engine_go.text = "GO";
			}
			else
			{
				_this.engine_go.textColor = 0xFF0000;
				_this.engine_go.text = "NO GO";
			}
			
			//2
			if (_cas_go == 0)
			{
				_this.cas_go.textColor = 0x00FF00;
				_this.cas_go.text = "GO";
			}
			else
			{
				_this.cas_go.textColor = 0xFF0000;
				_this.cas_go.text = "NO GO";
			}
			
			//3
			if (_eps_go == true)
			{
				_this.eps_go.textColor = 0x00FF00;
				_this.eps_go.text = "GO";
			}
			else
			{
				_this.eps_go.textColor = 0xFF0000;
				_this.eps_go.text = "NO GO";
			}
			
			//4
			if (_fcs_go == true)
			{
				_this.fcs_go.textColor = 0x00FF00;
				_this.fcs_go.text = "GO";
			}
			else
			{
				_this.fcs_go.textColor = 0xFF0000;
				_this.fcs_go.text = "NO GO";
			}
			
			//5
			if (_fps_go == true)
			{
				_this.fps_go.textColor = 0x00FF00;
				_this.fps_go.text = "GO";
			}
			else
			{
				_this.fps_go.textColor = 0xFF0000;
				_this.fps_go.text = "NO GO";
			}
			
			//6
			if (_fuel_go == 0)
			{
				_this.fuel_go.textColor = 0x00FF00;
				_this.fuel_go.text = "GO";
			}
			else
			{
				_this.fuel_go.textColor = 0xFF0000;
				_this.fuel_go.text = "NO GO";
			}
			
			//7
			if (_gear_go == true)
			{
				_this.gear_go.textColor = 0x00FF00;
				_this.gea_go.text = "GO";
			}
			else
			{
				_this.gear_go.textColor = 0xFF0000;
				_this.gear_go.text = "NO GO";
			}
			
			//8
			if (_hyd_go == true)
			{
				_this.hyd_go.textColor = 0x00FF00;
				_this.hyd_go.text = "GO";
			}
			else
			{
				_this.hyd_go.textColor = 0xFF0000;
				_this.hyd_go.text = "NO GO";
			}
			
			//9
			if (_lift_go == true)
			{
				_this.lift_go.textColor = 0x00FF00;
				_this.lift_go.text = "GO";
			}
			else
			{
				_this.lift_go.textColor = 0xFF0000;
				_this.lift_go.text = "NO GO";
			}
			
			//10
			if (_ptms_go == true)
			{
				_this.ptms_go.textColor = 0x00FF00;
				_this.ptms_go.text = "GO";
			}
			else
			{
				_this.ptms_go.textColor = 0xFF0000;
				_this.ptms_go.text = "NO GO";
			}
			
			//11
			if (_vsp_go == true)
			{
				_this.vsp_go.textColor = 0x00FF00;
				_this.vsp_go.text = "GO";
			}
			else
			{
				_this.vsp_go.textColor = 0xFF0000;
				_this.vsp_go.text = "NO GO";
			}
			
			//12
			if (_ews_go == true)
			{
				_this.ews_go.textColor = 0x00FF00;
				_this.ews_go.text = "GO";
			}
			else
			{
				_this.ews_go.textColor = 0xFF0000;
				_this.ews_go.text = "NO GO";
			}
			
			//13
			if (_com_go == 0)
			{
				_this.com_go.textColor = 0x00FF00;
				_this.com_go.text = "GO";
			}
			else
			{
				_this.com_go.textColor = 0xFF0000;
				_this.com_go.text = "NO GO";
			}
			
			//14
			if (_cni_go == true)
			{
				_this.cni_go.textColor = 0x00FF00;
				_this.cni_go.text = "GO";
			}
			else
			{
				_this.cni_go.textColor = 0xFF0000;
				_this.cni_go.text = "NO GO";
			}
			
			//15
			if (_das_go == true)
			{
				_this.das_go.textColor = 0x00FF00;
				_this.das_go.text = "GO";
			}
			else
			{
				_this.das_go.textColor = 0xFF0000;
				_this.das_go.text = "NO GO";
			}
			
			//16
			if (_eots_go == true)
			{
				_this.eots_go.textColor = 0x00FF00;
				_this.eots_go.text = "GO";
			}
			else
			{
				_this.eots_go.textColor = 0xFF0000;
				_this.eots_go.text = "NO GO";
			}
			
			//17
			if (_ew_go == true)
			{
				_this.ew_go.textColor = 0x00FF00;
				_this.ew_go.text = "GO";
			}
			else
			{
				_this.ew_go.textColor = 0xFF0000;
				_this.ew_go.text = "NO GO";
			}
			
			//18
			if (_gps_go == true)
			{
				_this.gps_go.textColor = 0x00FF00;
				_this.gps_go.text = "GO";
			}
			else
			{
				_this.gps_go.textColor = 0xFF0000;
				_this.gps_go.text = "NO GO";
			}
			
			//19
			if (_gun_go == true)
			{
				_this.gun_go.textColor = 0x00FF00;
				_this.gun_go.text = "GO";
			}
			else
			{
				_this.gun_go.textColor = 0xFF0000;
				_this.gun_go.text = "NO GO";
			}
			
			//20
			if (_icp_go == true)
			{
				_this.icp_go.textColor = 0x00FF00;
				_this.icp_go.text = "GO";
			}
			else
			{
				_this.icp_go.textColor = 0xFF0000;
				_this.icp_go.text = "NO GO";
			}
			
			//21
			if (_ins_go == true)
			{
				_this.ins_go.textColor = 0x00FF00;
				_this.ins_go.text = "GO";
			}
			else
			{
				_this.ins_go.textColor = 0xFF0000;
				_this.ins_go.text = "NO GO";
			}
			
			//22
			if (_pvi_go == true)
			{
				_this.pvi_go.textColor = 0x00FF00;
				_this.pvi_go.text = "GO";
			}
			else
			{
				_this.pvi_go.textColor = 0xFF0000;
				_this.pvi_go.text = "NO GO";
			}
			
			//23
			if (_radar_go == true)
			{
				_this.radar_go.textColor = 0x00FF00;
				_this.radar_go.text = "GO";
			}
			else
			{
				_this.radar_go.textColor = 0xFF0000;
				_this.engine_go.text = "NO GO";
			}
			
			//24
			if (_sms_go == true)
			{
				_this.sms_go.textColor = 0x00FF00;
				_this.sms_go.text = "GO";
			}
			else
			{
				_this.sms_go.textColor = 0xFF0000;
				_this.sms_go.text = "NO GO";
			}
			
		}
	}
	
}
