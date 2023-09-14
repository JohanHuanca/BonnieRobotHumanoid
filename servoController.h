AsyncWebServer server(80);

void homeRequest(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Servo: [...]");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  request->send(response);
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void handleOptions(AsyncWebServerRequest *request) {
  Serial.println("OPTIONS request received");
  AsyncWebServerResponse *response = request->beginResponse(200);
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  request->send(response);
}
void initServer()
{
  server.on("/", HTTP_GET, homeRequest);
  server.on("/servos", HTTP_GET, getRequest);
  server.on("/servos", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, postRequest);
  server.on("/servos", HTTP_PUT, [](AsyncWebServerRequest * request){}, NULL, putRequest);
  server.on("/servos", HTTP_DELETE, deleteRequest);
  server.on("/servos", HTTP_OPTIONS, handleOptions);
  
  server.onNotFound(notFound);

  server.begin();
    Serial.println("HTTP server started");
}
