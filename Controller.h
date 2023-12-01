AsyncWebServer server(80);

void initServer()
{
  server.on("/", HTTP_GET, homeRequest);

  server.on("/servos", HTTP_GET, getServo);
  server.on("/servos", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, createServo);
  server.on("/servos", HTTP_PUT, [](AsyncWebServerRequest * request){}, NULL, updateServo);
  server.on("/servos", HTTP_DELETE, deleteServo);
  server.on("/servos", HTTP_OPTIONS, handleOptions);

  server.on("/buttons", HTTP_GET, getButton);
  server.on("/buttons", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, createButton);
  server.on("/buttons", HTTP_PUT, [](AsyncWebServerRequest * request){}, NULL, updateButton);
  server.on("/buttons", HTTP_DELETE, deleteButton);
  server.on("/buttons", HTTP_OPTIONS, handleOptions);
  
  server.onNotFound(notFound);

  server.begin();
    Serial.println("HTTP server started");
}
