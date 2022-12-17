all: mysignal shm_consumer_org shm_consumer_race shm_consumer_sem shm_producer_org shm_producer_race shm_producer_sem con_client_sem con_server_sem pro_server_sem pro_client_sem

mysignal : mysignal.c
	gcc -o mysignal mysignal.c
shm_consumer_org : shm_consumer_org.c
	gcc -o shm_consumer_org shm_consumer_org.c
shm_consumer_race : shm_consumer_race.c
	gcc -o shm_consumer_race shm_consumer_race.c
shm_consumer_sem : shm_consumer_sem.c
	gcc -o shm_consumer_sem shm_consumer_sem.c
shm_producer_org : shm_producer_org.c
	gcc -o shm_producer_org shm_producer_org.c
shm_producer_race : shm_producer_race.c
	gcc -o shm_producer_race shm_producer_race.c
shm_producer_sem : shm_producer_sem.c
	gcc -o shm_producer_sem shm_producer_sem.c
con_server_sem : con_server_sem.c
	gcc -o con_server_sem con_server_sem.c
con_client_sem : con_client_sem.c
	gcc -o con_client_sem con_client_sem.c
pro_server_sem : pro_server_sem.c
	gcc -o pro_server_sem pro_server_sem.c
pro_client_sem : pro_client_sem.c
	gcc -o pro_client_sem pro_client_sem.c

clean : rm -f  mysignal shm_consumer_org shm_consumer_race shm_consumer_sem shm_producer_org shm_producer_race shm_producer_sem con_server_sem con_client_sem pro_server_sem pro_client_sem




