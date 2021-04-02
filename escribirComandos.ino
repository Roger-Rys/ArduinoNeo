//Escribir Comandos
bool escribirComando(String comando, const int timeout, bool mostrar)
{
  //Varlor que devuelve la funcion
  bool valido = false;
  //Guardara en caracteres lo que contenga comando
  char char_array[75];
  //Recoge por cada caracter lo en recepta de proA7
  String recoger = "";
  //Contendra el resultado
  String respuesta = "";

  Serial.println(comando);
  //Envia el comando a proA7
  proA7.println(comando);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    proA7.listen();
    while (proA7.available() > 0)
    {
      char c = proA7.read();
      recoger += c;
      if (c == '\n') { // Detecta "\n" (salto de linea)
        recoger.toCharArray(char_array, 70); // ValorStr a un arregloChar

        if ((strstr(char_array, "OK") != NULL)) {
          respuesta = "OK";
          recoger = "";
          valido = true;
        }
        else {
          recoger = ""; // Limpia valorStr
          memset(char_array, '\0', 70); // Limpia Trama
        }
      }
    }
  }
  if (mostrar)
  {
    Serial.println(respuesta);
  }
  return valido;
}


