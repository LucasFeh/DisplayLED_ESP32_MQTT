#include "main.h"


#define PANEL_WIDTH (64 * 1)
#define PANEL_HEIGHT (32 * 1)
#define PANELS_NUMBER 2
#define PIN_E 32
#define TOPIC_1 "DC"
#define TOPIC_2 "DC1"
#define TOPIC_3 "DC2"

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

    if ( char(payload[0]) == '1') {

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
    matrix->fillScreen(matrix->color444(0, 0, 0));
    matrix->setBrightness8(192);

    // Exibe "Hello World" na tela
    matrix->setCursor(0, 0);            // Define a posição do texto
    matrix->setTextColor(matrix->color444(255, 255, 255)); // Define a cor do texto (roxo)
    matrix->setTextSize(1);              // Tamanho do texto
    matrix->print("teste");

    mqtt.publishMessage("Mensagens", "Display Full White");
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

        xTaskCreate(taskGif, "TaskGif", 4096, NULL, 1, &GifTaskHandle);
        mqtt.publishMessage("Mensagens", "Gif Started");

  }
    if ( strncmp((char*)payload, "breakgif", length) == 0) {

          vTaskDelete(GifTaskHandle);
          mqtt.publishMessage("Mensagens", "Gif Stoped");
          ESP.restart();
        
  }

}

void setup() {

  xTaskCreate(taskAnimation, "TaskAnimation", 4096, NULL, 1, &animationTaskHandle);

  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER);
  mxconfig.gpio.e = PIN_E;
  mxconfig.gpio.e = 18;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;
    
  matrix = new MatrixPanel_I2S_DMA(mxconfig);
  
  Display_init();
  
  Serial.begin(115200);
  gif.begin(LITTLE_ENDIAN_PIXELS);

  wifi.connect();
  mqtt.connect(TOPIC_1, TOPIC_2, TOPIC_3);

  mqtt.publishMessage("Mensagens", "mqtt Iniciou");
  
  mqtt.setCallback(callback);

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

        Gif();
        
    }
}
