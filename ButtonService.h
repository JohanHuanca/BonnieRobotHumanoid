#include "ButtonResource.h"
#include <vector>

std::vector<ButtonResource> buttonData;

// Esta función permite obtener todos los botones
void getAllButtons(AsyncWebServerRequest *request)
{
    StaticJsonDocument<1024> doc;
    JsonArray array = doc.to<JsonArray>();
  
    for (ButtonResource buttonResource : buttonData) {
        JsonObject obj = array.createNestedObject();
        obj["id"] = buttonResource.id;
        obj["state"] = buttonResource.state;
    }
  
    String message;
    serializeJson(doc, message);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
    addCorsHeaders(response);
    request->send(response);
}

// Esta función permite obtener el estado de un botón específico mediante su ID
void getButtonById(AsyncWebServerRequest *request)
{
    int id = GetIdFromURL(request, "/buttons/");

    StaticJsonDocument<200> doc;

    for (ButtonResource buttonResource : buttonData) {
        if (buttonResource.id == id) {
            doc["id"] = buttonResource.id;
            doc["state"] = buttonResource.state;
            break;
        }
    }
    
    String message;
    serializeJson(doc, message);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
    addCorsHeaders(response);
    request->send(response);
}

void getButton(AsyncWebServerRequest *request)
{
    if(request->url().indexOf("/buttons/") != -1) {
        getButtonById(request);
    } else {
        getAllButtons(request);
    }
}

// Similar a postRequest pero para botones
void createButton(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{ 
    String bodyContent = GetBodyContent(data, len);
  
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, bodyContent);
    if (error) {
        request->send(400); return;
    }

    int id = doc["id"];
    bool state = doc["state"];
    ButtonResource buttonResource(id, state);
    buttonData.push_back(buttonResource);
  
    String message = "Create button with id " + String(id) + " and state " + (state ? "ON" : "OFF");
    Serial.println(message);
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
    addCorsHeaders(response);
    request->send(response);
}

// Esta función permite actualizar el estado de un botón específico mediante su ID
void updateButton(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) 
{
    int id = GetIdFromURL(request, "/buttons/");
    String bodyContent = GetBodyContent(data, len);
  
    StaticJsonDocument<200> doc;
    StaticJsonDocument<200> outputDoc;

    DeserializationError error = deserializeJson(doc, bodyContent);
    if (error) {request->send(400); return;}

    bool newState = doc["state"];

    bool updated = false;
    for (ButtonResource &buttonResource : buttonData) {
        if (buttonResource.id == id) {
            buttonResource.state = newState;
            
            outputDoc["id"] = buttonResource.id;
            outputDoc["state"] = buttonResource.state;
            break;
        }
    }
  String message;
  serializeJson(outputDoc, message);

  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
  addCorsHeaders(response);
  request->send(response);
}


// Esta función permite eliminar un botón específico mediante su ID
void deleteButton(AsyncWebServerRequest *request) {
    int id = GetIdFromURL(request, "/buttons/");

    for (auto it = buttonData.begin(); it != buttonData.end(); ++it) {
        if (it->id == id) {
            buttonData.erase(it);
            break;
        }
    }
    
    String message = "Delete button with id " + String(id);
    Serial.println(message);
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
    addCorsHeaders(response);
    request->send(response);
}
