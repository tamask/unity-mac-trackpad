using UnityEngine;

public class TestScene : MonoBehaviour
{
  public void Update()
  {
    Screen.lockCursor = true;

    MacTrackpad.Poll();

    if (MacTrackpad.deviceCount > 0)
      guiText.text = MacTrackpad.devices[0].touchCount.ToString();

    foreach (MacTrackpadDevice dev in MacTrackpad.devices)
      {
        foreach (MacTouch t in dev.touches)
          {
            Color c = Color.white;

            switch (t.phase)
              {
              case MacTouchPhase.Began:
                c = Color.blue;
                break;
              case MacTouchPhase.Moved:
                c = Color.green;
                break;
              case MacTouchPhase.Stationary:
                c = Color.yellow;
                break;
              case MacTouchPhase.Ended:
                c = Color.red;
                break;
              case MacTouchPhase.Cancelled:
                c = Color.magenta;
                break;
              }

            Debug.DrawRay(t.position, t.deltaPosition.sqrMagnitude == 0 ? Vector2.up * 0.01f : t.deltaPosition, c);
          }
      }
  }
}
