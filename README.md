# MacTrackpad plugin for Unity 4.5

Provides access to multi-touch devices on OSX.

## Install

Xcode project is provided under `Build/`.

Prebuilt version is available via `MacTrackpad.unitypackage`

## Usage

Provides a class `MacTrackpad`.  Call `MacTrackpad.Poll()` every frame
to collect touch events.

Behaviour is almost identical to `Input.touches`, except here it's
`MacTrackpad.devices[n].touches` since there can be multiple touch
devices connected.  `MacTrackpad.deviceCount` gives you the number of
active touch devices (that have sent events since first call to
`MacTrackpad.Poll()`.

Instead of `Touch` there's `MacTouch`, exact same properties, except
with an additional `time` property defining when the `MacTouch` object
was generated from runtime start.  And instead of a `TouchPhase` enum
there's a `MacTouchPhase` enum.

## Bugs

Thoroughly untested, for now.

## License

COPY-IT-RIGHT
