#ifndef mactrackpad_h
#define mactrackpad_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct mt_touch_t {
  int id;
  int device_id;
  int phase;
  int is_resting;
  float device_width;
  float device_height;
  float normalized_pos_x;
  float normalized_pos_y;
  struct mt_touch_t *next;
};

struct mt_touch_t;
typedef struct mt_touch_t mt_touch_t;

struct mt_touchevent_t {
  float time;
  int touch_count;
  mt_touch_t *touches_tail;
  mt_touch_t *touches_head;
  struct mt_touchevent_t *next;
};

struct mt_touchevent_t;
typedef struct mt_touchevent_t mt_touchevent_t;

struct mt_touchbuffer_t {
  float time_start;
  int max_events;
  int event_count;
  mt_touchevent_t *tail;
  mt_touchevent_t *head;
};

struct mt_touchbuffer_t;
typedef struct mt_touchbuffer_t mt_touchbuffer_t;

/* public methods */

int mt_init (mt_touchbuffer_t **buffer, int max_events);

int mt_free (mt_touchbuffer_t *buffer);

int mt_enqueue (mt_touchbuffer_t *buffer);

int mt_dequeue (mt_touchbuffer_t *buffer);

int mt_addtouch (mt_touchbuffer_t *buffer, int id, int device_id, int phase, int is_resting, float device_width, float device_height, float normalized_pos_x, float normalized_pos_y);

int mt_clear (mt_touchbuffer_t *buffer);

/* private methods */

int _mt_touchevent_init (mt_touchevent_t **event);

int _mt_touchevent_free (mt_touchevent_t *event);

int _mt_touch_init (mt_touch_t **touch);

int _mt_touch_free (mt_touch_t *touch);

#endif
