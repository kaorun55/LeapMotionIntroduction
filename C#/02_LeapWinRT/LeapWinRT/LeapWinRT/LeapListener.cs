using System.Threading;
using Leap;

namespace NaturalSoftware.Leap.Toolkit
{
    public class LeapListener : Listener
    {
        // UIスレッドに戻すためのコンテキスト
        SynchronizationContext context = SynchronizationContext.Current;

        public delegate void LeapListenerEvent( Controller leap );

        // イベントハンドラ
        public event LeapListenerEvent OnConnectEvent;
        public event LeapListenerEvent OnDisconnectEvent;
        public event LeapListenerEvent OnInitEvent;
        public event LeapListenerEvent OnExitEvent;
        public event LeapListenerEvent OnFocusGainedEvent;
        public event LeapListenerEvent OnFocusLostEvent;
        public event LeapListenerEvent OnFrameEvent;

        public override void OnConnect( Controller leap )
        {
            Invoke( leap, OnConnectEvent );
        }

        public override void OnDisconnect( Controller leap )
        {
            Invoke( leap, OnDisconnectEvent );
        }

        public override void OnInit( Controller leap )
        {
            Invoke( leap, OnInitEvent );
        }

        public override void OnExit( Controller leap )
        {
            Invoke( leap, OnExitEvent );
        }

        public override void OnFocusGained( Controller leap )
        {
            Invoke( leap, OnFocusGainedEvent );
        }

        public override void OnFocusLost( Controller leap )
        {
            Invoke( leap, OnFocusLostEvent );
        }

        public override void OnFrame( Controller leap )
        {
            Invoke( leap, OnFrameEvent );
        }

        // イベントを発行する
        private void Invoke( Controller leap, LeapListenerEvent handler )
        {
            // UIスレッドに同期的に処理を戻す
            context.Post( state =>
            {
                if ( handler != null ) {
                    handler( leap );
                }
            }, null );
        }
    }
}
