#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>
#include <FS.h>
#include <SPIFFS.h>

int solenoide = 16;
int sensor = 23;
bool alarma = false;

tiempoInicio = 0;
duracionAlarma = 10000;
solenoidemax = 5000; 
tiemposolenoido= 0;
AudioFileSourceFS *archivoWAV;
AudioGeneratorWAV *generador;
AudioOutputI2S *salidaAudio;

void setup() {
  // put your setup code here, to run once:

  pinMode(solenoide, OUTPUT);
  digitalWrite(solenoide, LOW);
  pinMode(sensor, INPUT);

  digitalWrite(rele1, LOW);
  salidaAudio = new AudioOutputI2S();
  salidaAudio->SetPinout(21, 22, 26);
  salidaAudio->SetGain(0.6);
  if (!SPIFFS.begin(true)) {
    Serial.println("No sepudo montar SPIFFS");
    return;
  }


  if (!SPIFFS.exists("/alarma.wav")) {
    Serial.println("/alarma.wav no existe");
    return;
  }
  archivoWAV = new AudioFileSourceFS(SPIFFS, "/alarma.wav");
  generador = new AudioGeneratorWAV();

  salidaAudio->SetPinout(21, 22, 26);
  salidaAudio->SetGain(0.77); 

}

 
void loop() {
  // put your main code here, to run repeatedly:
int movimiento = digitalRead(sensor);
if (movimiento == true && !alarma){
  digitalWrite(solenoide, HIGH);
  generador->begin(archivoWAV, salidaAudio);
  tiempoInicio = millis();
  alarma = true;
  
}

if (alarma && generador->isRunning()) {
    generador->loop();
}
 if (millis() - tiempoInicio > duracionAlarma) {
      digitalWrite(solenoide, LOW);
      alarma = false;

     delete archivoWAV;
    archivoWAV = new AudioFileSourceFS(SPIFFS, "/alarma.wav");
  }

}
