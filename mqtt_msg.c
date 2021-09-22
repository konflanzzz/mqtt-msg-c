#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <unistd.h>
#include <pthread.h>

void * enviarMensagem(){

	int rc;
	struct mosquitto * mosq;
	char user_name[100];
	char user_message[500];

	mosquitto_lib_init();

	mosq = mosquitto_new("publisher-test", true, NULL);

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

	mosquitto_publish(mosq, NULL, "msg-fernando/t1", 500, user_message, 0, false);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return NULL;

}

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	
	printf("ID: %d\n", * (int *) obj);

	if(rc){
		printf("Erro codigo: %d\n", rc);
		exit(-1);
	}

	mosquitto_subscribe(mosq, NULL, "msg-fernando/t1", 0);

}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg){
	printf("Nova mensagem de %s: %s\n", msg->topic, (char *) msg->payload);
}

void * receberMensagem() {
	int rc, id=12;

	struct mosquitto *mosq;

	mosq = mosquitto_new("subscriber-test", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	
	rc = mosquitto_connect(mosq, "broker.emqx.io", 1883, 10);
	
	if (rc){
		printf("Nao foi possivel se conectar com o broker. Erro: %d\n", rc);
		exit(-1);
	}

	mosquitto_loop_start(mosq);
	printf("Pressione Enter para sair...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return NULL;

}

int main()
{
	pthread_t msg_receber_mensagem;
	pthread_create(&msg_receber_mensagem, NULL, receberMensagem, NULL);

	pthread_t msg_enviar_mensagem;
	pthread_create(&msg_enviar_mensagem, NULL, enviarMensagem, NULL);

	pthread_exit(NULL);
	return 0;
}