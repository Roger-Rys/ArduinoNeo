#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial proA7(10, 11); // pin RX y TX
SoftwareSerial neo6(8, 9); // pin RX y TX

TinyGPSPlus GPS; // Nombre GPS

////////VARIABLES PINES
byte p_PWRKEY = 2; // Pin para activar la red GSM
byte ledSend = 7, ledLlamada = 6, ledGPSon = 4, onRed = 5, p_reset_A7 = 3, power = 12; // Leds de activacion

////////COMUNICACION_SERIAL
byte conteoRing = 0; // conteoRing
char valorChar = 0; //Recive los valores char de proA7
String valorStr; // Almacena valores de "valorChar"
char trama1[100]; // valorStr a trama1
char num_telefoYmas[100]; // valorStr a num_telefoYmas

//////////DETECTAR NUMERO TELEFONICO
char tramaPhone[50]; // Almacena Numero
String NumTelRec = "1"; //Numero Telefonico Recibido
String NumTelSav = "1"; //Numero Telefonico Guardado
//VARIABLES PARA SEPARAR TRAMA
char *strtok(char *str1, const char *str2); //Subfuncion separa la trama
char *numeroPhone = NULL; //No almacena en una memoria especifica

///////DATOS POR INTERNET
bool enviarDatos = false;//Defecto false


/*******FUNC SETUP***************************************************************/
void setup() {
  pinMode(5, INPUT);// Pin MUX como salida

  ////////DECLARAR PINES
  //PINES LEDS
  pinMode(ledSend, OUTPUT); pinMode(ledLlamada, OUTPUT);
  pinMode(ledGPSon, OUTPUT); pinMode(onRed, OUTPUT);
  //PIN PARA PWR KEY
  pinMode(p_PWRKEY, OUTPUT);
  //PIN PARA RESET MODULO A7
  pinMode(p_reset_A7, OUTPUT);
  //POWER
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);

  delay(20000);  //Esperar a que modulo A7 inicialice
  //Velocidad serial para arduino nano
  Serial.begin(9600);
  while (!Serial) {
    ;// Espera al puerto serial se conecte
  }
  // Velocidad serial para modulo A7
  proA7.begin(9600);
  neo6.begin(9600);
  Serial.println("----INICIO----");
  configuracionInicial(); // Configuracion inicial
}

/******************FUNC LOOP*****************************/
void loop() {
  ////////MODO NORMAL
  if (NumTelRec != NumTelSav) { // Son diferentes, ninguna similitud NULL
    NumTelSav = NumTelRec; //Se almacena el NumTelRec en NumTelSav
    Serial.print("Telefono Guardado: "); Serial.println(NumTelSav);
  }
  //ACTIVAR DATOS
  //Los numeros telefonicos son iguales, ConteoRign es un numero Par y mayor a cero, enviarDatos es falso
  else if ((conteoRing % 2) == 1 && NumTelSav == NumTelRec && conteoRing > 0 && enviarDatos == false) {
    Serial.println("---DATOS ENCENDIDO---");
    delay(500);
    enviarDatos = true; //enviarDatos sera true
  }
  //APAGAR DATOS
  //Los numeros telefonicos son iguales, ConteoRign es un numero Impar y mayor a cero
  else if ((conteoRing % 2) == 0 && NumTelSav == NumTelRec && conteoRing > 0) {
    ////////DESACTIVAR enviarDatos
    enviarDatos = false; //enviarDatos sera false
    Serial.println("--Conexion cerrada--");
    ComunicacionSerial(); //Llamar a funcion "ComunicacionSerial"
  }
  //POCESO DE ENVIO DE DATOS
  else if (enviarDatos) {
    enviarDatosGPS(); //Llamar a la funcion "enviarDatosGPS"
    serialA7();//___
    if (escribirComando("AT", 4000, true) != true) { // Envia AT y espera respuesta OK
      Serial.println("--Desconectando A7--");
      proA7.println("AT+CPOF"); delay(1000);
      configuracionInicial(); // Configuracion inicial
    }
    serialA7();//___

  }
  else {
    ComunicacionSerial(); //Llamar a funcion "ComunicacionSerial"
  }
}
