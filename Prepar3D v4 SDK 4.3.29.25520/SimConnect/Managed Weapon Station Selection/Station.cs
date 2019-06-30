using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Managed_Weapon_Station_Selection
{
    public delegate void StationSendDataHandler(StationEventArgs stationArgs);

    /// <summary>
    /// Create a data class for returned station information. This will make it possible
    /// to bind the data to a ItemsControl and also to manipulate and compare the data
    /// </summary>
    public class Station
    {
        public const int MAX_TIMES_TIL_RESEND = 10;

        public Station(Managed_Weapon_Station_Selection.DataModel model)
        {
        }

        /// <summary>
        /// The user should have the ability to change whether the station is selected. By changing this
        /// option though, we must set the appropriate SimVar in game which can be done in the callback to
        /// this event.
        /// </summary>
        public event StationSendDataHandler SelectionChanged;

        public string Name { get; set; }
        public int Index { get; set; }

        public bool BeingProcessed { get; set; }
        public int CurrentProcessRequests { get; set; }
        public bool ExpectedValueForIsSelected { get; set; }

        private bool isSelected;
        public bool IsSelected 
        { 
            get { return isSelected; }
            set 
            {
                // If value changed then store the change and
                // send out the change to anybody listening
                if (isSelected != value)
                {
                    isSelected = value;
                    if (SelectionChanged != null)
                    {
                        BeingProcessed             = true;
                        CurrentProcessRequests = 0;
                        ExpectedValueForIsSelected = isSelected;
                        SelectionChanged(new StationEventArgs(Index, isSelected));
                    }
                }

            }
        }
    }

    /// <summary>
    /// EventArgs that will be sent along with a Station selection change.
    /// </summary>
    public class StationEventArgs : EventArgs
    {
        public StationEventArgs(int index, bool isSelected)
        {
            Index = index;
            IsSelected = isSelected;
        }

        public int Index { get; private set; }
        public bool IsSelected { get; private set; }
    }
}
