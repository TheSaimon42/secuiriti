#include <FS.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputDAC.h>

int rele = 18;
int solenoide = 16;
int sensor = 23;

bool alarma = false;
unsigned long tiempoInicio = 0;
unsigned long duracionAlarma = 10000;
unsigned long solenoidemax = 5000;    
unsigned long tiemposolenoide = 0;

AudioFileSourceSPIFFS *archivoWAV;
AudioGeneratorWAV *generador;
AudioOutputDAC *salidaAudio;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(solenoide, OUTPUT);
  digitalWrite(solenoide, LOW);

  pinMode(sensor, INPUT);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);

  if (!SPIFFS.begin(true)) {
    Serial.println("No se pudo montar SPIFFS");
    return;
  }

  if (!SPIFFS.exists("/alarma.wav")) {
    Serial.println(" /alarma.wav no existe en SPIFFS");
    return;

  salidaAudio = new AudioOutputDAC();
  salidaAudio->SetGain(0.9);

  archivoWAV = new AudioFileSourceSPIFFS("/alarma.wav");
  generador = new AudioGeneratorWAV();
}

void loop() {
  int movimiento = digitalRead(sensor);

  if (movimiento == HIGH && !alarma) {
    Serial.println(" Movimiento detectado: activando alarma");
    digitalWrite(solenoide, HIGH);
    generador->begin(archivoWAV, salidaAudio);
    tiempoInicio = millis();
    alarma = true;
  }

  if (alarma && generador->isRunning()) {
    generador->loop();
  }
  if (alarma && millis() - tiempoInicio > duracionAlarma) {
    Serial.println("Qu Tiempo de alarma finalizado");
    digitalWrite(solenoide, LOW);
    alarma = false;

    delete archivoWAV;
    archivoWAV = new AudioFileSourceSPIFFS("/alarma.wav");
  }
}



