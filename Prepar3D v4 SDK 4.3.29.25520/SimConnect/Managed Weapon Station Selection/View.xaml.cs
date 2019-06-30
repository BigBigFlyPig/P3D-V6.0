//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//
//
// Managed Weapon Station Selection
//

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;

namespace Managed_Weapon_Station_Selection
{
    /// <summary>
    /// Interaction logic for View.xaml
    /// </summary>
    public partial class View : Window
    {

        public View()
        {
            InitializeComponent();
        }

        public DataModel DataModel
        { get; private set; }


        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.DataModel = new DataModel(this, StationsDataGrid);

            // View now has a specific context and can binds to members inside current datamodel
            this.DataContext = DataModel;
        }

      

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // Isn't connected, try to connect
            if (!this.DataModel.IsConnected)
            {
                string errorMessage;
                bool wasSuccess = this.DataModel.Connect(out errorMessage);

                // Start monitoring the user's SimObject. This will continously monitor information
                // about the user's Stations attached to their SimObject.
                if (wasSuccess)
                {
                    this.DataModel.StartMonitoring();
                }
                // Show error to user
                else
                {
                    MessageBox.Show(errorMessage);
                }
            }
            // Is connected, try to disconnect
            else
            {
                this.DataModel.Disconnect();
            }


        }

        private void Window_Closing(object sender, CancelEventArgs e)
        {
            if (this.DataModel.IsConnected)
            {
                this.DataModel.Disconnect();
            }
        }

     

    }
}
