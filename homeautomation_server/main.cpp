#include "mbed.h"
#include "cc3000.h"
#include "main.h"
#include "HTTPClient.h"
#include "SDFileSystem.h"
#include <string>

#include "TCPSocketConnection.h"
#include "TCPSocketServer.h"

mbed_cc3000::cc3000 wifi(D3, D5, D10, SPI(D11, D12, D13),
                         "HomeAutomationSystem", "", NONE, false);
                         //"CalVisitor", "", NONE, false);
                         
SDFileSystem sd(PTD6,PTD7, PTD5, PTD4 ,"sd"); 
Serial pc(USBTX, USBRX);

Ticker update;
Ticker connectInterrupt;
HTTPClient http;
DigitalOut led_red(LED_RED);
DigitalOut led_green(LED_GREEN);

int updateFlag = 0;
char resp[512];
char resp2[512];

//Interrupts
void updateCloud() { //Triggers a cloud update.
    if(updateFlag == 0){
        led_red = !led_red;
        led_green = !led_green;
        updateFlag = 1;
    }
}

void connectInternet(){ // Attempts to connect to internet, if not already.
    if(wifi.is_connected() == false){
        wifi.connect();
    }
}

void getSensorLocal(char* sensor, char* data){
    char filename[1024];
    int lines=0;
    char line [256];
    
    snprintf(filename, sizeof(filename), "/sd/homeautomation/%s.txt", sensor);
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        pc.printf("Error opening file.");
        strcpy(resp, "ERROR");
    }else{
        while (fgets(line, sizeof line, fp) != NULL)
        {   
            lines++;
            if(lines == atoi(data)){
                char* value;
                value = strtok (line, " ");
                strcpy(resp, value);
                break;
            }
        }      
    }
    fclose(fp);
}

void writeSensorLocal(char* sensor, char* data){
    char filename[1024];
    int lines=0;
    char line [256];
    
    snprintf(filename, sizeof(filename), "/sd/homeautomation/%s.txt", sensor);
    FILE *fp = fopen(filename, "a");
    fprintf(fp, "%s \r\n", data);
    fclose(fp);
    
    fp = fopen(filename, "r");
    lines=0;
    while (fgets ( line, sizeof line, fp) != NULL)
    {   
        lines++;
    }
    fclose(fp); 
    sprintf(resp, "%d", lines);
}

void writeSensorTemp(char* sensor, char* data){
    char filename[1024];
    char line[512];
    
    strcpy(filename, "/sd/homeautomation/temp.txt");
    FILE *fp = fopen(filename, "a");
    snprintf(line, sizeof(line), "%s %s %s \r\n", sensor, data, resp);
    fprintf(fp, "%s", line);
    fclose(fp);
}

void updateFromTemp(){
    char line [256];
    char url [512];
    char* id;
    char* sensor;
    char* data;
    int connected = 1;

    FILE *fp = fopen("/sd/homeautomation/temp.txt", "r");
    FILE *fp2 = fopen("/sd/homeautomation/temp_tmp.txt", "w");
    if(fp == NULL) {
        pc.printf("Error opening file.");
        strcpy(resp, "ERROR");
    }else{
        while (fgets(line, sizeof line, fp) != NULL)
        {   
            if(connected){
                sensor = strtok (line, " ");
                data = strtok (NULL, " ");
                id = strtok (NULL, " ");
                snprintf(url, sizeof(url), "http://ee149has.herokuapp.com/?sensor=%s&value=%s&id=%s", sensor, data, id);
                pc.printf(url);
                int ret = http.get(url, resp2, 128);
                //if (ret) {
                    //connected = 0;
                    //fprintf(fp2, "%s", line);
                //}
            }else{
                //Copy unupdated lines to new file, then rename.
                fprintf(fp2, "%s", line);
            }
        }
    }      
    fclose(fp);
    fclose(fp2);
    remove("/sd/homeautomation/temp.txt");
    rename("/sd/homeautomation/temp_tmp.txt", "/sd/homeautomation/temp.txt");
}

int main() {
    led_red = 1;
    led_green = 0;
    pc.printf("Home Automation Server \r\n");
    
    //Connect to wifi.
    wifi.init();
    if (wifi.connect() == -1) {
        pc.printf("Failed to connect. Please verify connection details and try again. \r\n");
    } else {
        pc.printf("IP address: %s \r\n", wifi.getIPAddress());
    }
    
    //Set up SD card.
    mkdir("/sd/homeautomation", 0777);
    
    //Set up interrupts.
    update.attach(&updateCloud, 60*5);
    connectInterrupt.attach(&connectInternet, 60*10);
    
    //Set up TCP server.
    const int ECHO_SERVER_PORT = 1895;
    TCPSocketServer server;
    server.bind(ECHO_SERVER_PORT);
    server.listen();
    
    while (true) {
        if (updateFlag){ //Update the cloud.
            updateFlag = 0;
            pc.printf("Updating to the cloud.");
            led_red = !led_red;
            led_green = !led_green;
            if(wifi.is_connected()){
                updateFromTemp();
                //Leaving this for now to make sure we can access the internet.
                //int ret = http.get("http://husk.eecs.berkeley.edu/projects/cc3000/hello.txt", resp2, 128);
//                  if (!ret) {
//                    pc.printf("Page fetched successfully - read %d characters\r\n",
//                              strlen(resp2));
//                    pc.printf("Result: %s\r\n", resp2);
//                  } else {
//                    pc.printf("Error - ret = %d - HTTP return code = %d\r\n",
//                              ret,
//                              http.getHTTPResponseCode());
//                  }
            }
        }

        //Listen on TCP socket.
        TCPSocketConnection client;
        server.accept(client);
        client.set_blocking(false, 3000); // Timeout after 3s

        char buffer[1024];
        char url[512];

        while (true) {
            int n = client.receive(buffer, sizeof(buffer));
            if (n <= 0) break;
            
            pc.printf("Received connection from: %s \r\n", client.get_address());
            
            //Parse socket buffer.
            char* request;
            char* sensor;
            char* data;
            request = strtok (buffer, " ");
            sensor = strtok (NULL, " ");
            data = strtok (NULL, " ");
            
            pc.printf("Received a %s request \r\n", request);
            if (strcmp(request, "GET") == 0){
                pc.printf("Getting %s sensor with id %s \r\n", sensor, data);
                if(wifi.is_connected()){
                    //If there is wifi, GET from the cloud server
                    snprintf(url, sizeof(url), "http://ee149has.herokuapp.com/%s/%s", sensor, data);
                    int ret = http.get(url, resp, 128);
                    if (ret) {
                        getSensorLocal(sensor, data);
                    }
                }else{
                    getSensorLocal(sensor, data);
                }
            }else{
                pc.printf("Adding data for %s sensor with value %s\r\n", sensor, data);
                writeSensorLocal(sensor, data);
                if(wifi.is_connected()){
                    snprintf(url, sizeof(url), "http://ee149has.herokuapp.com/?sensor=%s&value=%s&id=%s", sensor, data, resp);
                    int ret = http.get(url, resp2, 128);
                    if (ret) {
                        writeSensorTemp(sensor, data); 
                    } 
                }else{
                    writeSensorTemp(sensor, data);  
                }
            }
            
            //Send response.
            n = sizeof(resp);
            client.send_all(resp, n);
            if (n <= 0) break;
        }
        client.close();
    }
}

