package
{
	import flash.display.Sprite;
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import AppMenuWindow;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;

	public class AppMenuItem extends MovieClip
	{
		private const enabledAlpha:Number = 1.0;
		private const disabledAlpha:Number = 0.33;
		public var accKey:uint;
		public var id:uint;
		public var selectable:uint;
		private var isMenuBarItem: uint;
		public var index:uint;
	    public function getIndex() { return index}
		public function AppMenuItem( )
		{
			this.id = 0;
			buttonMode = true;
			highlightBox.visible = false;
			this.addEventListener(MouseEvent.MOUSE_UP, handleRelease);
			this.addEventListener(MouseEvent.MOUSE_DOWN, handlePress);
			this.addEventListener(MouseEvent.MOUSE_OVER, handleRollOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, handleRollOut);
			this.tabEnabled = false;
			
			isMenuBarItem = 0;
		}
		
		public function SetId(myId:uint, myIndex:uint)
		{
			id = myId;
			index = myIndex;
			var header:String = Prepar3D.GetMenuItemText(id);
			if(header.length == 0)
			{
				selectable = 0
				highlightBox.visible = true
				buttonText.text = null
				buttonText.visible = false;
				shortcutText.text = null
				checkBox.visible = false;
				groupArrow.visible = false;
			}
			else 
			{
				buttonText.visible = true;
				highlightBox.visible = false;
				checkBox.visible = true;
				shortcutText.text = Prepar3D.GetMenuItemShortcutText(id);
				if(Prepar3D.GetMenuItemChildCount(id) > 0 &&
				   AppMenuWindow(parent).IsMenuBar() == 0)
				{
					groupArrow.visible = true;
				}
				else
				{
					groupArrow.visible = false;
				}						
				if(Prepar3D.GetMenuItemEnabled(id) == false)
				{
					selectable = 0;
					buttonText.alpha = disabledAlpha;
					shortcutText.alpha = disabledAlpha;
				}
				else
				{
					selectable = 1;
					buttonText.alpha = enabledAlpha;
					shortcutText.alpha = enabledAlpha;
				}
				var check = Prepar3D.GetMenuItemCheckBoxState(id)
				if(check == 2)
				{
					checkBox.check.visible = true;
				}
				else if(check == 1)
				{
					checkBox.check.visible = false;
				}
				else if(check == 0)
				{
					checkBox.visible = false;
				}
					
				buttonText.htmlText = header;
				
				if(AppMenuWindow(parent).IsMenuBar() == 1)
				{
					isMenuBarItem = 1;
				}
			}
			
		}
		public function GetId() : uint
		{
			return id;
		}
		public function SetText(text: String)
		{
			buttonText.text = text;
		}

		public function SetWidth(Width: uint, maxShortcutTextWidth: uint = 0)
		{
			buttonText.width = Width;
			shortcutText.width = maxShortcutTextWidth;
			shortcutText.x = Width - maxShortcutTextWidth;
			//highlightBox.width = this.width
			groupArrow.x = Width - groupArrow.width - 5
			
			if (isMenuBarItem)
			{
				checkBox.x = buttonText.x + checkBox.width
			}
		}
		
		function handleRelease( e: MouseEvent): void
		{
			if(selectable == 1 && groupArrow.visible == false)
			{
				//this.alpha = 1.0;
				ActivateMenuItem();
				// todo remove click listener bg from stage
				
				if(AppMenuWindow(parent).IsMenuBar() == 0)
				{
					MovieClip(root).clearMenuEvent(null);
				}
			}
		}
		
		public function setFocusState(state: uint, showChildren:Boolean = true)
		{
			if(selectable == 1)
			{
				if(state == 0) // none
				{
					//this.alpha = 1.0;
					highlightBox.visible = false;				
				}
				if(state == 1) // selected
				{
					//this.alpha = 0.8;
					if(AppMenuWindow(parent).IsMenuBar() == 1)
					{
						MovieClip(root).hideContextMenu(null);
					}
					if (showChildren)
					{
						AppMenuWindow(parent).ShowChildWindow(this,id);
					}
					highlightBox.visible = true;
				}
				if (state == 3) // mouse over menu bar no focus
				{
					highlightBox.visible = true;
				}
			}
			else if (!selectable && state == 1) // Special case so mousing over non selectable items will cause any expanded child window to close
			{
				AppMenuWindow(parent).CloseChildWindow();
			}
		}
		
		function handlePress(e: MouseEvent): void
		{
			if (AppMenuWindow(parent).IsMenuBar() == 1)
			{
				if (!AppMenuWindow(parent).IsChildWindowOpen())
				{
					AppMenuWindow(parent).clicked = true;
					setFocusState(1);
					Prepar3D.SetMenuBarFocus(true);
				}
				else
				{
					AppMenuWindow(parent).clicked = false;
					AppMenuWindow(parent).CloseChildWindow();
					setFocusState(0);
					Prepar3D.SetMenuBarFocus(true);
				}
			}
			
			AppMenuWindow(parent).TakeFocus();
			AppMenuWindow(parent).UpdateMouseIndex(index);
		}
		
		function handleRollOver(e: MouseEvent): void
		{
			if (!AppMenuWindow(parent).manualRollInProgress)
			{
				if (AppMenuWindow(parent).IsMenuBar() == 0 || 
					AppMenuWindow(parent).clicked)
				{
					setFocusState(1);
					AppMenuWindow(parent).TakeFocus();
					AppMenuWindow(parent).UpdateMouseIndex(index);
				}
			}
			AppMenuWindow(parent).manualRollInProgress = false;
		}
		
		function handleRollOut(e: MouseEvent): void
		{
			if (!AppMenuWindow(parent).IsChildWindowOpen())
			{
				setFocusState(0)
			}
		}
		
		function ActivateMenuItem() : void
		{
			Prepar3D.ActivateMenuItem(id);
		}
	}
}