package
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.events.Event;
	import flash.text.TextFormat;
	
	public class PanelLabel extends P3DLabel
	{
		private var panelIndex:uint = 0;
		private var panelId:uint = 0;
		private var openCheckBox:P3DCheckBox;
		private var idText:TextField;
		private var duplicateButton:P3DButton;
		private var removeButton:P3DButton;
		private const margin:uint = 4;
		private var parentWindow:PanelWindow;
		private static var LabelWidth:uint = 200;
		private static var LabelHeight:uint = 30;
		private const DupRemButtonWidth:uint = 20;
		private const DuplicateId:uint = 40000;
		
		public function PanelLabel( panelName:String, panelId:uint, panelIndex:uint, parentWindow:PanelWindow )
		{
			SetWidth(LabelWidth);
			SetHeight(LabelHeight);
			
			labelText.text = "";
			this.panelId = panelId;
			this.panelIndex = panelIndex;
			this.parentWindow = parentWindow;
				
			openCheckBox = new P3DCheckBox();
			
			addChild(openCheckBox);
			openCheckBox.checkText.text = panelName + ": ID " + panelId;
			
			openCheckBox.x = margin;
			openCheckBox.y = margin;
			openCheckBox.checkText.width = openCheckBox.checkText.textWidth + margin;
			openCheckBox.addEventListener(MouseEvent.MOUSE_UP, HandleToggle);
			
			if (panelId < DuplicateId) // hardcoded to match start of duplicate IDs
			{
				duplicateButton = new P3DButton();
				addChild(duplicateButton);
				duplicateButton.SetText("+");

				duplicateButton.buttonText.width = DupRemButtonWidth;
				duplicateButton.UpdateSize();
				duplicateButton.addEventListener(MouseEvent.MOUSE_UP, HandleDuplicate);
				
				duplicateButton.x = LabelWidth - duplicateButton.width - margin;
				duplicateButton.y = openCheckBox.y;
			}
			else if (panelId != PanelWindow.invalidPanelId) // Panels with Invalid IDs (no ident set) cannot be duplicated
			{
				openCheckBox.checkText.text = openCheckBox.checkText.text.replace(" " + panelId.toString(), "");
				
				removeButton = new P3DButton();
				addChild(removeButton);
				removeButton.SetText("-");

				removeButton.buttonText.width = DupRemButtonWidth;
				removeButton.UpdateSize();
				removeButton.addEventListener(MouseEvent.MOUSE_UP, HandleRemove);
				
				removeButton.x = LabelWidth - removeButton.width - margin;
				removeButton.y = openCheckBox.y;
			}
			
			while ((openCheckBox.checkText.x + openCheckBox.checkText.textWidth) > (LabelWidth - DupRemButtonWidth - (margin * 3)))
			{
				LabelWidth += 10;
			}
			
			//UpdateSize();
			
			this.addEventListener(Event.ENTER_FRAME, UpdatePanelStates);
		}
		
		function HandleToggle( e: MouseEvent): void
		{
			if (!openCheckBox.IsDisabled())
			{
				Prepar3D.TogglePanelByIndex(panelIndex);
			}
		}
		
		function HandleDuplicate( e: MouseEvent): void
		{
			if (!duplicateButton.IsDisabled())
			{
				Prepar3D.DuplicatePanelByIndex(panelIndex);
				parentWindow.Initialize();
			}
		}
		
		function HandleRemove( e: MouseEvent): void
		{
			if (!removeButton.IsDisabled())
			{
				Prepar3D.RemoveDuplicatePanel(panelId);
				parentWindow.PanelRemoved(panelId);
			}
		}
		
		function UpdatePanelStates(e:Event)
		{
			// Sync check box states with any panel changes made outside of the UI
			var panelOpen:Boolean = Prepar3D.IsPanelOpenByIndex(panelIndex);
			var openChecked:Boolean = openCheckBox.IsChecked();
			var parentPanelOpen:Boolean = true;
			
			if (panelId >= DuplicateId && panelId != PanelWindow.invalidPanelId)
			{
				parentPanelOpen = parentWindow.IsParentPanelOpen(openCheckBox.checkText.text);
			}

			if (!parentPanelOpen)
			{
				openCheckBox.SetChecked(false);
				openCheckBox.SetDisabled(true);
				openCheckBox.checkText.alpha = 0.5;
				if (panelOpen)
				{
					Prepar3D.ClosePanel(panelId);
				}
			}
			else
			{
				openCheckBox.checkText.alpha = 1.0;
				openCheckBox.SetDisabled(false);
				if (panelOpen && !openChecked)
				{
					openCheckBox.SetChecked(true);
				}
				else if (!panelOpen && openChecked)
				{
					openCheckBox.SetChecked(false);
				}
			}
			SetWidth(LabelWidth);
			SetHeight(LabelHeight);
		}
		
		function UpdateButtonPositions()
		{			
			if (duplicateButton != null)
			{
				duplicateButton.x = LabelWidth - duplicateButton.width - margin;
				duplicateButton.y = openCheckBox.y;
			}
			if (removeButton != null)
			{				
				removeButton.x = LabelWidth - removeButton.width - margin;
				removeButton.y = openCheckBox.y;
			}
		}
	}
}
