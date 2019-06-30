package  {
	
	import Prepar3D;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class CommsScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var _selectionPosition:Number = 0;
		
		// CLASS PROPERTIES

		
		public function CommsScreen() {
			
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
			
			_this.upArrow.visible = false;
			if (Prepar3D.VarGet("L:TRANSPONDER STANDBY CODE", "number") == 0)
			{
				Prepar3D.VarSet("L:TRANSPONDER STANDBY CODE", "number", 323);
			}
			
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		

		public function update():void
		{
			// GET FROM PREPAR3D AIRCRAFT VARS
			
			_this.com1_selected.text = (Prepar3D.VarGet("A:COM ACTIVE FREQUENCY:1", "Frequency BCD16") / 100).toFixed(3);
			_this.com1_standby.text = (Prepar3D.VarGet("A:COM STANDBY FREQUENCY:1", "Frequency BCD16") / 100).toFixed(3);
			_this.com2_selected.text = (Prepar3D.VarGet("A:COM ACTIVE FREQUENCY:2", "Frequency BCD16") / 100).toFixed(3);
			_this.com2_standby.text = (Prepar3D.VarGet("A:COM STANDBY FREQUENCY:2", "Frequency BCD16") / 100).toFixed(3);

			_this.nav1_selected.text = Prepar3D.VarGet("A:NAV ACTIVE FREQUENCY:1", "MHz").toFixed(2);
			_this.nav1_standby.text = Prepar3D.VarGet("A:NAV STANDBY FREQUENCY:1", "MHz").toFixed(2);
			_this.nav2_selected.text = Prepar3D.VarGet("A:NAV ACTIVE FREQUENCY:2", "MHz").toFixed(2);
			_this.nav2_standby.text = Prepar3D.VarGet("A:NAV STANDBY FREQUENCY:2", "MHz").toFixed(2);

			_this.adf_selected.text = Prepar3D.VarGet("A:ADF ACTIVE FREQUENCY:1", "Hz").toFixed(1);
			_this.adf_standby.text = Prepar3D.VarGet("A:ADF STANDBY FREQUENCY:1", "Hz").toFixed(1);
			
			_this.atc_selected.text = Prepar3D.VarGet("A:TRANSPONDER CODE:1","BCO16").toFixed(0);
			_this.atc_standby.text = Prepar3D.VarGet("L:TRANSPONDER STANDBY CODE", "number").toFixed(0);
	
		}
		
		public function arrowDown():void
		{
			_selectionPosition ++;
			if (_selectionPosition > 5)
			{
				_selectionPosition = 5;
			}
			displayArrows();
			_this.comm_Selection.y = 200 + _selectionPosition * 50;
		}
		
		public function arrowUp():void
		{
			_selectionPosition --;
			if (_selectionPosition < 0)
			{
				_selectionPosition = 0;
			}
			displayArrows();
			_this.comm_Selection.y = 200 + _selectionPosition * 50;
		}
		
		private function displayArrows()
		{
			_this.upArrow.visible = true;
			_this.downArrow.visible = true;
			
			if (_selectionPosition == 5)
			{
				_this.downArrow.visible = false;
			}
			if (_selectionPosition == 0)
			{
				_this.upArrow.visible = false;
			}
		}
		
		public function swapValues()
		{
			switch (_selectionPosition)
			{
				case 0:  //COM1
					Prepar3D.VarSet("K:COM_STBY_RADIO_SWAP", "number", 1);
					break;
				case 1:  //COM2
					Prepar3D.VarSet("K:COM2_RADIO_SWAP", "number", 1);
					break;
				case 2:  //NAV1
					Prepar3D.VarSet("K:NAV1_RADIO_SWAP", "number", 1);
					break;
				case 3:  //NAV2
					Prepar3D.VarSet("K:NAV2_RADIO_SWAP", "number", 1);
					break;
				case 4:  //ADF
					var adf:Number = Prepar3D.VarGet("A:ADF ACTIVE FREQUENCY:1", "Hz");
					var adfstby:Number = Prepar3D.VarGet("A:ADF STANDBY FREQUENCY:1", "Hz");
					Prepar3D.VarSet("K:ADF1_PRIMARY_SET", "Hz", adfstby);
					Prepar3D.VarSet("K:ADF1_STBY_SET ", "Hz", adf);	
					break;
				case 5:  //ATC
					//var atc:Number = Prepar3D.VarGet("A:TRANSPONDER CODE:1", "BCO16");
					//var atcstby:Number = Prepar3D.VarGet("L:TRANSPONDER STANDBY CODE", "BCO16");				
					//Prepar3D.VarSet("K:XPNDR_SET", "BCO16", atcstby);
					//Prepar3D.VarSet("L:TRANSPONDER STANDBY CODE", "BCO16", atc);					
					break;
				
			}
		}
		
	}
}
