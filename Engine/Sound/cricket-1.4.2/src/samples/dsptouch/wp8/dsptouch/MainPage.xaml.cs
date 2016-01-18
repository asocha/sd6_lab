using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using dsptouch.Resources;
using dsptouch_native;

namespace dsptouch
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        private void LayoutRoot_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            SetParams(e);
            DspTouch.StartEffect();
        }

        private void LayoutRoot_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            DspTouch.StopEffect();
        }

        private void LayoutRoot_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            SetParams(e);
        }

        private void SetParams(System.Windows.Input.MouseEventArgs e)
        {
            Point pos = e.GetPosition(this);
            DspTouch.SetEffectParams((float) (pos.X/this.ActualWidth), (float) ((this.ActualHeight-pos.Y)/this.ActualHeight));
        }
    }
}
