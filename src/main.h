
// #include <FastLED.h>
#include <Arduino.h>
#include <AnimatedGIF.h>
#include <PubSubClient.h>
#include "MqttManager.h"
#include "WifiManager.h"
#include "Gifs.h"
#include <SPIFFS.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>



const char* ssid = "DiEletrons_Reuniao";
const char* password = "49433012";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient; // Cria um cliente WiFi para se conectar à rede
MQTTManager mqtt(mqtt_server, espClient);
WiFiManager wifi(ssid, password);


MatrixPanel_I2S_DMA *matrix = nullptr;
AnimatedGIF gif;

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

  matrix->setCursor(4, 0);            
  matrix->setTextColor(matrix->color444(255, 0, 0)); 
  matrix->setTextSize(2);             
  matrix->print("Teste");

  matrix->setCursor(3, 16);           
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


extern void GIFDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) // restore to background color
  {
    for (x = 0; x < iWidth; x++)
    {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }
  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) // if transparency used
  {
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + pDraw->iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < pDraw->iWidth)
    {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent) // done, stop
        {
          s--; // back up to treat it like transparent
        }
        else // opaque
        {
          *d++ = usPalette[c];
          iCount++;
        }
      }           // while looking for opaque pixels
      if (iCount) // any opaque pixels?
      {
        for (int xOffset = 0; xOffset < iCount; xOffset++)
        {
          matrix->drawPixel(x + xOffset, y, usTemp[xOffset]);
        }
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount)
      {
        x += iCount; // skip these
        iCount = 0;
      }
    }
  }
  else
  {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x = 0; x < pDraw->iWidth; x++)
    {
      matrix->drawPixel(x, y, usPalette[*s++]);
    }
  }
} /* GIFDraw() */


extern void Gif(){
 if (gif.open((uint8_t *)homer_tiny, sizeof(homer_tiny), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    while (gif.playFrame(true, NULL))
    {
    }
    gif.close();
  }
}
