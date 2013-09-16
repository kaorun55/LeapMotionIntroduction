using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Leap;

namespace _07_TrackingHandFingerAndTools
{
    class Program
    {
        class SampleListener : Listener
        {
            int handId = -1;

            public override void OnFrame( Controller leap )
            {
                var frame = leap.Frame();

#if true
                // 今回のフレームで検出したすべての手、指、ツール
                HandList hands = frame.Hands;
                FingerList fingers = frame.Fingers;
                ToolList tools = frame.Tools;
                PointableList pointables = frame.Pointables;
    
                Console.WriteLine( string.Format( "手 : {0} 指 : {1} ツール : {2} ポインタ : {3}",
                    hands.Count, fingers.Count, tools.Count, pointables.Count ) );
#endif
    
#if false
                // 手のIDから、同じ手を追跡し続ける
                if ( handId == -1 ) {
                    handId = frame.Hands[0].Id;
                }
                else {
                    Hand hand = frame.Hand( handId );
                    handId = hand.Id;

                    // 手の情報を表示する
                    Console.WriteLine( string.Format( "ID : {0} 位置 : {1} 速度 : {2} 法線 : {3} 向き : {4}",
                    hand.Id, hand.PalmPosition, hand.PalmVelocity, hand.PalmNormal, hand.Direction ) );
                }
#endif
    
#if false
                // 一番左、右、手前の手を取得する
                HandList hands = frame.Hands;
                Hand leftMost = hands.Leftmost;
                Hand rightMost = hands.Rightmost;
                Hand frontMost = hands.Frontmost;

                Console.WriteLine( string.Format( "左 : {0} 右 : {1} 手前 : {2}",
                    leftMost.PalmPosition, rightMost.PalmPosition, frontMost.PalmPosition ) );
#endif
    
#if false
                // 手に属している指とツールを取得する
                foreach ( var hand in frame.Hands ) {
                    Console.WriteLine( string.Format( "ID : {0} ポインタ : {1} 指: {2} ツール : {3}",
                        hand.Id, hand.Pointables.Count, hand.Fingers.Count, hand.Fingers.Count ) );
                }
#endif
    
#if false
                // 指の情報を表示する
                Finger finger = frame.Fingers[0];
                Console.WriteLine( string.Format( "ID : {0} 位置 : {1} 速度 : {2} 向き : {3}",
                    finger.Id, finger.TipPosition, finger.TipVelocity, finger.Direction ) );
#endif
            }
        }

        static void Main( string[] args )
        {
            SampleListener listener = new SampleListener();
            Controller leap = new Controller();
            leap.AddListener( listener );

            Console.WriteLine( "Press Enter to quit..." );
            Console.ReadLine();

            leap.RemoveListener( listener );
        }
    }
}
