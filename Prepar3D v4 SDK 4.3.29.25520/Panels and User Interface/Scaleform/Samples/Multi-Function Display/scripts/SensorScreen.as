package  {
	
	import Prepar3D;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.getDefinitionByName;
	import flash.utils.*;
	
	public class SensorScreen extends MovieClip {
		
		private var initialized:Boolean;
		private var _this = this;

		const BORDER_BUFFER:int = 24;
		const METERS_TO_FEET:Number = 3.28084;
		
		const HALF_HORIZONTAL_TICK_WIDTH:int = 701; // 1402 / 2
		const HALF_VERTICAL_TICK_HEIGHT:int= 252; // 504 / 2
		
		private var m_targetLat:Number;
		private var m_targetLon:Number;
		private var m_targetAlt:Number;
		private var m_targetDis:int;
		
		private var timer:Number;
		
		// CLASS PROPERTIES

		
		
		public function SensorScreen() {
			
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
			
			try {
				var bmp:Bitmap = new Bitmap();
				var mytest:Class = getDefinitionByName("TargetingPodView") as Class;
				var bmpdata:BitmapData = new mytest(915, 597);
				trace("created RTT: " + mytest);
				bmp.bitmapData = bmpdata;
					
				Sensor_Cam.addChild(bmp);
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

			UpdateTime();
			
			UpdateDate();
			
			UpdateSensorModeText();
			
			UpdatePitchHeadingCarets();
			
			UpdateOwnshipTargetInfoText();	
		}
		
		// call key events to control camera
		
		public function cycleMode():void
		{
			Prepar3D.VarSet("K:HOTAS_KEY_A0", "number", 1);
		}
		
		public function tiltUp():void
		{
			Prepar3D.VarSet("K:HOTAS_SLEW_AXIS_Y", "number", -10000);
			timer = setInterval(resetMove, 50);
		}
		
		public function tiltDown():void
		{	
			Prepar3D.VarSet("K:HOTAS_SLEW_AXIS_Y", "number", 10000);
			timer = setInterval(resetMove, 50);
		}
		
		public function panRight():void
		{
			Prepar3D.VarSet("K:HOTAS_SLEW_AXIS_X", "number", -10000);
			timer = setInterval(resetMove, 50);
		}
		
		public function panLeft():void
		{
			Prepar3D.VarSet("K:HOTAS_SLEW_AXIS_X", "number", 10000);
			timer = setInterval(resetMove, 50);
		}

		public function zoomIn():void
		{
			Prepar3D.VarSet("K:HOTAS_KEY_A2", "number", 1);
		}
		
		public function zoomOut():void
		{
			Prepar3D.VarSet("K:HOTAS_KEY_A3", "number", 1);
		}
		
		public function setLock():void
		{
			Prepar3D.VarSet("K:HOTAS_KEY_A1", "number", 1);
		}
		
		private function resetMove():void
		{
			clearInterval(timer);
			Prepar3D.VarSet("K:HOTAS_SLEW_AXIS_X", "number", 0);
			Prepar3D.VarSet("K:HOTAS_SLEW_AXIS_Y", "number", 0);
		}

		
		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// DEG_TO_RAD
		//
		// Utility function to convert degrees to radians
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function DEG_TO_RAD(degrees:Number):Number
		{
			return degrees * (Math.PI / 180.0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// UpdateTime
		//
		// Time is shown in bottom right
		// Synced with current Prepar3D time
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function UpdateTime()
		{
			var seconds:int = Prepar3D.VarGet("E:LOCAL TIME", "seconds");
			var minutes:int = seconds / 60;
			var hours:int = minutes / 60;
			
			seconds = seconds % 60;
			minutes = minutes % 60;
			
			var secondsString:String = "";
			if (seconds < 10)
			{
				secondsString = "0" + seconds.toString();
			}
			else
			{
				secondsString = seconds.toString();
			}
			
			var minutesString:String = "";
			if (minutes < 10)
			{
				minutesString = "0" + minutes.toString();
			}
			else
			{
				minutesString = minutes.toString();
			}
			
			var hoursString:String = "";
			if (hours < 10)
			{
				hoursString = "0" + hours.toString();
			}
			else
			{
				hoursString = hours.toString();
			}
			
			timeText.text = hoursString + ":" + minutesString + ":" + secondsString;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// UpdateDate
		//
		// Date is shown in bottom right
		// Synced with current Prepar3D date
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function UpdateDate()
		{
			var day:int = Prepar3D.VarGet("E:LOCAL DAY OF MONTH", "number");
			var month:int = Prepar3D.VarGet("E:LOCAL MONTH OF YEAR", "number");
			var year:int = Prepar3D.VarGet("E:LOCAL YEAR", "number");
			
			var dayString:String = (day < 10 ? "0" + day.toString() : day.toString());
			var monthString:String = (month < 10 ? "0" + month.toString() : month.toString());
			var yearString:String = year.toString();
			
			dateText.text = monthString + "-" + dayString + "-" + yearString;
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// UpdateSensorModeText
		//
		// Show sensor mode if active (IR White Hot or IR Black Hot)
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function UpdateSensorModeText()
		{
			var sensorMode:Number = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetSensorMode", "number");
			if (sensorMode == 1)
			{
				sensorModeText.text = "WHOT";
				IRModeText.visible = true;
			}
			else if (sensorMode == 2)
			{
				sensorModeText.text = "BHOT";
				IRModeText.visible = true;
			}
			else
			{
				sensorModeText.text = " ";
				IRModeText.visible = false;
			}
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// UpdatePitchHeadingCarets
		//
		// Moves the carets along horizontal and vertical tick marks
		// Converts pitch and heading values to a percentage
		// Updates text with exact pitch and heading values (rounded up)
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function UpdatePitchHeadingCarets()
		{
			var pitch:Number = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetPitch", "number");
			var percentY:Number = (pitch / 90.0);
			verticalTicks.caret.y = percentY * HALF_VERTICAL_TICK_HEIGHT; // Half of design height
			
			var pitchInt:int = Math.round(pitch);
			if (pitchInt > 0)
			{
				offsetTextY.text = "-" + pitchInt;
			}
			else if (pitchInt < 0)
			{
				offsetTextY.text = "+" + Math.abs(pitchInt);
			}
			else
			{
				offsetTextY.text = " " + pitchInt;
			}
			
			var acHeading:Number = Prepar3D.VarGet("A:PLANE HEADING DEGREES TRUE", "degrees");
			var heading:Number = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetHeading", "number");
			var headingDiff:Number = heading - acHeading;
			var percentX:Number = (NormalizeHeading(headingDiff) / 180.0)
			horizontalTicks.caret.x = percentX * HALF_HORIZONTAL_TICK_WIDTH; // Half of design width
			
			var headingDiffInt:int = Math.round(NormalizeHeading(headingDiff));
			if (headingDiffInt > 0)
			{
				offsetTextX.text = "+" + headingDiffInt;
			}
			else
			{
				offsetTextX.text = "" + headingDiffInt;
			}
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// UpdateOwnshipTargetInfoText
		//
		// Update text fields that show the ownship LLA and current target's LLA and distance
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function UpdateOwnshipTargetInfoText()
		{
			// Ownship lat, lon, alt
			var ownLat:Number = Prepar3D.VarGet("A:PLANE LATITUDE", "degrees");
			var ownLon:Number = Prepar3D.VarGet("A:PLANE LONGITUDE", "degrees");
			var ownAlt:Number = Prepar3D.VarGet("A:PLANE ALTITUDE", "feet");
			
			ownshipLatText.text = GetLatLonString(Math.abs(ownLat), false);
			ownshipLonText.text = GetLatLonString(Math.abs(ownLon), true);
			ownshipAltText.text = "ALT " + Math.round(ownAlt).toString();
		
			// Target lat, lon, alt, distance
			var isPosTrack:int = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:IsPositionTracking", "Number");
			var isEntTrack:int = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:IsEntityTracking", "Number");
			if (isPosTrack || isEntTrack)
			{
				m_targetLat = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetTargetLat", "Number");
				m_targetLon = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetTargetLon", "Number");
				m_targetAlt = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetTargetAlt", "Number") * METERS_TO_FEET;
			}
			
			if (m_targetLat != 0 && m_targetLon != 0)
			{
				var ownLatRad:Number = DEG_TO_RAD(ownLat);
				var ownLonRad:Number = DEG_TO_RAD(ownLon);
				var targetLatRad:Number = DEG_TO_RAD(m_targetLat);
				var targetLonRad:Number = DEG_TO_RAD(m_targetLon);
				
				var equiX:Number = (targetLonRad - ownLonRad) * (Math.cos(targetLatRad + ownLatRad) * 0.5);
				var equiY:Number = targetLatRad - ownLatRad;
				var equiD:Number = Math.sqrt((equiX * equiX) + (equiY * equiY)) * 6371000;
				m_targetDis = Math.floor(equiD);
			}
			else
			{
				m_targetLat = 0;
				m_targetLon = 0;
				m_targetDis = 0;
			}
			
			targetLatText.text = GetLatLonString(Math.abs(m_targetLat), false);
			targetLonText.text = GetLatLonString(Math.abs(m_targetLon), true);
			targetAltText.text = "ALT " + Math.round(m_targetAlt).toString();
			targetDisText.text = "DIS " + m_targetDis.toString();
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// GetLatLonString
		//
		// Utility function to convert lat and lon into a string with direction prefixed
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function GetLatLonString(ll:Number, isLon:Boolean) : String
		{
			var prefix:String;
			if (ll >= 0 && !isLon)
			{
				prefix = "N0";
			}
			else if (ll < 0 && !isLon)
			{
				prefix = "S0";
			}
			else if (ll >= 0 && isLon)
			{
				prefix = "E";
			}
			else
			{
				prefix = "W";
			}
				
			ll = Math.abs(ll);
			
			var llDegreesInt:int = Math.floor(ll);
			ll -= llDegreesInt;
			
			var llMinutes:Number = ll * 60;
			var llMinutesInt:int = Math.floor(llMinutes);
			var llMinutesString;
			if (llMinutesInt < 10)
			{
				llMinutesString = "0" + llMinutesInt.toString();
			}
			else
			{
				llMinutesString = llMinutesInt.toString();
			}
			
			var llSeconds:Number = (llMinutes - llMinutesInt) * 60;
			var llSecondsInt:int = Math.floor(llSeconds);
			var llSecondsString;
			if (llSecondsInt < 10)
			{
				llSecondsString = "0" + llSecondsInt.toString();
			}
			else
			{
				llSecondsString = llSecondsInt.toString();
			}

			var llDegreesString:String = llDegreesInt.toString();
			if (isLon && llDegreesInt < 100)
			{
				llDegreesString = "0" + llDegreesString;
			}
			if (llDegreesInt < 10)
			{
				llDegreesString = "0" + llDegreesString;
			}
			
			return prefix + llDegreesString + ":" + llMinutesString + ":" + llSecondsString;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// NormalizeHeading
		//
		// Utility function to keep heading between -180 and 180
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function NormalizeHeading(heading:Number) : Number
		{
			if (heading > 180)
			{
				heading -= 360;
			}
			else if (heading < -180)
			{
				heading += 360;
			}
			return heading;
		}		
	
		
	}
}
