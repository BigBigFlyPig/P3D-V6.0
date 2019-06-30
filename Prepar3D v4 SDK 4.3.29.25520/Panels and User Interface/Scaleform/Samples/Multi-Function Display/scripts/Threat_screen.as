package  {
	
	import Prepar3D;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.getDefinitionByName;
	
	public class Threat_screen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var map_ranges:Array = new Array("0.25", "0.5", "1", "2.5", "5", "10", "25", "50", "100");
		private var map_current_range:int = 3;
		
		// CLASS PROPERTIES

		
		public function Threat_screen() {
			
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
			// init values
			Prepar3D.VarSet("L:map_ZoomFactor2", "number", Number(map_ranges[map_current_range]) * 2); 
			
			// set some demo stuff
			RL_mws.gotoAndStop(1);
			RR_lws.gotoAndStop(2);
			
			
			try {
				var bmp:Bitmap = new Bitmap();
				var mytest:Class = getDefinitionByName("$MFD_Map_2") as Class;
				trace("created RTT: " + mytest);
				var bmpdata:BitmapData = new mytest(476, 476);
				bmp.bitmapData = bmpdata;
					
				TWD_Map.addChild(bmp);
			}
			catch (e:Error) {
				
			}	
			
			// set Range string
			_this.map_Range.text = map_ranges[map_current_range];
			
		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		

		public function update():void
		{
			// GET FROM PREPAR3D AIRCRAFT VARS
			
			var _flare_quantity:Number;
			var _chaff_quantity:Number;
			
			// Flares (station 0)
			_flare_quantity = int(Prepar3D.VarGet("A:COUNTERMEASURE SYSTEM STATION OBJECT COUNT:0", "number"));
			_this.chaff_count_left.text = appendZero(_flare_quantity);
			
			// Chaffs (station 1)
			_chaff_quantity = int(Prepar3D.VarGet("A:COUNTERMEASURE SYSTEM STATION OBJECT COUNT:1", "number"));
			_this.flare_count_left.text = appendZero(_chaff_quantity);	
			
			_this.chaff_count_left.text = 0;
			_this.flare_count_left.text = 0;
			
					
		}
		
		public function cycle_Program():void
		{
			var newVal:String = "AUTO";
			
			switch (_this.cmds_program.text)
			{
				case "AUTO":
					newVal = "1";
					break;
				case "1":
					newVal = "2";
					break;
				case "2":
					newVal = "3";
					break;
				case "3":
					newVal = "4";
					break;
				case "4":
					newVal = "AUTO";
					break;
					
			}
			_this.cmds_program.text = newVal;
		}
		
		public function rangeUp():void
		{
			if (map_current_range < map_ranges.length -1)
			{
				map_current_range ++;
			}
			Prepar3D.VarSet("L:map_ZoomFactor2", "number", Number(map_ranges[map_current_range]) * 2); 
			// set Range string
			_this.map_Range.text = map_ranges[map_current_range];
			
		}
		
		public function rangeDown():void
		{
			if (map_current_range > 0)
			{
				map_current_range --;
			}
			Prepar3D.VarSet("L:map_ZoomFactor2", "number", Number(map_ranges[map_current_range]) * 2); 
			// set Range string
			_this.map_Range.text = map_ranges[map_current_range];
			
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
		
		
	}
	
}
