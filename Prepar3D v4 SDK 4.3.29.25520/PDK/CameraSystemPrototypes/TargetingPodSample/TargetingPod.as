package
{	
	import flash.events.Event;
	import Prepar3D;
	import flash.display.Shape;
	import flash.geom.Point;
	import flash.geom.Matrix;
	import fl.motion.MatrixTransformer;
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.display.Stage;
	
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// Document Class
	//
	// Vast majority of work is done here
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	public class TargetingPod extends MovieClip
	{
		// Change design size to match desired aspect ratio
		const DESIGN_SIZE_X:int = 1920;
		const DESIGN_SIZE_Y:int = 1920;
		
		const BORDER_BUFFER:int = 24;
		const METERS_TO_FEET:Number = 3.28084;
		
		const HALF_HORIZONTAL_TICK_WIDTH:int = 701; // 1402 / 2
		const HALF_VERTICAL_TICK_HEIGHT:int= 252; // 504 / 2
		
		// Vars passed in from .fla
		var m_stage:Stage;
		var m_crosshair:MovieClip;
		var m_timeText:TextField;
		var m_dateText:TextField;
		var m_sensorModeText:TextField;
		var m_ownshipLabel:TextField;
		var m_ownshipLatText:TextField;
		var m_ownshipLonText:TextField;
		var m_ownshipAltText:TextField;
		var m_targetLabel:TextField;
		var m_targetLatText:TextField;
		var m_targetLonText:TextField;
		var m_targetAltText:TextField;
		var m_targetDisText:TextField;
		var m_horizontalTicks:MovieClip;
		var m_verticalTicks:MovieClip;
		var	m_offsetTextX:TextField;
		var	m_offsetTextY:TextField;
		var	m_v1Text:TextField;
		var	m_v2Text:TextField;
		var	m_v3Text:TextField;
		var	m_v4Text:TextField;
		var	m_v5Text:TextField;
		var	m_v6Text:TextField;
		var	m_v7Text:TextField;
		var	m_h1Text:TextField;
		var	m_h2Text:TextField;
		var	m_h3Text:TextField;
		var	m_h4Text:TextField;
		var	m_h5Text:TextField;
		var m_targetLat:Number;
		var m_targetLon:Number;
		var m_targetAlt:Number;
		var m_targetDis:int;

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// TargetingPod
		//
		// Empty Constructor
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		public function TargetingPod()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// InitReticle
		//
		// Takes in all text fields and movie clips from .fla
		// Sets these to member vars within document class
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		public function InitReticle(
			rootStage:Stage,
			crosshair:MovieClip,
			timeText:TextField,
			dateText:TextField,
			sensorModeText:TextField,
		    ownshipLabel:TextField,
			ownshipLatText:TextField,
			ownshipLonText:TextField,
			ownshipAltText:TextField,
			targetLabel:TextField,
			targetLatText:TextField,
			targetLonText:TextField,
			targetAltText:TextField,
			targetDisText:TextField,
			horizontalTicks:MovieClip,
			verticalTicks:MovieClip,
			offsetTextX:TextField,
			offsetTextY:TextField,
			h1Text:TextField,
			h2Text:TextField,
			h3Text:TextField,
			h4Text:TextField,
			h5Text:TextField,
			v1Text:TextField,
			v2Text:TextField,
			v3Text:TextField,
			v4Text:TextField,
			v5Text:TextField,
			v6Text:TextField,
			v7Text:TextField
		)
		{
			m_stage = rootStage;
			m_crosshair = crosshair;
			m_timeText = timeText;
			m_dateText = dateText;
			m_sensorModeText = sensorModeText;
			m_ownshipLabel = ownshipLabel;
			m_ownshipLatText = ownshipLatText;
			m_ownshipLonText = ownshipLonText;
			m_ownshipAltText = ownshipAltText;
			m_targetLabel = targetLabel;
			m_targetLatText = targetLatText;
			m_targetLonText = targetLonText;
			m_targetAltText = targetAltText;
			m_targetDisText = targetDisText;
			m_horizontalTicks = horizontalTicks;
			m_verticalTicks = verticalTicks;
			m_offsetTextX = offsetTextX;
			m_offsetTextY = offsetTextY;
			m_h1Text = h1Text;
			m_h2Text = h2Text;
			m_h3Text = h3Text;
			m_h4Text = h4Text;
			m_h5Text = h5Text;
			m_v1Text = v1Text;
			m_v2Text = v2Text;
			m_v3Text = v3Text;
			m_v4Text = v4Text;
			m_v5Text = v5Text;
			m_v6Text = v6Text;
			m_v7Text = v7Text;
			
			m_targetLat = 0;
			m_targetLon = 0;
			m_targetAlt = 0;
			m_targetDis = 0;
			
			Prepar3D.Init();
			
			ResizeReticle();
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
		// UpdateReticle
		//
		// Main update function
		// Called each frame
		// Calls individual update functions for each specific component of the targeting pod overlay
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function UpdateReticle()
		{
			UpdateTime();
			
			UpdateDate();
			
			UpdateSensorModeText();
			
			UpdatePitchHeadingCarets();
			
			UpdateOwnshipTargetInfoText();
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
			
			m_timeText.text = hoursString + ":" + minutesString + ":" + secondsString;
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
			
			m_dateText.text = monthString + "-" + dayString + "-" + yearString;
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
				m_sensorModeText.text = "IR    WH";
			}
			else if (sensorMode == 2)
			{
				m_sensorModeText.text = "IR    BH";
			}
			else
			{
				m_sensorModeText.text = " ";
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
			m_verticalTicks.caret.y = percentY * HALF_VERTICAL_TICK_HEIGHT; // Half of design height
			
			var pitchInt:int = Math.round(pitch);
			if (pitchInt > 0)
			{
				m_offsetTextY.text = "-" + pitchInt;
			}
			else if (pitchInt < 0)
			{
				m_offsetTextY.text = "+" + Math.abs(pitchInt);
			}
			else
			{
				m_offsetTextY.text = " " + pitchInt;
			}
			
			var acHeading:Number = Prepar3D.VarGet("A:PLANE HEADING DEGREES TRUE", "degrees");
			var heading:Number = Prepar3D.VarGet("C:P3DCONTROLLABLECAMERA:GetHeading", "number");
			var headingDiff:Number = heading - acHeading;
			var percentX:Number = (NormalizeHeading(headingDiff) / 180.0)
			m_horizontalTicks.caret.x = percentX * HALF_HORIZONTAL_TICK_WIDTH; // Half of design width
			
			var headingDiffInt:int = Math.round(NormalizeHeading(headingDiff));
			if (headingDiffInt > 0)
			{
				m_offsetTextX.text = "+" + headingDiffInt;
			}
			else
			{
				m_offsetTextX.text = "" + headingDiffInt;
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
			
			m_ownshipLatText.text = GetLatLonString(Math.abs(ownLat), false);
			m_ownshipLonText.text = GetLatLonString(Math.abs(ownLon), true);
			m_ownshipAltText.text = "ALT " + Math.round(ownAlt).toString();
			
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
			
			m_targetLatText.text = GetLatLonString(Math.abs(m_targetLat), false);
			m_targetLonText.text = GetLatLonString(Math.abs(m_targetLon), true);
			m_targetAltText.text = "ALT " + Math.round(m_targetAlt).toString();
			m_targetDisText.text = "DIS " + m_targetDis.toString();
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

		///////////////////////////////////////////////////////////////////////////////////////////
		//
		// ResizeReticle
		//
		// Keeps all components sized and positioned correctly when the window is resized
		//
		///////////////////////////////////////////////////////////////////////////////////////////
		function ResizeReticle()
		{
			var w:uint = m_stage.stageWidth;
			var h:uint = m_stage.stageHeight;
			
			var stageCenterX = w / 2;
			var stageCenterY = h / 2;
			
			var sizeX:Number = w / DESIGN_SIZE_X;
			var sizeY:Number = h / DESIGN_SIZE_Y;
			
			// Sizing x and y the same keeps proportions equal. If this is not desired remove these lines.
			var maxSize:Number = Math.max(sizeX, sizeY);
			sizeX = maxSize;
			sizeY = maxSize;
			
			// Top Left Items
			m_ownshipLabel.scaleX = sizeX;
			m_ownshipLabel.scaleY = sizeY;
			m_ownshipLabel.x = BORDER_BUFFER;
			m_ownshipLabel.y = BORDER_BUFFER;
			
			m_ownshipLatText.scaleX = sizeX;
			m_ownshipLatText.scaleY = sizeY;
			m_ownshipLatText.x = BORDER_BUFFER;
			m_ownshipLatText.y = m_ownshipLabel.y + m_ownshipLabel.height;
			
			m_ownshipLonText.scaleX = sizeX;
			m_ownshipLonText.scaleY = sizeY;
			m_ownshipLonText.x = BORDER_BUFFER;
			m_ownshipLonText.y = m_ownshipLatText.y + m_ownshipLatText.height;
			
			m_ownshipAltText.scaleX = sizeX;
			m_ownshipAltText.scaleY = sizeY;
			m_ownshipAltText.x = BORDER_BUFFER;
			m_ownshipAltText.y = m_ownshipLonText.y + m_ownshipLonText.height;
			
			// Top Right Items
			m_targetLabel.scaleX = sizeX;
			m_targetLabel.scaleY = sizeY;
			m_targetLabel.x = w - m_targetLabel.width - BORDER_BUFFER;
			m_targetLabel.y = BORDER_BUFFER;
			
			m_targetLatText.scaleX = sizeX;
			m_targetLatText.scaleY = sizeY;
			m_targetLatText.x = w - m_targetLatText.width - BORDER_BUFFER;
			m_targetLatText.y = m_targetLabel.y + m_targetLabel.height;
			
			m_targetLonText.scaleX = sizeX;
			m_targetLonText.scaleY = sizeY;
			m_targetLonText.x = w - m_targetLonText.width - BORDER_BUFFER;
			m_targetLonText.y = m_targetLatText.y + m_targetLatText.height;
			
			m_targetAltText.scaleX = sizeX;
			m_targetAltText.scaleY = sizeY;
			m_targetAltText.x = w - m_targetAltText.width - BORDER_BUFFER;
			m_targetAltText.y = m_targetLonText.y + m_targetLonText.height;
			
			m_targetDisText.scaleX = sizeX;
			m_targetDisText.scaleY = sizeY;
			m_targetDisText.x = w - m_targetDisText.width - BORDER_BUFFER;
			m_targetDisText.y = m_targetAltText.y + m_targetLonText.height;
			
			// Bottom Left Items
			
			m_sensorModeText.scaleX = sizeX;
			m_sensorModeText.scaleY = sizeY;
			m_sensorModeText.x = BORDER_BUFFER;
			m_sensorModeText.y = h - m_sensorModeText.height - BORDER_BUFFER;
			
			// Bottom Right Items
			m_timeText.scaleX = sizeX;
			m_timeText.scaleY = sizeY;
			m_timeText.x = w - m_timeText.width - BORDER_BUFFER;
			m_timeText.y = h - m_timeText.height - BORDER_BUFFER;
			
			m_dateText.scaleX = sizeX;
			m_dateText.scaleY = sizeY;
			m_dateText.x = w - m_dateText.width - BORDER_BUFFER;
			m_dateText.y = m_timeText.y - m_dateText.height;
			
			// Ticks
			m_horizontalTicks.scaleX = sizeX;
			m_horizontalTicks.scaleY = sizeY;
			
			m_horizontalTicks.x = stageCenterX;
			m_horizontalTicks.y = m_dateText.y - m_dateText.height - (m_horizontalTicks.height * 2);
			
			m_verticalTicks.scaleX = sizeX;
			m_verticalTicks.scaleY = sizeY;
			
			m_verticalTicks.x = m_ownshipLabel.x + (m_ownshipLabel.width / 2) - (m_verticalTicks.width / 2);
			m_verticalTicks.y = stageCenterY;
			
			// Tick Text
			m_h1Text.scaleX = sizeX;
			m_h1Text.scaleY = sizeY;
			m_h1Text.x = m_horizontalTicks.x - (m_horizontalTicks.width / 2) - (m_h1Text.width / 2);
			m_h1Text.y = m_horizontalTicks.y + m_horizontalTicks.height;
			
			m_h2Text.scaleX = sizeX;
			m_h2Text.scaleY = sizeY;
			m_h2Text.x = m_horizontalTicks.x - (m_horizontalTicks.width / 4) - (m_h2Text.width / 2);
			m_h2Text.y = m_horizontalTicks.y + m_horizontalTicks.height;
			
			m_h3Text.scaleX = sizeX;
			m_h3Text.scaleY = sizeY;
			m_h3Text.x = m_horizontalTicks.x - (m_h3Text.width / 2);
			m_h3Text.y = m_horizontalTicks.y + m_horizontalTicks.height;
			
			m_h4Text.scaleX = sizeX;
			m_h4Text.scaleY = sizeY;
			m_h4Text.x = m_horizontalTicks.x + (m_horizontalTicks.width / 4) - (m_h4Text.width / 2);
			m_h4Text.y = m_horizontalTicks.y + m_horizontalTicks.height;
			
			m_h5Text.scaleX = sizeX;
			m_h5Text.scaleY = sizeY;
			m_h5Text.x = m_horizontalTicks.x + (m_horizontalTicks.width / 2) - (m_h5Text.width / 2);
			m_h5Text.y = m_horizontalTicks.y + m_horizontalTicks.height;

			m_offsetTextX.scaleX = sizeX;
			m_offsetTextX.scaleY = sizeY;
			m_offsetTextX.x = m_h3Text.x;
			m_offsetTextX.y = m_h3Text.y + m_h3Text.height;

			m_v1Text.scaleX = sizeX;
			m_v1Text.scaleY = sizeY;
			m_v1Text.x = m_verticalTicks.x - m_v1Text.width;
			m_v1Text.y = m_verticalTicks.y - (m_verticalTicks.height / 2) - (m_v1Text.height / 2);
			
			m_v2Text.scaleX = sizeX;
			m_v2Text.scaleY = sizeY;
			m_v2Text.x = m_verticalTicks.x - m_v2Text.width;
			m_v2Text.y = m_verticalTicks.y - (m_verticalTicks.height / 3) - (m_v2Text.height / 2);
			
			m_v3Text.scaleX = sizeX;
			m_v3Text.scaleY = sizeY;
			m_v3Text.x = m_verticalTicks.x - m_v3Text.width;
			m_v3Text.y = m_verticalTicks.y - (m_verticalTicks.height / 6) - (m_v3Text.height / 2);
			
			m_v4Text.scaleX = sizeX;
			m_v4Text.scaleY = sizeY;
			m_v4Text.x = m_verticalTicks.x - m_v4Text.width;
			m_v4Text.y = m_verticalTicks.y - (m_v4Text.height / 2);
			
			m_v5Text.scaleX = sizeX;
			m_v5Text.scaleY = sizeY;
			m_v5Text.x = m_verticalTicks.x - m_v5Text.width;
			m_v5Text.y = m_verticalTicks.y + (m_verticalTicks.height / 6) - (m_v5Text.height / 2);
			
			m_v6Text.scaleX = sizeX;
			m_v6Text.scaleY = sizeY;
			m_v6Text.x = m_verticalTicks.x - m_v6Text.width;
			m_v6Text.y = m_verticalTicks.y + (m_verticalTicks.height / 3) - (m_v6Text.height / 2);
			
			m_v7Text.scaleX = sizeX;
			m_v7Text.scaleY = sizeY;
			m_v7Text.x = m_verticalTicks.x - m_v7Text.width;
			m_v7Text.y = m_verticalTicks.y + (m_verticalTicks.height / 2) - (m_v7Text.height / 2);
			
			m_offsetTextY.scaleX = sizeX;
			m_offsetTextY.scaleY = sizeY;
			m_offsetTextY.x = m_v4Text.x + m_v4Text.width + m_verticalTicks.width;
			m_offsetTextY.y = m_v4Text.y;
			
			// Crosshair
			m_crosshair.scaleX = sizeX;
			m_crosshair.scaleY = sizeY;
			
			m_crosshair.x = stageCenterX;
			m_crosshair.y = stageCenterY;
		}
	}
}
