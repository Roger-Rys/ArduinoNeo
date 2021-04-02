#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial neo6(10, 11); // pin RX y TX
SoftwareSerial proA7(8, 9); // pin RX y TX

TinyGPSPlus gps; // Nombre GPS

////////VARIABLES PINES
const byte power_A7 = 4, p_reset_A7 = 3, p_PWRKEY = 2;
const byte ledOn = A5, ledSend = A4, ledLlamada = A3, ledGPSon = A2, onRed = A1, ledPWRKEY = A0; // Leds de activacion
///////VARIABLES PARA ENCENDER Y APAGAR LED
const int encender = 255, apagar = 0;

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

///////Para enviar datos posicion
bool encenderGPS = false;
String coordenadas = "";


/*******FUNC SETUP***************************************************************/
void setup() {
  pinMode(5, INPUT);// Pin MUX como salida

  ////////DECLARAR PINES
  //POWER
  pinMode(power_A7, OUTPUT);
  digitalWrite(power_A7, HIGH);

  //LED
  analogWrite(ledOn, encender);

  delay(20000);  //Esperar a que modulo A7 inicialice
  //Velocidad serial para arduino nano
  Serial.begin(9600);
  while (!Serial) {
    ;// Espera al puerto serial se conecte
  }
  // Velocidad serial para modulo A7
  proA7.begin(9600);
  // Velocidad serial para modulo Neo6m
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
  else if ((conteoRing % 2) == 1 && NumTelSav == NumTelRec && conteoRing > 0 && enviarDatos == false && encenderGPS == false) {
    Serial.println("---DATOS ENCENDIDO---");
    delay(500);
    encenderGPS = true;
    enviarDatos = false; //enviarDatos sera true
    Serial.println("GPS on");
  }
  //PROCESO DE RECOGER COORDENADAS
  else if (encenderGPS) {
    //Encender led GPS
    analogWrite(ledGPSon, encender);
    //Escuchar al modulo neo6m por puerto serial
    neo6.listen();
    //Neo6m habilitado
    while (neo6.available() > 0) {
      //Codificar los valores NMEA entregada por modulo Neo6m
      gps.encode(neo6.read());
      //Deteccion de nuevas coordenadas
      if (gps.location.isUpdated()) {
        //Trama a enviar con datos de posicion en los campos field1 y field2
        coordenadas = "&field1=" + String(gps.location.lat(), 6) + "&field2=" + String(gps.location.lng(), 6);
        //Asignacion de variables
        encenderGPS = false;
        enviarDatos = true; //enviarDatos sera verdadera
        //Apagar led GPS
        analogWrite(ledGPSon, apagar);
      }
    }
  }
  //POCESO DE ENVIO DE DATOS
  else if (enviarDatos) {
    enviarDatosGPS(coordenadas); //Llamar a la funcion "enviarDatosGPS"
    serialA7();//___
    if (escribirComando("AT", 4000, true) != true) { // Envia AT y espera respuesta OK
      Serial.println("--Desconectando A7--");
      proA7.println("AT+CPOF"); delay(1000);
      configuracionInicial(); // Configuracion inicial
    }
    serialA7();//___
    encenderGPS = true;
    serialA7();//___
    enviarDatos = false; //enviarDatos sera true
    Serial.println("GPS on");
    serialA7();//___
    delay(2000);
    serialA7();//___
    delay(2000);
    serialA7();//___
    delay(2000);
    serialA7();//___
    delay(2000);
    serialA7();//___
    delay(2000);
  }
  //APAGAR DATOS
  //Los numeros telefonicos son iguales, ConteoRign es un numero Impar y mayor a cero
  else if ((conteoRing % 2) == 0 && NumTelSav == NumTelRec && conteoRing > 0) {
    ////////DESACTIVAR enviarDatos
    enviarDatos = false; //enviarDatos sera false
    ////////DESACTIVAR enviarDatos
    encenderGPS = false; //enviarDatos sera false
    Serial.println("--Conexion cerrada--");
    ComunicacionSerial(); //Llamar a funcion "ComunicacionSerial"
  }
  else {
    ComunicacionSerial(); //Llamar a funcion "ComunicacionSerial"
  }
}
