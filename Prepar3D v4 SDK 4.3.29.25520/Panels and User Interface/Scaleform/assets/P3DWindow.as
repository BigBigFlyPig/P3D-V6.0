package
{
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.display.DisplayObject;
	import Prepar3D;
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.display.Stage;
	
	public class P3DWindow extends MovieClip
	{
		private var dpiScale:Number = 1;
		
		private var flowDirection:Boolean; // true = vertical, false = horizontal
		private var flowStartX:int = 0;
		private var flowStartY:int = 0;
		private var flowMarginX:int = 0;
		private var flowMarginY:int = 0;
		private var flowContainer:Sprite;
		private const mouseWheelPixels:int = 10;
		
		public function P3DWindow()
		{
			Prepar3D.Init();
			dpiScale = Prepar3D.GetScaleformDpiScaling();
			
			scaleX = dpiScale;
			scaleY = dpiScale;
			
			flowContainer = new Sprite();
			addChild(flowContainer);
			
			//flowList = new Array();
			flowDirection = true;
			
			addEventListener(Event.ENTER_FRAME, HandleUpdate);
			//stage.addEventListener(MouseEvent.MOUSE_WHEEL, FlowHandleWheel);
			//flowContainer.x = 0;
			//flowContainer.y = 0;
			//flowContainer.width = 300;
			//flowContainer.height = 300;
		}
		
		public function SetTitle(text:String)
		{
			titleText.text = text;
		}
		
		public function SetFlowDirection(direction:Boolean)
		{
			flowDirection = direction;
		}
		
		public function SetFlowStart(startX:int, startY:int)
		{
			flowStartX = startX;
			flowStartY = startY;
		}
		
		public function SetFlowMargins(marginX:int, marginY:int)
		{
			flowMarginX = marginX;
			flowMarginY = marginY;
		}
		
		public function AddFlowItem(item:DisplayObject)
		{
			flowContainer.addChild(item);
		}
		
		function HandleUpdate(e:Event) 
		{
			window.x = 0;
			window.y = 0;
			window.width = (stage.stageWidth / dpiScale);
			window.height = (stage.stageHeight / dpiScale);
			titleBar.width = window.width;
			titleText.width = window.width;
			
			UpdateFlow();
		}
		
		function UpdateFlow()
		{
			if (flowContainer.numChildren > 0)
			{
				for (var i:int = 0; i < flowContainer.numChildren; i++)
				{
					var disObj:DisplayObject = flowContainer.getChildAt(i);
			
					if (i == 0)
					{
						disObj.x = flowStartX;
						disObj.y = flowStartY;
					}
					else
					{
						var prevDisObj:DisplayObject = flowContainer.getChildAt(i-1);
						
						if (flowDirection) // vertical
						{
							disObj.x = prevDisObj.x;
							disObj.y = prevDisObj.y + prevDisObj.height + flowMarginY;
							
							if ((disObj.y + disObj.height) > window.height)
							{
								disObj.x = prevDisObj.x + prevDisObj.width + flowMarginX;
								disObj.y = flowStartY;
							}
						}
						else // horizontal
						{
							disObj.x = prevDisObj.x + prevDisObj.width + flowMarginX;
							disObj.y = prevDisObj.y;
							
							if ((disObj.x + disObj.width) > window.width)
							{
								disObj.x = flowStartX;;
								disObj.y = prevDisObj.y + prevDisObj.height + flowMarginY;
							}
						}
					}
				}
			}
		}
		
		public function ClearFlowItems()
		{
			flowContainer.removeChildren();
		}
		
		public function FlowHandleWheel(e:MouseEvent): void
		{
			Prepar3D.P3DLog("Mouse wheel: " + e.delta + "\n");
			if (e.delta > 0)
			{
				if (flowDirection) // vertical
				{
					flowStartX += mouseWheelPixels;
				}
				else
				{
					flowStartY += mouseWheelPixels;
				}
			}
			else if (e.delta < 0)
			{
				if (flowDirection) // vertical
				{
					flowStartX -= mouseWheelPixels;
				}
				else
				{
					flowStartY -= mouseWheelPixels;
				}
			}
		}
	}
}
