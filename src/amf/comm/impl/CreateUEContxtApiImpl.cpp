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

#include "CreateUEContxtApiImpl.h"
#include <unordered_map>

extern std::unordered_map<std::string,org::openapitools::server::model::UeContext> RecordUEContext;

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::model;

CreateUEContxtApiImpl::CreateUEContxtApiImpl(std::shared_ptr<Pistache::Rest::Router> rtr)
    : CreateUEContxtApi(rtr)
    { }

void CreateUEContxtApiImpl::create_ue_context(const std::string &ueContextId, const UeContextCreateData &ueContextCreateData, Pistache::Http::ResponseWriter &response) {
    std::cout<<"Recieved::\nueContextId:"<<ueContextId<<"\t\nUeContextCreateData:"<<ueContextCreateData.toJson()<<"\n"\
        <<"ueContext:"<<ueContextCreateData.getUeContext().toJson()<<"\n";
    if(RecordUEContext.find(ueContextId)==RecordUEContext.end())
    {
        RecordUEContext[ueContextId]=ueContextCreateData.getUeContext();
        std::cout<<"Record::\nueContextId:"<<ueContextId<<"\t\nUeContext:"<<ueContextCreateData.getUeContext().toJson()<<"\n";
        std::string && ResponseContent =  RecordUEContext[ueContextId].toJson().dump();
        response.send(
                Pistache::Http::Code::Created,
                ResponseContent,
                Pistache::Http::Mime::MediaType(Pistache::Http::Mime::Type::Application,Pistache::Http::Mime::Subtype::Json)
                );


        //std::string myResponseContent( "{ status : \"1\"  }"  );
        //response.send(
                //Pistache::Http::Code::Ok,
                //myResponseContent,
                //Pistache::Http::Mime::MediaType(Pistache::Http::Mime::Type::Application,Pistache::Http::Mime::Subtype::Json)
                //);

    }else
    {
        response.send(Pistache::Http::Code::Forbidden, "The UEContextId already exits.\n");
    }
}

}
}
}
}

