#include <MQTTClient.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define ADDRESS     "tcp://broker.mqtt-dashboard.com:1883"
#define TOPIC       "cobadata"
#define QOS         1
#define TIMEOUT     10000L

typedef struct {
    char time[20];
    char model[20];
    char type[20];
    char id[20];
    int wheel;
    int pressure;
    int temperature;
    char state;
    int flags;
    char integrity[20];
} DeviceData;

static void mqtt_publisher(const char* message) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, "ExampleClientPub", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    char payload[100];
    sprintf(payload, "%s", message);

    printf("Publishing message: %s\n", payload);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Message published successfully\n");

    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
}

static void print_and_save_device_data(DeviceData *devices, unsigned num_devices, int exit_code, const char *hasildata) {
    unsigned i;
    char disabledc;

    // Buka file untuk menulis
    FILE *file = fopen(hasildata, "w");
    if (file == NULL) {
        fprintf(stderr, "Gagal membuka file untuk penulisan.\n");
        mqtt_publisher("Gagal membuka file untuk penulisan.\n");
        exit(exit_code);
    }

    if (devices) {
        // Mencetak header ke layar dan menulisnya ke file
        fprintf(stdout, "\t\t= Supported device protocols =\n");
        fprintf(file, "\t\t= Supported device protocols =\n");

        // Iterasi melalui data dan mencetak informasi ke layar serta menulisnya ke file
        for (i = 0; i < num_devices; i++) {
            disabledc = devices[i].state == 'a' ? '*' : ' ';
            if (devices[i].state == 'a') { // Jika state bukan hidden
                // Cetak ke layar
                printf("time      : %s\n", devices[i].time);
                printf("model     : %s\ttype      : %s\tid        : %s\n", devices[i].model, devices[i].type, devices[i].id);
                printf("wheel     : %d\tPressure  : %d kPa\tTemperature: %d C\n", devices[i].wheel, devices[i].pressure, devices[i].temperature);
                printf("State?    : %c\tFlags?    : %d\tIntegrity : %s\n", devices[i].state, devices[i].flags, devices[i].integrity);

                // publish mqtt
                char stringConstruct[128];
                sprintf(stringConstruct, "time      : %s\n", devices[i].time);
                mqtt_publisher(stringConstruct); 
                sprintf(stringConstruct, "model     : %s\ttype      : %s\tid        : %s\n", devices[i].model, devices[i].type, devices[i].id);
                mqtt_publisher(stringConstruct);
                sprintf("wheel     : %d\tPressure  : %d kPa\tTemperature: %d C\n", devices[i].wheel, devices[i].pressure, devices[i].temperature);
                mqtt_publisher(stringConstruct);
                sprintf("State?    : %c\tFlags?    : %d\tIntegrity : %s\n", devices[i].state, devices[i].flags, devices[i].integrity);
                mqtt_publisher(stringConstruct);

                // Tulis ke file
                fprintf(file, "time      : %s\n", devices[i].time);
                fprintf(file, "model     : %s\ttype      : %s\tid        : %s\n", devices[i].model, devices[i].type, devices[i].id);
                fprintf(file, "wheel     : %d\tPressure  : %d kPa\tTemperature: %d C\n", devices[i].wheel, devices[i].pressure, devices[i].temperature);
                fprintf(file, "State?    : %c\tFlags?    : %d\tIntegrity : %s\n", devices[i].state, devices[i].flags, devices[i].integrity);
            
            }
        }

        fprintf(stdout, "\n* Hidden devices are not displayed\n");
        fprintf(file, "\n* Hidden devices are not displayed\n");
    }

    // Tutup file setelah selesai menulis
    fclose(file);

    // Keluar dari program dengan kode keluaran yang diberikan
    exit(exit_code);
}

int main() {
    // Contoh data perangkat
    DeviceData devices[] = {
        {"2023-11-29 12:50:45", "Truck", "TPMS", "38e5ba67", 1, 0, 24, 'a', 3, "CHECKSUM"},
        {"2023-11-29 12:52:50", "Truck", "TPMS", "390ca286", 4, 0, 25, 'a', 3, "CHECKSUM"},
        // Tambahkan data perangkat lainnya sesuai kebutuhan
    };

    // Hitung jumlah data perangkat
    unsigned num_devices = sizeof(devices) / sizeof(devices[0]);

    // Tampilkan dan simpan data ke file .txt
    print_and_save_device_data(devices, num_devices, 1, "output.txt");

    return 0;
}