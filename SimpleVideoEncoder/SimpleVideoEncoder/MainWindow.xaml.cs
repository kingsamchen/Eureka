/*
 @ 0xCCCCCCCC
*/

using System.Diagnostics;
using System.Threading;
using System.Windows;

using Win32 = Microsoft.Win32;

namespace SimpleVideoEncoder
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            Debug.WriteLine($"UI thread id: {Thread.CurrentThread.ManagedThreadId}");

            VideoEncodingDriver.EncodingProgressUpdated += (sender, tuple) =>
            {
                Dispatcher.Invoke(() =>
                {
                    PgbCompletion.Value = tuple.Item1;
                    TxbEta.Text = tuple.Item2 + "s";
                });
            };

            VideoEncodingDriver.EncodingSuccess += (sender, args) =>
            {
                Debug.WriteLine("Encoding success");
                Dispatcher.Invoke(() =>
                {
                    BtnNewEncoding.IsEnabled = true;
                    MessageBox.Show("Re-encode completes successfully!");
                });
            };

            VideoEncodingDriver.EncodingFailure += (sender, args) =>
            {
                Debug.WriteLine("Encoding failed");
                Dispatcher.Invoke(() =>
                {
                    BtnNewEncoding.IsEnabled = true;
                });
            };
        }

        private void BtnNewEncoding_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Win32.OpenFileDialog();
            bool? result = dlg.ShowDialog();
            if (result != true)
            {
                TxtVideo.Clear();
                return;
            }

            var path = dlg.FileName;

            TxtVideo.Text = path;

            BtnNewEncoding.IsEnabled = false;
            VideoEncodingDriver.EncodeVideo(path);
        }
    }
}
