void addCorsHeaders(AsyncWebServerResponse *response) {
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
}

void handleOptions(AsyncWebServerRequest *request) {
  Serial.println("OPTIONS request received");
  AsyncWebServerResponse *response = request->beginResponse(200);
  addCorsHeaders(response);
  request->send(response);
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void homeRequest(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "{servos:[...],buttons:[...]}");
  addCorsHeaders(response);
  request->send(response);
}

int GetIdFromURL(AsyncWebServerRequest *request, String root)
{
  String string_id = request->url();
  string_id.replace(root, "");
  int id = string_id.toInt();
  return id;
}

String GetBodyContent(uint8_t *data, size_t len)
{
  String content = "";
  for (size_t i = 0; i < len; i++) {
    content .concat((char)data[i]);
  }
  return content;
}
