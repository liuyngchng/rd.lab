#ifndef KFK_H
#define KFK_H


/**
 * init kfk client
 */
int kfk_init(const char *broker) ;

/**
 * snd msg to kfk
 *
 */
int kfk_wrt(const char *topic, const char *key, const char *msg);

#endif
