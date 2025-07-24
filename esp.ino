#include "esp_camera.h" 
#include<wifi.h>

 #define CAMERA_MODEL_AI_THINKER // Has PSRAM

 #include "camera_pins.h"

 const char *ssid = "Realme GT 2 Pro"; 
const char *password = "nopassword";
 void startCameraServer(); 
void setupLedFlash(int pin); 
void setup() {
 Serial.begin(115200); Serial.setDebugOutput(true);
 Serial.println();
 camera_config_t config;
 config.ledc_channel = LEDC_CHANNEL_0; config.ledc_timer = LEDC_TIMER_0;
 config.pin_d0 = Y2_GPIO_NUM; 
config.pin_d1 = Y3_GPIO_NUM; 
config.pin_d2 = Y4_GPIO_NUM; 
config.pin_d3 = Y5_GPIO_NUM;

config.pin_d4 = Y6_GPIO_NUM; 
config.pin_d5 = Y7_GPIO_NUM; 
config.pin_d6 = Y8_GPIO_NUM;
 config.pin_d7 = Y9_GPIO_NUM;
 config.pin_xclk = XCLK_GPIO_NUM; config.pin_pclk = PCLK_GPIO_NUM; config.pin_vsync = VSYNC_GPIO_NUM; config.pin_href = HREF_GPIO_NUM; config.pin_sccb_sda = SIOD_GPIO_NUM; config.pin_sccb_scl = SIOC_GPIO_NUM; config.pin_pwdn = PWDN_GPIO_NUM; config.pin_reset = RESET_GPIO_NUM; config.xclk_freq_hz = 20000000; config.frame_size = FRAMESIZE_UXGA; config.pixel_format = PIXFORMAT_JPEG; //config.pixel_format = PIXFORMAT_RGB565; config.grab_mode=CAMERA_GRAB_WHEN_EMPTY; config.fb_location = CAMERA_FB_IN_PSRAM; config.jpeg_quality = 12; 
config.fb_count = 1;
 if (config.pixel_format == PIXFORMAT_JPEG) { 
if (psramFound()) { 
config.jpeg_quality = 10; 
config.fb_count = 2; 
config.grab_mode = CAMERA_GRAB_LATEST;
 } else {
 // Limit the frame size when PSRAM is not available 
config.frame_size = FRAMESIZE_SVGA; config.fb_location = CAMERA_FB_IN_DRAM;
 } 
} else { 
config.frame_size = FRAMESIZE_240X240; 
#if CONFIG_IDF_TARGET_ESP32S3 config.fb_count = 2;
 #endif 
}
 #if defined(CAMERA_MODEL_ESP_EYE) pinMode(13, INPUT_PULLUP);
 pinMode(14, INPUT_PULLUP);
 #endif
 esp_err_t err = esp_camera_init(&config); 
if (err != ESP_OK) { 
Serial.printf("Camera init failed with error 0x%x", err); 
return; 
}
 sensor_t *s = esp_camera_sensor_get(); 
if (s->id.PID == OV3660_PID) {
 s->set_vflip(s, 1); 
s->set_brightness(s, 1); 
s->set_saturation(s, -2);
 }
 // drop down frame size for higher initial frame rate
 if (config.pixel_format == PIXFORMAT_JPEG) { 
s->set_framesize(s, FRAMESIZE_QVGA);
 }
 #if defined(CAMERA_MODEL_M5STACK_WIDE) ||defined(CAMERA_MODEL_M5STACK_ESP32CAM) s->set_vflip(s, 1);

s->set_hmirror(s, 1); 
#endif 
#if defined(CAMERA_MODEL_ESP32S3_EYE) 
s->set_vflip(s, 1);
 #endif 
#if defined(LED_GPIO_NUM) setupLedFlash(LED_GPIO_NUM);
 #endif WiFi.begin(ssid, password); WiFi.setSleep(false); 
while (WiFi.status() != WL_CONNECTED) 
{ delay(500); Serial.print(".");
 }
 Serial.println(""); 
Serial.println("WiFi connected"); startCameraServer(); 
Serial.print("Camera Ready! Use ’http://"); Serial.print(WiFi.localIP());
 Serial.println("’ to connect");
 } 
void loop() { 
delay(10000); 
}
 #define IN1 48
 #define IN2 49
 #define IN3 50 
#define IN4 51 
void setup() { 
Serial.begin(9600); 
pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT); 
} 
void loop() { 
if (Serial.available() > 0) { 
char value = Serial.read(); 
Serial.println(value);
 if (value == ’F’) { 
Forward();
 } else if (value == ’B’) {
 Backward(); 
} else if (value == ’S’) {
 Stop(); 
}else if(value == ’L’){ 
Left();
 }else if(value == ’R’){
 Right(); 
} 
} 
} 
void Forward() { 
digitalWrite(IN1, HIGH); 
digitalWrite(IN2, LOW);
 digitalWrite(IN3, HIGH); 
digitalWrite(IN4, LOW); 
} 
void Backward() {
 digitalWrite(IN1, LOW); 
digitalWrite(IN2, HIGH);
 digitalWrite(IN3, LOW); 
digitalWrite(IN4, HIGH);
 } 
void Stop() { 
digitalWrite(IN1, LOW);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW); 
digitalWrite(IN4, LOW); 
} 
void Left() { 
digitalWrite(IN1, HIGH); 
digitalWrite(IN2, LOW); 
digitalWrite(IN3, LOW); 
digitalWrite(IN4, HIGH);
 } 
void Right() { 
digitalWrite(IN1, LOW); 
digitalWrite(IN2, HIGH); 
digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);
 } 
