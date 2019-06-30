package
{
	import Prepar3D
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.geom.Rectangle;
	import fl.controls.UIScrollBar;
	import flash.text.TextFormat;
	import flash.events.MouseEvent;

	public class MenuWindow extends MovieClip
	{
		// Layout variables
		private var dpiScale:Number = 1;
		private const pad = 4;
		private var messageHeight = 0;
		private var optionCount: int;
		private var buttonArray: Array;
		private var messageFormat:TextFormat;
		private var messageCount: int;
		private var font = new Font1();
		public var isAtcWindow = false;
		private var isAtcDisabled:Boolean = false;
		public static var buttonModifierY:int = 0;
		private var  messageScroll;
		private var  mainScroll;
		
		public function MenuWindow(isAtc:Boolean)
		{
			dpiScale = Prepar3D.GetScaleformDpiScaling();
			scaleX = dpiScale;
			scaleY = dpiScale;
			
			isAtcWindow = isAtc;
			
			buttonArray = new Array()
			this.addEventListener(Event.ENTER_FRAME, HandleUpdate);
			menuBar.scale9Grid = new Rectangle(2,2,490,20)

			
			if (isAtcWindow && Prepar3D.ShouldShowATCMessageLog())
			{
				messageCount = 0;
				// create vertical scroll bar, move and size it, and add it to the stage
				messageScroll = new UIScrollBar();
				messageScroll.direction = "vertical"; 
				messageScroll.setSize(10, scrollingTextBox.height);
				scrollingTextBox.width -= messageScroll.width;
				messageScroll.move(scrollingTextBox.x + scrollingTextBox.width, scrollingTextBox.y);
				messageScroll.scrollTarget = scrollingTextBox;
				scrollingTextBox.mouseWheelEnabled = true;
				
				addChild(messageScroll); 
				
				messageFormat = new TextFormat();
				messageFormat.font = font.fontName;
				messageFormat.color = 0xFFFFFF;
				scrollingTextBox.defaultTextFormat = messageFormat;
			}
			else
			{
				scrollingTextBox.y = menuBar.height;
				scrollingTextBox.height = 0;
			}
			mainScroll = new UIScrollBar();
			mainScroll.direction = "vertical"; 
			mainScroll.setSize(10, menuDescription.height);
			menuDescription.width -= mainScroll.width;
			mainScroll.move(menuDescription.x + menuDescription.width, menuDescription.y);
			mainScroll.scrollTarget = menuDescription;
			menuDescription.mouseWheelEnabled = true;
		
			menuDescription.multiline = true; 
			menuDescription.wordWrap = true; 
		
			addChild(mainScroll); 
		}
		
		public function Initialize()
		{
			buttonModifierY = 0;
			var textFormat:TextFormat = new TextFormat();
			textFormat.font = font.fontName;
			
			menuWindow.width = stage.stageWidth;
			menuWindow.height = stage.stageHeight;
			//messageBG.width = menuWindow.width
			menuBar.width = (menuWindow.width / scaleX);
			windowTitle.defaultTextFormat = textFormat;
			menuDescription.width = menuWindow.width / scaleX - (2 * menuDescription.x);
			windowTitle.text = Prepar3D.GetWindowTitle();
			windowTitle.x = menuWindow.x;
			windowTitle.width = menuWindow.width / scaleX;
			
			if (isAtcWindow)
			{
				if (Prepar3D.ShouldShowATCMessageLog())
				{
					scrollingTextBox.multiline = true; 
					scrollingTextBox.wordWrap = true; 
					scrollingTextBox.width = (menuWindow.width / scaleX) - 2 * scrollingTextBox.x
					scrollingTextBox.backgroundColor = 0;
					scrollingTextBox.y = menuBar.height;
					messageScroll.y = scrollingTextBox.y;
					messageBG.y = scrollingTextBox.y;
					messageBG.width = menuWindow.width;
					messageScroll.move(scrollingTextBox.x + scrollingTextBox.width, scrollingTextBox.y); 
					messageScroll.update();
					messageScroll.alpha = 0.5;
				}
				
				isAtcDisabled = Prepar3D.IsATCDisabled();
				if (isAtcDisabled)
				{
					windowTitle.text = Prepar3D.GetATCDisableReason();
					menuDescription.alpha = 0.5;
				}
				
			}
			
			mainScroll.y = menuDescription.y;
			mainScroll.height = menuDescription.height;
			mainScroll.move(menuDescription.x + menuDescription.width, menuDescription.y); 
			mainScroll.update();
			mainScroll.alpha = 0.5;
        }
		
		function HandleUpdate(e:Event) 
		{
			UpdateWindow();
			UpdateButtons();
		}
		
		public function UpdateButtons()
		{
			var i:int = 0;
			var topIndexModify:int = -1;
			var bottomIndexModify:int = -1;
			
			for (i = 0; i < buttonArray.length; i++)
			{
				if ((buttonArray[i].y) < (menuDescription.y + menuDescription.height))
				{
					topIndexModify = i + 1;
					AddButton("", i);
				}
				else if ((buttonArray[i].y + buttonArray[i].height) > ((menuWindow.height / scaleY) - pad))
				{
					if (bottomIndexModify == -1)
					{
						bottomIndexModify = i - 1;
					}
					AddButton("", i);
				}
			}
			
			if (topIndexModify > -1 && buttonArray[topIndexModify].visible)
			{
				AddButton("Previous...", topIndexModify);
			}
			
			if (bottomIndexModify > -1 && buttonArray[bottomIndexModify].visible)
			{
				AddButton("Next...", bottomIndexModify);
			}
			
			// Sync up button press behavior
			for (i = 0; i < buttonArray.length; i++)
			{
				buttonArray[i].shiftUpOnPress = false;
				buttonArray[i].shiftDownOnPress = false;
				
				if (topIndexModify != -1 && i <= topIndexModify)
				{
					buttonArray[i].shiftUpOnPress = true;
				}
				else if (bottomIndexModify != -1 && i >= bottomIndexModify)
				{
					buttonArray[i].shiftDownOnPress = true;
				}
			}
		}
		
		public function UpdateWindow()
		{
			var i = 0;
			
			if (isAtcWindow)
			{
				if (Prepar3D.ShouldShowATCMessageLog())
				{
					scrollingTextBox.height = (stage.stageHeight / 2) / dpiScale;
					messageScroll.height = scrollingTextBox.height;
					menuDescription.y = messageScroll.y + messageScroll.height;
				}
				else
				{
					scrollingTextBox.height = 0;
					menuDescription.y = menuBar.height;
				}
				
				var atcPromptText = Prepar3D.GetATCPrompt();
				if (atcPromptText != "")
				{
					SetWindowDescription(atcPromptText);
				}

				if (Prepar3D.ShouldShowATCMessageLog())
				{
					// Clear messages if necessary
					if (Prepar3D.ClearATCMessages())
					{
						scrollingTextBox.text = "";	
						messageCount = 0;
					}
					
					// Add queryable messages
					for (i = messageCount; i < Prepar3D.GetATCMessageCount(); i++)
					{
						var prevLength = scrollingTextBox.length;
						AppendMessageText(Prepar3D.GetATCMessage(i));
						var curLength = scrollingTextBox.length;
						messageFormat.color = Prepar3D.GetATCMessageColor(i);
						scrollingTextBox.setTextFormat(messageFormat, prevLength, curLength);
					}
					
					
					messageCount =  Prepar3D.GetATCMessageCount();
				}
					
				optionCount = Prepar3D.GetATCOptionCount();
			
				for( i = 0; i < optionCount; ++i )
				{
					AddButton( Prepar3D.GetATCOption( i ), i );
				}
				for( i = optionCount; i < 10; ++i )
				{
					AddButton("", i );
				}
				optionCount = 10;
				
				if (Prepar3D.ShouldShowATCMessageLog())
				{
					messageScroll.y = scrollingTextBox.y;
					messageScroll.update();
				}
				
				var disabled:Boolean = Prepar3D.IsATCDisabled();
				
				for (i = 0; i < buttonArray.length; i++)
				{
					buttonArray[i].SetDisabled(disabled); // buttons manage own state
				}
				if (disabled)
				{
					windowTitle.text = Prepar3D.GetATCDisableReason();
					menuDescription.alpha = 0.5;
				}
				else
				{
					windowTitle.text = Prepar3D.GetWindowTitle();
					menuDescription.alpha = 1.0;
				}
				isAtcDisabled = Prepar3D.IsATCDisabled();
			}
			else
			{
				menuDescription.y = windowTitle.height + (2 * pad);
				var menuPromptText = Prepar3D.GetMenuPromptPrompt();
				if (menuPromptText != "")
				{
					SetWindowDescription(menuPromptText);
				}

				optionCount = Prepar3D.GetMenuPromptOptionCount();
			
				for( i = 0; i < optionCount; ++i )
				{
					AddButton( Prepar3D.GetMenuPromptOption( i ), i );
				}
				for( i = optionCount; i < 10; ++i )
				{
					AddButton("", i );
				}
				optionCount = 10;
				windowTitle.text = Prepar3D.GetWindowTitle();
			}
		}
				
		private function SetWindowDescription( newDescription: String )
		{	
			menuDescription.text = newDescription;
			menuDescription.height = Math.min(menuDescription.textHeight + pad, 150);
			mainScroll.y = menuDescription.y;
			mainScroll.height = menuDescription.height;
			mainScroll.visible = menuDescription.textHeight > 150;
			mainScroll.update();
		}
		
		public function AppendMessageText( newText: String )
		{
			if(newText.length > 0)
			{
				scrollingTextBox.appendText( newText + "\n\n" );
				scrollingTextBox.scrollV = scrollingTextBox.numLines;
			}			
		}
		
		private function AddButton( buttonText: String, index: int )
		{
			var menuButton;
			if( index >= buttonArray.length )
			{
				menuButton = new MenuPromptButton( index );
				buttonArray[ index ] = menuButton;
				addChild( menuButton );
			}
			else
			{
				menuButton = buttonArray[index];
			}
			
			menuButton.x = stage.x + pad;
			menuButton.width = (menuWindow.width / scaleX) - (2 * pad);
			menuButton.SetText( buttonText, pad );
						
			if (index < buttonModifierY)
			{
				menuButton.y = 0;
			}
			else if( index == buttonModifierY )
			{
				var fromBottom;
								
				if (isAtcWindow && Prepar3D.ShouldShowATCMessageLog())
				{
					fromBottom = menuWindow.height / 2;
				}
				else
				{
					fromBottom = menuWindow.height - (menuButton.height + pad) * 10;
				}
				
				var fromTop = menuDescription.y + menuDescription.height;
				
				if(fromBottom > fromTop && isAtcWindow && Prepar3D.ShouldShowATCMessageLog())
				{
					menuButton.y = fromBottom;
					
					if (isAtcWindow)
					{
						messageBG.height = scrollingTextBox.height;
					}
					else
					{
						messageBG.height = 0
					}
				}
				else
				{
					menuButton.y =  fromTop + (pad * 2);
					
					if (isAtcWindow)
					{
						messageBG.height = scrollingTextBox.height;
					}
					else
					{
						messageBG.height = 0
					}
				}
				menuButton.y = menuDescription.y + menuDescription.height + pad;
			}
			else
			{
				var buttonAbove = buttonArray[ index - 1 ];
				menuButton.y = buttonAbove.y + buttonAbove.height + pad;
			}
		}
		
		public function activateOption(option:int)
		{
			if (buttonArray.length > option)
			{
				buttonArray[option].handlePress(null);
			}
		}
		
		public function releaseButton(option:int)
		{
			if (buttonArray.length > option)
			{
				buttonArray[option].handleRelease(null)
			}
		}
		
		public function handleWheelATC(e:MouseEvent): void
		{
			if (e.delta > 0)
			{
				scrollingTextBox.scrollV = scrollingTextBox.scrollV - 1;
			}
			else if (e.delta < 0)
			{
				scrollingTextBox.scrollV = scrollingTextBox.scrollV + 1;
			}
		}
		
	    public function handleWheelGeneral(e:MouseEvent): void
		{
			if (e.delta > 0)
			{
				menuDescription.scrollV = menuDescription.scrollV - 1;
			}
			else if (e.delta < 0)
			{
				menuDescription.scrollV = menuDescription.scrollV + 1;
			}
		}
		
	}
}