#include <EduBot.h>
//#include <WiFi.h>
#include <image/EduBoy.h>
#include <image/EduBotFace.h>

#define AP_NAME     "U+NetB277"
#define AP_PASSWD   "32F2021315"
#define IFTTT_Host  "maker.ifttt.com"
#define IFTTT_KEY   "bY6fGh8DzR-4PYBtRTe2Siae5wLyD7WBhxzZXWmjFcn"
const int hostPort = 80;
char * MakerIFTTT_Event = "TV_OFF";
uint32_t SamsungTV_Power_Key = 0x19E60707;
uint8_t faceAnim1 = 0;
uint8_t faceAnim2 = 0;
uint8_t faceAnimState ;
uint32_t pre_ani_time = 0;

uint32_t ani_dur_time = 3000 ;
uint32_t ani1_dur_time = 3000 ;
uint32_t ani2_dur_time = 3000 ;
uint32_t end_time = 0 ;

enum 
{
  FACE_ANIM1 = 0,   // 0
  FACE_ANIM2,       // 1
} ANIM_Image;

enum 
{
  IMG_OROCA_LOGO_1 = 0,   // 0
  IMG_OROCA_LOGO_2,       // 1
  IMG_OROCA_LOGO_3,       // 2
  IMG_OROCA_LOGO_4,       // 3
  IMG_FACE_LEFT,          // 4
  IMG_FACE_RIGHT,         // 5
  IMG_FACE_SAD_1,         // 6
  IMG_FACE_SAD_2,         // 7
  IMG_FACE_SLEEP,         // 8
  IMG_FACE_SMILE,         // 9
  IMG_FACE_SPEAK,         // 10
  IMG_FACE_Rabbit_SLEEP,         // 11
  IMG_FACE_Rabbit_IDLE1,         // 12
  IMG_FACE_Rabbit_IDLE2,         // 13
  IMG_FACE_Rabbit_IDLE3,         // 14
  IMG_FACE_Rabbit_LAUGH1,         // 15
  IMG_FACE_Rabbit_LAUGH2,         // 16
  
  IMG_FACE_LEFT_INV,      // 11
  IMG_FACE_RIGHT_INV,     // 12
  IMG_FACE_SAD_1_INV,     // 13
  IMG_FACE_SAD_2_INV,     // 14
  IMG_FACE_SLEEP_INV,     // 15
  IMG_FACE_SMILE_INV,     // 16
  IMG_FACE_SPEAK_INV,     // 17
  IMG_FACE_Rabbit_SLEEP_INV,         // 18
  IMG_FACE_Rabbit_IDLE1_INV,         // 19
  IMG_FACE_Rabbit_IDLE2_INV,         // 20
  IMG_FACE_Rabbit_IDLE3_INV,         // 21
  IMG_FACE_Rabbit_LAUGH1_INV,         // 22
  IMG_FACE_Rabbit_LAUGH2_INV,         // 23

  IMG_EDUBOT_MAX
} EduBotImage;

typedef struct 
{
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint8_t *p_data;
  uint8_t  color;
} img_tbl_t;

