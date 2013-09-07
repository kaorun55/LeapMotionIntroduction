using Leap;
using Windows.UI;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Shapes;

// 空白ページのアイテム テンプレートについては、http://go.microsoft.com/fwlink/?LinkId=234238 を参照してください

namespace LeapWinRT
{
    /// <summary>
    /// それ自体で使用できる空白ページまたはフレーム内に移動できる空白ページ。
    /// </summary>
    public sealed partial class MainPage : Page
    {
#if true
        Controller leap;

        public MainPage()
        {
            this.InitializeComponent();

            leap = new Controller();

            CompositionTarget.Rendering += CompositionTarget_Rendering;
        }

        void CompositionTarget_Rendering( object sender, object e )
        {
            CanvasPaint.Children.Clear();
            var windowWidth = (float)ActualWidth;
            var windowHeight = (float)ActualHeight;

            Leap.Frame frame = leap.Frame();
            InteractionBox interactionBox = leap.Frame().InteractionBox;

            foreach ( Pointable pointable in leap.Frame().Pointables ) {
                Leap.Vector normalizedPosition = interactionBox.NormalizePoint( pointable.StabilizedTipPosition );
                float tx = normalizedPosition.x * windowWidth;
                float ty = windowHeight - normalizedPosition.y * windowHeight;

                Color color = Color.FromArgb( 0, 0, 0, 0 );
                if ( pointable.TouchDistance > 0 && pointable.TouchZone != Pointable.Zone.ZONENONE ) {
                    var alpha = 255 - (int)(255 * pointable.TouchDistance);
                    color = Color.FromArgb( (byte)alpha, 0x0, 0xff, 0x0 );
                }
                else if ( pointable.TouchDistance <= 0 ) {
                    var alpha = -(int)(255 * pointable.TouchDistance);
                    color = Color.FromArgb( (byte)alpha, 0xff, 0x0, 0x0 );
                }
                else {
                    var alpha = 50;
                    color = Color.FromArgb( (byte)alpha, 0x0, 0x0, 0xff );
                }

                // 点を表示する
                var ellipse  = new Ellipse()
                {
                    Width = 40,
                    Height = 40,
                    Fill = new SolidColorBrush( color ),
                };

                Canvas.SetLeft( ellipse, tx );
                Canvas.SetTop( ellipse, ty );

                CanvasPaint.Children.Add( ellipse );
            }
        }
#else
        Controller leap;
        LeapListener listener;

        public MainPage()
        {
            this.InitializeComponent();

            listener = new LeapListener();
            listener.OnFrameEvent += listener_OnFrameEvent;

            leap = new Controller();
            leap.AddListener( listener );
        }

        void listener_OnFrameEvent( Controller leap )
        {
            CanvasPaint.Children.Clear();
            var windowWidth = (float)ActualWidth;
            var windowHeight = (float)ActualHeight;

            Leap.Frame frame = leap.Frame();
            InteractionBox interactionBox = leap.Frame().InteractionBox;

            foreach ( Pointable pointable in leap.Frame().Pointables ) {
                Leap.Vector normalizedPosition = interactionBox.NormalizePoint( pointable.StabilizedTipPosition );
                float tx = normalizedPosition.x * windowWidth;
                float ty = windowHeight - normalizedPosition.y * windowHeight;

                Color color = Color.FromArgb( 0, 0, 0, 0 );
                if ( pointable.TouchDistance > 0 && pointable.TouchZone != Pointable.Zone.ZONENONE ) {
                    var alpha = 255 - (int)(255 * pointable.TouchDistance);
                    color = Color.FromArgb( (byte)alpha, 0x0, 0xff, 0x0 );
                }
                else if ( pointable.TouchDistance <= 0 ) {
                    var alpha = -(int)(255 * pointable.TouchDistance);
                    color = Color.FromArgb( (byte)alpha, 0xff, 0x0, 0x0 );
                }
                else {
                    var alpha = 50;
                    color = Color.FromArgb( (byte)alpha, 0x0, 0x0, 0xff );
                }

                // 点を表示する
                var ellipse  = new Ellipse()
                {
                    Width = 40,
                    Height = 40,
                    Fill = new SolidColorBrush( color ),
                };

                Canvas.SetLeft( ellipse, tx );
                Canvas.SetTop( ellipse, ty );

                CanvasPaint.Children.Add( ellipse );
            }
        }
#endif

        /// <summary>
        /// このページがフレームに表示されるときに呼び出されます。
        /// </summary>
        /// <param name="e">このページにどのように到達したかを説明するイベント データ。Parameter 
        /// プロパティは、通常、ページを構成するために使用します。</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }
    }
}
