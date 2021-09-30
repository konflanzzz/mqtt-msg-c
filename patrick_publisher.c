#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>

int main()
{

    int rc;
    struct mosquitto *mosq;
    char user_name[100];
    char user_message[500];
    char destino[50];

    mosquitto_lib_init();

    mosq = mosquitto_new("publisher-patrick", true, NULL);

    rc = mosquitto_connect(mosq, "broker.emqx.io", 1883, 60);

    if (rc != 0)
    {
        printf("Client nao pode se conectar ao broker. Erro codigo: %d\n", rc);
        mosquitto_destroy(mosq);
        exit(-1);
    }

    printf("conexao estabelecida com sucesso! \n");

    printf("Digite sua mensagem: ");
    scanf("%[^\n]", user_message);
    
    mosquitto_publish(mosq, NULL, "msg-grupo/t1", 500, user_message, 0, false);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
