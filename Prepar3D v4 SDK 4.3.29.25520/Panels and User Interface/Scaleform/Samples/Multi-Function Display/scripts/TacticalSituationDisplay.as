package  {
	
	import Prepar3D;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.getDefinitionByName;
	
	public class TacticalSituationDisplay extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;
		
		private var map_ranges:Array = new Array("0.25", "0.5", "1", "2.5", "5", "10", "25", "50", "100");
		private var map_current_range:int = 3;
		
		// CLASS PROPERTIES

		
		public function TacticalSituationDisplay() {
			
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
			Prepar3D.VarSet("L:LayerTerrain1", "number", 1);
			Prepar3D.VarSet("L:LayerCompass1", "number", 0);
			Prepar3D.VarSet("L:map_ZoomFactor1", "number", Number(map_ranges[map_current_range]) * 2); 
			
			try {
				var bmp:Bitmap = new Bitmap();
				var mytest:Class = getDefinitionByName("$MFD_Map_1") as Class;
				trace("created RTT: " + mytest);
				var bmpdata:BitmapData = new mytest(855, 590);
				bmp.bitmapData = bmpdata;
					
				TSD_Map.addChild(bmp);
			}
			catch (e:Error) {
				
			}

		}
		
		public function updateSymbolBrightness(newAlpha:Number):void
		{
			// here we would change the alpha property of all the symbols within this component.
		}
		

		public function update():void
		{
			// GET FROM PREPAR3D AIRCRAFT VARS
			
			var PlaneHeading_String:String;
			var PlaneHeading:int = Math.floor(Prepar3D.VarGet("A:PLANE HEADING DEGREES GYRO", "degrees"));
			
			if (PlaneHeading > 359)
			{
				PlaneHeading %= 360;
			}
			if (PlaneHeading < 0)
			{
				PlaneHeading = PlaneHeading * -1;
			}
			
			// we need to display the result string as a 3 digit integer
			if (PlaneHeading < 10)
			{
				PlaneHeading_String = "00" + String(PlaneHeading);
			}
			else if (PlaneHeading < 100)
			{
				PlaneHeading_String = "0" + String(PlaneHeading);
			}
			else
			{
				PlaneHeading_String = String(PlaneHeading);
			}
			//
			_this.planeHeading.text = PlaneHeading_String;
			
			_this.TSD_compass.rotation = 360 - PlaneHeading;  // need to rotate CCW
			
			// set Range string
			_this.map_Range.text = map_ranges[map_current_range];
			
		}
		
		public function toggle_Map():void
		{
			var terrain:Number = Prepar3D.VarGet("L:LayerTerrain1", "number");
			if (terrain == 0)
			{
				Prepar3D.VarSet("L:LayerTerrain1", "number", 1);
			}
			else
			{
				Prepar3D.VarSet("L:LayerTerrain1", "number", 0);
			}

		}
		
		public function toggle_Declutter():void
		{
			//_this.TSD_compass.visible = !_this.TSD_compass.visible;
			var aspace:Number = Prepar3D.VarGet("L:LayerAirspaces1", "number");
			if (aspace == 0)
			{
				Prepar3D.VarSet("L:LayerAirspaces1", "number", 1);
			}
			else
			{
				Prepar3D.VarSet("L:LayerAirspaces1", "number", 0);
			}
			
		}
		
		public function rangeUp():void
		{
			if (map_current_range < map_ranges.length -1)
			{
				map_current_range ++;
			}
			Prepar3D.VarSet("L:map_ZoomFactor1", "number", Number(map_ranges[map_current_range]) * 2); 
			
		}
		
		public function rangeDown():void
		{
			if (map_current_range > 0)
			{
				map_current_range --;
			}
			Prepar3D.VarSet("L:map_ZoomFactor1", "number", Number(map_ranges[map_current_range]) * 2); 
			
		}
		
		
	}
	
}
