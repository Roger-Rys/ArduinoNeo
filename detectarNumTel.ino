/**********DETECTAR NUMERO TELEFONICO*********************************/
String detectarNumTel(char *tramaPhone){ //Devolvera un valor String
  String strNumTel = ""; // StrNumTel almacenara el numero telefonico
  char *cadenaSeparar; // Cadena a separar
  char *token;  // token es SubStrings de un String almacena la separacion
  byte conteo = 0; // contador

  if (tramaPhone[0] != NULL ) { // Si tramaPhone no esta vacio
    cadenaSeparar = tramaPhone; // Almacena "tramaPhone" en "cadenaSeparar"
    // "," es el delimitador para separar
    while ((token = strtok_r(cadenaSeparar, ",", &cadenaSeparar)) != NULL) { 
      conteo ++; // Aumenta "conteo"
      if (conteo == 6) { // Si "conteo"=6
        strNumTel = token; // Token se almacena en "strNumTel"
      }
    }
    if (strNumTel != NULL) { // Identifica si strNumTel no esta vacio
      return strNumTel; //Devuelve un string con el numero telefonico
    }
    else{
      return "Numero no guardado"; //Devuelve si strNumTel esta vacio
    }
  }
}

