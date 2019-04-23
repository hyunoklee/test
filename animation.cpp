#include <EduBot.h>
#include "animation.h"


typedef struct
{
  bool is_init;  
} animation_node_t;


animation_node_t animation_play;

volatile SemaphoreHandle_t play_semaphore = NULL;
volatile TaskHandle_t _play_task_handle = NULL;


void animationThread(void *arg)
{
  Animation *p_animation = (Animation *)arg;

  while(1)
  {

  }
}


Animation::Animation(void)
{
  animation_play.is_init = false;
}

Animation::~Animation()
{
	
}

bool Animation::isInit(void)
{
  return animation_play.is_init;
}

bool Animation::begin(void)
{ 
  Serial.print("Animation begin");  
  if (animation_play.is_init == false)
  {
    speaker.begin();

    if (play_semaphore == NULL)
    {
      play_semaphore = xSemaphoreCreateBinary();
    }
    xTaskCreate(animationThread, "animationThread", 4096, this, 1, (TaskHandle_t*)&_play_task_handle);    
    

    if (!_play_task_handle)
    {
      animation_play.is_init = false;
    }
    else
    {
      animation_play.is_init = true;
    }    
    
  }
  

  return animation_play.is_init;
}

void Animation::drawLcdImage(uint16_t index)
{
  ani_img_tbl_t *p_img = (ani_img_tbl_t *)&ani_img_tbl[index];

  if (index >= IMG_EDUBOT_MAX)
  {
    return;
  }

  edubot.lcd.drawBitmap(p_img->x, p_img->y, p_img->p_data, p_img->width, p_img->height, p_img->color, !p_img->color);
}
