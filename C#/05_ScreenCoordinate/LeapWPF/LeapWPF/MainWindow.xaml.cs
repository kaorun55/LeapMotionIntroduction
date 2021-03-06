﻿using System;
using System.Windows;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using Leap;

namespace LeapWPF
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        Controller leap = new Controller();
        float windowWidth = 1400;
        float windowHeight = 800;
        DrawingAttributes touchIndicator = new DrawingAttributes();

        Screen locatedScreen;

        public MainWindow()
        {
            InitializeComponent();
            CompositionTarget.Rendering += Update;
            touchIndicator.Width = 20;
            touchIndicator.Height = 20;
            touchIndicator.StylusTip = StylusTip.Ellipse;

            locatedScreen = leap.LocatedScreens[0];

            foreach ( var screen in leap.LocatedScreens ) {
                TextScreen.Text = string.Format(
                    "Screen Pixels: {1},{2}{0}Horizontal axis: {3}{0}Vertical axis: {4}{0}Bottom left corner: {5}", Environment.NewLine,
                    screen.WidthPixels, screen.HeightPixels,
                    screen.HorizontalAxis,
                    screen.VerticalAxis,
                    screen.BottomLeftCorner );
            }
        }

        protected void Update( object sender, EventArgs e )
        {
            paintCanvas.Strokes.Clear();
            windowWidth = (float)this.Width;
            windowHeight = (float)this.Height;

            Frame frame = leap.Frame();
            InteractionBox interactionBox = leap.Frame().InteractionBox;

            foreach ( Pointable pointable in leap.Frame().Pointables ) {
                // InteractionBox を利用した座標変換
                Leap.Vector normalizedPosition = interactionBox.NormalizePoint( pointable.StabilizedTipPosition );

                // Intersect Point を利用した座標変換
                //Leap.Vector normalizedPosition = locatedScreen.Intersect( pointable, true );

                // Projection Point を利用した座標変換
                //Leap.Vector normalizedPosition = locatedScreen.Project( pointable.TipPosition, false );

                float tx = normalizedPosition.x * windowWidth;
                float ty = windowHeight - normalizedPosition.y * windowHeight;

                int alpha = 255;
                // ホバー状態
                if ( pointable.TouchDistance > 0 && pointable.TouchZone != Pointable.Zone.ZONENONE ) {
                    alpha = 255 - (int)(255 * pointable.TouchDistance);
                    touchIndicator.Color = Color.FromArgb( (byte)alpha, 0x0, 0xff, 0x0 );
                }
                // タッチ状態
                else if ( pointable.TouchDistance <= 0 ) {
                    alpha = -(int)(255 * pointable.TouchDistance);
                    touchIndicator.Color = Color.FromArgb( (byte)alpha, 0xff, 0x0, 0x0 );
                }
                // タッチ対象外
                else {
                    alpha = 50;
                    touchIndicator.Color = Color.FromArgb( (byte)alpha, 0x0, 0x0, 0xff );
                }

                StylusPoint touchPoint = new StylusPoint( tx, ty );
                StylusPointCollection tips = new StylusPointCollection( new StylusPoint[] { touchPoint } );
                Stroke touchStroke = new Stroke( tips, touchIndicator );
                paintCanvas.Strokes.Add( touchStroke );
            }
        }
    }
}
