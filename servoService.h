#include "publicService.h"
#include "ServoResource.h"
#include "Vector.h"

Vector<ServoResource> servoData;

void addCorsHeaders(AsyncWebServerResponse *response) {
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
}

void getAll(AsyncWebServerRequest *request)
{
  StaticJsonDocument<1024> doc;
  JsonArray array = doc.to<JsonArray>();
  
  for (ServoResource servoResource : servoData) {
    JsonObject obj = array.createNestedObject();
    obj["id"] = servoResource.id;
    obj["angle"] = servoResource.angle;
  }
  
  String message;
  serializeJson(doc, message);
  
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
  addCorsHeaders(response);
  request->send(response);
}
void getById(AsyncWebServerRequest *request)
{
  int id = GetIdFromURL(request, "/servos/");

  StaticJsonDocument<200> doc;
  
  for (ServoResource servoResource : servoData) {
    if (servoResource.id == id) {
      doc["id"] = servoResource.id;
      doc["angle"] = servoResource.angle;
      break;
    }
  }
  
  String message;
  serializeJson(doc, message);
  
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
  addCorsHeaders(response);
  request->send(response);
}

void getRequest(AsyncWebServerRequest *request) {
  
  if(request->url().indexOf("/servos/") != -1)
  {
    getById(request);
  }
  else {
    getAll(request);
  }
}
void postRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{ 
  String bodyContent = GetBodyContent(data, len);
  
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  int id = doc["id"];
  float angle = doc["angle"];
  ServoResource servoResource(id, angle);
  servoData.push_back(servoResource);
  
  String message = "Create servo with id " + String(id) + " and angle " + String(angle);
  Serial.println(message);
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
  addCorsHeaders(response);
  request->send(response);
}

void putRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{
  int id = GetIdFromURL(request, "/servos/");
  String bodyContent = GetBodyContent(data, len);
   
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  float angle = doc["angle"];
  
  for (ServoResource& servoResource : servoData) {
    if (servoResource.id == id) {
      servoResource.angle = angle;
      break;
    }
  }
  
  String message = "Replace servo with id " + String(id) + " with angle " + String(angle);
  Serial.println(message);
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
  addCorsHeaders(response);
  request->send(response);
}

void deleteRequest(AsyncWebServerRequest *request) {
  int id = GetIdFromURL(request, "/servos/");

  for (int i = 0; i < servoData.size(); i++) {
    if (servoData[i].id == id) {
      servoData.remove(i);
      break;
    }
  }
  
  String message = "Delete servo with id " + String(id);
  Serial.println(message);
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
  addCorsHeaders(response);
  request->send(response);
}
