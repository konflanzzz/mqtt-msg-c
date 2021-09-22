#include <stdio.h>
#include <stdlb.h>

#include <mosquitto.h>


int main(){

}

int enviarMensagem(char user_message){

	int resposta;
	struct mosquitto * mosq;

	mosquito_lib_init();

	mosq_pub = mosquitto_new("msg-publisher", true, 10);

	resposta = mosquitto_connect(mosq_pub, "broker.emqx.io", 1883, 60);

	if (resposta !=0) {
		printf("Nao foi possivel se conectar ao broker. Erro: CODE: %d\n", resposta);
		mosquitto_destroy(mosq_pub);
		return -1;
	}

	printf("Conexao estabelecida com sucesso! "\n);
	
	mosquitto_publish(mosq_pub, NULL, "msg-topic/t1", 250, user_message, 0, false);
	mosquitto_disconnect(mosq_pub);
	mosquitto_destroy(mosq_pub);
	mosquito_lib_cleanup();
	
	return 0;
}

int receberMensagem(){
	int resposta;

	struct mosquitto * mosq_sub;

	mosq_sub = mosquitto_new("msg-subscriber", true, 11);
	mosquitto_connect_callback_set(mosq_sub, conexao);
	mosquitto_message_callback_set(mosq_sub, on message);

	resposta = mosquitto_connect(mosq_sub, "broker.emqx.io", 1883, 60);

	if (resposta != 0 ){
		printf("Nao foi possivel se conectar ao broker. Erro: CODE: %d\n", resposta);
		return -1;
	}
	
	mosquitto_loop_start(mosq_sub);
	printf("Pressione Enter para sair...\n");
	getchar();
	mosquitto_loop_stop(mosq_sub, true);

}

void conexao(struct mosquitto * mosq_sub, void * obj, int resposta){
	
	printf("ID: %d\n", * ( int *) obj);
	
	if (resposta){
		printf("Erro: CODE: %d\n", resposta);
		exit(-1);
	}
	
	mosquitto_subscribe(mosq_sub, null, "msg-topic/t1", 0);

}

void mensagem(struct mosquitto * mosq_sub, void * obj, const struct mosquitto_message * msg){
	printf("Nova mensagem: %s: %s\n", msg->topic,( char * ) msg->payload);
}
