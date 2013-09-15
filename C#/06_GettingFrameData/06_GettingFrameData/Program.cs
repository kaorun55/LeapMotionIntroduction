using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Leap;

namespace _06_GettingFrameData
{
    class Program
    {
        static void Main( string[] args )
        {
            // ポーリング方式でのアプリケーション実装例
            //PollingBasics1();

            // IsConnectedプロパティおよびHasFocusプロパティを利用したイベントの検出
            // PollingBasics2();

            // 現在のフレームと、直前の5フレームを取得する
            //GetBeforeFrame();

            // 指のIDとフレーム履歴を利用して、10フレームの指の平均座標を求める
            AverageFingerPosition();
        }

        // ポーリング方式でのアプリケーション実装例
        static void PollingBasics1()
        {
            Controller leap = new Controller();
            long previousFrameId = -1;

            // 初期化処理（OnInit()相当）をここに書く
            // 無限ループ内で、前回のフレームのIDと比較して新しいフレームを取得する
            while ( true ) {
                var frame = leap.Frame();
                if ( previousFrameId == frame.Id ) {
                    continue;
                }

                previousFrameId = frame.Id;
                Console.WriteLine( "Frame ID : " + frame.Id );

                // フレーム更新処理（OnFrame()相当）をここに書く
            }

            // 終了処理（OnExit()相当）をここに書く
        }

        // IsConnectedプロパティおよびHasFocusプロパティを利用したイベントの検出
        static void PollingBasics2()
        {
            Controller leap = new Controller();

            bool isPrevConnected = false;
            bool hadPrevFocus = false;
            long previousFrameId = -1;

            // 初期化処理（OnInit()相当）をここに書く
            // 無限ループ内で、前回のフレームのIDと比較して新しいフレームを取得する
            while ( true ) {
                var frame = leap.Frame();

                // 接続状態を確認する
                {
                    bool isCurrentConnected = leap.IsConnected;
                    if ( isPrevConnected != isCurrentConnected ) {
                        if ( isCurrentConnected ) {
                            // Leap Motionコントローラーが接続された（OnConnected()相当）
                            Console.WriteLine( "Leap Motion connected." );
                        }
                        else {
                            // Leap Motionコントローラーが抜かれた（OnDisconnected()相当）
                            Console.WriteLine( "Leap Motion disconnected." );
                        }
                    }

                    // 今回の接続状態を保持する
                    isPrevConnected = isCurrentConnected;
                }

                // フォーカス状態を確認する
                {
                    bool hadCurrentFocus = leap.HasFocus;
                    if ( hadPrevFocus != hadCurrentFocus ) {
                        if ( hadCurrentFocus ) {
                            // アプリケーションのフォーカスが有効になった（OnFocusGained()相当）
                            Console.WriteLine( "Focus gained." );
                        }
                        else {
                            // アプリケーションのフォーカスが無効になった（OnFocusLost()相当）
                            Console.WriteLine( "Focus lost." );
                        }
                    }

                    // 今回のフォーカス状態を保持する
                    hadPrevFocus = hadCurrentFocus;
                }

                // フレームが更新されていなければ何もしない
                {
                    if ( previousFrameId == frame.Id ) {
                        continue;
                    }

                    Console.WriteLine( "Frame ID : " + frame.Id );
                    previousFrameId = frame.Id;
                }

                // フレーム更新処理（onFrame()相当）をここに書く
            }

            // 終了処理（OnExit()相当）をここに書く
        }

        // 現在のフレームと、直前の5フレームを取得する
        static void GetBeforeFrame()
        {
            Controller leap = new Controller();
            long previousFrameId = -1;

            while ( true ) {
                // 最新のフレームを取得する（leap.Frame( 0 ) と同じ）
                var currentFrame = leap.Frame();
                if ( previousFrameId == currentFrame.Id ) {
                    continue;
                }

                previousFrameId = currentFrame.Id;

                // 直前の5フレームを取得する
                Console.Write( currentFrame.Id + ", " );
                for ( int i = 1; i <= 5; ++i ) {
                    var previousFrame = leap.Frame( i );
                    Console.Write( previousFrame.Id + ", " );
                }

                Console.WriteLine();
            }

            // 終了処理（onExit()相当）
        }

        // 指のIDとフレーム履歴を利用して、10フレームの指の平均座標を求める
        static void AverageFingerPosition()
        {
            Controller leap = new Controller();
            long previousFrameId = -1;

            // 初期化処理（OnInit()相当）をここに書く
            // 無限ループ内で、前回のフレームのIDと比較して新しいフレームを取得する
            while ( true ) {
                var frame = leap.Frame();
                if ( previousFrameId == frame.Id ) {
                    continue;
                }

                previousFrameId = frame.Id;
                //Console.WriteLine( "Frame ID : " + frame.Id );

                // フレーム更新処理（OnFrame()相当）をここに書く

                //Average a finger position for the last 10 frames
                int count = 0;
                Vector average = new Vector ();

                // 最初のに検出された指の平均を取得する
                Finger fingerToAverage = frame.Fingers[0];

                for ( int i = 0; i < 10; i++ ) {
                    // 指定したフレームの、指定した指IDの、指データを取得する
                    Finger fingerFromFrame = leap.Frame( i ).Finger( fingerToAverage.Id );
                    if ( fingerFromFrame.IsValid ) {
                        // 取得した指データが有効であれば平均値の算出に利用する
                        average += fingerFromFrame.TipPosition;
                        count++;
                    }

                    average /= count;
                }

                // 平均値を表示する
                Console.WriteLine( average );
            }

            // 終了処理（OnExit()相当）をここに書く
        }
    }
}
