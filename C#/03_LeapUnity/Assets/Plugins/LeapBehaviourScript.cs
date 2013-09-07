using UnityEngine;
using System.Collections;
using Leap;

public class LeapBehaviourScript : MonoBehaviour {
    Controller controller = new Controller();

	public int FingerCount;
	public GameObject[] Fingers;
	
	void SetVisible( GameObject obj, bool visible )
	{
		foreach( Renderer component in obj.GetComponents<Renderer>() ) {
			component.enabled = visible;
		}
	}
	
    void Start ()
    {
    }

    void Update ()
    {
        Frame frame = controller.Frame();
		FingerCount = frame.Fingers.Count;
		
        InteractionBox interactionBox = frame.InteractionBox;

		for ( int i = 0; i  < Fingers.Length; i++ ) {
			var finger = frame.Fingers[i];
			var gameObject = Fingers[i];
			SetVisible( Fingers[i], finger.IsValid );
			if ( finger.IsValid ) {
	            Vector normalizedPosition = interactionBox.NormalizePoint( finger.TipPosition );
				normalizedPosition *= 10;
				normalizedPosition.z = -normalizedPosition.z;
				gameObject.transform.localPosition = ToVector3( normalizedPosition );
			}
		}
    }
				
	Vector3 ToVector3( Vector v ) 	
	{
		return new UnityEngine.Vector3( v.x, v.y, v.z );
	}
}
