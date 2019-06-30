package
{
	import Prepar3D
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.geom.Rectangle;
	import flash.text.TextFormat;
	import flash.events.MouseEvent;

	public class PanelWindow extends P3DWindow
	{
		// Layout variables
		private const margin = 4;
		private var messageHeight = 0;
		private var optionCount: int;
		private var messageFormat:TextFormat;
		private var messageCount: int;
		private var font = new P3DFont();
		public var isAtcWindow = false;
		private var isAtcDisabled:Boolean = false;
		public static var labelModifierY:int = 0;
		private var  messageScroll;
		private var  mainScroll;
		private var removedIdList:Array;
		private var panelNames:Array;
		private var panelLabels:Array;
		public static const invalidPanelId:uint = 4294967295;
		
		public function PanelWindow()
		{
			SetTitle("Panel Manager");
			SetFlowStart(margin, titleBar.height + margin);
			SetFlowMargins(margin, margin);
			SetFlowDirection(true);
			
			removedIdList = new Array();
			panelNames = new Array();
			panelLabels = new Array();
			
			Initialize();
		}
				
		public function PanelRemoved(id:uint)
		{
			removedIdList.push(id);
			Initialize();
		}
		
		public function Initialize()
		{
			ClearFlowItems();
			const numPanels:uint = Prepar3D.GetNumPanels();
			while (panelNames.length > 0)
			{
				panelNames.pop();
			}
			
			for (var panelIndex:uint = 0; panelIndex < numPanels; panelIndex++)
			{
				var panelId:int = Prepar3D.GetPanelIdByIndex(panelIndex);
				var idRemoved:Boolean = false;
				for (var removedIndex:uint = 0; removedIndex < removedIdList.length;  removedIndex++)
				{
					if (removedIdList[removedIndex] == panelId)
					{
						idRemoved = true;
						break;
					}
				}
				
				if (!idRemoved)
				{
					var panelName:String = Prepar3D.GetPanelNameByIndex(panelIndex);
					if (panelName.charAt(0) != '$' && panelName != "PanelManager") // Filter out vc panels and the Panel Manager itself
					{
						panelNames.push({PanelName:panelName, PanelId:panelId, PanelIndex:panelIndex});
					}
				}
			}
			
			while (panelLabels.length > 0)
			{
				panelLabels.pop();
			}
			panelNames.sortOn("PanelName");
			for (var index:uint = 0; index < panelNames.length; index++)
			{
				var panelLabel:PanelLabel = new PanelLabel(panelNames[index].PanelName, panelNames[index].PanelId, panelNames[index].PanelIndex, this);
				AddFlowItem(panelLabel);
				panelLabels.push(panelLabel);
			}
			
			for (index = 0; index < panelLabels.length; index++)
			{
				panelLabels[index].UpdateButtonPositions();
			}
        }
		
		public function IsParentPanelOpen(panelName:String) : Boolean
		{
			var parentIndex:uint = 0;
			var panelNameCompare:String = panelName.substr(0, panelName.indexOf(":"));
			for (var i:uint = 0; i < panelNames.length; i++)
			{
				var parentName:String = panelNames[i].PanelName;
				if (panelNameCompare == parentName)
				{
					parentIndex = panelNames[i].PanelIndex;
					break;
				}
			}
			return Prepar3D.IsPanelOpenByIndex(parentIndex);
		}
	}
}