static const img_tbl_t img_tbl[IMG_EDUBOT_MAX] = 
{
  {(128-48)/2, (64-48)/2, 48, 48, (uint8_t *)&edubot_logo[0*48*48/8], 1},
  {(128-48)/2, (64-48)/2, 48, 48, (uint8_t *)&edubot_logo[1*48*48/8], 1},
  {(128-48)/2, (64-48)/2, 48, 48, (uint8_t *)&edubot_logo[2*48*48/8], 1},
  {(128-48)/2, (64-48)/2, 48, 48, (uint8_t *)&edubot_logo[3*48*48/8], 1},

  {0, 0, 128, 64, (uint8_t *)&face_left[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_right[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_sad1[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_sad2[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_sleep[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_smile[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_speak[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_sleep[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_idle1[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_idle2[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_idle3[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_laugh1[0], 1},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_laugh2[0], 1},

  {0, 0, 128, 64, (uint8_t *)&face_left[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_right[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_sad1[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_sad2[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_sleep[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_smile[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_speak[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_sleep[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_idle1[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_idle2[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_idle3[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_laugh1[0], 0},
  {0, 0, 128, 64, (uint8_t *)&face_rabbit_laugh2[0], 0},
};

void drawEdubotImage(uint16_t index)
{
  img_tbl_t *p_img = (img_tbl_t *)&img_tbl[index];

  if (index >= IMG_EDUBOT_MAX)
  {
    return;
  }

  edubot.lcd.drawBitmap(p_img->x, p_img->y, p_img->p_data, p_img->width, p_img->height, p_img->color, !p_img->color);
}

void setup() {

  bool ret ;
  
  Serial.begin(115200);
  edubot.begin(115200);   
  edubot.tofEnd();   
  delay(10);
  
  /*WiFi.mode(WIFI_STA);
  WiFi.begin(AP_NAME, AP_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/

  ret = edubot.wifi.begin();

  Serial.println("WiFi connected");
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());

  faceAnimationSetting(IMG_FACE_Rabbit_SLEEP_INV, 100 , IMG_FACE_Rabbit_SLEEP_INV, 100 );
  pre_ani_time = millis(); 
  faceAnimState = FACE_ANIM1 ;

  //edubot.audio.setVolume(100);
  //Serial.println("playTTS start");
  //playTTS(); 
  //Serial.println("playTTS end");
  edubot.audio.setVolume(500);
}


void loop() {

  faceAnimation();
  uint32_t key_code;
    
  if (edubot.ir_remote.available() > 0  )
  {
    

    key_code = edubot.ir_remote.read();

    Serial.print("Key : 0x");
    Serial.print(key_code, HEX);
    Serial.print(" ");
    Serial.println(edubot.ir_remote.getTime());  
    if ( SamsungTV_Power_Key == key_code ){
    	faceAnimationSetting(IMG_FACE_Rabbit_IDLE2_INV, 1000 , IMG_FACE_Rabbit_IDLE1_INV, 100 );
     	//delay(7000);
      Serial.println("senario start"); 
      edubot.audio.playFile("/tv_off.wav", false);
      end_time = millis() + 7000 ;
     	    	
    }
  } 

  if( end_time < millis() && end_time != 0){
    Serial.println("senario finish"); 
    faceAnimationSetting(IMG_FACE_Rabbit_SLEEP_INV, 100 , IMG_FACE_Rabbit_SLEEP_INV, 100 );
    end_time = 0;
    sendSms(key_code);
    delay(2);
    sendSms(key_code);
    delay(2);
    sendSms(key_code);
    delay(2);
  }

   
}


void faceAnimationSetting( uint8_t anim_image1, uint32_t anim_image1_duration, uint8_t anim_image2,  uint32_t anim_image2_duration ) {
    
  faceAnim1 =  anim_image1;
  ani1_dur_time = anim_image1_duration ;
  faceAnim2 =  anim_image2;
  ani2_dur_time = anim_image2_duration ; 
  
  drawEdubotImage( faceAnim2 );
  drawEdubotImage( faceAnim1 );
  edubot.lcd.display();
  faceAnimState = FACE_ANIM1;
  ani_dur_time =  ani1_dur_time;  
        
  //faceAnimState = FACE_ANIM2 ;
  //faceAnimation();
    
}

void faceAnimation( void ) {

  if ( millis()- pre_ani_time >= ani_dur_time )
  {
      if( faceAnimState == FACE_ANIM1 ){
        drawEdubotImage( faceAnim2 );
        edubot.lcd.display();
        faceAnimState = FACE_ANIM2;
        ani_dur_time =  ani2_dur_time;    
                
      }else if( faceAnimState == FACE_ANIM2 ){
        drawEdubotImage( faceAnim1 );
        edubot.lcd.display();
        faceAnimState = FACE_ANIM1;
        ani_dur_time =  ani1_dur_time; 
      }
      pre_ani_time = millis();
  }    
}
  
void sendSms(uint32_t value ) {

  WiFiClient client;
  if (!client.connect(IFTTT_Host, hostPort)) 
  {
    Serial.println("Connection failed");
    return;
  }  
  
  String value1 = String(value, HEX) ;
  //String PostData = "{\"value1\" : \"ESP32 test!\" }";  
  String PostData = "{\"value1\" : \""+ value1 +"\" }";  
  Serial.println("connected to server… Getting name…");
  // send the HTTP PUT request:
  String request = "POST /trigger/";
  request += MakerIFTTT_Event;
  request += "/with/key/";
  request += IFTTT_KEY;
  request += " HTTP/1.1";
  
  Serial.println(request);
  client.println(request);  
  client.println("Host: " + String(IFTTT_Host));
  client.println("User-Agent: Energia/1.1");
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(PostData.length());
  client.println();
  client.println(PostData);
  client.println(); 

  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else 
    {
      delay(50);
    };
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();  
}
