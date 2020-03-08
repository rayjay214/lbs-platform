#ifndef _KAFKA_OP_H_
#define _KAFKA_OP_H_

#include "global.h"
#include <librdkafka/rdkafka.h>
 

class ProducerKafka
{
public:
    ProducerKafka(){};
    ~ProducerKafka(){}
 
    int init_kafka(int partition, const char *brokers, const char *topic);
    int push_data_to_kafka(const char* buf, const int buf_len);
    void destroy();
 
private:
    int partition_;    
    
    //rd
    rd_kafka_t* handler_;
    rd_kafka_conf_t *conf_;
    
    //topic
    rd_kafka_topic_t *topic_;
    rd_kafka_topic_conf_t *topic_conf_;
};

#endif
