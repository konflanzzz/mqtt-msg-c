#include <stdio.h>
#include <mosquitto.h>

int main(){
	int rc;
	struct mosquitto * mosq;
	char user_message;

	mosquitto_lib_init();
	
	mosq = mosquitto_new("publisher-test", true, NULL);

	rc = mosquitto_connect(mosq, "broker.emqx.io", 1883, 60);

	if (rc !=0) {
		printf("Client nao pode se conectar ao broker. Erro codigo: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1; 
	}

	printf("conexao estabelecida com sucesso! \n");	

	mosquitto_publish(mosq, NULL, "test/t1", 100, "fernando diz oi", 0, false);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}

