#include "mactrackpad.h"

int
mt_init (mt_touchbuffer_t **buffer, int max_events)
{
  struct timeval tv;

  *buffer = malloc (sizeof (mt_touchbuffer_t));

  if ((*buffer) == 0)
    return 0;

  if (max_events < 0)
    max_events = 0;

  gettimeofday (&tv, NULL);

  (*buffer)->time_start = (float)(1000000 * tv.tv_sec + tv.tv_usec) / 1000000.0;
  (*buffer)->max_events = max_events;
  (*buffer)->event_count = 0;
  (*buffer)->tail = 0;
  (*buffer)->head = 0;

  return 0;
}

int
mt_free (mt_touchbuffer_t *buffer)
{
  mt_touchevent_t *event;
  mt_touchevent_t *next;

  event = buffer->tail;

  while (event != 0)
    {
      next = event->next;
      _mt_touchevent_free (event);
      event = next;
    }

  buffer->tail = 0;
  buffer->head = 0;

  return 0;
}

int
mt_enqueue (mt_touchbuffer_t *buffer)
{
  mt_touchevent_t *event;

  if (buffer->head != 0 && buffer->head->touch_count == 0)
    return 1;

  if (_mt_touchevent_init (&event))
    return 2;

  if (buffer->tail == 0)
    buffer->tail = buffer->head = event;
  else
    {
      buffer->head->next = event;
      buffer->head = event;
    }

  buffer->event_count++;
  event->time -= buffer->time_start;

  while (buffer->event_count > buffer->max_events)
    mt_dequeue (buffer);

  return 0;
}

int
mt_dequeue (mt_touchbuffer_t *buffer)
{
  mt_touchevent_t *event;

  if (buffer->event_count == 0)
    return 1;

  event = buffer->tail;

  if (event != 0)
    {
      buffer->tail = event->next;
      _mt_touchevent_free (event);
      buffer->event_count--;
    }

  if (buffer->event_count == 0)
    {
      buffer->head = 0;
      buffer->tail = 0;
    }

  return 0;
}

int
mt_addtouch (mt_touchbuffer_t *buffer, int id, int device_id, int phase, int is_resting, float device_width, float device_height, float normalized_pos_x, float normalized_pos_y)
{
  mt_touchevent_t *event;
  mt_touch_t *touch;

  event = buffer->head;

  if (event == 0)
    return 1;

  _mt_touch_init (&touch);

  if (touch == 0)
    return 2;

  touch->id = id;
  touch->device_id = device_id;
  touch->phase = phase;
  touch->is_resting = is_resting;
  touch->device_width = device_width;
  touch->device_height = device_height;
  touch->normalized_pos_x = normalized_pos_x;
  touch->normalized_pos_y = normalized_pos_y;

  if (event->touches_tail == 0)
    event->touches_tail = event->touches_head = touch;
  else
    {
      event->touches_head->next = touch;
      event->touches_head = touch;
    }

  event->touch_count++;

  return 0;
}

int
mt_clear (mt_touchbuffer_t *buffer)
{
  while (buffer->event_count > 0)
    {
      if (mt_dequeue (buffer))
        return 1;
    }

  return 0;
}

int
_mt_touchevent_init (mt_touchevent_t **event)
{
  struct timeval tv;

  *event = malloc (sizeof (mt_touchevent_t));

  if (*event == 0)
    return 0;

  gettimeofday (&tv, NULL);

  (*event)->time = (float)(1000000 * tv.tv_sec + tv.tv_usec) / 1000000.0;
  (*event)->touch_count = 0;
  (*event)->touches_tail = 0;
  (*event)->touches_head = 0;
  (*event)->next = 0;

  return 0;
}

int
_mt_touchevent_free (mt_touchevent_t *event)
{
  mt_touch_t *touch;
  mt_touch_t *next;

  touch = event->touches_tail;

  while (touch != 0)
    {
      next = touch->next;
      _mt_touch_free (touch);
      touch = next;
    }

  free (event);

  return 0;
}

int
_mt_touch_init (mt_touch_t **touch)
{
  *touch = malloc (sizeof (mt_touch_t));

  if (*touch == 0)
    return 1;

  (*touch)->id = 0;
  (*touch)->device_id = 0;
  (*touch)->phase = 0;
  (*touch)->is_resting = 0;
  (*touch)->device_width = 0.0f;
  (*touch)->device_height = 0.0f;
  (*touch)->normalized_pos_x = 0.0f;
  (*touch)->normalized_pos_y = 0.0f;
  (*touch)->next = 0;

  return 0;
}

int
_mt_touch_free (mt_touch_t *touch)
{
  free (touch);

  return 0;
}
