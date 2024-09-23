// How to use this library with a FM6126 panel, thanks goes to:
// https://github.com/hzeller/rpi-rgb-led-matrix/issues/746

// #ifdef IDF_BUILD
// #include <stdio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <driver/gpio.h>
// #include "sdkconfig.h"
// #endif

#include <Arduino.h>
#include "xtensa/core-macros.h"
#ifdef VIRTUAL_PANE
#include <ESP32-VirtualMatrixPanel-I2S-DMA.h>
#else
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#endif
#include "main.h"
#include <WiFi.h>
#include <PubSubClient.h>

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// Definir a configuração do painel (largura e altura do painel LED)
#define PANEL_WIDTH (64 * 2)
#define PANEL_HEIGHT 32
#define PANELS_NUMBER 1
#define PIN_E 32
int i = 0;
int j = 0;
boolean directionx = true;
boolean directiony = true;
boolean AnimationActive;

const char* ssid = "DiEletrons_Reuniao";
const char* password = "49433012";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient; // Cria um cliente WiFi para se conectar à rede
PubSubClient client(espClient); // Cria um cliente MQTT para se conectar ao servidor MQTT
unsigned long lastMsg = 0; // Variável para armazenar o tempo da última mensagem enviada
#define MSG_BUFFER_SIZE (50) // Define o tamanho do buffer de mensagens
char msg[MSG_BUFFER_SIZE]; // Cria um buffer de mensagens

MatrixPanel_I2S_DMA *matrix = nullptr;


TaskHandle_t animationTaskHandle = NULL;

void taskAnimation(void * parameter);

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  // Loop até que estejamos reconectados
  while (!client.connected()){
    Serial.print("Tentando se conectar ao servidor MQTT...");
    // Tentar se conectar
    String clientId = "Dieletrons-";
    // clientId += String(random(0xffff), HEX);

 if(client.connect(clientId.c_str())){
      Serial.println("Conectado");
      client.subscribe("LED");
      client.subscribe("DC");
      client.subscribe("Mensagens");

    }else{
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      // Esperar 5 segundos antes de tentar novamente
      delay(100);
    }
  }
}


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

    snprintf (msg, MSG_BUFFER_SIZE, "display mudou vermelho");
    client.publish("DC", msg);
  }
    if (char(payload[0]) == '2') {

      matrix-> clearScreen(); 
      matrix->fillScreen(matrix->color444(0, 255, 0));
    // Enviar a mensagem MQTT (você precisará de um publish)

    snprintf (msg, MSG_BUFFER_SIZE, "display mudou verde");
    client.publish("DC", msg);
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
  
    snprintf (msg, MSG_BUFFER_SIZE, "display mudou verde");
    client.publish("DC", msg);
  }
    if ( strncmp((char*)payload, "clear", length) == 0) {

    matrix-> clearScreen(); 
    matrix->fillScreen(matrix->color444(0, 0, 0));
    matrix->setBrightness8(192);

    // Exibe "Hello World" na tela
    matrix->setCursor(4, 0);            // Define a posição do texto
    matrix->setTextColor(matrix->color444(255, 0, 0)); // Define a cor do texto (roxo)
    matrix->setTextSize(1);              // Tamanho do texto
    matrix->print("LIMPOU :D");

    delay(2000);
    matrix-> clearScreen(); 

  
    snprintf (msg, MSG_BUFFER_SIZE, "display mudou verde");
    client.publish("DC", msg);
  }
    if ( strncmp((char*)payload, "Animation", length) == 0) {

            AnimationActive = true;

            vTaskResume(animationTaskHandle);

            snprintf(msg, MSG_BUFFER_SIZE, "Animation started");
            client.publish("DC", msg);
  }
    if ( strncmp((char*)payload, "break", length) == 0) {

           if (animationTaskHandle != NULL) {
            vTaskSuspend(animationTaskHandle);
        }
        ESP.restart();
  }

}

void setup() {

   setCpuFrequencyMhz(160);
  // Inicializa a comunicação serial
  
  // Configura o painel de LED
  xTaskCreate(taskAnimation, "TaskAnimation", 4096, NULL, 1, &animationTaskHandle);
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER);

       // Quantidade de painéis em cadeia (se houver mais, ajuste aqui)
  // Cria a instância do painel
  mxconfig.gpio.e = PIN_E;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;
    
  matrix = new MatrixPanel_I2S_DMA(mxconfig);



  // Inicializa o painel
  if (matrix->begin()) {
    Serial.println("Painel inicializado com sucesso");
  } else {
    Serial.println("Falha ao inicializar o painel");
  }

  // Preenche a tela com preto (desliga os LEDs)

          // Mensagem a ser exibida
            
  matrix->fillScreen(matrix->color444(0, 0, 0));
  matrix->setBrightness8(192);

  // Exibe "Hello World" na tela
  matrix->setCursor(0, 0);            // Define a posição do texto
  matrix->setTextColor(matrix->color444(255, 0, 0)); // Define a cor do texto (roxo)
  matrix->setTextSize(2);              // Tamanho do texto
  matrix->print("Teste");

  matrix->setCursor(0, 16);            // Define a posição do texto
  matrix->setTextColor(matrix->color444(0, 255, 0)); 
  matrix->print("2 displays");
  
  
  Serial.begin(115200);

    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback); // Define a função de retorno de chamada para o cliente MQTT

      if (!client.connected()){
      reconnect();
    }
    client.loop();

   sprintf(msg, "teste pra vê se funciona");
   client.publish("Mensagens", msg);

}

void loop() {
// !client.connected()?reconnect():client.loop();
    if (!client.connected()){
      reconnect();
    }
    client.loop();
    delay(2000);
  // O loop pode ser usado para animações futuras

}


void taskAnimation(void * parameter) {
    for (;;) {
        client.loop();
        
        if (AnimationActive) {
            matrix->clearScreen();
            
            // Animação no eixo X
            if (directionx) {
                matrix->setCursor(i, j);
                i++;
                matrix->setTextColor(matrix->color444(0, 255, 0));
                matrix->setTextSize(1);
                size_t aa = matrix->print("8===D :(");
                int T = 127 - aa*6;
                if (i >= 75) {
                    directionx = false;
                }
            } else {
                matrix->setCursor(i, j);
                i--;
                matrix->setTextColor(matrix->color444(0, 255, 0));
                matrix->setTextSize(1);
                matrix->print("C|===8 :D");
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

            vTaskDelay(50 / portTICK_PERIOD_MS);  // Espera por 50ms
        }
    }
}
