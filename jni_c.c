#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "jni_c.h"

//fifo
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>



//pipe na komunikaciu s PKCS implementaciou
char * pkcs11fifo_read_path = "/tmp/pkcs11fifo2";
char * pkcs11fifo_write_path = "/tmp/pkcs11fifo1";
int pkcs11fifo_read_desc;
int pkcs11fifo_write_desc;
int MAX_PKSC11FIFO_BUF = 1024;

char* defaultNavratovaSprava = "TOTO SA VRATI VZDY";


void dajSpravuZPipe(char sprava[]){    
    pkcs11fifo_read_desc = open(pkcs11fifo_read_path, O_RDONLY); 
    read(pkcs11fifo_read_desc, sprava, MAX_PKSC11FIFO_BUF);   
    close(pkcs11fifo_read_desc);
}

void napisDoPipe(char * sprava){  
    strcat(sprava, "\0");
    pkcs11fifo_write_desc = open(pkcs11fifo_write_path, O_WRONLY); 
    write(pkcs11fifo_write_desc, sprava, strlen(sprava));
    close(pkcs11fifo_write_desc);
}

JNIEXPORT void JNICALL Java_jni_JNI_init
  (JNIEnv * env, jclass class){
    //init pipe komunikacia
    mkfifo(pkcs11fifo_read_path, 0666);
    mkfifo(pkcs11fifo_write_path, 0666);
}

JNIEXPORT jstring JNICALL Java_jni_JNI_process
  (JNIEnv * env, jclass class, jstring message){    
            
    //const char *spravaZJavy= (*env)->GetStringUTFChars(env,message,0);        
    
    //poslem uvodnu spravu     
    napisDoPipe("TEST"); 
    
    //need to release this string when done with it in order to
    //avoid memory leak
    //(*env)->ReleaseStringUTFChars(env, message, spravaZJavy);
    
    //ziskam si spravu z PIPE    
    char sprava[MAX_PKSC11FIFO_BUF];
    dajSpravuZPipe(sprava);    
    
    //vratim prijatu spravu
    jstring jstrBuf = (*env)->NewStringUTF(env, sprava);       
    return jstrBuf;    
}





