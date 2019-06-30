package
{
	import Prepar3D
	import flash.external.*;
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.ui.Keyboard;
	import flash.geom.Point;
	import flash.events.Event;

	public class AppMenuWindow extends MovieClip
	{
		public var accKeys:Vector.<uint>;
		public var accKeysIndexed:Vector.<int>;
		
		// Layout variables
		private	var dpiScale:Number = 1;
		private var buttonPadding = 0;
		private var buttonWidth = 128;
		private var buttonHeight = 36;
		private static var menuBar: AppMenuWindow;
		private var childCount: uint;
		private var childArray: Array;
		private var maxTextWidth: int;
		private var maxShortcutTextWidth: int;
		private var parentWindow: AppMenuWindow;
		private var childWindow: AppMenuWindow;
		private var hasFocus: int;
		private var rootItem: uint;
		private var isMenuBar: uint;
		private var subMenuIndex: int;
		private var isContextMenu: Boolean;
		public var clicked:Boolean;
		public var openLeft:Boolean;
		private var childWindowOpen:Boolean;
		private var parentMenuItem:AppMenuItem;
		public var expandChildren:Boolean;
		private var moveFactorArrow:int = 5;
		private var moveFactorWheel:int = 5;
		private var mouseWheelMove:Boolean = false;
		private var moveFactor:int;
		public var bUseMainViewSize:Boolean = false;
		public var manualRollInProgress:Boolean = false;
		
		public function IsMenuBar() : uint
		{
			return isMenuBar
		}
		public function AppMenuWindow(parentWin: AppMenuWindow, parentMenu:AppMenuItem = null, isContextMenuParam:Boolean = false)
		{
			//Prepar3D.P3DLog("AppMenuWindow Create\n");
			dpiScale = Prepar3D.GetScaleformDpiScaling();
			
			if (parentWin == null)
			{
				scaleX = dpiScale
				scaleY = dpiScale
			}
			accKeys = new Vector.<uint>(128);
			accKeysIndexed = new Vector.<int>(128);
			/*for (var i:uint = 0; i < accKeys.length; i++)
			{
				accKeys[i] = -1;
			}*/
			for (var i:uint = 0; i < accKeysIndexed.length; i++)
			{
				accKeysIndexed[i] = -1;
			}
			childArray = new Array();
			buttonWidth = menuWindow.width;
			parentWindow = parentWin;
			isContextMenu = isContextMenuParam;
			if(parentWin == null && !isContextMenu)
			{
				isMenuBar = 1;
				menuBar = this;
			}
			else
			{
				isMenuBar = 0;
			}
			menuWindow.visible = false;
			menuWindow.alpha = 0.0
			hasFocus = 0;
			subMenuIndex = 0;
			clicked = false;
			openLeft = false;
			childWindowOpen = false;
			parentMenuItem = parentMenu;
			expandChildren = true;
			
			scrollTop.visible = false;
			scrollBottom.visible = false;
			scrollTopBg.visible = false;
			scrollBottomBg.visible = false;
			
			scrollTop.addEventListener(MouseEvent.MOUSE_OVER, handleRollOverTop);
			scrollTop.addEventListener(MouseEvent.MOUSE_OUT, handleRollOutTop);
			scrollBottom.addEventListener(MouseEvent.MOUSE_OVER, handleRollOverBottom);
			scrollBottom.addEventListener(MouseEvent.MOUSE_OUT, handleRollOutBottom);
			
			scrollTopBg.addEventListener(MouseEvent.MOUSE_OVER, handleRollOverTop);
			scrollTopBg.addEventListener(MouseEvent.MOUSE_OUT, handleRollOutTop);
			scrollBottomBg.addEventListener(MouseEvent.MOUSE_OVER, handleRollOverBottom);
			scrollBottomBg.addEventListener(MouseEvent.MOUSE_OUT, handleRollOutBottom);
			
			scrollTop.addEventListener(MouseEvent.MOUSE_DOWN, handleClickTop);
			scrollBottom.addEventListener(MouseEvent.MOUSE_DOWN, handleClickBottom);
			
			scrollTopBg.addEventListener(MouseEvent.MOUSE_DOWN, handleClickTop);
			scrollBottomBg.addEventListener(MouseEvent.MOUSE_DOWN, handleClickBottom);
			
			this.addEventListener(Event.ENTER_FRAME, enterFrame);
		}
		
		function rollTop()
		{
			var globalPoint:Point;
			var windowBottom:int;
			
			if (IsChildWindowOpen())
			{
				CloseChildWindow();
			}
			
			scrollTop.alpha = 1.0;
			scrollTopBg.alpha = 0.75;
			scrollBottom.alpha = 0.75;
			scrollBottomBg.alpha = 0.5;
			
			this.y+=moveFactor;
			
			scrollTop.y-=moveFactor;
			scrollBottom.y-=moveFactor;
			scrollTopBg.y-=moveFactor;
			scrollBottomBg.y-=moveFactor;
			
			globalPoint = menuWindow.localToGlobal(new Point(0,0));
			windowBottom = globalPoint.y + menuWindow.height * dpiScale;
			
			var topPos:int = stage.y + MovieClip(root).GetMenuBarHeight();
			if (globalPoint.y >= topPos)
			{
				scrollTop.visible = false;
				scrollTopBg.visible = false;
			}
			if (windowBottom > stage.stageHeight)// - topPos)
			{
				scrollBottom.visible = true;
				scrollBottomBg.visible = true;
			}
			
			graphics.clear()
			graphics.lineStyle(1,0x000000,1.0);
			graphics.beginFill(0x555555,0.5);
			graphics.drawRect(0, scrollTopBg.y, menuWindow.width, menuWindow.height);
			graphics.endFill();
		}
		
		function rollBottom()
		{
			var globalPoint:Point;
			var windowBottom:int;
			
			if (IsChildWindowOpen())
			{
				CloseChildWindow();
			}
			
			scrollBottom.alpha = 1.0;
			scrollBottomBg.alpha = 0.75;
			scrollTop.alpha = 0.75;
			scrollTopBg.alpha = 0.5;
			
			this.y-=moveFactor;
			
			scrollTop.y+=moveFactor;
			scrollBottom.y+=moveFactor;
			scrollTopBg.y+=moveFactor;
			scrollBottomBg.y+=moveFactor;
			
			globalPoint = menuWindow.localToGlobal(new Point(0,0));
			windowBottom = globalPoint.y + menuWindow.height * dpiScale;
			
			var topPos:int = stage.y + MovieClip(root).GetMenuBarHeight();
			
			if (windowBottom <= stage.stageHeight)// - topPos)
			{
				scrollBottom.visible = false;
				scrollBottomBg.visible = false;
			}
			
			if (globalPoint.y < topPos)
			{
				scrollTop.visible = true;
				scrollTopBg.visible = true;
			}
			
			graphics.clear()
			graphics.lineStyle(1,0x000000,1.0);
			graphics.beginFill(0x555555,0.5);
			graphics.drawRect(0, scrollTopBg.y, menuWindow.width, menuWindow.height);
			graphics.endFill();
		}
		
		function enterFrame(e:Event): void
		{
			if (scrollTopBg.visible || scrollBottomBg.visible)
			{
				for( var i:uint = 0; i != childCount; ++i )
				{
					childArray[i].visible = true
				}
				for( i = 0; i != childCount; ++i )
				{
					if (scrollTopBg.visible && ((childArray[i].y ) < (scrollTopBg.y)))
					{
						childArray[i].visible = false
					}
					if (scrollBottomBg.visible && ((childArray[i].y + childArray[i].height ) > (scrollBottomBg.y + scrollBottomBg.height)))
					{
						childArray[i].visible = false
					}
				}
			}
		}
		
		public function handleWheel(e:MouseEvent): void
		{
			if (e.delta > 0)
			{
				if (scrollTop.visible)
				{
					FocusFirst();
					return;
					mouseWheelMove = true;
					moveFactor = moveFactorWheel
					rollTop();
					handleRollOutTop(null);
					mouseWheelMove = false;
					
					var firstInView:int = 0;
					for( var i:int = 0; i != childCount; ++i )
					{
						if (childArray[i].visible == true)
						{
							firstInView = i;
							break;
						}
					}
					if (firstInView != -1)
					{
					//	KeepItemInView(true, firstInView);
					}
				}
			}
			else if (e.delta < 0)
			{
				if (scrollBottom.visible)
				{
					FocusLast();
					return;
					mouseWheelMove = true;
					moveFactor = moveFactorWheel;
					if (!scrollTop.visible)
					{
						moveFactor -= scrollTopBg.height
					}
					rollBottom();
					handleRollOutBottom(null);
					mouseWheelMove = false;
				}
			}
		}

		function handleClickTop(e:MouseEvent): void
		{
			FocusFirst();
		}
		
		function handleClickBottom(e:MouseEvent): void
		{
			FocusLast();
		}

		function handleRollOverTop(e: MouseEvent): void
		{
			scrollTop.alpha = 1.0;
			scrollTopBg.alpha = 0.75;
			scrollBottom.alpha = 0.75;
			scrollBottomBg.alpha = 0.5;
		}
		
		function handleRollOverBottom(e: MouseEvent): void
		{
			scrollBottom.alpha = 1.0;
			scrollBottomBg.alpha = 0.75;
			scrollTop.alpha = 0.75;
			scrollTopBg.alpha = 0.5;
		}
		
		function handleRollOutTop(e: MouseEvent): void
		{
			scrollTop.alpha = 0.75;
			scrollTopBg.alpha = 0.5;
		}
		
		function handleRollOutBottom(e: MouseEvent): void
		{
			scrollBottom.alpha = 0.75;
			scrollBottomBg.alpha = 0.5;
		}
		
		public function UpdateMouseIndex(index:int)
		{
			for( var i:uint = 0; i != childCount; ++i )
			{
				if (i != index)
				{
					childArray[i].setFocusState(0);
				}
			}
			subMenuIndex = index;
		}
	
		public function CloseChildWindow()
		{
			if(childWindow != null)
			{
				childWindowOpen = false;
				childWindow.SetSubMenuIndex(-1);
				childWindow.CloseChildWindow();
				removeChild(childWindow);
				childWindow = null;
				if (parentMenuItem != null)
				{
					parentMenuItem.setFocusState(0);
				}
			}
		}
		
		public function IsChildWindowOpen() : Boolean
		{
			return childWindowOpen;
		}
		
		public function ShowChildWindow(item: AppMenuItem, id: uint)
		{
			MovieClip(root).addClickListener();
			CloseChildWindow();
			var itemChildCount = Prepar3D.GetMenuItemChildCount(id);
			if( itemChildCount > 0 )
			{
				childWindowOpen = true;
				if(childWindow == null)
				{
					childWindow = new AppMenuWindow(this, item, isContextMenu);
					childWindow.bUseMainViewSize = bUseMainViewSize;
					addChild( childWindow );
				}
				if( isMenuBar != 1 )
				{
					childWindow.y = item.y
					childWindow.x = menuWindow.width
				}
				else
				{
					Prepar3D.SetMenuBarFocus(true);
				    childWindow.x = item.x
					childWindow.y = menuWindow.height
				}
				childWindow.UpdateWindow(id);

				if (isMenuBar != 1 )
				{
					var childWidth:int = childWindow.menuWindow.width;
					if (childWindow.x < (menuWindow.x + menuWindow.width))
					{
						var diff:int = (menuWindow.x + menuWindow.width) - childWindow.x;
						childWindow.x -= menuWindow.width - diff
						childWindow.x -= childWidth
					}
					var globalPoint:Point = childWindow.localToGlobal(new Point(0,0));
					if (globalPoint.x < 0)
					{
						childWindow.x -= globalPoint.x;
					}
				}
				item.setFocusState(3);
				parentMenuItem = item;
			}
			else
			{
				CloseChildWindow();
			}
		}		
		
		public function GetContextMenuWidth():uint
		{
			return menuWindow.width;
		}
		
		public function ClearChildArray()
		{			
			while (childArray.length > 0)
			{
				childArray.pop();
			}
		}
		
		public function UpdateWindow(rootId:uint)
		{
			rootItem = rootId
			childCount = Prepar3D.GetMenuItemChildCount(rootItem);
			maxTextWidth = 0;
			maxShortcutTextWidth = 0;
			for( var i = 0; i != childArray.length; ++i )
			{
				removeChild(childArray[i])
			}
			
			ClearChildArray();
			graphics.clear()
			for( i = 0; i != childCount; ++i )
			{
				AddButton( i );
			}
				
			if( isMenuBar != 1 )
			{
				menuWindow.width = (maxTextWidth + maxShortcutTextWidth)

				for( i = 0; i != childCount; ++i )
				{
					childArray[ i ].SetWidth( menuWindow.width, (maxShortcutTextWidth))
					childArray[ i ].highlightBox.width = menuWindow.width;
				}
			}
			else
			{
				menuWindow.width = stage.stageWidth;
			}
			
			var globalPoint:Point = localToGlobal(new Point(0,0));
			var globalPointHeight:Point = localToGlobal(new Point(menuWindow.width,0));
			//var stageHeightLocal:int = globalToLocal(new Point(0, stage.stageHeight)).y
					
			var leftPos: int = globalPoint.x;
			var topPos: int = globalPoint.y;
				
			var rightPos: int = globalPoint.x + menuWindow.width * dpiScale;
			var bottomPos: int = globalPoint.y + menuWindow.height * dpiScale;
				
			if (isMenuBar != 1)
			{
				var windowWidth = Prepar3D.GetWindowWidth(bUseMainViewSize);
				var windowHeight = Prepar3D.GetWindowHeight(bUseMainViewSize);
				
				if (rightPos > windowWidth)
				{
					x -= rightPos - windowWidth;
					openLeft = true;
				}
				else if (rightPos > (windowWidth - menuWindow.width))
				{
					openLeft = true;
				}
			}
			
			graphics.lineStyle(1,0x000000,1.0);
			graphics.beginFill(0x555555,0.5);
			graphics.drawRect(0, 0, menuWindow.width, menuWindow.height);
			graphics.endFill();
			
			// See if we need to add scroll arrows
			if (bottomPos > stage.stageHeight)// - topPos)
			{
				if (isMenuBar == 0)
				{
					var diff:int = bottomPos - stage.stageHeight;
					Prepar3D.P3DLog("height diff: " + diff + "\n");
					this.y -= diff
						
					globalPoint = localToGlobal(new Point(0,0));
					
					leftPos = globalPoint.x;
					topPos = globalPoint.y;
					
					rightPos = globalPoint.x + menuWindow.width * dpiScale;
					bottomPos = globalPoint.y + menuWindow.height * dpiScale;
					
					if (topPos / dpiScale < MovieClip(root).GetMenuBarHeight()/ dpiScale)
					{
						this.y -= (topPos / dpiScale) - (MovieClip(root).GetMenuBarHeight() / dpiScale)
						//if (localToGlobal(new Point(0,0)).y < (MovieClip(root).GetMenuBarHeight() / dpiScale))
						//{
						//	this.y += MovieClip(root).GetMenuBarHeight() / dpiScale
						//}
					}
						
					
					var stageHeightLocal:int = globalToLocal(new Point(0, stage.stageHeight)).y
					globalPoint = localToGlobal(new Point(0,0));
					
					leftPos = globalPoint.x;
					topPos = globalPoint.y;
					
					rightPos = globalPoint.x + menuWindow.width * dpiScale;
					bottomPos = globalPoint.y + menuWindow.height * dpiScale;
					
					if (bottomPos > stage.stageHeight)// - topPos)
					{
					
						scrollTop.width = 20//menuWindow.width / 2
						scrollTop.height = 5
						scrollTop.y = 3
						scrollTop.x = menuWindow.width / 2 - 10
						
						scrollTopBg.width = menuWindow.width
						scrollTopBg.height = 10
						scrollTopBg.y = 0
						
						scrollBottom.visible = true
						scrollBottom.width = 20
						scrollBottom.height = 5
						scrollBottom.y = stageHeightLocal - 7
						scrollBottom.x = menuWindow.width / 2 - 10
						
						scrollBottomBg.visible = true
						scrollBottomBg.width = menuWindow.width
						scrollBottomBg.height = 10
						scrollBottomBg.y = stageHeightLocal - 10
						
						Prepar3D.P3DLog("scroll bottom pos: " + scrollBottomBg.y + "\n\n");
					}
				}
			}
		}
		
		private function AddButton( index: int )
		{
			var menuButton : AppMenuItem;
			var childId : uint = Prepar3D.GetMenuItemChildID(rootItem, index);
			var accKey : uint = Prepar3D.GetMenuItemAccKey(childId);
			if( accKey > 0 )
			{
				accKeys[accKey] = childId;
				accKeysIndexed[accKey] = index;
				//Prepar3D.P3DLog("Acc: " + accKey + "\n");
			}
			if( index >= childArray.length )
			{
				menuButton = new AppMenuItem( );
				menuButton.SetWidth( buttonWidth )
				childArray[ index ] = menuButton;
			}
			else
			{
				menuButton = childArray[index];
			}
			addChild( menuButton );
			setChildIndex(menuButton, 0);
			menuButton.SetId(childId, index)
			if(menuButton.buttonText.visible == false)
			{
				if (index > 0)
				{
					menuButton.y = childArray[ index - 1 ].y + childArray[ index - 1 ].height;
				}
				else
				{
					menuButton.y = 0
				}
				menuButton.height = 1;
				menuButton.highlightBox.height = menuButton.height * dpiScale;
				menuButton.alpha = 0.5;
				menuWindow.height += menuButton.height;
			}
			else
			{
				if(maxTextWidth < menuButton.buttonText.textWidth)
				{
					maxTextWidth = menuButton.buttonText.textWidth;
				}
				if(maxShortcutTextWidth < menuButton.shortcutText.textWidth)
				{
					maxShortcutTextWidth = menuButton.shortcutText.textWidth;
				}
				menuButton.x = buttonPadding;
				if( isMenuBar == 1 )
				{
					menuButton.checkBox.visible = false
					trace("menu button width: " + menuButton.width);
					menuButton.SetWidth(menuButton.buttonText.textWidth + menuButton.checkBox.x )
					trace("menu button width new: " + menuButton.width);
					menuButton.highlightBox.width = menuButton.buttonText.width
					
					menuButton.buttonText.x -= menuButton.checkBox.x;
					menuButton.highlightBox.width -= menuButton.checkBox.x
					if(index != 0)
					{
						menuButton.x = (childArray[index - 1].x + childArray[index - 1].highlightBox.width)
					}
				}
				if( index == 0 || isMenuBar == 1 )
				{
					menuButton.y = buttonPadding;
					menuWindow.height = childArray[0].y + childArray[0].highlightBox.height
					//moveFactorWheel = menuButton.height// + 1;
				}
				else
				{
					menuButton.y = childArray[ index - 1 ].y + childArray[ index - 1 ].height;
					menuWindow.height += childArray[0].y + (childArray[index].height)
				}
				menuButton.highlightBox.height = menuButton.height;
			}
		}
		
		public function TakeFocus()
		{
			MovieClip(root).SetFocusedMenu(this);
		}
		
		public function SetSubMenuIndex(index:int, showChildren:Boolean = true)
		{
			var step:int = 1;
			if (index < subMenuIndex)
			{
				step = -1;
			}
			
			//Prepar3D.P3DLog("SetSubMenuIndex "+ index +"\n");
			if (subMenuIndex > -1 && subMenuIndex < childCount)
			{
				childArray[subMenuIndex].setFocusState(0);
			}
			
			while (index > -1 && index < childCount)
			{
				if (childArray[index].selectable > 0)
				{
					subMenuIndex = index;
					childArray[subMenuIndex].setFocusState(1, showChildren);
					break;
				}
				index += step;
			}
		}
		
		public function UnfocusAll()
		{
			for (var i:uint = 0; i < childCount; i++)
			{
				childArray[i].setFocusState(0);
			}
		}
		
		public function FocusPrev()
		{
			if(subMenuIndex < 1)
			{
				SetSubMenuIndex(childCount - 1, expandChildren)
			}
			else
			{
				SetSubMenuIndex(subMenuIndex - 1, expandChildren);
			}
		}
		
		public function FocusParent()
		{
			SetSubMenuIndex(-1)
			if(isMenuBar != 1)
			{
				AppMenuWindow(parent).TakeFocus();
			}
		}
		
		public function FocusChild()
		{
			if(childWindow != null)
			{
				childWindow.TakeFocus();
				childWindow.SetSubMenuIndex(0);
			}
			else if(!isContextMenu)
			{
				menuBar.TakeFocus();
				menuBar.FocusNext();
				if (isMenuBar)
				{
					menuBar.FocusPrev();
				}
				menuBar.FocusChild();
			}
		}		
		
		public function FocusChildId(id:uint)
		{
			if(childWindow != null)
			{
				childWindow.TakeFocus();
				childWindow.SetSubMenuIndex(id);
			}
			else
			{
				menuBar.TakeFocus();
				//menuBar.FocusNext();
				menuBar.FocusChildId(id);
			}
		}		
		
		public function ActivateChild()
		{
			childArray[subMenuIndex].ActivateMenuItem();
		}
		
		public function FocusNext()
		{
			if(subMenuIndex + 1 >= childCount)
			{
				SetSubMenuIndex(0, expandChildren)
			}
			else
			{
				SetSubMenuIndex(subMenuIndex + 1, expandChildren);
			}
		}
		
		public function FocusFirst()
		{
			Prepar3D.P3DLog("Focus First" + "\n\n");
			var index:int = 0;
			for(var i:int = 0; i < childCount; i++)
			{
				if (childArray[i].visible)
				{
					index = i;
					Prepar3D.P3DLog("Focus First found: " + i + "\n\n");
					break;
				}
			}
			//subMenuIndex = index;
			//FocusPrev();
		    KeepItemInView(false, --index);
		}
		
		public function FocusLast()
		{
			Prepar3D.P3DLog("Focus Last" + "\n\n");
			var index:int = 0;
			for(var i:int = childCount - 1; i >= 0; i--)
			{
				if (childArray[i].visible)
				{
					index = i;
					Prepar3D.P3DLog("Focus Last found: " + i + "\n\n");
					break;
				}
			}
			//subMenuIndex = index;
			//FocusNext();
		    KeepItemInView(false, ++index);
		}
		
		private function KeepItemInView(mouseWheel:Boolean, index:int)
		{
			if ((scrollTopBg.visible) && (childArray[index].y ) < (scrollTopBg.y + scrollTopBg.height))
			{
				manualRollInProgress = true;
				moveFactor = (scrollTopBg.y + scrollTopBg.height) - (childArray[index].y )
				//moveFactor = 26//(childArray[index].y + childArray[index].height )//scrollTopBg.y - childArray[index].y
				/*if (index == 0)
				{
					var topPos:int = stage.y + MovieClip(root).GetMenuBarHeight();
					moveFactor = 0 - localToGlobal(new Point(0, this.y)).y// - topPos
				}*/
				Prepar3D.P3DLog("scrollTopBg.y: " + scrollTopBg.y + "\n\n");
				Prepar3D.P3DLog("scrollTopBg.height: " + scrollTopBg.height + "\n\n");
				Prepar3D.P3DLog("childArray[index].y: " + childArray[index].y + "\n\n");
								
				Prepar3D.P3DLog("move factor: " + moveFactor + "\n\n");
				rollTop();
			}
			else if (scrollBottomBg.visible && (childArray[index].y + childArray[index].height ) > (scrollBottomBg.y))
			{
				manualRollInProgress = true;
				moveFactor = (childArray[index].y + childArray[index].height ) - (scrollBottomBg.y)
				Prepar3D.P3DLog("move factor: " + moveFactor + "\n\n");				
				rollBottom();
			}
		}
		
		public function HasFocus() : int
		{
			return hasFocus;
		}
		
		public function handleKey(key:uint, shiftDown:Boolean): Boolean
		{
			var clearMenu:Boolean = false;

			if(isMenuBar != 1)
			{
			  //Prepar3D.P3DLog("Key (context) "+ key +"\n");
				
				if (key == Keyboard.TAB)
				{
					if (shiftDown)
					{
						key = Keyboard.UP
					}
					else
					{
						key = Keyboard.DOWN
					}
				}
				
			  switch (key) {
				  case Keyboard.LEFT :
					FocusParent();
					if(parent == menuBar)
					{
						menuBar.FocusPrev();
						menuBar.FocusChild();
					}
					break;
				  case Keyboard.RIGHT :
					FocusChild();
					break;
				  case Keyboard.UP :
					FocusPrev();
				    KeepItemInView(false, subMenuIndex);
					break;
				  case Keyboard.DOWN :
					FocusNext();
				    KeepItemInView(false, subMenuIndex);
					break;
				  case Keyboard.ENTER:
					if(childArray[subMenuIndex].selectable == 1 && childArray[subMenuIndex].groupArrow.visible == false)
					{
						ActivateChild();
						menuBar.CloseChildWindow();
						clearMenu = true;
					}
					break;
				  case Keyboard.SPACE:
					if(childArray[subMenuIndex].selectable == 1 && childArray[subMenuIndex].groupArrow.visible == false)
					{
						ActivateChild();
						menuBar.CloseChildWindow();
						clearMenu = true;
					}
					break;
				  default:
					if (accKeysIndexed[key] >= 0)
					{
						SetSubMenuIndex(accKeysIndexed[key]);
						if (childArray[subMenuIndex].groupArrow.visible == false)
						{
							ActivateChild();
							menuBar.CloseChildWindow();
							clearMenu = true;
						}
						else
						{
							FocusChild();
						}
					}
					//Prepar3D.ActivateMenuItem(accKeys[key - 65]);
				    //
					break;
			  }
		  }
		  else
		  {
				//Prepar3D.P3DLog("Key (menu bar) "+ key +"\n");
				
			  if (key == Keyboard.TAB)
				{
					if (shiftDown)
					{
						key = Keyboard.LEFT
					}
					else
					{
						key = Keyboard.RIGHT
					}
				}
				
				switch (key) {
				  case Keyboard.LEFT :
					FocusPrev();
					break;
				  case Keyboard.RIGHT :
					FocusNext();
					break;
				  case Keyboard.UP :
					expandChildren = true;
					FocusChild();
				    if(childWindow != null)
					{
						childWindow.FocusPrev();
					}
					break;
				  case Keyboard.DOWN :
				    expandChildren = true;
					FocusChild();
					break;
				  default:
					if (accKeysIndexed[key] >= 0)
					{
						expandChildren = true;
						SetSubMenuIndex(accKeysIndexed[key]);
						FocusChild();
					}
					break;
			  }		  
		  }
		  return clearMenu;
	   }
	}
}