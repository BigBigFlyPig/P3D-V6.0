//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//
//
// Managed Mission Objects
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

namespace Managed_Mission_Objects
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public DataModel Model { get; private set; }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Model = new DataModel(this, this.RichTextBox);

            // View now has a specific context and can binds to members inside current datamodel
            this.DataContext = Model;
        }

        private void OnConnectClick(object sender, RoutedEventArgs e)
        {
            Model.Connect();
        }

        private void OnDisconnectClick(object sender, RoutedEventArgs e)
        {
            Model.Disconnect();
        }

        private void OnRequestGoalsClick(object sender, RoutedEventArgs e)
        {
            Model.RequestGoals();
        }

        private void OnRequestMissionObjectivesClick(object sender, RoutedEventArgs e)
        {
            Model.RequestMissionObjectives();
        }

        private void OnRequestFlightSegmentsClick(object sender, RoutedEventArgs e)
        {
            Model.RequestFlightSegments();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Model.Disconnect();
        }

        private void OnRequestAllMobileSceneryClick(object sender, RoutedEventArgs e)
        {
            Model.RequestAllMobileSceneryObjects();
        }

    }
}
