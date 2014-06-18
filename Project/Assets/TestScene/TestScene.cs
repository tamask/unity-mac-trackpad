using UnityEngine;

public class TestScene : MonoBehaviour
{
  public void Update()
  {
    Screen.lockCursor = true;

    MacTrackpad.Poll();

    if (MacTrackpad.deviceCount > 0)
      guiText.text = MacTrackpad.devices[0].touchCount.ToString();
  }
}
