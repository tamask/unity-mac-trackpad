#import "Plugin.h"
#import "mactrackpad.h"

mt_touchbuffer_t *touch_buffer = nil;

NSView* view = nil;

@interface TrackingObject : NSResponder
{
}
- (void)touchesBeganWithEvent:(NSEvent *)event;
- (void)touchesMovedWithEvent:(NSEvent *)event;
- (void)touchesEndedWithEvent:(NSEvent *)event;
- (void)touchesCancelledWithEvent:(NSEvent *)event;
- (void)recordTouch:(NSTouch *)touch;
@end

@implementation TrackingObject

- (void)recordTouch:(NSTouch *)touch
{
  mt_addtouch (
    touch_buffer,
    (int)[touch identity],
    (int)[touch device],
    (int)[touch phase],
    touch.isResting ? 1 : 0,
    (float)touch.deviceSize.width,
    (float)touch.deviceSize.height,
    (float)touch.normalizedPosition.x,
    (float)touch.normalizedPosition.y);
}

- (void)touchesBeganWithEvent:(NSEvent *)event
{
  NSSet *touches = [event touchesMatchingPhase:NSTouchPhaseAny inView:view];

  if (mt_enqueue (touch_buffer) == 0)
    {
      for (NSTouch *touch in touches)
        [self recordTouch:touch];
    }
}

- (void)touchesMovedWithEvent:(NSEvent *)event
{
  NSSet *touches = [event touchesMatchingPhase:NSTouchPhaseAny inView:view];

  if (mt_enqueue (touch_buffer) == 0)
    {
      for (NSTouch *touch in touches)
        [self recordTouch:touch];
    }
}

- (void)touchesEndedWithEvent:(NSEvent *)event
{
  NSSet *touches = [event touchesMatchingPhase:NSTouchPhaseAny inView:view];

  if (mt_enqueue (touch_buffer) == 0)
    {
      for (NSTouch *touch in touches)
        [self recordTouch:touch];
    }
}

- (void)touchesCancelledWithEvent:(NSEvent *)event
{
  NSSet *touches = [event touchesMatchingPhase:NSTouchPhaseAny inView:view];

  if (mt_enqueue (touch_buffer) == 0)
    {
      for (NSTouch *touch in touches)
        [self recordTouch:touch];
    }
}

@end

TrackingObject* trackingObject = nil;

int ExtInit()
{
  NSApplication* app = [NSApplication sharedApplication];
  NSWindow* window = [app mainWindow];

  /* touch buffer init */
  if (touch_buffer != 0)
    {
      if (mt_free (touch_buffer))
        return 2;
    }
  if (mt_init (&touch_buffer, 32))
    return 1;

  /* tracking buffer init */
  if (trackingObject != nil)
    {
      [trackingObject release];
      trackingObject = nil;
    }
  trackingObject = [TrackingObject alloc];

  /* ns view init */
  view = [window contentView];
  [view setAcceptsTouchEvents:YES];
  [view setWantsRestingTouches:YES];
  [view setNextResponder:trackingObject];

  NSLog(@"MacTrackpad: initialized");

  return 0;
}

int ExtClear()
{
  return mt_clear (touch_buffer);
}

mt_touchbuffer_t *ExtGetBuffer()
{
  return touch_buffer;
}
