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


namespace AIDestroyer
{
    /// <summary>
    /// Interaction logic for View.xaml
    /// </summary>
    public partial class View : Window
    {
        public DestroyerModel Model
        { get; private set; }

        public View()
        {
            InitializeComponent();

            DataContext = Model;

            Model.ConnectToSimulation();
            Model.UpdateSimList();
        }

        protected override void OnInitialized(EventArgs e)
        {
            base.OnInitialized(e);

            Model = new DestroyerModel();
            Model.Error += new EventHandler<MessageEventArgs>(ModelError);

            Model.Init();
        }

        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            base.OnClosing(e);

            Model.DisconnectFromSimulation();
        }

        private void ModelError(object sender, MessageEventArgs e)
        {
            MessageBox.Show(e.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error, MessageBoxResult.OK);
        }

        private void QueryButtonClick(object sender, RoutedEventArgs e)
        {
            Model.UpdateSimList();
        }

        private void SimObjectSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {
                Model.SelectSimObject(e.AddedItems[0] as SimObject);
            }
        }

        private void CheckBoxChecked(object sender, RoutedEventArgs e)
        {
            CheckBox box;

            box = sender as CheckBox;
            if (box != null && Model.SelectedObject != null)
            {
                Model.IsSimObjectAlive = (box.IsChecked != null) ? (bool)box.IsChecked : false;
            }
        }
    }
}
