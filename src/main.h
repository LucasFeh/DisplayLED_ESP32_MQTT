
// #include <FastLED.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include "MqttManager.h"
#include "WifiManager.h"
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


const char* ssid = "DiEletrons_Reuniao";
const char* password = "49433012";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient; // Cria um cliente WiFi para se conectar à rede
MQTTManager mqtt(mqtt_server, espClient);
WiFiManager wifi(ssid, password);


MatrixPanel_I2S_DMA *matrix = nullptr;

int i = 0;
int j = 0;
boolean directionx = true;
boolean directiony = true;
boolean AnimationActive;



extern void Display_init(){

 // Inicializa o painel
  if (matrix->begin()) {
    Serial.println("Painel inicializado com sucesso");
  } else {
    Serial.println("Falha ao inicializar o painel");
  }

            
  matrix->fillScreen(matrix->color444(0, 0, 0));
  matrix->setBrightness8(192);

  matrix->setCursor(0, 0);            
  matrix->setTextColor(matrix->color444(255, 0, 0)); 
  matrix->setTextSize(2);             
  matrix->print("Teste");

  matrix->setCursor(0, 16);           
  matrix->setTextColor(matrix->color444(0, 255, 0)); 
  matrix->print("2 displays");

}


extern void Animation(char* txt, long delay){

    if (AnimationActive) {

            matrix->clearScreen();
            
            // Animação no eixo X
            if (directionx) {
                matrix->setCursor(i, j);
                i++;
                matrix->setTextColor(matrix->color444(random(0, 256),random(0, 256), random(0, 256)));
                matrix->setTextSize(1);
                size_t aa = matrix->print(txt);
                int T = 127 - aa*6;
                if (i >= T) {
                    directionx = false;
                }
            } else {
                matrix->setCursor(i, j);
                i--;
                matrix->setTextColor(matrix->color444(random(0, 256),random(0, 256), random(0, 256)));
                matrix->setTextSize(1);
                matrix->print(txt);
                if (i < 1) {
                    directionx = true;
                }
            }

            // Controle do eixo Y
            if (directiony) {
                j++;
                if (j >= 25) {
                    directiony = false;
                }
            } else {
                j--;
                if (j <= 0) {
                    directiony = true;
                }
            }

            vTaskDelay(delay / portTICK_PERIOD_MS);  // Espera por 50ms
        }
}


extern void clear_display(){
    
    matrix-> clearScreen(); 

    matrix->fillScreen(matrix->color444(0, 0, 0));
    matrix->setBrightness8(192);

    // Exibe "Hello World" na tela
    matrix->setCursor(4, 4);            // Define a posição do texto
    matrix->setTextColor(matrix->color444(255,255,255)); // Define a cor do texto (roxo)
    matrix->setTextSize(2);              // Tamanho do texto
    matrix->print("LIMPOU :D");

    delay(2000);
    matrix-> clearScreen(); 

    mqtt.publishMessage("Mensagens", "Clear");
}