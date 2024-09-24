#include "main.h"
#include "xtensa/core-macros.h"
// #include "DisplayControl.h"
// #include "WifiManager.h"
// #include "MqttManager.h"
// #include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


// const char* ssid = "DiEletrons_Reuniao";
// const char* password = "49433012";
// const char* mqtt_server = "broker.hivemq.com";

#define PANEL_WIDTH (64 * 2)
#define PANEL_HEIGHT 32
#define PANELS_NUMBER 1
#define PIN_E 32
#define TOPIC_1 "DC"
#define TOPIC_2 "DC1"
#define TOPIC_3 "DC2"


// unsigned long lastMsg = 0; // Variável para armazenar o tempo da última mensagem enviada
// #define MSG_BUFFER_SIZE (50) // Define o tamanho do buffer de mensagens
// char msg[MSG_BUFFER_SIZE]; // Cria um buffer de mensagens

// WiFiClient espClient; // Cria um cliente WiFi para se conectar à rede
// MQTTManager mqtt(mqtt_server, espClient);
// WiFiManager wifi(ssid, password);

TaskHandle_t animationTaskHandle = NULL;

void taskAnimation(void * parameter);

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
        }
  }

}

void setup() {

  // xTaskCreate(taskAnimation, "TaskAnimation", 4096, NULL, 1, &animationTaskHandle);
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER);
  mxconfig.gpio.e = PIN_E;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;
    
  matrix = new MatrixPanel_I2S_DMA(mxconfig);
  
  Display_init();
  
  Serial.begin(115200);

  wifi.connect();
  mqtt.connect(TOPIC_1, TOPIC_2, TOPIC_3);

  mqtt.publishMessage("Mensagens", "mqtt Iniciou");
  
  mqtt.setCallback(callback);

}

void loop() {
  mqtt.connect(TOPIC_1, TOPIC_2, TOPIC_3);
  mqtt.loop();
  
  Animation("DI",3000);

}


void taskAnimation(void * parameter) {

    for (;;) {

       
        
    }
}
