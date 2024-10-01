#include "main.h"

TaskHandle_t animationTaskHandle = NULL;
TaskHandle_t GifTaskHandle = NULL;

void taskAnimation(void * parameter);
void taskGif(void * parameter);

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();

    if ( char(payload[0]) == '5') {

      Serial.println('entrou');
      matrix-> clearScreen();
      matrix->fillScreen(matrix->color444(255, 0, 0));
    // Enviar a mensagem MQTT (você precisará de um publish)

    mqtt.publishMessage("Mensagens", "Display Full Red");
  }
    if (char(payload[0]) == '2') {

      matrix-> clearScreen(); 
      matrix->fillScreen(matrix->color444(0, 255, 0));
    // Enviar a mensagem MQTT (você precisará de um publish)

    mqtt.publishMessage("Mensagens", "Display Full Green");
  }
    if (char(payload[0]) == '3') {

      matrix-> clearScreen(); 
      matrix->fillScreen(matrix->color444(0, 0, 255));
    mqtt.publishMessage("Mensagens", "Display Full blue");

  }
    if (char(payload[0]) == '4') {

      matrix-> clearScreen(); 
      matrix->fillScreen(matrix->color444(255, 255, 255));
    mqtt.publishMessage("Mensagens", "Display Full white");

  }
    if (strncmp((char*)payload, "clear", length) == 0) {

        clear_display();
  }
    if ( strncmp((char*)payload, "Animation", length) == 0) {

    AnimationActive = true;

    vTaskResume(animationTaskHandle);

    mqtt.publishMessage("Mensagens", "Animation Started");

  }
    if ( strncmp((char*)payload, "break", length) == 0) {

           if (animationTaskHandle != NULL) {
            vTaskSuspend(animationTaskHandle);
            mqtt.publishMessage("Mensagens", "Animation Stoped");
            Display_init();
        }
  }

    if ( strncmp((char*)payload, "gif", length) == 0) {

        gifActive = true;
        vTaskResume(GifTaskHandle);
        mqtt.publishMessage("Mensagens", "Gif Started");

  }
    if ( strncmp((char*)payload, "breakgif", length) == 0) {

          gifActive = false;
          vTaskSuspend(GifTaskHandle);
          mqtt.publishMessage("Mensagens", "Gif Stoped");
  }
      if (strncmp((char*)topic, "key2", length) == 0) {
        
               for ( u_int8_t i = 0; i < 8; i++) {
                 bit = (payload[i] == '1') ? 1 : 0; 
                    ascii += (bit << 7-i);  
                }

                Serial.println(ascii);
                AscII((char)ascii);
                ascii = 0;

      }

  strncmp((char*)payload, "time", length) == 0 ? time() : (void)0;


      for (int i = 0; i < num_fonts; i++) {
          if (strncmp((char*)payload, fonts[i].name, length) == 0) {
              Font_test(fonts[i].font, i);
              break; 
      }
    }


}

void setup() {
  Serial.begin(115200);

  wifi.connect();
   
  mqtt.connect(TOPIC_1, TOPIC_2, TOPIC_3);

  delay(5000);
  mqtt.publishMessage("Mensagens", "mqtt Iniciou");
  
  mqtt.setCallback(callback);

  xTaskCreate(taskAnimation, "TaskAnimation", 4096, NULL, 1, &animationTaskHandle);
  xTaskCreate(taskGif, "TaskGif", 4096, NULL, 1, &GifTaskHandle);

  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER);
  // FourScanPanel = new VirtualMatrixPanel((*matrix), NUM_ROWS, NUM_COLS, PANEL_WIDTH, PANEL_HEIGHT, VIRTUAL_MATRIX_CHAIN_TYPE);
  // FourScanPanel->setPhysicalPanelScanRate(FOUR_SCAN_32PX_HIGH);
  mxconfig.gpio.e = PIN_E;
    
  matrix = new MatrixPanel_I2S_DMA(mxconfig);
 

  Display_init();
  
  gif.begin(LITTLE_ENDIAN_PIXELS);
}

void loop() {
  mqtt.connect(TOPIC_1, TOPIC_2, TOPIC_3);
  mqtt.loop();
  
}


void taskAnimation(void * parameter) {

    for (;;) {
       Animation("DI", 50);
        
    }
}
void taskGif(void * parameter) {

    for (;;) {

      if(gifActive){
            Gif();
      }
        vTaskDelay(50);
    }
}
