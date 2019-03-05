/**
* AMF Communicaiton Service
* AMF Communication Service
*
* OpenAPI spec version: 1.R15.0.0
*
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/


#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#ifdef __linux__
#include <vector>
#include <signal.h>
#include <unistd.h>


#endif

#include <iostream>


//
#include <unordered_map>

#include "AMFStatusChangeSubscribeApiImpl.h"
#include "AMFStatusChangeSubscribeModifyApiImpl.h"
#include "AMFStatusChangeUnSubscribeApiImpl.h"
#include "CreateUEContxtApiImpl.h"
#include "EBIAssignmentApiImpl.h"
#include "N1N2MessageSubscribeApiImpl.h"
#include "N1N2MessageTransferApiImpl.h"
#include "N1N2MessageUnSubscribeApiImpl.h"
#include "NonUEN2InfoSubscribeApiImpl.h"
#include "NonUEN2InfoUnSubscribeApiImpl.h"
#include "NonUEN2MessageTransferApiImpl.h"
#include "ReleaseUEContxtApiImpl.h"
#include "UEContextTransferApiImpl.h"
////////////////////////////////////////////////// itti header /////////////////////////////////
extern "C"{
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_init.h"
}
///////////////////////////////////////////////////////////////////////////////////////////////

#define PISTACHE_SERVER_THREADS 2

static Pistache::Http::Endpoint *httpEndpoint;
#ifdef __linux__
static void sigHandler(int sig){
    switch(sig){
        case SIGINT:
            std::cout<<"Received signal SIGINT\n";
            httpEndpoint->shutdown();
            exit(1);
        case SIGQUIT:
        case SIGTERM:
        case SIGHUP:
        default:
            httpEndpoint->shutdown();
            break;
    }
    exit(0);
}

static void setUpUnixSignals(std::vector<int> quitSignals) {
    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = sigHandler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}
#endif

using namespace org::openapitools::server::api;
using namespace std;

void * demo_receiver_thread(__attribute__((unused)) void *args)
{
        itti_mark_task_ready (TASK_DEMO_RECEIVER);
        while (1) {
                cout<<" itti in receiving message----\n"<<endl;
                MessageDef *received_message_p = NULL;
                // void itti_receiver_msg(task_id_t task_id, MessageDef **rx_msg);
                itti_receive_msg (TASK_DEMO_RECEIVER, &received_message_p);
                DevAssert (received_message_p != NULL);
                // #define ITTI_MSG_ID(mSGpTR) ((mSGpTR)->ittiMsgHeader.messageId)
                switch (ITTI_MSG_ID (received_message_p)) {
                        case MESSAGE_DEMO:
                                cout<<"TASK_DEMO_RECEIVER received a MESSAGE_DEMO\n This demo will exit in 3 seconds.\n"<<endl;
                                break;
                        default:
                                cout<<"TASK_DEMO_RECEIVER received a unknown message\n"<<endl;
                }
                itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
                received_message_p = NULL;
        }
}

std::unordered_map<std::string,org::openapitools::server::model::UeContext> RecordUEContext;
//RecordUEContext.clear();
//
int main() {

         CHECK_INIT_RETURN (itti_init (TASK_MAX, THREAD_MAX, MESSAGES_ID_MAX, tasks_info, messages_info,
#if ENABLE_ITTI_ANALYZER
                                           messages_definition_xml,
#else
                                                     NULL,
#endif
                                                               NULL));

        // int itti_create_task(task_id_t task_id, void *(*function)(void *), void *args_p);
        if (itti_create_task (TASK_DEMO_RECEIVER, &demo_receiver_thread, NULL) < 0) {
        cout<<"Error while creating TASK_DEMO_RECEIVER task\n"<<endl;
        return -1;
        }

        // MessageDef *itti_alloc_new_message(task_id_t origin_tid, MessagesIds msg_id);
        MessageDef *message_p = itti_alloc_new_message(TASK_DEMO_SENDER, MESSAGE_DEMO);
        if (message_p) {
                //use the macro which is defined in the demo_messages_types.h
                DEMO_DATA_IND (message_p).name = "zgw";
                DEMO_DATA_IND (message_p).university = "SYSU";
                //int itti_send_msg_to_task(task_id_t dest_tid, instance_t inst, MessageDef *mesg);
                int send_res = itti_send_msg_to_task(TASK_DEMO_RECEIVER, INSTANCE_DEFAULT, message_p);
        }
    cout<<"init itti success"<<endl;

#ifdef __linux__
    std::vector<int> sigs{SIGQUIT, SIGINT, SIGTERM, SIGHUP};
    setUpUnixSignals(sigs);
#endif
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(5000));

    {//log
        cout<<"amf server:"<<addr.host()<<":"<<addr.port().toString()<<endl;
    }

    httpEndpoint = new Pistache::Http::Endpoint((addr));
    auto router = std::make_shared<Pistache::Rest::Router>();

    auto opts = Pistache::Http::Endpoint::options()
        .threads(PISTACHE_SERVER_THREADS);
    httpEndpoint->init(opts);


    AMFStatusChangeSubscribeApiImpl AMFStatusChangeSubscribeApiserver(router);
    AMFStatusChangeSubscribeApiserver.init();
    AMFStatusChangeSubscribeModifyApiImpl AMFStatusChangeSubscribeModifyApiserver(router);
    AMFStatusChangeSubscribeModifyApiserver.init();
    AMFStatusChangeUnSubscribeApiImpl AMFStatusChangeUnSubscribeApiserver(router);
    AMFStatusChangeUnSubscribeApiserver.init();
    CreateUEContxtApiImpl CreateUEContxtApiserver(router);
    CreateUEContxtApiserver.init();
    EBIAssignmentApiImpl EBIAssignmentApiserver(router);
    EBIAssignmentApiserver.init();
    N1N2MessageSubscribeApiImpl N1N2MessageSubscribeApiserver(router);
    N1N2MessageSubscribeApiserver.init();
    N1N2MessageTransferApiImpl N1N2MessageTransferApiserver(router);
    N1N2MessageTransferApiserver.init();
    N1N2MessageUnSubscribeApiImpl N1N2MessageUnSubscribeApiserver(router);
    N1N2MessageUnSubscribeApiserver.init();
    NonUEN2InfoSubscribeApiImpl NonUEN2InfoSubscribeApiserver(router);
    NonUEN2InfoSubscribeApiserver.init();
    NonUEN2InfoUnSubscribeApiImpl NonUEN2InfoUnSubscribeApiserver(router);
    NonUEN2InfoUnSubscribeApiserver.init();
    NonUEN2MessageTransferApiImpl NonUEN2MessageTransferApiserver(router);
    NonUEN2MessageTransferApiserver.init();
    ReleaseUEContxtApiImpl ReleaseUEContxtApiserver(router);
    ReleaseUEContxtApiserver.init();
    UEContextTransferApiImpl UEContextTransferApiserver(router);
    UEContextTransferApiserver.init();

    httpEndpoint->setHandler(router->handler());
    httpEndpoint->serve();

    httpEndpoint->shutdown();

}

